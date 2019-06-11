#ifndef _STEREODYNAMICS_H_
#define _STEREODYNAMICS_H_

#include "pch.h"
#include <iostream>

class STEREO {
public:
	double rms_amplitude = 0;
	double rms_width = 0.1;
	double rms_dB = 0;
	double min_amplitude = 0;
	double max_amplitude = 0;
	double comp_scale = 0;
	double comp_slope = 0.3;
	double comp_threshold = -15;
	double exp_threshold = -25;
	double exp_slope = -0.05;
	double attack = 0.05;
	double release = 0.005;
	double scaling_factor;
	double exp_scale;
	double gain;
	double level = 10;
	double bufptr;

	double calc_rms_amplitude(double* outputbuffer, int bufptr, double rms_width, double rms_amplitude);
	double calc_rms_dB(double rms_amplitude, double RMS_dB);
	// positve comp slope
	double calc_comp_scale(double comp_slope, double comp_threshold, double rms_dB);
	// negative comp slope
	double calc_exp_scale(double exp_slope, double exp_threshold, double rms_dB);
	double calc_scaling_factor(double min_amplitude);
};

#endif