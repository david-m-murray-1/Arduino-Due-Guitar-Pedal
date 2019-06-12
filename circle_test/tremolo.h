#ifndef _TREMOLO_H_
#define _TREMOLO_H_

class Tremolo {
public:
	double rate;
	double depth;
	double mod;
	double control;
	double offset;
	double process_samples(double *inputbuffer, double *outputbuffer, int bufptr);
	void sweep(void);
	void setRate(double rate);
	void setDepth(double depth);
};

#endif
