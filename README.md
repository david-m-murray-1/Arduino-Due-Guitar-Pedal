# Arduino-Due-Guitar-Pedal

ARDUINO DUE 24-bit resolution guitar pedal.
Cirrus CS4271 audio codec. 
I2S Comm. with codec.
https://www.adafruit.com/product/326

4 potentiometers to change effect parameters

4 digital effects: Distortion, Ring Modulator, Tremolo, and Reverb.

TODO: effects

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

