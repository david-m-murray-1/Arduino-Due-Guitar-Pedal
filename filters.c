#include "filters.h"
#include <math.h>
#include <limits.h>
#include <string.h>


void lowpass(float critFreq, int W, int sampRate, float *result)

{

	int i;

	float t, h, sum = 0.0f;

	int M = 2 * W;



	for (i = 0; i <= M; i++) {

		// t is symmetric around the Y axis

		t = (i - W) / (float)sampRate;

		if (t == 0) {

			// avoid discontinuity at t = 0

			result[i] = 1.0f;

			sum += 1.0f;

		}
		else {

			// sinc function

			h = sin(2 * M_PI * critFreq * t) / (2 * M_PI * critFreq * t);

			// hamming window

			h *= (0.54 - 0.46 * cos(2 * M_PI * i / M));

			result[i] = h;

			sum += h;

		}

	}



	// normalize so that DC gain is 1

	for (i = 0; i <= M; i++) {

		result[i] /= sum;

	}

}



void highpass(float critFreq, int W, int sampRate, float *result)

{

	int i;



	lowpass(critFreq, W, sampRate, result);



	for (i = 0; i <= 2 * W; i++) {

		result[i] = -result[i];

	}



	result[W + 1] += 1.0f;

}



int16_t convolute(int16_t *audio, int16_t *kernel, int start, int size)

{

	int i, ai;

	int32_t sum = 0;



	for (i = 0; i < size; i++) {

		ai = (i + start) % size;

		sum += audio[ai] * kernel[i];

	}



	return sum >> 16;

}