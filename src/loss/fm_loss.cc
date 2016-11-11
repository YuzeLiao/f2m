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
 
 This file implements FMLoss
 */

#include <vector>
#include <cmath>

#include "src/base/common.h"
#include "src/data/data_structure.h"
#include "src/data/model_parameters.h"
#include "src/loss/fm_loss.h"
#include "regularize_normalize.h"

namespace f2m {

// Given the input DMatrix and current model, return
// the prediction results. Math:
//  [ pred = <x, w> ]
void FMLoss::Predict(const DMatrix* matrix,
             Model& model,
             vector<real_t>& pred) {
   CHECK_NOTNULL(matrix);
   CHECK_GT(pred.size(), 0);
   CHECK_EQ(pred.size(), matrix->row_size);
   vector<real_t>* weight = model.GetParameter();
   // each line of test examples
   for (index_t i = 0; i < matrix->row_size; ++i) {
      SparseRow* row = matrix->row[i];
      pred[i] = wTx(row, weight, model);
   }
}
   
// Given the prediction results and the current model, return
// the calculated gradient. Math:
//  [ (-y / ( (1/exp(-y*<w,x>))  + 1 )) * X ]
void FMLoss::CalcGrad(const DMatrix* matrix,
               Model& model,
               SparseGrad& grad) {
   CHECK_NOTNULL(matrix);
   CHECK_GT(matrix->row_size, 0);
   CHECK_GT(model.GetSizeOfVector(),0)
   vector<real_t>* weight = model.GetParameter();
   index_t num = 0;
   index_t model_k = model.GetSizeOfVector();
   index_t feature_num = model.GetNumberOfFeatures();
   real_t lambda = model.GetLambda();
   
   for (index_t i = 0; i < matrix->row_size; i++) {
      SparseRow* row = matrix->row[i];
      real_t y = matrix->Y[i];
      real_t partial_grad = -y / (exp(y * wTx(row, weight, model) + 1));
      // calculate gradient of bias term
      grad.bias = partial_grad;
      // calculate gradient of linear term
      for (index_t j = 0; j < row->size; j++) {
         // idx begin with 0
         index_t pos = row->idx[j] + 1;
         real_t w_j = partial_grad * row->X[j] + lambda * (REGU_GRAD_TERM(m_regu_type, (*weight)[pos]));
         if (num >= grad.w.size()) {
            grad.w.resize(grad.w.size() * 2);
         }
         grad.w[num] = w_j;
         grad.pos_w[num] = pos;
         num++;
      }
      // calculate gradient of latent vector
      num = 0;
      for (index_t j = 0; j < row->size; j++) {
         for (index_t k = j + 1; k < row->size;k++) {
            index_t pos_j = row->idx[j] * model_k + feature_num + 1;
            index_t pos_k = row->idx[k] * model_k + feature_num + 1;
            for (index_t l = 0; l < model_k; l++) {
               real_t w_j = partial_grad * row->X[j] * row->X[k] * (*weight)[pos_k + l] +
                            lambda * (REGU_GRAD_TERM(m_regu_type, (*weight)[pos_j + l]));
               real_t w_k = partial_grad * row->X[j] * row->X[k] * (*weight)[pos_j + l] +
                            lambda * (REGU_GRAD_TERM(m_regu_type, (*weight)[pos_k + l]));
               if (num >= grad.v.size()) {
                  // resize
                  grad.w.resize(grad.v.size() * 2);
               }
               grad.v[num] = w_j;
               grad.pos_v[num] = pos_j;
               grad.v[num + 1] = w_k;
               grad.pos_v[num] = pos_k;
               num += 2;
            }
         }
      }
      
   }
}
   
inline real_t FMLoss::wTx(const SparseRow* row, const vector<real_t>* w, const Model& model) {
   index_t model_k = model.GetSizeOfVector();
   index_t feature_num = model.GetNumberOfFeatures();
   // initialize val to bias
   real_t val = (*w)[BIAS];
   // linear term
   for (index_t j = 0; j < row->size; j++) {
      index_t pos = row->idx[j];
      val += (*w)[pos] * row->X[j];
   }
   for (index_t j = 0; j < row->size; j++) {
      for (index_t k = j + 1; k < row->size; k++) {
         index_t pos_j = row->idx[j] * model_k + feature_num + 1;
         index_t pos_k = row->idx[k] * model_k + feature_num + 1;
         for (index_t l = 0; l < model_k; l++) {
            val += (*w)[pos_j] * (*w)[pos_k] * row->X[j] * row->X[k];
         }
      }
   }
   return val;
}


}