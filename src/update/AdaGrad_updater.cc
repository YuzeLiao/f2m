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
 
 This file implements AdaGrad_updater.h.
 */

#include "src/update/AdaGrad_updater.h"

#include "src/base/common.h"
#include "src/data/data_structure.h"
#include "cmath"                      // for sqrt()

namespace f2m {
AdaGrad_updater::AdaGrad_updater(Model* model,
                                 real_t learning_rate,
                                 real_t regu_lamda,
                                 real_t ada_eta,
                                 real_t ada_epsilon,
                                 RegularType regu_type)
   : Updater(model, learning_rate, regu_lamda, regu_type) {
      CHECK_GE(ada_epsilon, 0);
      m_ada_epsilon = ada_epsilon;
      // ada_eta equals to learning rate by default
      if (ada_eta <= 0) {
         m_ada_eta = learning_rate;
      }
      else {
         m_ada_eta = ada_eta;
      }
      
      try {
         m_g_parameters.resize(model->GetNumberOfParameters(), m_ada_epsilon);
      }
      catch (std::bad_alloc&) {
         LOG(FATAL) << "Cannot allocate enough memory for \
                        AdaGrad updater.";
      }
   }
   
void AdaGrad_updater::Update(const SparseGrad& grad) {
   vector<real_t>* param = m_model->GetParameter();
   CHECK_NOTNULL(param);
   ModelType type = m_model->GetModelType();
   if (type == LR || type == FM || type == FFM) {
      index_t end_linear = grad.size_w;
      for (index_t i = 0; i < end_linear; i++) {
         index_t pos = grad.pos_w[i];
         m_g_parameters[pos] += grad.w[i] * grad.w[i];
         (*param)[pos] -= m_ada_eta * grad.w[pos] / sqrt(m_g_parameters[pos]);
      }
      if (type != LR) {
         index_t end_V = grad.size_v;
         for (index_t i = 0; i < end_V; i++) {
            index_t pos = grad.pos_v[i];
            m_g_parameters[pos] += grad.w[i] * grad.w[i];
            (*param)[pos] -= m_ada_eta * grad.w[pos] / sqrt(m_g_parameters[pos]);
         }
      }
      
   }
   else {
      LOG(FATAL) << "Unknown model type: " << type;
   }
}

} // namespace f2m