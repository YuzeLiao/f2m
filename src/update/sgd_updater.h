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

This files defines the SGD updater.
*/

#ifndef F2M_UPDATE_SGD_UPDATER_H_
#define F2M_UPDATE_SGD_UPDATER_H_

#include "src/base/common.h"
#include "src/data/model_parameters.h"
#include "src/data/data_structure.h"
#include "src/update/updater.h"

namespace f2m {

// Math: [ w -= learning_rate * gradient ]
//       [ gradient = loss_grad + lamda * regu_grad ]
class SGDUpdater : public Updater {
 public:
  SGDUpdater(Model* model, 
             real_t learning_rate, 
             real_t regu_lamda,
             ModelType model_type = LR,
             RegularType regu_type = L2)
    : Updater(model, 
              learning_rate, 
              regu_lamda, 
              model_type, 
              regu_type) {}

  ~SGDUpdater() {}

 private:
  void UpdateLR(const SparseGrad& grad);
  void UpdateFM(const SparseGrad& grad);
  void UpdateFFM(const SparseGrad& grad);

  DISALLOW_COPY_AND_ASSIGN(Updater);
};   

} // namespace f2m

#endif // F2M_UPDATE_SGD_UPDATER_H_
