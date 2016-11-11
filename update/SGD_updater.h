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

This file defines SGD_updater, which implements "updater"
 with SGD.
*/

#ifndef F2M_UPDATE_SGD_UPDATER_H
#define F2M_UPDATE_SGD_UPDATER_H

#include "src/update/updater.h"

#include "src/base/common.h"
#include "src/data/data_structure.h"

namespace f2m{
class SGD_updater : private Updater {
 public:
   SGD_updater(Model* model,
               real_t learning_rate,
               real_t regu_lamda,
               RegularType regu_type = L2)
   : Updater(model, learning_rate,
             regu_lamda, regu_type) {}
   
   ~SGD_updater() {}
   
   void Update(const SparseGrad& grad);
   
   DISALLOW_COPY_AND_ASSIGN(SGD_updater);
};

}

#endif /* F2M_UPDATE_SGD_UPDATER_H */
