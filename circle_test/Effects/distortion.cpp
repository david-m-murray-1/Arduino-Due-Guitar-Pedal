#include "distortion.h"


void Distortion::setTimbre(double timbre){
  this->timbre = timbre;
}

void Distortion::setDepth(double depth){
  this->depth = depth;
}

//processing samples
double Distortion::process_samples(double *inputbuffer, double *outputbuffer, int bufptr, int mode)
	switch(mode){
		Case 1:	//linear_distortion
			inputbuffer[bufptr] = inputbuffer[bufptr] * depth;
			inputbuffer[bufptr] = tanh((inputbuffer[bufptr] * (timbre + 1)));
			inputbuffer[bufptr] = (inputbuffer[bufptr] * ((0.1 + timbre) * timbreInverse));
			inputbuffer[bufptr] = cos((inputbuffer[bufptr] + (timbre + 0.25)));
			inputbuffer[bufptr] = tanh(inputbuffer[bufptr] * (timbre + 1));
			inputbuffer[bufptr] = inputbuffer[bufptr] * 0.125;
			outputbuffer[bufptr] = inputbuffer[bufptr];
			return outputbuffer[bufptr];
			break;
		Case 2:	// Non-linear distortion
			outputbuffer[bufptr] = (inputbuffer[bufptr]/fabs(inputbuffer[bufptr])) * (1-exp((alpha*(inputbuffer[bufptr]*inputbuffer[bufptr])/fabs(inputbuffer[bufptr]))));
			break;			
	}
}
