# Arduino-Due-Guitar-Pedal

ARDUINO DUE 24-bit resolution guitar pedal.
I2S comm. with Cirrus CS4271 audio codec. 
96 kHz sampling rate.
Linearly normalized across all samples.
Circularly buffered.

sample in.
Linearly normalize every x samples, retaining the previous samples maximum amplitude.  Update according to current max amplitude.
Modify signal based on effect.
Apply stereo dynamics to dynamically scale the output.
output sample, value limited by volume POT.

TODO: Footswitch to enable/disable bypass.
TODO: finish flanger, tremolo, delay effects libraries

TODO: add footswitch to multisim and ultiboard.
TODO: verify pins used on Arduino and re-map CS4271 with the new footprint.
TODO: verify 28L TSSOP footprint on ultiboard
TODO: add grounds for PCB

TODO: Ultiboard footprints for parts: 2x stereo jacks

TODO: Change Crystal to 22.2MHz.

TODO: Metal casing for guitar pedal.
TODO: Glass casing for guitar pedal.
TODO: Test Hardware interface. Test codec for 48 kHz and 96 kHz.
