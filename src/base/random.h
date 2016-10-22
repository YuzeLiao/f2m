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

This file implements sampling methods.
*/

#ifndef F2M_BASE_RANDOM_H_
#define F2M_BASE_RANDOM_H_

#include <stdlib.h>
#include <cmath>

typedef float real_t;

// random gaussion distribution

real_t ran_gaussion() {
  real_t u,v,x,y,Q;
  do {
    do {
      u = ran_uniform();
    } while (u == 0.0);
    v = 1.7156 * (ran_uniform() - 0.5);
    x = u - 0.449871;
    y = std::abs(v) + 0.386595;
    Q = x*x + y*(0.19600*y-0.25472*x);
    if (Q < 0.27597) { break; }
  } while ((Q > 0.27846) || ((v*v) > (-4.0*u*u*std::log(u))));
  return v / u;
}

real_t ran_gaussion(real_t mean, real_t stdev) {
  if ((stdev == 0.0 || (std::isnan(stdev)))) {
    return mean;    
  } else {
    return mean + stdev * ran_gaussion();
  }
}

real_t ran_uniform() {
  return rand() / ((real_t) RANDOM_MAX + 1);
}

#endif // F2M_BASE_RANDOM_H_
