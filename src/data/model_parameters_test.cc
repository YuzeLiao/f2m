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

This file tests model_parameters.h
*/

#include "gtest/gtest.h"

#include <string>

#include "src/data/model_parameters.h"
#include "src/data/data_structure.h"

using std::string;

namespace f2m {

const uint32 kFeature_num = 1000000;
const uint32 kFactor = 10;
const uint32 kField = 10;
const string kFilename = "/tmp/test_model.binary";

/*TEST(MODEL_TEST, Init) {
  // Init LR using gaussion.
  Model model_lr(kFeature_num, LR, 0, 0, true);
  vector<real_t>* para = model_lr.GetParameter();
  EXPECT_EQ(para->size(), kFeature_num + 1);
  // Init FM
  Model model_fm(kFeature_num, FM, kFactor);
  para = model_fm.GetParameter();
  EXPECT_EQ(para->size(), kFeature_num + 1 + 
                          kFeature_num * kFactor);
  // Init FFM
  Model model_ffm(kFeature_num, FFM, kFactor, kField);
  para = model_ffm.GetParameter();
  EXPECT_EQ(para->size(), kFeature_num + 1 + 
                          kFeature_num * kFactor * kField);
}

TEST(MODEL_TEST, SaveModel) {
  // Init a FFM model 
  Model ffm(kFeature_num, FFM, kFactor, kField);
  ffm.SaveModel(kFilename);
}

TEST(MODEL_TEST, LoadModel) {
  // Init a FFM model with gaussion distribution.
  Model ffm(kFeature_num, FFM, kFactor, kField, true);
  // parameters become 1.0
  ffm.LoadModel(kFilename);
  index_t num_parameter = kFeature_num + 1 + 
                          kFeature_num * kField * kFactor;
  vector<real_t>* para = ffm.GetParameter();
  for (index_t i = 0; i < num_parameter; ++i) {
    EXPECT_EQ((*para)[i], 1.0);
  }
}
*/
} // namespace f2m
