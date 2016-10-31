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

This file defines the basic data structures used by f2m.
*/

#ifndef F2M_DATA_DATA_STRUCTURE_H_
#define F2M_DATA_DATA_STRUCTURE_H_

#include <vector>

#include "src/base/common.h"

using std::vector;

namespace f2m {

// We use the 32 bits float type to store the real number,
// such as the parameters and gradients. 
typedef float real_t;

// We use the 32 bits unsigned int type to store the index,
// or the size of model parameters. 
typedef uint32 index_t;

// What model we use in this task.
enum ModelType { LR, FM, FFM };

// What regularzation we use in this task.
enum RegularType { L1, L2, NONE };

// We use the SparseRow to store one line of the input data,
// which is parsed from the StringList.
// Notice that the entry of 'field' is optional, and 
// it used only for the FFM task. 
struct SparseRow {
  // In different iteration the row size may be 
  // different, so we need to resize it.
  void resize(index_t len, ModelType type) {
    CHECK_GE(len, 0);
    size = len;
    X.resize(len);
    idx.resize(len);
    if (type == FFM) 
      field.resize(len);
  }
  // X can be used to store both the numerical feature
  // and the categorical features. 
  vector<real_t> X;
  // Storing the feature index.
  vector<index_t> idx;
  // The 'field' is optional, only used for FFM.
  vector<index_t> field;
  // Size of the SparseRow. 
  index_t size;
};

// DMatrix (data matrix) is used to store batch of data, 
// which can be used for both trainning and prediction. 
// Note that DMatrix can also be a memory buffer to store
// all the trainning data, which will be sampled by another 
// DMatrix in each iteration.
struct DMatrix {
  // resize
  void resize(index_t size, ModelType type) {
    CHECK_GE(size, 0);
    row.resize(size);
    Y.resize(size);
    row_size = size;
    model_type = type;
  }
  // When we sample from disk, we need to use this
  // function to initialize the SparseRow pointers. 
  void InitSparseRow() {
    for (index_t i = 0; i < row_size; ++i) {
      row[i] = new SparseRow();
    }   
  }
  // Storing a set of SparseRow.
  // Note that here we use pointer to implement zero copy
  // when we load all data into the memory buffer.
  vector<SparseRow*> row;
  // Y can be either -1 or 0 (for negetive examples),
  // and be 1 (for positive examples). 
  vector<real_t> Y;
  // Size of DMatrix
  index_t row_size;
  // model type
  ModelType model_type;
};

// SparseGrad is used to store the calculated gradient.
// Here we use the 'flat model' to represent the model of LR,
// FM, as well as FFM. Flat model stores all the parameters in 
// a big array.
struct SparseGrad {
  // Constructor
  SparseGrad(index_t size) {
    w.resize(size);
    v.resize(size);
    pos_w.resize(size);
    pos_v.resize(size);
    size_w = 0;
    size_v = 0;
  }
  // The gradient w.
  vector<real_t> w;
  // The gradient V
  vector<real_t> v;
  // The position of w.
  vector<index_t> pos_w;
  // The postition of v.
  vector<index_t> pos_v;
  // note that size_w may not equal to w.size().
  index_t size_w;
  // note that size_v may not equal to v.size().
  index_t size_v;
};

} // namespace f2m

#endif // F2M_DATA_DATA_STRUCTURE_H_
