#include "ringModulator.h"

double mod_phase=0;

void RingModulator::setFs(double Fs){
  this->Fs = Fs;
}

void RingModulator::setFc(double Fc){
  this->Fc = Fc;
}

//processing samples
double RingModulator::process_samples(double *inputbuffer, double *outputbuffer, int bufptr)
{
      outputbuffer[bufptr] = 0.005 * inputbuffer[bufptr] * (1 + Fs*sin(mod_phase));
      mod_phase+=Fc*2*M_PI/SAMPLERATE;
	  return *outputbuffer;
}
