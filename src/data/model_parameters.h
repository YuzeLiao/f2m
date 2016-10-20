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

This file defines the model parameters used by f2m.
*/

#ifndef F2M_DATA_MODEL_PARAMETERS_H_
#define F2M_DATA_MODEL_PARAMETERS_H_

#include <vector>

#include "src/base/common.h"
#include "src/data/data_structure.h"

namespace f2m {

const real_t kInitMean = 0.0;
const real_t kInitStdev = 0.01;

enum ModelType { LR, FM, FFM };

// Model is responsible for storing the global model prameters.
// Note that, we represent the model parameters in a flat way, that is,
// no matter in LR, FM, or FFM, we store all the parameters in a big array.
class Model {
 public:
  // By default, the constructor is used for LR.
  Model(index_t feature_num, ModelType type = LR,
        int k = 0, index_t field_num = 0) :
  m_feature_num(feature_num),
  m_type(type),
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

 private:
  ModelType m_type;              // enum { LR, FM, FFM };
  vector<real_t> m_parameters;   // To store the global model parameters.
  index_t m_feature_num;         // number of features.
  index_t m_parameters_num;      // number of parameters.
  int m_k;                       // factor size for FM and FFM.
  int m_field_num;               // number of field. (only for FFM).

  void InitModel() {
     // Init bias and linear terms.
     for (index_t i = 0; i < m_feature_num + 1; ++i) {
       m_parameters[i] = 0.0;
     }
     // Init V or Matrix
     for (index_t i = m_feature_num + 1; i < m_parameters_num; ++i) {
       // gaussian distribution
       m_parameters[i] = ran_gaussian(kInitMean, kInitStdev);
     }
  }

  DISALLOW_COPY_AND_ASSIGN(Model);
};

} // namespace f2m

#endif // F2M_DATA_MODEL_PARAMETERS_H_
