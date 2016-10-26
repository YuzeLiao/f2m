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

This file tests parser.h
*/

#include "gtest/gtest.h"

#include <string>
#include <vector>

#include "src/reader/parser.h"
#include "src/data/data_structure.h"

using std::vector;
using std::string;

namespace f2m {
    
const string kStr = "0:0.123\t1:0.123\t2:0.123\t3:0.123\t4:0.123\t0";
const string kStrFFM = "0:0.123:0\t1:0.123:1\t2:0.123:2\t3:0.123:3\t4:0.123:4\t1";
const index_t kNum_lines = 100000;
const index_t kLen = 5;

TEST(PARSER_TEST, Parse_LR_FM) {
  StringList list(kNum_lines);
  for (index_t i = 0; i < kNum_lines; ++i) {
    list[i] = kStr;
  }
  DMatrix matrix(kNum_lines, LR);
  matrix.InitSparseRow();
  Parser parser;
  parser.Parse(list, matrix);
  EXPECT_EQ(matrix.row_size, kNum_lines);
  EXPECT_EQ(matrix.row.size(), kNum_lines);
  EXPECT_EQ(matrix.Y.size(), kNum_lines);
  for (index_t i = 0; i < kNum_lines; ++i) {
    EXPECT_EQ(matrix.row[i]->X.size(), kLen);
    EXPECT_EQ(matrix.row[i]->idx.size(), kLen);
    EXPECT_EQ(matrix.Y[i], (real_t(0)));
    for (index_t j = 0; j < kLen; ++j) {
      EXPECT_EQ(matrix.row[i]->X[j], (real_t)(0.123));
      EXPECT_EQ(matrix.row[i]->idx[j], j);
    }
  }
}

TEST(PARSER_TEST, Parse_FFM) {
  StringList list(kNum_lines);
  for (index_t i = 0; i < kNum_lines; ++i) {
    list[i] = kStrFFM;
  }
  DMatrix matrix(kNum_lines, FFM);
  matrix.InitSparseRow();
  Parser parser;
  parser.Parse(list, matrix);
  EXPECT_EQ(matrix.row_size, kNum_lines);
  EXPECT_EQ(matrix.row.size(), kNum_lines);
  EXPECT_EQ(matrix.Y.size(), kNum_lines);
  for (index_t i = 0; i < kNum_lines; ++i) {
    EXPECT_EQ(matrix.row[i]->X.size(), kLen);
    EXPECT_EQ(matrix.row[i]->idx.size(), kLen);
    EXPECT_EQ(matrix.row[i]->field.size(), kLen);
    EXPECT_EQ(matrix.Y[i], (real_t)(1));
    for (index_t j = 0; j < kLen; ++j) {
      EXPECT_EQ(matrix.row[i]->X[j], (real_t(0.123)));
      EXPECT_EQ(matrix.row[i]->idx[j], j);
      EXPECT_EQ(matrix.row[i]->field[j], j);
    }
  }
}

} // namespace f2m
