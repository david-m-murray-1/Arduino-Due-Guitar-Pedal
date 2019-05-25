# Arduino-Due-Guitar-Pedal

ARDUINO DUE 24-bit resolution guitar pedal.
Cirrus CS4271 audio codec. 
I2S Comm. with codec.

TODO: Create ring buffer
https://en.wikipedia.org/wiki/Circular_buffer

https://www.silabs.com/community/blog.entry.html/2016/10/18/high_quality_audiow-tiTN
must set master clock: 11.2896 MHz    LRCLK: 44.1 kHz
https://microchip.wdfiles.com/local--files/8bit%3Alp/crystal.png

parts: OLED SCREEN: https://www.adafruit.com/product/326
       11.2896 MHz CRYSTAL: https://www.mouser.com/ProductDetail/IQD/LFXTAL016562Reel?qs=sGAEpiMZZMsBj6bBr9Q9aR%2FuGiDjvlISludcXAEXB8flK7hy2VGqhg%3D%3D 
       74HC_HCT04 HEX INVERTER https://assets.nexperia.com/documents/data-sheet/74HC_HCT04.pdf

ASF Source Code documentation: https://asf.microchip.com/docs/latest/

4 potentiometers to change effect parameters

4 digital effects: Distortion, Ring Modulator, Tremolo, and Reverb.


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



