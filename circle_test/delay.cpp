#include "delay.h"
#include <cmath>


/*****************************************************************************
*				Fractional delay line implementation in Cpp
*
*                    ---------[d_mix]->-------------------------
*                    |                                         |
*                    |										   |
*                    |x1                                       v
*     xin ------>[+]----->[z^-M]--[interp.]----[d_fw]-------->[+]-----> yout
*                 ^                         |
*                 |                         |
*                 |----------[d_fb]<--------|
*******************************************************************************/

void Delay::Delay_init(double delay_samples, double dfb, double dfw, double d_mix) {
	set_delay(delay_samples);
	set_dfb(dfb);
	set_dfw(dfw);
	set_dmix(d_mix);
	del.wrtPtr = &d_buffer[MAX_BUF_SIZE-1];
}
		
void Delay::set_dfb(double val) {
	del.dfb = val;
}

void Delay::set_dfw(double val) {
	del.dfw = val;
}
		
void Delay::set_dmix(double val) {
	del.d_mix = val;
}

void Delay::set_delay(double n_delay) {
	// get integer part of delay
	del.d_samples = (short)floor(n_delay);
	// get the fractional part of the delay
	del.n_fract = (n_delay - del.d_samples);
}

void Delay::get_dfb(void) {
	return del.dfb;
}

void Delay::set_dfw(void) {
	return del.dfw;
}

void Delay::get_dmix(void) {
	return del.d_mix;
}

double Delay::Delay_task(double *inputbuffer, double *outputbuffer, int bufptr) {

	/*Calculates current read pointer position*/
	del.rdPtr = del.wrtPtr - (short)del.d_samples;
	/* wraps read pointer */
	if (del.rdPtr < d_buffer) {
		del.rdPtr += MAX_BUF_SIZE - 1;
	}
	y0 = del.rdPtr - 1;
	y1 = MAX_BUF_SIZE - 1;
	if (y0 < d_buffer) {
		y0 += MAX_BUF_SIZE - 1;
	}

	x_est = (*(y0)-*(y1)) * del.n_fract + *(y1);

	/* calculate next value to store in buffer */
	x1 = inputbuffer[bufptr] + x_est * del.dfb;
	/* store value in buffer */
	*(del.wrtPtr) = x1;

	/* output value calculation*/
	outputbuffer[bufptr] = x1 * del.d_mix + x_est * del.dfw;
	del.wrtPtr++;

	/*wrap delay write pointer*/
	if (del.wrtPtr - &d_buffer[0] > MAX_BUF_SIZE - 1) {
		del.wrtPtr = &d_buffer[0];
	}

	return *outputbuffer;
};