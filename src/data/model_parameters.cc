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

This file is the implementation of model parameters.
*/

#include "src/data/model_parameters.h"

#include <vector>
#include <string>
#include <fstream>

#include "src/base/common.h"
#include "src/base/logging.h"

using std::vector;
using std::string;

namespace f2m {

// parameters for gaussian distribution.
const real_t kInitMean = 0.0;
const real_t kInitStdev = 0.01;

Model::Model(index_t feature_num, ModelType type,
             int k, index_t field_num) :
  m_type(type),
  m_feature_num(feature_num),
  m_k(k),
  m_field_num(field_num) {
    CHECK_GT(m_feature_num, 0);
    CHECK_GE(m_k, 0);
    CHECK_GE(m_field_num, 0);
    // allocate memory and initialize model parameters.
    if (type == LR) {
      // bias term + linear terms.
      m_parameters_num = m_feature_num + 1;
    } else if (type == FM) {
      // bias term + linear terms + V
      m_parameters_num = m_feature_num + 1 + 
                         k * m_feature_num;
    } else if (type == FFM) {
      // bias term + linear terms + Matrix
      m_parameters_num = m_feature_num + 1 + 
                         k * m_field_num * m_feature_num;
    } else {
      LOG(FATAL) << "Unknow model type: " << type;
    }
    try {
      m_parameters.resize(m_parameters_num);
      InitModel();
    } catch (std::bad_alloc&) {
      LOG(FATAL) << "Cannot allocate enough memory for \
                     current model parameters.";
    }
  }

void Model::SaveModel(const string& filename) {
}

void Model::LoadModel(const string& filename) {
}

// Initialize model parameters using 
// a random gaussian distribution
void Model::InitModel() {
  for (index_t i = 0; i < m_parameters_num; ++i) {
    m_parameters[i] = ran_gaussion(kInitMean, kInitStdev);
  }
}

} // namespace f2m
