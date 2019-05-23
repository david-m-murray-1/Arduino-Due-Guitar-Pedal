# Arduino-Due-Guitar-Pedal

ARDUINO DUE 24-bit resolution guitar pedal.
Cirrus CS4271 audio codec. 
I2S Comm. with codec.

https://www.silabs.com/community/blog.entry.html/2016/10/18/high_quality_audiow-tiTN
must set master clock: 11.2896 MHz    LRCLK: 44.1 kHz

parts: https://www.adafruit.com/product/326

ASF Source Code documentation: https://asf.microchip.com/docs/latest/

4 potentiometers to change effect parameters

4 digital effects: Distortion, Ring Modulator, Tremolo, and Reverb.

TODO: LINEARLY NORMALIZE INPUT SIGNAL.  THEN ADD EFFECTS.

TODO: MATLAB: Find TFs for effects
  find num and denom of TF
  find s transform
  find z transform (fn: c2d) convert from continuous model to discrete time
  convert to c

TODO: research parameters for effects
TODO: add potentiometers as inputs to process_samples functions for each effect

DONE: OLED screen
TODO: in C: access registers for I2C master/slave comm. with OLED screen

https://stackoverflow.com/questions/56196903/how-do-i-make-an-external-interrupt-in-c-on-arduino-due-sam3x8e-using-only-t

https://en.wikipedia.org/wiki/Ring_modulation
https://en.wikibooks.org/wiki/Sound_Synthesis_Theory/Modulation_Synthesis

http://www.ti.com/lit/ml/sprp499/sprp499.pdf
http://ee.bradley.edu/projects/proj2008/gegudps/Czubak_Raheja_Final_Paper.pdf

echo and reverb direct transforms
https://www.eurasip.org/Proceedings/Eusipco/Eusipco2006/papers/1568980587.pdf

more reverb. direct transform II
https://ccrma.stanford.edu/~jos/pasp/History_FDNs_Artificial_Reverberation.html

Schematic
https://www.analog.com/en/analog-dialogue/articles/high-speed-amplifiers-make-clock-buffers.html



