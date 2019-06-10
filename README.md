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


TODO: class for stereo dynamics. class for linear normalization
TODO: finish ringModulator header and cpp


Nearly finished with the accompanying pedal shield schematic.
TODO: Ultiboard footprints for parts: 2x stereo jacks

TODO: 96kHz sampling rate.
PWM clock set to 96khz.  Change Crystal to 22.2MHz.
