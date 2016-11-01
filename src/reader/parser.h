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

This files defines Parser class, which parse the Reader's 
output to a DMatrix format.
*/

#ifndef F2M_READER_PARSER_H_
#define F2M_READER_PARSER_H_

#include <stdlib.h>

#include <vector>
#include <string>

#include "src/base/common.h"
#include "src/base/split_string.h"
#include "src/data/data_structure.h"

using std::vector;
using std::string;

namespace f2m {

typedef vector<string> StringList;

// Given a StringList, parse it to the DMatrix format.
class Parser {
 public:
  virtual void Parse(const StringList& list, 
                     DMatrix& matrix) {
    CHECK_GE(list.size(), 0);
    CHECK_GE(matrix.row_size, 0);
    // Note that here we need to use the matrix.row_size 
    // as the bound because sometimes this size could be smaller
    // than the size of StringList.
    for (index_t i = 0; i < matrix.row_size; ++i) {
      // Parse the following format:
      // [ Y idx:value \table idx:value ... ] for LR and FM, or
      // [ Y field:idx:value \table field:idx:value ... ] for FFM.
      StringList items;
      SplitStringUsing(list[i], "\t", &items);
      int len = items.size();
      // parse Y
      matrix.Y[i] = atof(items[0].c_str());
      // parse row
      CHECK_NOTNULL(matrix.row[i]);
      matrix.row[i]->resize(len-1);
      for (int j = 1; j < len; ++j) {
        StringList tmp_item; 
        SplitStringUsing(items[j], ":", &tmp_item);
        if (matrix.model_type == FFM) {
          CHECK_EQ(tmp_item.size(), 3);
          matrix.row[i]->field[j-1] = atoi(tmp_item[0].c_str());
          matrix.row[i]->idx[j-1] = atoi(tmp_item[1].c_str());
          matrix.row[i]->X[j-1] = atof(tmp_item[2].c_str());
        } else { // LR or FM
          CHECK_EQ(tmp_item.size(), 2);
          matrix.row[i]->idx[j-1] = atoi(tmp_item[0].c_str());
          matrix.row[i]->X[j-1] = atof(tmp_item[1].c_str());
        }
      }
    }
  }
};

} // namespace f2m

#endif // F2M_READER_PARSER_H_
