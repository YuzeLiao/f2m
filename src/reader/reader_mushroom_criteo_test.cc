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

This file tests reader.h for real kaggle datasets.
*/

#include "gtest/gtest.h"

#include <string>

#include "src/reader/reader.h"
#include "src/data/data_structure.h"

using std::string;

namespace f2m {

const string file_mushroom = "/tmp/agaricus.txt.train";
const string file_criteo = "/tmp/Criteo.txt.train";
const int iteration_num = 1000;
const index_t kNumSamples = 150;

TEST(READER_TEST, MushRoom_Test) {
  Reader reader_lr(file_mushroom, kNumSamples, LR, true, true);
  DMatrix* matrix = NULL;
  for (int i = 0; i < iteration_num; ++i) {
    matrix = reader_lr.Samples();    
  }
}

TEST(READER_TEST, Criteo_Test) {
  Reader reader_ffm(file_criteo, kNumSamples, FFM, true, true);
  DMatrix* matrix = NULL;
  for (int i = 0; i < iteration_num; ++i) {
    matrix = reader_ffm.Samples();    
  }
}

} // namespace f2m
