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

// We use the 32 bits float to store the real number,
// such as the model parameters and gradients. 
typedef float real_t;

// We use the 32 bits unsigned int to store the index,
// as well as the size of model parameters. 
typedef uint32 index_t;

// Identify which model we use in this task.
enum ModelType { LR, FM, FFM };

// Identify which regularizer we use in this task.
enum RegularType { L1, L2, NONE };

// SparseRow is used to store one line of parsed data.
// Notice that the entry of 'field' is used only for 
// the FFM task. 
struct SparseRow {
  // Constructors
  SparseRow(ModelType type = LR) 
    : model_type(type) {}
  
  SparseRow(index_t len, ModelType type = LR)
    : model_type(type) {
    CHECK_GE(len, 0);
    resize(len);
  }
  // Resize current row.
  void resize(index_t len) {
    CHECK_GE(len, 0);
    size = len;
    X.resize(len);
    idx.resize(len);
    if (model_type == FFM) { 
      field.resize(len);
    }
  }
  // X can be used to store both the numerical 
  // features and the categorical features. 
  vector<real_t> X;
  // The idx is used to store the feature index.
  vector<index_t> idx;
  // The 'field' is optional, only used for FFM.
  vector<index_t> field;
  // Identify how many features are stored in 
  // current SparseRow. 
  index_t size;
  // enum  ModelType { LR, FM, FFM }
  ModelType model_type;
};

// DMatrix (data matrix) is used to store a batch 
// of data for both trainning and prediction process. 
struct DMatrix {
  // Constructors
  DMatrix(ModelType type = LR) 
    : model_type(type) {}

  DMatrix(index_t size, ModelType type = LR)
    : model_type(type) {
    CHECK_GE(size, 0);
    resize(size);
  }
  // Resize current data matrix.
  void resize(index_t size) {
    CHECK_GE(size, 0);
    row_size = size;
    row.resize(size);
    Y.resize(size);
  }
  // Initialize the row pointers. 
  void InitSparseRow() {
    for (index_t i = 0; i < row_size; ++i) {
      row[i] = new SparseRow(model_type);
    }   
  }
  // Storing a set of SparseRow.
  // Note that here we use pointer to implement zero copy
  // when we load all data into the memory buffer.
  vector<SparseRow*> row;
  // Y can be either -1 or 0 (for negetive examples),
  // and be 1 (for positive examples). 
  vector<real_t> Y;
  // Row size of current DMatrix
  index_t row_size;
  // enum ModelType { LR, FM, FFM }
  ModelType model_type;
};

// SparseGrad is used to store the calculated gradient.
// Note that here we do not use map<index_t, real_t> to
// store the sparse data becasue of the poor 
// performance of map or hash_map (unordered_map).
struct SparseGrad {
  // Constructors
  SparseGrad(ModelType type = LR)
    : size_w(0), size_v(0), model_type(type) {}

  SparseGrad(index_t size, ModelType type = LR) 
    : size_w(0), size_v(0), model_type(type) {
    CHECK_GE(size, 0);
    resize(size);
  }
  // Resize current gradient vector.
  resize(index_t size) {
    w.resize(size);
    pos_w.resize(size);
    if (type == FM || type == FFM) {
      v.resize(size);
      pos_v.resize(size);
    }
  }
  // Store the bias term and linear terms.
  vector<real_t> w;
  // Store the factor vectors.
  vector<real_t> v;
  // The position of w.
  vector<index_t> pos_w;
  // The postition of v.
  vector<index_t> pos_v;
  // How many gradients stored in w.
  // Note that the size_w != w.size().
  index_t size_w;
  // How many gradients. stored in v.
  // Note that the size_v != v.size().
  index_t size_v;
  // enum ModelType { LR, FM, FFM }
  ModelType model_type;
};

} // namespace f2m

#endif // F2M_DATA_DATA_STRUCTURE_H_
