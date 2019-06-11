#pragma once
#ifndef __DELAY_H__
#define __DELAY_H_

class Delay {
public:
	void Delay_Init(double delay_samples, double dfb, double dfw, double dmix);
	void Delay_set_fb(double val);
	void Delay_set_fw(double val);
	void Delay_set_mix(double val);
	void Delay_set_delay(double n_delay);
	double Delay_get_fb(void);
	double Delay_get_fw(void);
	double Delay_get_mix(void);
	double Delay_task(double xin);
private:
	double d_buffer[MAX_BUF_SIZE];
	struct fract_delay {
		double d_mix;       /*delay blend parameter*/
		short d_samples;	/*delay duration in samples*/
		double d_fb;	    /*feedback volume*/
		double d_fw;	    /*delay tap mix volume*/
		double n_fract;     /*fractional part of the delay*/
		double *rdPtr;      /*delay read pointer*/
		double *wrtPtr;     /*delay write pointer*/
	}
};
#endif
