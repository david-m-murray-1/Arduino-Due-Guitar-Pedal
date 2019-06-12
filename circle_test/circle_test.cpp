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
#include "flanger.h"
#include "tremolo.h"
#include "delay.h"
#define POT2 1
#define POT3 1


// additional buffer to hold samples to be linearly normalized. apply normalization 
extern double InputSignal_f32_1kHz_15kHz[345];

using namespace std;

int bufptr = 0;
double inputbuffer_left[345] = { 0 };
double inputbuffer_right[345] = { 0 };
double outputbuffer_left[345] = { 0 };
double outputbuffer_right[345] = { 0 };

double min_amplitude;

int bit_depth = 32;


int main(void) {

	STEREO stereo_left;
	STEREO stereo_right;

	Distortion Distortion;
	Distortion.setDepth(1);
	Distortion.setTimbre(1);
	RingModulator RingModulation;
	RingModulation.setFc(440);
	RingModulation.setFs(300);
	Flanger Flanger;
	Tremolo Tremolo;
	Tremolo.setRate(POT2);
	Tremolo.setDepth(POT3);
	
	circular_buffer<double> circle_left(345);
	circular_buffer<double> circle_right(345);

	while (bufptr < 345) {
		// circle_left.put(Hifi.read());
		circle_left.put(InputSignal_f32_1kHz_15kHz[bufptr]);		// load buffer	'
		//circle_right.put(InputSignal_f32_1kHz_15kHz[bufptr++]);		// load buffer	
		// linearly normalize input
		inputbuffer_left[bufptr] = circle_left.get_head();

		cout << "input = .get_head(): " << inputbuffer_left[bufptr] << endl;

		/*if (bufptr > 300) {
			inputbuffer_left[bufptr] = (circle_left.get_head()) / (*max_element(begin(inputbuffer_left), end(inputbuffer_left)) * stereo_left.level);
		}
		else {
			inputbuffer_left[bufptr] = circle_left.get_head();
		}*/

		//circle_right.put(InputSignal_f32_1kHz_15kHz[bufptr++]);		// load buffer	
		// linearly normalize input
		//inputbuffer_right[bufptr] = (circle_right.get_head()) / (*max_element(begin(inputbuffer_right), end(inputbuffer_right)) * stereo_left.level);

		int Effect = 1;
		switch (Effect) {
		case 1:
			Distortion.setDepth(POT2);
			Distortion.setTimbre(POT3);
			Distortion.process_samples(&inputbuffer_left[0], &outputbuffer_left[0], bufptr);
			//Distortion.process_samples(&inputbuffer_right[0], &outputbuffer_right[0], bufptr);
			cout << "distortion effect output: " << outputbuffer_left[0] << endl;
			break;
		case 2:
			RingModulation.setFc(POT2);
			RingModulation.setFs(POT3);
			RingModulation.process_samples(&inputbuffer_left[0], &outputbuffer_right[0], bufptr);
			//RingModulation.process_samples(&inputbuffer_right[0], &outputbuffer_left[0], bufptr);
			break;
		case 3:
			Tremolo.setRate(POT2);
			Tremolo.setDepth(POT3);
			Tremolo.process_samples(&inputbuffer_left[0], &outputbuffer_left[0], bufptr);
			//Tremolo.process_samples(&inputbuffer_right[0], &outputbuffer_right[0], bufptr);
			break;
		case 4:
			Flanger.process_samples(&inputbuffer_left[0], &outputbuffer_left[0], bufptr);
			//Flanger.process_samples(&inputbuffer_right[0], &outputbuffer_right[0], bufptr);
			break;
		case 5:																								//////////////////// BYPASS ////////////////////////
			outputbuffer_left[bufptr] = inputbuffer_left[bufptr];
			//outputbuffer_right[bufptr] = inputbuffer_right[bufptr];*/
		default:
			cout << "broken case statement: " << endl;
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
		stereo_left.calc_rms_amplitude(&outputbuffer_left[0], bufptr, stereo_left.rms_width, stereo_left.rms_amplitude);
		stereo_left.calc_rms_dB(stereo_left.rms_amplitude, stereo_left.rms_dB);
		// positve comp slope
		stereo_left.calc_comp_scale(stereo_left.comp_slope, stereo_left.comp_threshold, stereo_left.rms_dB);
		// negative comp slope
		stereo_left.calc_exp_scale(stereo_left.exp_slope, stereo_left.exp_threshold, stereo_left.rms_dB);
		min_amplitude = *min_element(begin(outputbuffer_left), end(outputbuffer_left));;

		if (stereo_left.calc_scaling_factor(min_amplitude) < stereo_left.gain) {
			stereo_left.gain = (1 - stereo_left.attack) * stereo_left.gain + (stereo_left.attack * stereo_left.calc_scaling_factor(min_amplitude));
		} else {
			stereo_left.gain = (1 - stereo_left.release) * stereo_left.gain + (stereo_left.release * stereo_left.calc_scaling_factor(min_amplitude));
		}

		outputbuffer_left[bufptr] = stereo_left.gain * outputbuffer_left[bufptr];
		circle_left.put_back(outputbuffer_left[bufptr]);
		cout << "circle.get(): "<< circle_left.get_head() << endl;
		bufptr++;
	}
}
