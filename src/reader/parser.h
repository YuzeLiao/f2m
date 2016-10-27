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

#include <stdlib.h>

#include <vector>
#include <string>

#include "src/base/common.h"
#include "src/base/logging.h"
#include "src/base/split_string.h"
#include "src/data/data_structure.h"

using std::vector;
using std::string;

namespace f2m {

typedef vector<string> StringList;

// Given a StringList, parse it to a DMatrix format.
class Parser {
 public:
  virtual void Parse(const StringList& list, 
                     DMatrix& matrix) {
    CHECK_GE(list.size(), 0);
    CHECK_GE(matrix.row_size, 0);
    for (index_t i = 0; i < matrix.row_size; ++i) {
      // parse the following format:
      // [0:12.234 3:0.123 6:0.21 7:1234] for LR and FM, or
      // [0:12.234:0 3:0.123:1 6:0.21:2 7:1234:3] for FFM
      StringList items;
      SplitStringUsing(list[i], "\t", &items);
      index_t len = items.size();
      matrix.Y[i] = atof(items[0].c_str());
      CHECK_NOTNULL(matrix.row[i]);
      matrix.row[i]->resize(len-1, matrix.model_type);
      for (index_t j = 1; j < len; ++j) {
        StringList tmp_item; 
        SplitStringUsing(items[j], ":", &tmp_item);
        if (matrix.model_type == FFM) {
          CHECK_EQ(tmp_item.size(), 3);
        } else {
          CHECK_EQ(tmp_item.size(), 2);
        }
        if (matrix.model_type == FFM) {
          matrix.row[i]->field[j-1] = atoi(tmp_item[0].c_str());
          matrix.row[i]->idx[j-1] = atoi(tmp_item[1].c_str());
          matrix.row[i]->X[j-1] = atof(tmp_item[2].c_str());
        } else {
          matrix.row[i]->idx[j-1] = atoi(tmp_item[0].c_str());
          matrix.row[i]->X[j-1] = atof(tmp_item[1].c_str());
        }
      }
    }
  }
};

} // namespace f2m

#endif // F2M_READER_PARSER_H_
