#include "pch.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include "stereodynamics.h"
#include "distortion.h"
#include "ringbuffer2.h"
#include "ringModulator.h"


// additional buffer to hold samples to be linearly normalized. apply normalization 
extern double InputSignal_f32_1kHz_15kHz[320];

using namespace std;

int bufptr = 0;
double inputbuffer_left[345] = { 0 };
double inputbuffer_right[345] = { 0 };
double outputbuffer_left[345] = { 0 };
double outputbuffer_right[345] = { 0 };

int bit_depth = 32;

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
double gain = 2;
double level = 10;


int main(void) {
	Stereo stereo_left;
	Stereo stereo_right;

	volatile int Effect = 1;

	Distortion Distortion;
	Distortion.setDepth(1);
	Distortion.setTimbre(1);
	RingModulator RingModulation;
	RingModulation.setFc(440);
	RingModulation.setFs(300);
	/*Tremolo Tremolo;
	Tremolo.setRate();
	Tremolo.setDepth();
	Reverb Reverb;
	Reverb.set();
	Reverb.set();
	*/
	circular_buffer<double> circle_left(345);
	circular_buffer<double> circle_right(345);

	while (bufptr < 345) {
		// circle_left.put(Hifi.read());
		circle_left.put(InputSignal_f32_1kHz_15kHz[bufptr++]);		// load buffer	'
		circle_right.put(InputSignal_f32_1kHz_15kHz[bufptr++]);		// load buffer	
		// linearly normalize input
		inputbuffer_left[bufptr] = circle_left.get_head();

		if (bufptr > 300) {
			inputbuffer_left[bufptr] = (circle_left.get_head()) / (*max_element(begin(inputbuffer_left), end(inputbuffer_left)) * level);
		}

		circle_right.put(InputSignal_f32_1kHz_15kHz[bufptr++]);		// load buffer	
		// linearly normalize input
		inputbuffer_right[bufptr] = (circle_right.get_head()) / (*max_element(begin(inputbuffer_right), end(inputbuffer_right)) * level);

		cout << "input: " << inputbuffer_left[bufptr] << endl;

		switch (Effect) {
		case 1:
			Distortion.process_samples(&inputbuffer_left[0], &outputbuffer_left[0], bufptr);
			Distortion.process_samples(&inputbuffer_right[0], &outputbuffer_right[0], bufptr);
			break;
		case 2:
			RingModulation.process_samples(&inputbuffer_left[0], &outputbuffer_right[0], bufptr);
			RingModulation.process_samples(&inputbuffer_right[0], &outputbuffer_left[0], bufptr);
			break;
		case 3:
			Tremolo.process_samples(&inputbuffer_left[0], &outputbuffer_left[0], bufptr);
			Tremolo.process_samples(&inputbuffer_right[0], &outputbuffer_right[0], bufptr);
			break;
		case 4:
			Reverb.process_samples(&inputbuffer_left[0], &outputbuffer_left[0], bufptr);
			Reverb.process_samples(&inputbuffer_right[0], &outputbuffer_right[0], bufptr);
			break;
		default:
			outputbuffer_left[bufptr] = inputbuffer_left[bufptr];
			outputbuffer_right[bufptr] = inputbuffer_right[bufptr];
			break;
		}
		// stereo dynamics
/*
 |     ,.----.._ | :::  -attack |
 |   ,`:: / / / o'''.| ///  - sustain |
 |  , ::::/ / / / oooooo`'..| ooo - release |
 | ,::::: / / / ooooooooo`''.| ________________ |
 | _:::::/ / / / oooooooooooo`'''.____________________time
*/
		stereo_left.rms_amplitude(&outputbuffer_left[bufptr]);
		stereo_left.rms_dB(rms_amplitude);
		// positve comp slope
		stereo_left.comp_scale(rms_dB);
		// negative comp slope
		stereo_left.exp_scale(rms_dB);
		//min_amplitude(outputbuffer_left, outputbuffer_left) = *min_element(begin(outputbuffer_left), end(outputbuffer_left));
		stereo_left.min_amplitude(outputbuffer_left, outputbuffer_left);

		if (stereo_left.scaling_factor(min_amplitude) < gain) {
			stereo_left.gain = (1 - stereo_left.attack) * stereo_left.gain + (stereo_left.attack * stereo_left.scaling_factor);
		} else {
			stereo_left.gain = (1 - stereo_left.release) * stereo_left.gain + (stereo_left.release * stereo_left.scaling_factor);
		}

		outputbuffer_left[bufptr] = stereo_left.gain * outputbuffer_left[bufptr];
		circle_left.put_back(outputbuffer_left[bufptr]);
		cout << circle_left.get() << endl;
	}
}
