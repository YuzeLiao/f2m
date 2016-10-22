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

Open and Close the file.
*/

#ifndef F2M_BASE_FILE_UTIL_H_
#define F2M_BASE_FILE_UTIL_H_

#include <string>

#include "src/base/common.h"
#include "src/base/logging.h"

using std::string;

// Open file using C API.
inline FILE* OpenFileOrDie(const char* filename, const char* mode) {
  FILE* input_stream = fopen(filename, mode);
  if (input_stream == NULL) {
    LOG(FATAL) << "Cannot open file: " << filename << " with mode: " << mode;
  }
  return input_stream;
}

// Close file using C API.
inline void Close(FILE *file) {
  if(fclose(file) == -1) {
    LOG(FATAL) << "Cannot close the file.";
  }
}

#endif // F2M_BASE_FILE_UTIL_H_
