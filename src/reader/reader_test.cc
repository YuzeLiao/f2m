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

This file tests reader.h
*/

#include "gtest/gtest.h"

#include <string>
#include <vector>

#include "src/base/file_util.h"
#include "src/reader/reader.h"
#include "src/data/data_structure.h"

using std::vector;
using std::string;

namespace f2m {
    
const string kTestfilename = "/tmp/test_reader";
const string kStr = "0\t0:0.123\t1:0.123\t2:0.123\n";
const string kStrFFM = "1\t0:0:0.123\t1:1:0.123\t2:2:0.123\n";
const index_t kFeatureNum = 3;
const index_t kNumLines = 100000;
const index_t kNumSamples = 1000;
const int iteration_num = 1000;

class ReaderTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    string lr_file = kTestfilename + "_LR.txt";
    string ffm_file = kTestfilename + "_ffm.txt";
    FILE* file_lr = OpenFileOrDie(lr_file.c_str(), "w");
    for (index_t i = 0; i < kNumLines; ++i) {
      uint32 write_len = fwrite(kStr.c_str(), 1, kStr.size(), file_lr);
      EXPECT_EQ(write_len, kStr.size());
    }
    Close(file_lr);
    FILE* file_ffm = OpenFileOrDie(ffm_file.c_str(), "w");
    for (index_t i = 0; i < kNumLines; ++i) {
      uint32 write_len = fwrite(kStrFFM.c_str(), 1, kStrFFM.size(), file_ffm);
      EXPECT_EQ(write_len, kStrFFM.size());
    }
    Close(file_ffm);
  }
};

void CheckLR(const DMatrix* matrix) {
  EXPECT_EQ(matrix->row_size, kNumSamples);
  // check the first element
  EXPECT_EQ(matrix->Y[0], (real_t)0);
  EXPECT_EQ(matrix->row[0]->X[0], (real_t)0.123);
  EXPECT_EQ(matrix->row[0]->X[kFeatureNum-1], (real_t)0.123);
  EXPECT_EQ(matrix->row[0]->idx[0], (index_t)0);
  EXPECT_EQ(matrix->row[0]->idx[kFeatureNum-1], (index_t)(kFeatureNum-1));
  // check the last element
  EXPECT_EQ(matrix->Y[kNumSamples-1], (real_t)0);
  EXPECT_EQ(matrix->row[kNumSamples-1]->X[0], (real_t)0.123);
  EXPECT_EQ(matrix->row[kNumSamples-1]->X[kFeatureNum-1], (real_t)0.123);
  EXPECT_EQ(matrix->row[kNumSamples-1]->idx[0], (index_t)0);
  EXPECT_EQ(matrix->row[kNumSamples-1]->idx[kFeatureNum-1], (index_t)(kFeatureNum-1));
}

void CheckFFM(const DMatrix* matrix) {
  EXPECT_EQ(matrix->row_size, kNumSamples);
  // check the first element
  EXPECT_EQ(matrix->Y[0], (real_t)1);
  EXPECT_EQ(matrix->row[0]->X[0], (real_t)0.123);
  EXPECT_EQ(matrix->row[0]->X[kFeatureNum-1], (real_t)0.123);
  EXPECT_EQ(matrix->row[0]->idx[0], (index_t)0);
  EXPECT_EQ(matrix->row[0]->idx[kFeatureNum-1], (index_t)(kFeatureNum-1));
  // check the last element
  EXPECT_EQ(matrix->Y[kNumSamples-1], (real_t)1);
  EXPECT_EQ(matrix->row[kNumSamples-1]->X[0], (real_t)0.123);
  EXPECT_EQ(matrix->row[kNumSamples-1]->X[kFeatureNum-1], (real_t)0.123);
  EXPECT_EQ(matrix->row[kNumSamples-1]->idx[0], (index_t)0);
  EXPECT_EQ(matrix->row[kNumSamples-1]->idx[kFeatureNum-1], (index_t)(kFeatureNum-1));
  EXPECT_EQ(matrix->row[kNumSamples-1]->field[0], (index_t)0);
  EXPECT_EQ(matrix->row[kNumSamples-1]->field[kFeatureNum-1], (index_t)(kFeatureNum-1));
}

TEST_F(ReaderTest, SampleFromDiskNoLoop) {
  string lr_file = kTestfilename + "_LR.txt";
  string ffm_file = kTestfilename + "_ffm.txt";
  // lr 
  Reader reader_lr(lr_file, kNumSamples, LR, false);
  DMatrix *matrix = NULL;
  while (1) {
    matrix = reader_lr.Samples();
    if (matrix->row_size != kNumSamples) break;
    CheckLR(matrix);
  }
  // ffm
  Reader reader_ffm(ffm_file, kNumSamples, FFM, false);
  while (1) {
    matrix = reader_ffm.Samples();
    if (matrix->row_size != kNumSamples) break;
    CheckFFM(matrix);
  }
}

TEST_F(ReaderTest, SampleFromDiskInLoop) {
  string lr_file = kTestfilename + "_LR.txt";
  string ffm_file = kTestfilename + "_ffm.txt";
  // lr
  Reader reader_lr(lr_file, kNumSamples, LR, true);
  DMatrix *matrix = NULL;
  for (int i = 0; i < iteration_num; ++i) {
    matrix = reader_lr.Samples();
    CheckLR(matrix);
  }
  // ffm
  Reader reader_ffm(ffm_file, kNumSamples, FFM, true);
  for (int i = 0; i < iteration_num; ++i) {
    matrix = reader_ffm.Samples();
    CheckFFM(matrix);
  }
}

TEST_F(ReaderTest, SampleFromMemoryNoLoop) {
  string lr_file = kTestfilename + "_LR.txt";
  string ffm_file = kTestfilename + "_ffm.txt";
  // lr
  Reader reader_lr(lr_file, kNumSamples, LR, false, true);
  DMatrix* matrix = NULL;
  while (1) {
    matrix = reader_lr.Samples();
    if (matrix->row_size != kNumSamples) break;
    CheckLR(matrix);
  }
  // ffm
  Reader reader_ffm(ffm_file, kNumSamples, FFM, false, true);
  while (1) {
    matrix = reader_ffm.Samples();
    if (matrix->row_size != kNumSamples) break;
    CheckFFM(matrix);
  }
}

TEST_F(ReaderTest, SampleFromMemoryInLoop) {
  string lr_file = kTestfilename + "_LR.txt";
  string ffm_file = kTestfilename + "_ffm.txt";
  // lr
  Reader reader_lr(lr_file, kNumSamples, LR, true, true);
  DMatrix* matrix = NULL;
  for (int i = 0; i < iteration_num; ++i) {
    matrix = reader_lr.Samples();
    CheckLR(matrix);
  }
  // ffm
  Reader reader_ffm(ffm_file, kNumSamples, FFM, true, true);
  for (int i = 0; i < iteration_num; ++i) {
    matrix = reader_ffm.Samples();
    CheckFFM(matrix);
  }
}
} // namespace f2m
