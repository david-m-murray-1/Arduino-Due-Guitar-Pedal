#include <algorithm>
#include <iterator>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "stereodynamics.h"


double STEREO::calc_rms_amplitude(double* outputbuffer, int bufptr, double rms_width, double rms_amplitude) {
	rms_amplitude = (1 - rms_width) * rms_amplitude + (rms_width * outputbuffer[bufptr]);
	return rms_amplitude;
}

double STEREO::calc_rms_dB(double rms_amplitude, double rms_dB) {
	rms_dB = 10 * log10(rms_amplitude); // Hifi.write(Effect2.process_samples(circle.get());
	return rms_dB;
}
// positve comp slope
double STEREO::calc_comp_scale(double comp_slope, double comp_threshold, double rms_dB) {
	comp_scale = comp_slope * (comp_threshold - rms_dB);
	return comp_scale;
}
// negative comp slope
double STEREO::calc_exp_scale(double exp_slope, double exp_threshold, double rms_dB) {
	exp_scale = exp_slope * (exp_threshold - rms_dB);
	return exp_scale;
}

double STEREO::calc_scaling_factor(double min_amplitude) {
	scaling_factor = pow(10, min_amplitude);
	return scaling_factor;
}