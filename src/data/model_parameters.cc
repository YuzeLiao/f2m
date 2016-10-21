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

using std::vector;
using std::string;
using std::ofstream;

namespace f2m {

const real_t kInitMean = 0.0;
const real_t kInitStdev = 0.01;

Model::Model(index_t feature_num, ModelType type = LR,
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

void Model::SaveModel(const string& filename) {
  ofstream out_model;
  out_model.open(filename.c_str());
  // output bias term.
  out_model << "#global bias W0" << std::endl;
  out_model << m_parameters[0] << std::endl;
  // output linear term.
  out_model << "#unary iteractions Wi" << std::endl;
  for (index_t i = 1; i < m_feature_num + 1; ++i) {
    out_model << m_parameters[i] << std::endl;
  }
  // output V
  int index = m_feature_num + 1;
  if (m_type == FM) {
    out_model << "#pairwise interactions Vi" << std::endl;
    for (index_t i = 0; i < m_feature_num; ++i) {
      out_model << "feature " << i << "# : ";
      for (int n = 0; n < m_k; ++n) {
        out_model << m_parameters[index];
        if (n != m_k - 1) {
            output << " ";
        }
        out_model << std::endl;
        ++index;
      }
    }
  }
  // output Matrix
  else if (m_type == FFM) {
    out_model << "#pairwise interactions Vj,f" << std::endl;
    for (index_t i = 0; i < m_feature_num; ++i) {
      out_model << "feature " << i << "# : ";
      for (index_t f = 0; f < m_field_num; ++f) {
        out_model << "field " << f << "# : ";
        for (int n = 0; n < m_k; ++n) {
          out_model << m_parameters[index];
          if (n != m_k - 1) {
            output << " ";
          }
          out_model << std::endl;
          ++index;
        }
      }
    }
  }
}

void Model::LoadModel(const string& filename) {
  CHECK_EQ(m_parameters.size(), m_parameters_num);
  string line;
  ifstream model_file(filename.c_str());
  if (model_file.is_open()) {
    if (!std::getline(model_file, line)) { // #global bais W0
      LOG(FATAL) << "Read file error: " << filename ; 
    }
    if (!std::getline(model_file, line)) { // W0
      LOG(FATAL) << "Read file error: " << filename;
    }
    m_parameters[0] = std::atof(line.c_str());
    if (!std::getline(model_file, line)) { // #unary interactions Wj
      LOG(FATAL) << "Read file error: " << filename;
    }
    int index = 1;
    for (index_t i = 0; i < m_feature_num; ++i) { // Wj
      if (!std::getline(model_file, line)) {
        LOG(FATAL) << "Read file error: " << filename;
      }
      m_feature_num[index] = std::atof(line.c_str());
      ++index;
    }
    if (m_type == FM) {
      if (!std::getline(model_file, line)) { // pairwise interactions Vi
        LOG(FATAL) << "Read file error: " << filename;
      }
      for (index_t i = 0; i < m_feature_num; ++i) {
        if (!std::getline(model_file, line)) { // feature i: 
          LOG(FATAL) << "Read file error: " << filename;
        }
        
      }
    } else if (m_type == FFM) {

    }

  } else {
    LOG(FATAL) << "Open file error: " << filename;
  }
}

void Model::InitModel() {
  // Init bias and linear terms.
  for (index_t i = 0; i < m_feature_num + 1; ++i) {
       m_parameters[i] = 0.0;
  }
  if (m_type == FM || m_type == FFM) {
    // Init V or Matrix
    for (index_t i = m_feature_num + 1; i < m_parameters_num; ++i) {
       // gaussian distribution
       m_parameters[i] = ran_gaussian(kInitMean, kInitStdev);
    }	
  }
}

} // namespace f2m
