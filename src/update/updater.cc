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

This files implement updater.h
*/

#include "src/update/updater.h"

#include "src/base/common.h"
#include "src/data/data_structure.h"

namespace f2m {

// Using simple SGD by defualt.
void Updater::Update(const SparseGrad& grad) {
  vector<real_t>* param = m_model->GetParameter();
}    

};
