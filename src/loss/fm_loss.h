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
 
 This file defines the factorization machine loss.
 */

#ifndef _F2M_LOSS_FM_LOSS_H
#define _F2M_LOSS_FM_LOSS_H

#include "src/base/common.h"
#include "src/data/data_structure.h"
#include "src/data/model_parameters.h"
#include "src/loss/loss.h"

using std::vector;

namespace f2m {
   
class FMLoss : public Loss {
 public:
   FMLoss (RegularType regu_type) : Loss(regu_type) {}
   ~FMLoss () {}
   
   void Predict(const DMatrix* matrix,
                Model& model,
                vector<real_t>& pred);
   
   void CalcGrad(const DMatrix* matrix,
                 Model& model,
                 SparseGrad& grad);
      
      
 private:
   inline real_t wTx(const SparseRow* row, const vector<real_t>* w, const Model& model);
   
   DISALLOW_COPY_AND_ASSIGN(FMLoss);
};
   
   
   
   
   
   
}



#endif // _F2M_LOSS_FM_LOSS_H