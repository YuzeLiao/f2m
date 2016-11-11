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
 
 This file implements regularization(L1、L2) and 
 normalization.
 */

#ifndef F2M_SRC_BASE_REGULARIZATION_NORMALIZATION_H
#define F2M_SRC_BASE_REGULARIZATION_NORMALIZATION_H

#include "src/base/common.h"
#include "src/data/data_structure.h"

namespace f2m {
// calculate the gradient of regulization term
#define REGU_GRAD_TERM(regu_type, w) \
(regu_type == NONE) ? 0 :            \
((regu_type == L1) ? ((w > 0) ? 1 : ((w < 0) ? -1 : 0)) : w)

}

#endif /* F2M_SRC_BASE_REGULARIZATION_NORMALIZATION_H */
