#pragma once
#ifndef _DELAY_H_
#define _DELAY_H_

#define MAX_BUF_SIZE 1



class Delay {
public:
	void set_dfb(double val);
	void set_dfw(double val);
	void set_dmix(double val);
	void set_delay(double n_delay);
	double set_dfb(void);
	double set_dfw(void);
	double set_dmix(void);
	double get_dfb(void);
	double get_dfw(void);
	double get_dmix(void);
	double Delay_task(double *inputbuffer, double *outputbuffer, int bufptr);

	double d_buffer[MAX_BUF_SIZE];
	struct fract_delay {
		double d_mix;       /*delay blend parameter*/
		short d_samples;	/*delay duration in samples*/
		double dfb;	    /*feedback volume*/
		double dfw;	    /*delay tap mix volume*/
		double n_fract;     /*fractional part of the delay*/
		double *rdPtr;      /*delay read pointer*/
		double *wrtPtr;     /*delay write pointer*/
		void Delay_init(double delay_samples, double dfb, double dfw, double dmix);
	} del;

	// delay_task variables
	double * y0;
	double * y1;
	double x1;
	double x_est;
};
#endif
