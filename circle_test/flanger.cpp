#include "flanger.h"
#include "delay.h"

Delay Delay;

void Flanger::init(short effect_rate, short sampling, short maxd, short mind, double fwv, double stepd, double fbv) {
	del.Delay_init(2, fbv, fwv, 1);
	samp_freq = sampling;
	counter = effect_rate;
	control = 1;
	var_delay = mind;
	//User Parameters
	counter_limit = effect_rate;
	max_delay = maxd;
	min_delay = mind;
	mix_vol = 1;
	delay_step = stepd;
}

/*This is the flanging process task
that uses the delay task inside*/

double Flanger::process_samples(double *inputbuffer, double *outputbuffer, int bufptr) {
	outputbuffer[bufptr] = Delay_task(&inputbuffer[0]);
	return *outputbuffer;
}

/*

This sweep function creates a slow frequency
ramp that will go up and down changing the
delay value at the same time. The counter
variable is a counter of amount of samples that
the function waits before it can change the delay.

*/
void Flanger::sweep(void) {
	if (!--counter) {
		var_delay += control * delay_step;
		if (var_delay > max_delay) {
			control = -1;
		}
		if (var_delay < min_delay) {
			control = 1;
		}
		Delay_set_delay(var_delay);
		counter = counter_limit;
	}
};