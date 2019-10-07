#include "effect.h"
#include <math.h>
#include <complex.h>
#include <iostream>



class Distortion : public Effect
{
private:
  float timbre = 1;
  float timbreInverse;
  float depth = 1;
  int bypass = 1;
public:
  void setTimbre(float timbre);
  void setDepth(float depth);
  void Distortion_process_samples(float *inputbuffer, float *outputbuffer);

}; // distortion
