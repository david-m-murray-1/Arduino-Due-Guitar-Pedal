#include "pch.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include "distortion.h"
#include "ringbuffer2.h"

// additional buffer to hold samples to be linearly normalized. apply normalization 
extern double InputSignal_f32_1kHz_15kHz[320];

using namespace std;

int bufptr = 0;
double inputbuffer[345] = { 0 };
double outputbuffer[345] = { 0 };
int bit_depth = 32;
double max;

double rms_amplitude = 0;
double rms_width = 0.1;
double rms_dB = 0;
double min_amplitude = 0;
double max_amplitude = 0;
double comp_scale;
double comp_slope = 0.3;
double comp_threshold = -15;
double exp_threshold = -25;
double exp_slope = -0.05;
double attack = 0.05;
double release = 0.005;
double scaling_factor;
double exp_scale;
double gain;
double level = 10;


int main(void) {
	volatile int Effect = 1;
	Distortion Effect1;
	Effect1.setDepth(1);
	Effect1.setTimbre(1);
	/*RingModulator Effect2;
	Effect2.setFc(440);
	Effect2.setFs(300);
	*/
	circular_buffer<double> circle(345);
	while (bufptr < 345) {
		gain = 2;
		circle.put(InputSignal_f32_1kHz_15kHz[bufptr]);		// load buffer	
		// linearly normalize input
		inputbuffer[bufptr] = (circle.get_head()); 
		if (bufptr > 30) {
			inputbuffer[bufptr] = (inputbuffer[bufptr]) / (*max_element(begin(inputbuffer), end(inputbuffer)) * level);
		}
		cout << "input: " << inputbuffer[bufptr] << endl;
		switch (Effect) {
		case 1:
			Effect1.process_samples(&inputbuffer[0], &outputbuffer[0], bufptr);
			cout << "effect output: " << outputbuffer[bufptr] << endl;
			break;
		case 2:
			// Hifi.write(Effect2.process_samples(circle.get());
			//cout << Effect2.process_samples(circle.get()) << endl;
			break;
		default:
			// Hifi.write(circle.get());
			break;
		}
		// stereo dynamics
/*
 |     ,.---..._ | :::  -attack |
 |   ,`:: / / / o'''.| ///  - sustain |
 |  , ::::/ / / / oooooo`'..| ooo - release |
 | ,::::: / / / ooooooooo`''.| ________________ |
 | _:::::/ / / / oooooooooooo`'''.____________________time
*/
		rms_amplitude = (1 - rms_width) * rms_amplitude + (rms_width * outputbuffer[bufptr]);
		rms_dB = 10 * log10(rms_amplitude); // Hifi.write(Effect2.process_samples(circle.get());
		// positve comp slope
		comp_scale = comp_slope * (comp_threshold - rms_dB);
		// negative comp slope
		exp_scale = exp_slope * (exp_threshold - rms_dB);
		min_amplitude = *min_element(begin(inputbuffer), end(inputbuffer));
		scaling_factor = pow(10, min_amplitude);
		if (scaling_factor < gain) {
			gain = (1 - attack) * gain + (attack * scaling_factor);
		} else {
			gain = (1 - release) * gain + (release * scaling_factor);
		}
		outputbuffer[bufptr] = gain * outputbuffer[bufptr];
		cout << "output: " << outputbuffer[bufptr] << endl;
		circle.put(outputbuffer[bufptr]); 
		cout << "get_head: " << circle.get_head() << endl;
		cout << "get_head to head ratio: " << circle.get_head() / circle.get() << endl;
		bufptr++;
	}
}
