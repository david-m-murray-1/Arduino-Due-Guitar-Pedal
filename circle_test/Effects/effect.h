#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "pch.h"
#include "audio_io.h"

#define SAMPLERATE		44100
#define NROFCHANNELS		1
#define FRAMESPERBUFFER		256


class Effect
{
public:
  Effect(); // constructor for base class
  virtual ~Effect(); // destructor for base class
  virtual void process_samples(float *inputbuffer,float *outputbuffer);
private:
  static int bufptr = 0;
  unsigned long framesperbuffer;
};

#endif // _EFFECT_H_

