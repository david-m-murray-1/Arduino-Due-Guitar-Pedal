#pragma once
#ifndef __FLANGER_H__
#define __FLANGER_H_
#include "delay.h"

class Flanger {
public:
	short samp_freq;
	double var_delay;
	short counter;
	short counter_limit;
	short control;
	short max_delay;
	short min_delay;
	double mix_vol;
	double delay_step;
	void init(short effect_rate, short sampling, short maxd, short mind, double fwv, double stepd, double fbv);
	void sweep();
	double process_samples(double *inputbuffer, double *outputbuffer, int bufptr);
};

#endif
