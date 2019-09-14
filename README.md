# Arduino-Due-Guitar-Pedal

ARDUINO DUE 24-bit voltage resolution guitar pedal.
Cirrus CS4271 I2S enabled audio codec. 
48 kHz sampling rate.
Dynamically scaled (RMS scaling dynamic range compression), logarithmic output (dB).
Circularly buffered.

The codec is actually using 32-bits to transfer data, but only 24 of these bits define the voltage

Depending on which mode you have put the CS4271 audio codec in, it may require a 22MHz Crystal-not an 11MHz crystal

To use this code, refer to: http://www.cloud-rocket.com/2014/05/programming-arduino-due-atmel-studio-asf/ to compile the sketch with its libraries.

True bypass with 3PDT switch and LED bypass indicator.

TODO: Sine tables for effects to get rid of sine computation during play

TODO: I2C for OLED screen.  I am currently using the ASF to write the I2C protocol routine to write to an OLED display, as was done for the other parts of this project.  I will be updating the code periodically to move away from the ASF to writing to the DMA & I2C registers directly.

TODO: Shield PCB.  Once the pedal testing is finished, the pedal schematics in multisim will be ported to Ultiboard, and a board house will print the board.
