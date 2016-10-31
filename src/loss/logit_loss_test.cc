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

This file tests logit_loss.h
*/

#include "gtest/gtest.h"

#include <string>
#include <vector>

#include "src/loss/logit_loss.h"
#include "src/data/data_structure.h"
#include "src/data/model_parameters.h"

namespace f2m {

TEST(LogitLoss, Evaluate) {
  // all of the answer all right
  vector<real_t> pred(100, 200);
  vector<real_t> label(100, 1.0);
  LogitLoss loss;
  real_t objv = loss.Evaluate(pred, label);
  EXPECT_EQ(objv, 0.0);
}

TEST(LogitLoss, Predict) {
  // DMatrix
  DMatrix matrix;
  matrix.resize(100, LR);
  matrix.InitSparseRow();
  for (int i = 0; i < matrix.row_size; ++i) {
    SparseRow* row = matrix.row[i];
    row->resize(100, LR);
    for (int j = 0; j < 100; ++j) {
      row->X[j] = 2.0;
      row->idx[j] = j;
    }
  }  
  // Model
  Model model(99); // 99 linear terms + 1 bias iterm
  // Loss
  LogitLoss loss;
  vector<real_t> pred(100);
  loss.Predict(&matrix, model, pred);
  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(pred[i], 200.0);
  }
}

TEST(LogitLoss, CalcGrad) {
        
}

} // namespace f2m
