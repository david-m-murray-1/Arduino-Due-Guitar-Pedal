# Arduino-Due-Guitar-Pedal

ARDUINO DUE 24-bit voltage resolution guitar pedal.
Cirrus CS4271 I2S enabled audio codec. 
I2C OLED Mount to display power on mesage and current effect selection.
48 kHz sampling rate.
Dynamically scaled output.

Due spec:
80Mhz cpu clock cpeed
1666 instructions per cycle at 48000 khz

TODO:
Set I2C OLED priority as highest level.  This is done so that the effect can be changed whenever the user desires, and is not interrupted by the I2S or SPI NVIC.  

Resize PCB in Eagle.  

Get guitar jack foot print for Eagle.
