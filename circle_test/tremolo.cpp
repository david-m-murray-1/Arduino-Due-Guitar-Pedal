#include "tremolo.h"



//processing samples
double Tremolo::process_samples(double *inputbuffer, double *outputbuffer, int bufptr)
{
	double m;
	m = mod * depth / rate;
	outputbuffer[bufptr] = (m + offset) * inputbuffer[bufptr];
	return *outputbuffer;
}

void Tremolo::sweep(void) {
	mod += control;
	if (mod > rate) {
		control = -1;
	}
	else if (!mod) {
		control = 1;
	}
}

void Tremolo::setRate(double rate) {
	this->rate = rate;
}

void Tremolo::setDepth(double depth) {
	this->depth = depth;
}

