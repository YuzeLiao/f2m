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

This file defines the hyper-parameter used by f2m.
*/

#ifndef F2M_DATA_HYPER_PARAMETER_H_
#define F2M_DATA_HYPER_PARAMETER_H_

#include "src/data/data_structure.h"

namespace f2m {

struct F2M_PARAM {
  real_t learning_rate;
  real_t regu_lambda;
  RegularType regu_type;
  std::string model_checkpoint_file;
};

} // namespace f2m

#endif // F2M_DATA_HYPER_PARAMETER_H_
