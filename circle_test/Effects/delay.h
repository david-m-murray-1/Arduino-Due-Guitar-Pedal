#include "Effect.h"

#define DELAYBUFFERSIZE 44100

class Delay : public Effect
{
private:
  int delayTime = 5000;
  int input = 0;
  int output = 0;
  float feedback = 0.5;
public:
  //functions
  void setDelayTime(float delayTime);
  void setFeedback(float feedback);
  void process_samples(float *inputbuffer, float *outputbuffer, int bufptr);
  float delayBuffer[DELAYBUFFERSIZE]; // 

};
