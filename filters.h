#pragma once
#ifndef __FILTERS_H__
#define __FILTERS_H__
#include <stdint.h>
#include <stdio.h>
#include "int24_datatype.h"


void lowpass(float critFreq, int W, int sampRate, float *result);
void highpass(float critFreq, int W, int sampRate, float *result);
void to_fixed_point(float *f_data, Int24 *i_data, int n);
Int24 convolve(Int24 *audio, Int24 *kernel, int start, int size);
void swap_endian_if_needed(Int24 *kernel, int size);

#endif
