#include "distortion.h"


void Distortion::setTimbre(float timbre){
  cout << bypass;
  this->timbre = timbre;
}

void Distortion::setDepth(float depth){
  this->depth = depth;
}

//processing samples
void Distortion::Distortion_process_samples(float *inputbuffer, float *outputbuffer)
{
    timbreInverse = (1 - (timbre * 0.099)) * 10; //inverse scaling from timbre
      inputbuffer[bufptr] = inputbuffer[bufptr] * depth;                               
      inputbuffer[bufptr] = tanh((inputbuffer[bufptr] * (timbre + 1)));                
      inputbuffer[bufptr] = (inputbuffer[bufptr] * ((0.1 + timbre) * timbreInverse));  
      inputbuffer[bufptr] = cos((inputbuffer[bufptr] + (timbre + 0.25)));              
      inputbuffer[bufptr] = tanh(inputbuffer[bufptr] * (timbre + 1));                  
      inputbuffer[bufptr] = inputbuffer[bufptr] * 0.125;                      
      
      outputbuffer[bufptr] = inputbuffer[bufptr];
    }
  }
  

}
