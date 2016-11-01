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

This file defines the base class Loss, which is the 
core part of ML algorithms.
*/

#ifndef F2M_LOSS_LOSS_H_
#define F2M_LOSS_LOSS_H_

#include <vector>
#include <cmath> // for log() and exp()

#include "src/base/common.h"
#include "src/data/data_structure.h"
#include "src/data/model_parameters.h"

using std::vector;

namespace f2m {

// The basic class of loss function.
// Loss is an abstract class, which can be implemented by real
// loss functions such as logistic regression loss (logit_loss.h),
// FM loss (fm_loss.h), and FFM loss (ffm_loss.h)
class Loss {
 public:
  Loss() {}
  virtual ~Loss() {}

  // Given the input DMatrix and current model, return 
  // the prediction results. 
  virtual void Predict(const DMatrix* matrix,
                       Model& param,
                       vector<real_t>& pred) = 0;

  // Given the input DMatrix and current model, return
  // the calculated gradient.
  virtual void CalcGrad(const DMatrix* matrix,
                        Model& param,
                        SparseGrad& grad) = 0;

  // Given the prediction results and the groudtruth, return 
  // current loss value. Here we use the cross-enropy loss by default.
  // Note that the cross-enropy loss takes 1 and -1 for positive and
  // negative examples, respectivly.
  virtual real_t Evaluate(const vector<real_t>& pred,
                          const vector<real_t>& label) const {
    real_t objv = 0.0;
    for (index_t i = 0; i < pred.size(); ++i) {
      real_t y = label[i] > 0 ? 1 : -1;
      objv += log(1 + exp(-y*pred[i]));
    }
    objv /= pred.size();
    return objv;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(Loss);
};

} // namespace f2m

#endif // F2M_LOSS_LOSS_H_
