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

This files implements the SGD updater.
*/

#include "src/update/update.h"

namespace f2m {

void SGDUpdater::UpdateLR(const SparseGrad& grad) {
  // weight
  vector<real_t>* param = m_model->GetModelParameter();
  // loss gradient
  for (index_t i = 0; i < grad.size_w; ++i) {
    index_t pos = grad.pos_w[i];
    param[pos] -= m_learning_rate * grad.w[i];
  }
  // regular gradient
  if (m_regu_type == L1) {
    for (index_t i = 0; i < param->size(); ++i) {
      param[i] -= m_learning_rate * m_regu_lamda * param[i];
    }
  } else if (m_regu_type == L2) {

  }
}

void SGDUpdater::UpdateFM(const SparseGrad& grad) {
  // weight
  vector<real_t>* param = m_model->GetModelParameter();
  // loss gradient
}

void SGDUpdater::UpdateFFM(const SparseGrad& grad) {
  // weight
  vector<real_t>* param = m_model->GetModelParameter();
}

} // namespace f2m
