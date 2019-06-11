#ifndef _TREMOLO_H_
#define _TREMOLO_H_

class Tremolo {
public:
	static double rate;
	static double depth;
	static double mod;
	static double control;
	static double offset;
	double process_samples(double *inputbuffer, double *outputbuffer, int bufptr);
	void sweep(void);
	void setRate(double rate);
	void setDepth(double depth);

};

#endif
