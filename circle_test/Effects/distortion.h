#ifndef _DISTORTION_H_
#define _DISTORTION_H_
#include <math.h>
#include <complex.h>
#include <iostream>

class Distortion
{
public:
  void setTimbre(double timbre);
  void setDepth(double depth);
  double process_samples(double *inputbuffer, double *outputbuffer, int bufptr);

  float timbre = 1;
  float depth = 1;
  float timbreInverse = 1;


}; // distortion

#endif
