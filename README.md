# Arduino-Due-Guitar-Pedal

ARDUINO DUE 24-bit resolution guitar pedal.
I2S comm. with Cirrus CS4271 audio codec. 
96 kHz sampling rate.
Dynamically scaled (RMS scaling dynamic range compression), logarithmic output (dB).
Circularly buffered.
True bypass with 3PDT switch and LED bypass indicator.

may apply normalization to the input instead of the output.  Or at both places.  I.e.: Linearly scale, then RMS scaling at the end.

Dynamic Range Compression:
  Reduces the volume of loud sounds or amplifies quiet sounds thus reducing or compressing an audio signal's dynamic range.
  
Sine table for flanger.

TODO: test analog output buffers. cannot use negative voltage rails with arduino.

TODO: Metal casing for guitar pedal.

TODO: Glass casing for guitar pedal.

TODO: Test Hardware interface. Test codec for 48 kHz and 96 kHz.
