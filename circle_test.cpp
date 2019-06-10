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
double inputbuffer[256];
double outputbuffer[256];
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


int main(void) {
	volatile int Effect = 1;
	Distortion Effect1;
	Effect1.setDepth(1);
	Effect1.setTimbre(1);
	/*RingModulator Effect2;
	Effect2.setFc(440);
	Effect2.setFs(300);
	*/
	circular_buffer<uint32_t> circle(bit_depth*size(InputSignal_f32_1kHz_15kHz));
	while (1) {
		//getline(cin, Effect);
		circle.put(InputSignal_f32_1kHz_15kHz[bufptr]);		// load buffer
		inputbuffer[bufptr] = static_cast<double>(circle.get_head());
		// linearly normalize input
		inputbuffer[bufptr] = inputbuffer[bufptr] / *max_element(begin(inputbuffer), end(inputbuffer));

		switch (Effect) {
		case 1:
			Effect1.process_samples(inputbuffer, outputbuffer, bufptr);
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
 |   , ': / / / o'''.| ///  - sustain |
 |    :::/ / / / oooooo`'..| ooo - release |
 | ,::::: / / / ooooooooo`''.| ________________ |
 | _:::::/ / / / oooooooooooo`'''.____________________time
*/
		rms_amplitude = (1 - rms_width) * rms_amplitude + (rms_width * outputbuffer[bufptr]);
		rms_dB = 10 * log(rms_amplitude); // Hifi.write(Effect2.process_samples(circle.get());
		// positve comp slope
		comp_scale = comp_slope * (comp_threshold - rms_dB);
		// negative comp slope
		exp_scale = exp_slope * (exp_threshold - rms_dB);
		min_amplitude = *min_element(begin(outputbuffer), end(outputbuffer));
		scaling_factor = pow(10, min_amplitude);
		if (scaling_factor < gain) {
			gain = (1 - attack) * gain + (attack * scaling_factor);
		} else {
			gain = (1 - release) * gain + (release * scaling_factor);
		}
		outputbuffer[bufptr] = gain * outputbuffer[bufptr];
		cout << outputbuffer[bufptr] << endl;
		bufptr++;
	}
}
