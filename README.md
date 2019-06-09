# Arduino-Due-Guitar-Pedal

ARDUINO DUE 24-bit resolution guitar pedal.
I2S comm. with Cirrus CS4271 audio codec. 
48 kHz sampling rate.
Linearly normalized across samples.
Circularly buffered.

sample in.
Linearly normalize every x samples, retaining the previous samples maximum amplitude.  Update according to current max amplitude.
Modify signal based on effect.
Apply stereo dynamics to dynamically scale the output.
output sample, value limited by volume POT.



Nearly finished with the accompanying pedal shield schematic.
TODO: Ultiboard footprints for parts: 2x stereo jacks

TODO: 96kHz sampling rate.
PWM clock set to 96khz.  Change Crystal to 22.2MHz.

TODO: 2-channel true analog bypass. test impedance against digital bypass.
http://fuzzcentral.ssguitar.com/
http://giovanettihandwired.blogspot.com/2014/02/clonare-pedali-i-forum-la-morale-e_19.html
