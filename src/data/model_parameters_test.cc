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

#include "src/data/model_parameters.h"
#include "src/data/data_structure.h"

namespace f2m {

const uint32 kFeature_num = 1000000;
const uint32 kFactor = 10;
const uint32 kField = 10;

TEST(MODEL_TEST, Init) {
  // Init LR
  Model model_lr(kFeature_num, LR);
  vector<real_t>* para = model_lr.GetParameter();
  EXPECT_EQ(para->size(), kFeature_num + 1);
  // Init FM
  Model model_fm(kFeature_num, FM, kFactor);
  para = model_fm.GetParameter();
  EXPECT_EQ(para->size(), kFeature_num + 1 + 
                          kFeature_num * kFactor);
  // Init FFM
  Model model_ffm(kFeature_num, FFM, kFactor, kField, true);
  para = model_ffm.GetParameter();
  EXPECT_EQ(para->size(), kFeature_num + 1 + 
                          kFeature_num * kFactor * kField);
}

TEST(MODEL_TEST, SaveModel) {

}

TEST(MODEL_TEST, LoadModel) {
        
}

} // namespace f2m
