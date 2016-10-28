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

using std::vector;

namespace f2m {

// Logistic regression Loss, Math:
//  [ loss(x, y, w) = log(1 + exp(-y * <w, x>)) ]
class LogitLoss : public Loss {
 public:
  ~LogitLoss() {}

  // Given the input DMatrix and current model, return
  // the prediction results. Math:
  //  [ pred = <x, w> ]
  void Predict(const DMatrix* matrix,
               const Madel& param,
               vector<real_t>& pred) {
    
  }

  // Given the prediction results and the current model, return
  // the calculated gradient. Math:
  //  [ (-y / ( (1/exp(-y*<w,x>))  + 1 )) * xi ]
  void CalcGrad(const DMatrix& matrix,
                const Model& param,
                SparseGrad& grad) {

  }
};

} // namespace f2m

#endif // F2M_LOSS_LOGIT_LOSS_H_
