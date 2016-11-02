/* -------------------------------------------------------------------------- *
 * Copyright (c) 2016 by contributors. All Rights Reserved.                   *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License");            *
 * you may not use this file except in compliance with the License.           *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 *     http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                            *
 *  Unless required by applicable law or agreed to in writing, software       *
 *  distributed under the License is distributed on an "AS IS" BASIS,         *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *  See the License for the specific language governing permissions and       *
 *  limitations under the License.                                            *
 * -------------------------------------------------------------------------- */

/*
Author: Chao Ma (mctt90@gmail.com)

This file is the implementation of reader.h
*/

#include "src/reader/reader.h"

#include <vector>
#include <string>

#include <string.h>

#include "src/base/common.h"
#include "src/base/file_util.h"

using std::vector;
using std::string;

namespace f2m {

const uint32 kMaxLineSize = 100 * 1024; // 100 KB one line

typedef vector<string> StringList;

uint32 ReadLineFromMemory(char* line, char* buf, 
                          uint32 start_pos, uint32 len);

Reader::Reader(const string& filename,
               int num_samples,
               ModelType type,
               bool loop,
               bool in_memory) :
  m_filename(filename),
  m_num_samples(num_samples),
  m_loop(loop),
  m_in_memory(in_memory),
  m_type(type),
  m_data_samples(num_samples, type) {
    CHECK_GT(m_num_samples, 0);
    CHECK_NE(m_filename.empty(), true);
    m_file_ptr = OpenFileOrDie(m_filename.c_str(), "r");
    // If we have ennough memory, 
    // we can read all data into m_data_buf.
    if (m_in_memory) {
      // get the size of current file.
      fseek(m_file_ptr, 0, SEEK_END);
      uint64 total_size = ftell(m_file_ptr);
      rewind(m_file_ptr);
      char* buffer = NULL;
      try {
        buffer = new char[total_size];
      } catch (std::bad_alloc&) {
        LOG(FATAL) << "Cannot allocate enough memory for Reader.";
      }
      // read all data
      uint64 read_size = fread(buffer, 1, total_size, m_file_ptr);
      if (read_size != total_size) {
        LOG(FATAL) << "Read file error: " << m_filename;
      }
      // get the number of line.
      uint32 num_line = 0;
      for (uint64 i = 0; i < total_size; ++i) {
        if (buffer[i] == '\n') {
          num_line++;
        }
      }
      m_data_buf.resize(num_line, m_type);
      m_data_buf.InitSparseRow();
      // parse line to StringList.
      StringList list(num_line);
      char* line = new char[kMaxLineSize];
      uint32 start_pos = 0;
      for (uint32 i = 0; i < num_line; ++i) {
        uint32 read_size = ReadLineFromMemory(line, buffer,
                                              start_pos,
                                              total_size);
        if (read_size == 0) {
          LOG(FATAL) << "Read error: " << read_size << " bytes.";
        }
        start_pos += read_size;
        line[read_size-1] = '\0';
        // Handle some windows txt format.
        if (read_size > 1 && line[read_size-2] == '\r') {
          line[read_size-2] = '\0';
        }
        list[i].assign(line);
      }
      // parse StringList to DMatrix
      m_parser.Parse(list, m_data_buf);
    } else { // Sample data from disk file.
      m_data_samples.InitSparseRow();
    }
}

Reader::~Reader() {
  if (m_file_ptr != NULL) {
    Close(m_file_ptr);
    m_file_ptr = NULL;
  }
}

DMatrix* Reader::Samples() {
  return m_in_memory ? SampleFromMemory() :
                       SampleFromDisk();
}

DMatrix* Reader::SampleFromDisk() {
  static char* line = new char[kMaxLineSize];
  static StringList list(m_num_samples);
  // Sample m_num_samples lines data from disk
  uint32 num_line = 0;
  for (uint32 i = 0; i < m_num_samples; ++i) {
    if (fgets(line, kMaxLineSize, m_file_ptr) == NULL) {
      // Either ferror or feof. 
      if (m_loop) {
        fseek(m_file_ptr, 0, SEEK_SET);
        i--; // re-read
        continue;
      } else break;
    }
    uint32 read_len = strlen(line);
    if (line[read_len-1] != '\n') {
      LOG(FATAL) << "Encountered a too-long line.";
    } else {
      line[read_len-1] = '\0';
      // Handle some windows txt format.
      if (read_len > 1 && line[read_len-2] == '\r') {
        line[read_len-2] = '\0';
      }
    }
    list[i].assign(line);
    num_line++;
  }
  // End of file
  if (num_line != m_num_samples) {
    m_data_samples.resize(num_line, m_type);
  }
  m_parser.Parse(list, m_data_samples);
  return &m_data_samples;
}

DMatrix* Reader::SampleFromMemory() {
  static index_t pos = 0;
  uint32 num_line = 0;
  for (index_t i = 0; i < m_num_samples; ++i) {
    // End of file
    if (pos >= m_data_buf.row_size) {
      if (m_loop) {
        pos = 0;
      } else {
        break;
      }
    }
    // Copy data from buffer to data samples
    m_data_samples.row[i] = m_data_buf.row[pos];
    m_data_samples.Y[i] = m_data_buf.Y[pos];
    pos++;
    num_line++;
  }
  // End of file
  if (num_line != m_num_samples) {
    m_data_samples.resize(num_line, m_type);
  }
  return &m_data_samples;
}

uint32 ReadLineFromMemory(char* line, char* buf, 
                          uint32 start_pos, uint32 len) {
  // End of the file
  if (start_pos >= len) return 0;
  uint32 end_pos = start_pos;
  while (*(buf+end_pos) != '\n') {++end_pos;}
  uint32 read_size = end_pos - start_pos + 1;
  if (read_size > kMaxLineSize) {
    LOG(FATAL) << "Encountered a too-long line.";
  }
  memcpy(line, buf+start_pos, read_size);
  return read_size;
}

} // namespace f2m
