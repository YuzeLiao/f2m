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
 
 This file defines AdaGrad_updater, which implements "updater"
 with AdaGradient.
 */

#ifndef F2M_UPDATE_ADAGRAD_UPDATER_H
#define F2M_UPDATE_ADAGRAD_UPDATER_H

#include "src/update/updater.h"

#include "src/base/common.h"
#include "src/data/data_structure.h"

namespace f2m {
class AdaGrad_updater : private Updater {
 public:
   AdaGrad_updater(Model* model,
                   real_t learning_rate,
                   real_t regu_lamda,
                   real_t ada_eta,
                   real_t ada_epsilon = 1,
                   RegularType regu_type = L2);
   ~AdaGrad_updater() {};
   // this function implements Adagrad-updating
   void Update(const SparseGrad& grad);
   
 private:
   vector<real_t> m_g_parameters;      // store the sum of squares of gradients
   real_t m_ada_eta;                   // learning rate in AdaGrad
   real_t m_ada_epsilon;               // initial value of m_g_parameters

   DISALLOW_COPY_AND_ASSIGN(AdaGrad_updater);
};
   
   
   
}




#endif /* F2M_UPDATE_ADAGRAD_UPDATER_H */