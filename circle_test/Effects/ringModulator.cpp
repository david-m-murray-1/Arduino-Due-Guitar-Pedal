#include "ringModulator.h"

float mod_phase=0;

void RingModulator::setFs(float Fs){
  this->Fs = Fs;
}

void RingModulator::setFc(float Fc){
  this->Fc = Fc;
}

//processing samples
void RingModulator::RingModulation_process_samples(int circle_buffer)
{
      outputbuffer[bufptr]= 0.005 * circle_buffer * (1 + Fs*sin(mod_phase) );
      mod_phase+=Fc*2*M_PI/SAMPLERATE;                                                    
}
