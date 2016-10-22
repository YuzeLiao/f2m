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

using std::vector;
using std::string;

namespace f2m {

const uint32 kMaxLineSize = 100 * 1024; // 100 KB one line

Reader::Reader(const string& filename,
               uint32 num_samples,
               bool loop,
               bool in_memory) :
  m_filename(filename),
  m_num_samples(num_samples),
  m_loop(loop),
  m_in_memory(in_memory) {
    CHECK_GT(m_num_samples, 0);
    CHECK_NE(filename.empty(), true);

    m_file_ptr = OpenFileOrDie(m_filename.c_str(), "r");

    // allocate memory for memory buffer
    if (m_in_memory) {
      // get the size of current file.
      fseek(m_file_ptr, 0, SEEK_END);
      m_buf_size = ftell(m_file_ptr);
      rewind(m_file_ptr);
      try {
        m_memory_buffer = new char[m_buf_size];
      } catch (std::bad_alloc&) {
        LOG(FATAL) << "Cannot allocate enough memory for Reader.";
      }
      // read all the data from disk to buffer.
      uint32 read_len = fread(m_memory_buffer, 1, 
                              m_buf_size, m_file_ptr);
      CHECK_EQ(read_len, m_buf_size);
    } else {
      m_memory_buffer = NULL;
    }
}

Reader::~Reader() {
  if (m_file_ptr != NULL) {
    Close(m_file_ptr);
    m_file_ptr = NULL;
  }

  if (m_memory_buffer != NULL) {
    delete m_memory_buffer;
  }

  if (m_data_samples != NULL) {
    delete m_data_samples;
  }
}

StringList* Reader::Samples() {
  m_data_samples.clear();
  return m_in_memory ? SampleFromMemory() :
                       SampleFromDisk();
}

StringList* Reader::SampleFromDisk() {
  static char* line = new char[kMaxLineSize];
  // Sample m_num_samples lines data from disk
  for (uint32 i = 0; i < m_num_samples; ++i) {
    if (fgets(line, kMaxLineSize, m_file_ptr) == NULL) {
      // Either ferror or feof. 
      if (m_loop) { // return to the begining of the file.
        fseek(m_file_ptr, 0, SEEK_SET);
        i--;
        continue;
      } else {
        break;
      }
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
    (*m_data_samples)[i].assign(line);
  }
  return m_data_samples;
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

StringList* Reader::SampleFromMemory() {
  static char* line = new char[kMaxLineSize];
  // sample m_num_samples lines of data from memory.
  for (uint32 i = 0; i < m_num_samples; ++i) {
    uint32 read_size = ReadLineFromMemory(line, m_memory_buffer,
                                          m_buf_size);
    if (read_size == 0) { // end of the file
      break;
    }
    line[read_size-1] = '\0';
    // Handle some windows txt format.
    if (read_size > 1 && line[read_size-2] == '\r') {
      line[read_size-2] = '\0';
    }
    (*m_data_samples)[i].assign(line);
  }
  return m_data_samples;
}

} // namespace f2m
