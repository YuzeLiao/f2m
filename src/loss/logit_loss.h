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

This file defines the logistic regression loss.
*/

#ifndef F2M_LOSS_LOGIT_LOSS_H_
#define F2M_LOSS_LOGIT_LOSS_H_

#include <vector>

#include "src/base/common.h"
#include "src/data/data_structure.h"
#include "src/data/model_parameters.h"
#include "src/loss/loss.h"

using std::vector;

namespace f2m {

// Logistic regression Loss, Math:
//  [ loss(x, y, w) = log(1 + exp(-y * <w, x>)) ]
class LogitLoss : public Loss {
 public:
  LogitLoss() {}
  ~LogitLoss() {}

  // Given the input DMatrix and current model, return
  // the prediction results. Math:
  //  [ pred = <x, w> ]
  void Predict(const DMatrix* matrix,
               Model& param,
               vector<real_t>& pred) {
    CHECK_NOTNULL(matrix);
    CHECK_GT(pred.size(), 0);
    CHECK_EQ(pred.size(), matrix->row_size);
    vector<real_t>* weight = param.GetParameter();
    // each line of test examples
    for (index_t i = 0; i < matrix->row_size; ++i) {
      SparseRow* row = matrix->row[i];
      pred[i] = wTx(row, weight);
    }
  }

  // Given the prediction results and the current model, return
  // the calculated gradient. Math:
  //  [ (-y / ( (1/exp(-y*<w,x>))  + 1 )) * X ]
  void CalcGrad(const DMatrix* matrix,
                Model& param,
                SparseGrad& grad) {
    CHECK_NOTNULL(matrix);
    CHECK_GT(matrix->row_size, 0);
    vector<real_t>* weight = param.GetParameter();
    index_t num  = 0;
    // each line of trainning examples
    for (index_t i = 0; i < matrix->row_size; ++i) {
      // partial gradient 
      SparseRow* row = matrix->row[i];
      real_t y = matrix->Y[i];
      real_t partial_grad = -y / ((1.0 / exp(-y*wTx(row, weight))) + 1);
      // for every entry
      for (index_t j = 0; j < row->size; ++j) {
        index_t pos = row->idx[j];
        real_t w_j = partial_grad * row->X[j];
        if (num >= grad.w.size()) {
          // resize
          grad.w.resize(grad.w.size()*2);
        } 
        grad.w[num] = w_j;
        grad.pos_w[num] = pos;
        num++;
      }
    }
    grad.size_w = num;
  }

 private:
  // Calculate <w,x>
  inline real_t wTx(const SparseRow* row, const vector<real_t>* w) {
    real_t val = 0.0;
    for (index_t j = 0; j < row->size; ++j) {
      index_t pos = row->idx[j];
      val += (*w)[pos] * row->X[j];
    }
    return val;
  }

  DISALLOW_COPY_AND_ASSIGN(LogitLoss);
};

} // namespace f2m

#endif // F2M_LOSS_LOGIT_LOSS_H_
