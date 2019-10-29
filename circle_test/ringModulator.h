#ifndef _RINGMODULATOR_H_
#define _RINGMODULATOR_H_
#include <iostream>
#include <math.h>
#include <complex.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define SAMPLERATE 96000;

class RingModulator: public Effect
{
public:
  void setFs(double Fs);
  void setFc(double Fc);
  double process_samples(double *inputbuffer, double *outputbuffer, int bufptr);
  double Fc = 440;
  double Fs = 300;
};
#endif
