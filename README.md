# Arduino-Due-Guitar-Pedal

ARDUINO DUE 24-bit voltage resolution guitar pedal.
Cirrus CS4271 I2S enabled audio codec. 
Digital Signal Processor (FPGA) for DSP work load.
I2C OLED Mount to display power on mesage and current effect selection.
48 kHz sampling rate.
Dynamically scaled.

DSP slave communicates with the Due master via SPI to handle the signal processing math.  The Due grabs a sample from the codec and sends it to the DSP.  The DSP dynamically scales the signal, performs FFT, modifies the sample, performs IFFT, then sends the sample back to the due.  The modded sample is sent out via I2S back to the codec and is output on its DAC.

TODO:
Sync DSP with the codec timing to perform just one iteration of calculations on each sample, then wait for a new sample.
Set I2C OLED priority as highest level.  This is done so that the effect can be changed whenever the user desires, and is not interrupted by the I2S or SPI NVIC.
Program the DSP in Verilog HDL.  
Research 32-bit FFT calculations in Verilog.
