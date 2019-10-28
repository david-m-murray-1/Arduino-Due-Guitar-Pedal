#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "pch.h"
#include "audio_io.h"

#define SAMPLERATE		44100
#define NROFCHANNELS		1
#define FRAMESPERBUFFER		256


class Effect
{
protected:
  static int bufptr = 0;
public:
  Effect(); // constructor for base class
  virtual ~Effect(); // destructor for base class
  virtual void process_samples(float *inputbuffer,float *outputbuffer, bufptr);
  void reset_bufptr();
};

#endif // _EFFECT_H_

