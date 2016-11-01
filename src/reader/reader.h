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

This files defines Reader class, which returns N data samples 
in each iteration.
*/

#ifndef F2M_READER_READER_H_
#define F2M_READER_READER_H_

#include <string>
#include <vector>

#include "src/base/common.h"
#include "src/data/data_structure.h"
#include "src/reader/parser.h"

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
 *                 model_type = LR);                                            *
 *                                                                              *
 *   Loop until converge {                                                      *
 *                                                                              *
 *      Data = reader.Samples(); // return N data samples from disk file        *
 *                                                                              *
 *      ... use the data samples to train model                                 *
 *                                                                              *
 *   }                                                                          *
 *                                                                              *
 * The reader will return to the beginning of the file automatically when       *
 * it reach the end of file. For some tasks such as testing, we just need       *
 * to scan the data once (break to read when reach the end of tje file).        *
 * For this task, we can use Reader like this:                                  *
 *                                                                              *
 *   #include "reader.h"                                                        *
 *                                                                              *
 *   // Constructor                                                             *
 *   Reader reader(filename = "/tmp/testdata",                                  *
 *                 num_samples = 100,                                           *
 *                 model_type = LR,                                             *
 *                 loop = false); // loop = true by default.                    *
 *                                                                              *
 *   Loop until end of file {                                                   *
 *                                                                              *
 *      Data = reader.Samples(); // return N data samples from disk file        *
 *                                                                              *
 *      ... use the data                                                        *
 *                                                                              *
 *   }                                                                          * 
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
 *                 model_type = LR,                                             *
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

class Reader {
 public:
  Reader(const string& filename,
         int num_samples,
         ModelType type = LR,
         bool loop = true, // Continue to sample data in a loop.
         bool in_memory = false); // Reader samples data from disk file 
                                  // by default.
  ~Reader();

  // Return a pointer to the DMatrix.
  DMatrix* Samples();

 private:
  string m_filename;                // indentify the input file.
  int m_num_samples;                // the number of data samples in each sampling.
  bool m_loop;                      // sample data in a loop.
  bool m_in_memory;                 // load all data into memory.
  FILE* m_file_ptr;                 // maintain current file pointer.
  ModelType m_type;                 // enum ModelType { LR, FM, FFM }

  DMatrix m_data_buf;               // bufferring all parsed data in memory.
  DMatrix m_data_samples;           // data samples
  Parser m_parser;                  // Parse StringList to the DMatrix format.

  DMatrix* SampleFromDisk();
  DMatrix* SampleFromMemory();

  DISALLOW_COPY_AND_ASSIGN(Reader);
};

} // namespace f2m

#endif // F2M_READER_READER_H_
