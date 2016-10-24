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
#include "src/base/logging.h"
#include "src/base/file_util.h"
#include "src/reader/parser.h"

using std::vector;
using std::string;

namespace f2m {

const uint32 kMaxLineSize = 100 * 1024; // 100 KB one line

typedef vector<string> StringList;

Reader::Reader(const string& filename,
               uint32 num_samples,
               bool in_memory) :
  m_filename(filename),
  m_num_samples(num_samples),
  m_in_memory(in_memory) {
    CHECK_GT(m_num_samples, 0);
    CHECK_NE(filename.empty(), true);
    m_file_ptr = OpenFileOrDie(m_filename.c_str(), "r");
    m_data_samples = new DMatrix(m_num_samples);
}

void Reader:Init() {
  // If the input data is small, 
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
    // get the number of line.
    uint32 num_line = 0;
    for (uint64 i = 0; i < total_size; ++i) {
      if (buffer[i] == '\n') {
        num_line++;
      }
    }
    // parse line to m_data_buf.
    StringList list(num_line);
    char* line = new char[kMaxLineSize];
    for (uint32 i = 0; i < num_line; ++i) {
      uint32 read_size = ReadLineFromMemory(line, buffer, 
                                            total_size);
      line[read_size-1] = '\0';
      // Handle some windows txt format.
      if (read_size > 1 && line[read_size-2] == '\r') {
        line[read_size-2] = '\0';
      }
      list[i].assign(line);
    }
    // parse StringList to DMatrix
    Parser.parse(&list, m_data_buf);
  } else { // Sample data from disk file.
    m_data_buf = NULL;
  }
}

Reader::~Reader() {
  if (m_file_ptr != NULL) {
    Close(m_file_ptr);
    m_file_ptr = NULL;
  }

  if (m_data_buf != NULL) {
    delete m_data_buf;
  }

  if (m_data_samples != NULL) {
    delete m_data_samples;
  }
}

DMatrix* Reader::Samples() {
  return m_in_memory ? SampleFromMemory() :
                       SampleFromDisk();
}

DMatrix* Reader::SampleFromDisk() {
  static char* line = new char[kMaxLineSize];
  // Sample m_num_samples lines data from disk
  for (uint32 i = 0; i < m_num_samples; ++i) {
    if (fgets(line, kMaxLineSize, m_file_ptr) == NULL) {
      // Either ferror or feof. 
      break;
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
    m_data_samples.row[i].assign(line);
    m_data_samples.num_samples++;
  }
  return &m_data_samples;
}

DMatrix* Reader::SampleFromMemory() {
  
}

uint32 Reader::ReadLineFromMemory(char* line, char* buf, uint32 len) {
  static uint32 start_pos = 0;
  static uint32 end_pos = 0;
  // End of the file
  if (end_pos >= buf_len) {
    if (m_loop) { // return to the begining of the file.
      start_pos = 0;
      end_pos = 0;
    } else {
      return 0;
    }
  }
  // read one line
  while (*(buf+end_pos) != '\n') {++end_pos;}
  uint32 read_size = end_pos - start_pos + 1;
  if (read_size > kMaxLineSize) {
    LOG(FATAL) << "Encountered a too-long line.";
  }
  memcpy(line, buf+start_pos, read_size);
  start_pos = ++end_pos;
  return read_size;
}

} // namespace f2m
