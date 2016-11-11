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
 
 This file implements SGD_updater.h.
 */

#include "src/update/SGD_updater.h"

#include "src/base/common.h"
#include "src/data/data_structure.h"

namespace f2m {
void SGD_updater::Update(const SparseGrad& grad)
{
   vector<real_t>* param = m_model->GetParameter();
   CHECK_NOTNULL(param);
   ModelType type = m_model->GetModelType();
   if (type == LR)
   {
      int end = grad.size_w;
      for (int i = 0; i < end; i++)
      {
         int pos = grad.pos_w[i];
         (*param)[pos] += grad.w[i];
      }
   }
   else
      if (type == FM || type ==  FFM)
      {
         int end_linear = grad.size_w;
         for (int i = 0; i < end_linear; i++)
         {
            int pos = grad.pos_w[i];
            (*param)[pos] += grad.w[i];
         }
         int end_V = grad.size_v;
         for (int i = 0; i < end_V; i++)
         {
            int pos = grad.pos_v[i];
            (*param)[pos] += grad.v[i];
         }
      }
   else
   {
      LOG(FATAL) << "Unknown model type: " << type;
   }
}
   
} // namespace f2m
