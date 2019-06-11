#pragma once
#ifndef __FLANGER_H__
#define __FLANGER_H_
#include "delay.h"

class Flanger {
private:
	static short samp_freq;
	static double var_delay;
	static short counter;
	static short counter_limit;
	static short control;
	static short max_delay;
	static short min_delay;
	static double mix_vol;
	static double delay_step;
public:
	void init(short effect_rate, short sampling, short maxd, short mind, double fwv, double stepd, double fbv);
	void sweep();
	double process_samples(double *inputbuffer, double *outputbuffer, int bufptr);
};


#endif
