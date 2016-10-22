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

This files defines Reader class, which returns N data samples (string) 
in each iteration.
*/

#ifndef F2M_READER_READER_H_
#define F2M_READER_READER_H_

#include <string>
#include <vector>

#include "src/base/common.h"

using std::vector;
using std::string;

namespace f2m {

/* -----------------------------------------------------------------------------
 * We can use Reader class like this (Pseudocode):                              *
 *                                                                              *
 *   #include "reader.h"                                                        *
 *                                                                              *
 *   // Constructor                                                             *
 *   Reader reader(filename = "/tmp/testdata",                                  *
 *                 num_samples = 100,                                           *
 *                 loop = true);                                                *
 *                                                                              *
 *   Loop until converge {                                                      *
 *                                                                              *
 *      Data = reader.Samples(); // return N data samples from disk file        *
 *                                                                              *
 *      ... use the data samples to train model                                 *
 *                                                                              *
 *   }                                                                          *
 *                                                                              *
 * If we set 'loop = true', the reader will return to the beginning of the      *
 * file automatically when it reach the end of file.                            *
 *                                                                              *
 * If we set 'loop = false', the reader will finish its job when it reach       *
 * the end of file.                                                             *
 *                                                                              *
 * We can set the number of N in construct funtion of Reader:                   *
 *                                                                              *
 *   N = 1 indicates that we use SGD.                                           *
 *   N = total_samples indicates that we use Bactch GD.                         *
 *   others indicate that we use mini-batch GD.                                 *
 *                                                                              *
 * Before sampling, we can load all data into memory if the capacity            *
 * of your main memory is big enough for current training task:                 *
 *                                                                              *
 *   #include "reader.h"                                                        *
 *                                                                              *
 *   // Constructor                                                             *
 *   Reader reader(filename = "/tmp/testdata",                                  *
 *                 num_samples = 100,                                           *
 *                 loop = true,                                                 *
 *                 in_memory = true); // read from memory                       *
 *                                                                              *
 *   Loop until converge {                                                      *
 *                                                                              *
 *      Data = reader.Samples(); // return N data samples from memory           *
 *                                                                              *
 *      ... use the data samples to train model                                 *
 *                                                                              *
 *   }                                                                          *
 *                                                                              *
 * Reader is an algorithm-agnostic class and can mask the details of            *
 * the data source (on disk or in memory), and it is flexible for               *
 * different gradient descent methods (e.g., SGD, mini-batch GD, and            *
 * batch GD)                                                                    *
 * -----------------------------------------------------------------------------
 */

typedef vector<string> StringList;

class Reader {
 public:
  Reader(const string& filename,
         uint32 num_samples,
         bool loop = false,        
         bool in_memory = false); /* Reader samples data from disk file 
                                     by default. */
  ~Reader();

  // Return a pointer to N lines of data samples.
  StringList* Samples();

 private:
  string m_filename;                // Indentify the input file.
  uint32 m_num_samples;             // How many data samples we need.
  bool m_loop;                      // if in a loop model.
  bool m_in_memory;                 // load all data into memory.

  FILE* m_file_ptr;                 // maintain current file pointer.
  char* m_memory_buffer;            // in-memory buffer
  uint32 m_buf_size;                // buffer size

  StringList m_data_samples;        // current data samples.

  StringList* SampleFromDisk();
  StringList* SampleFromMemory();
  uint32 ReadLineFromMemory(char* line, char* buf, uint32 len);

  DISALLOW_COPY_AND_ASSIGN(Reader);
};

} // namespace f2m

#endif // F2M_READER_READER_H_
