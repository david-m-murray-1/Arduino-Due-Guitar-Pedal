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

//Delay Delay;
struct fract_delay {
	double d_buffer[MAX_BUF_SIZE];
	double d_mix;       /*delay blend parameter*/
	short d_samples;	/*delay duration in samples*/
	double d_fb;	    /*feedback volume*/
	double d_fw;	    /*delay tap mix volume*/
	double n_fract;     /*fractional part of the delay*/
	double *rdPtr;      /*delay read pointer*/
	double *wrtPtr;     /*delay write pointer*/
} del;


//static struct fract_delay del;

void Delay_init(double delay_samples, double dfb, double dfw, double dmix) {
	Delay_set_delay(delay_samples);
	Delay_set_dfb(dfb);
	Delay_set_dfw(dfw);
	Delay_set_dmix(dmix);
	del.wrtPtr = &del.d_buffer[MAX_BUF_SIZE-1];
}
		
void Delay_set_dfb(double val) {
	del.d_fb = val;
}

void Delay_set_dfw(double val) {
	del.d_fw = val;
}
		
void Delay_set_dmix(double val) {
	del.d_mix = val;
}

void Delay_set_delay(double n_delay) {
	// get integer part of delay
	del.d_samples = (short)floor(n_delay);
	// get the fractional part of the delay
	del.n_fract = (n_delay - del.d_samples);
}

double Delay_get_dfb(void) {
	return del.d_fb;
}

double Delay_get_dfw(void) {
	return del.d_fw;
}

double Delay_get_dmix(void) {
	return del.d_mix;
}

double Delay_task(double *inputbuffer, double *outputbuffer, int bufptr) {
	double * y0;
	double * y1;
	double x1;
	double x_est;

	/*Calculates current read pointer position*/
	del.rdPtr = del.wrtPtr - (short)del.d_samples;
	/* wraps read pointer */
	if (del.rdPtr < del.d_buffer) {
		del.rdPtr += MAX_BUF_SIZE - 1;
	}
	y0 = del.rdPtr - 1;
	y1 = MAX_BUF_SIZE - 1;
	if (y0 < del.d_buffer) {
		y0 += MAX_BUF_SIZE - 1;
	}

	x_est = (*(y0)-*(y1)) * del.n_fract + *(y1);

	/* calculate next value to store in buffer */
	x1 = inputbuffer[bufptr] + x_est * del.d_fb;
	/* store value in buffer */
	*(del.wrtPtr) = x1;

	/* output value calculation*/
	outputbuffer[bufptr] = x1 * del.d_mix + x_est * del.d_fw;
	del.wrtPtr++;

	/*wrap delay write pointer*/
	if (del.wrtPtr - &del.d_buffer[0] > MAX_BUF_SIZE - 1) {
		del.wrtPtr = &del.d_buffer[0];
	}

	return *outputbuffer;
};