# Arduino-Due-Guitar-Pedal

ARDUINO DUE 24-bit resolution guitar pedal.
I2S comm. with Cirrus CS4271 audio codec. 
48 kHz sampling rate.
Dynamically scaled (RMS scaling dynamic range compression), logarithmic output (dB).
Circularly buffered.

To use this code, refer to: http://www.cloud-rocket.com/2014/05/programming-arduino-due-atmel-studio-asf/ to compile the sketch with its libraries.

True bypass with 3PDT switch and LED bypass indicator.

TODO: Sine tables for effects to get rid of sine computation during play

TODO: I2C for OLED screen.

TODO: Shield PCB.  Once the pedal testing is finished, the pedal schematics in multisim will be ported to Ultiboard, and a board house will print the board.  The pedal will have a glass case and an OLED screen mounted on top to display the pedal effect in use.  LED effect indicators will still be used.
