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
#include <string>

#include "src/base/common.h"
#include "src/data/data_structure.h"

using std::vector;
using std::string;

namespace f2m {

// Model is responsible for storing the global model prameters.
// Note that, we represent the model parameters in a flat way, that is,
// no matter in LR, FM, or FFM, we store all the parameters in a big array.
class Model {
 public:
  // By default, the constructor is used for LR.
  Model(index_t feature_num, 
        ModelType type = LR,
        int k = 0, 
        index_t field_num = 0,
        bool gaussian = false);

  // Save model to disk file. 
  void SaveModel(const string& filename);

  // Load model from disk file.
  void LoadModel(const string& filename);

  vector<real_t>* GetParameter() { return &m_parameters; }

 private:
  ModelType m_type;               // enum { LR, FM, FFM };
  vector<real_t> m_parameters;    // To store the global model parameters.
  index_t m_feature_num;          // number of features.
  index_t m_parameters_num;       // number of parameters.
  int m_k;                        // factor size for FM and FFM.
  index_t m_field_num;            // number of field. (only for FFM).

  // Initialize model parameters using 
  // the random gaussian distribution
  void InitModelUsingGaussian();

  DISALLOW_COPY_AND_ASSIGN(Model);
};

} // namespace f2m

#endif // F2M_DATA_MODEL_PARAMETERS_H_
