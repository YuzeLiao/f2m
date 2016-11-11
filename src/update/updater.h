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

This files defines updateer class, which is responsible for updating
current model parameters.
*/

#ifndef F2M_UPDATE_UPDATER_H_
#define F2M_UPDATE_UPDATER_H_

#include "src/base/common.h"
#include "src/data/model_parameters.h"
#include "src/data/data_structure.h"

namespace f2m {
  
// Updater function is responsible for updating current 
// model parameters. Updtaer is an abstract class that can be 
// implemented by different update methods such as SGD, 
// Momentum, Nesterov Momentum, AdaGard, RMSprop, and Adam.
class Updater {
 public:
  Updater(Model* model, 
          real_t learning_rate, 
          real_t regu_lamda,
          RegularType regu_type = L2)
    : m_model(model), 
      m_learning_rate(learning_rate),
      m_regu_lamda(regu_lamda),
      m_regu_type(regu_type) {}

  virtual ~Updater() {}
    
  // Using simple SGD by default.
  virtual void Update(const SparseGrad& grad);
  
 protected:
  Model* m_model;               // point to current model parameters.
  real_t m_learning_rate;       // control the step size.
  real_t m_regu_lamda;          // control the regularzation.
  RegularType m_regu_type;      // enum RegularType { L1, L2, NONE }

  DISALLOW_COPY_AND_ASSIGN(Updater);
};

} // namespace f2m

#endif // F2M_UPDATE_UPDATER_H_
