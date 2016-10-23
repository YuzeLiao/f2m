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
Copyright (c) 2016 by contributors.
Author: Chao Ma (mctt90@gmail.com)

This files defines Parser class, which parse the Reader's 
output to a DMatrix format.
*/

#ifndef F2M_READER_PARSER_H_
#define F2M_READER_PARSER_H_

#include <vector>
#include <string>

#include "src/base/common.h"
#include "src/base/logging.h"
#include "src/data/data_structure.h"

using std::vector;
using std::string;

namespace f2m {
    
typedef vector<string> StringList;

// Given a StringList, parse it to a DMatrix.
class Parser {
 public:
  static void Parse(const StringList* list, DMatrix* matrix) {
    CHECK_NOTNULL(list);
    CHECK_NOTNULL(matrix);
    CHECK_GT(list->size());
    CHECK_GT()
  }
};

} // namespace f2m

#endif // F2M_READER_PARSER_H_
