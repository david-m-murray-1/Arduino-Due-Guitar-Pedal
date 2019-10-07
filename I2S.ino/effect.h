#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "lib/audio_IO/audio_io.h"

#define SAMPLERATE		44100
#define NROFCHANNELS		1
#define FRAMESPERBUFFER		256


class Effect
{
private:
  volatile int bufptr;
  unsigned long framesperbuffer;
public:
  Effect(); // constructor for base class
  virtual ~Effect(); // destructor for base class
  virtual void process_samples(float *inputbuffer,float *outputbuffer, volatile int bufptr);
};

#endif // _EFFECT_H_

