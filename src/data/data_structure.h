/* -------------------------------------------------------------------------- 
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

This file defines the basic data structures used by f2m.
*/

#ifndef F2M_DATA_DATA_STRUCTURE_H_
#define F2M_DATA_DATA_STRUCTURE_H_

#include <vector>

#include "src/base/common.h"

using std::vector;

namespace f2m {

/* We use the 32 bits float type to store the real number,
   such as the parameters and gradients. */

typedef float real_t;

/* We use the 32 bits unsigned int type to store the index,
   or the size of model parameters. */

typedef uint32 index_t;

/* We use the SparseRow to store one line of the input data.
   Notice that the entry of 'field' is optional, and 
   it used only for the FFM task. */

struct SparseRow {

  /* For LR and FM, field = false (default).
     For FFM, we need to set 'field = true' . */

  explicit SparseRow(index_t len, bool field = false) {
  	CHECK_GT(len, 0);
  	X.resize(len);
  	idx.resize(len);
  	if (!field) 
  		field.resize(len);
  	size = len;
  }
  
  /* X can be used to store both the numerical feature
     and the categorical features. */

  vector<real_t> X;

  /* Storing the feature index. */ 

  vector<index_t> idx;

  /* The 'field' is optional, only used for FFM. */

  vector<index_t> field;

  /* Size of the SparseRow. */

  index_t size;

};

/* DMatrix (data matrix) is used to store batch of data, 
   which can be used for both trainning and prediction. */

struct DMatrix {

  /* Storing a set of SparseRow. */
  
  vector<SparseRow> row;

  /* Y can be either -1 or 0 (for negetive examples),
     and be 1 (for positive examples). 
     Note that, for some loss functions such as the cross-entropy loss,
     we can only use -1 (not 0) and 1 to represent negetive and positive 
     example, respectivly. But here we do not need to care about that because
     our loss function do the transformation by itself. */

  vector<real_t> Y;

  index_t row_size;

};

} // namespace f2m

#endif // F2M_DATA_DATA_STRUCTURE_H_
