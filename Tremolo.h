#pragma once
#ifndef _TREMOLO_H_
#define _TREMOLO_H_
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

class Tremolo {
private:
	double phase = 0; // modulation phase index
	double ModulationSignal_Frequency;
	double samplerate;
	double Depth;

public:
	Tremolo() {};
	~Tremolo() {};

	Tremolo(double samplerate, double Depth, double ModulationSignal_Frequency) {
		setSampleRate(samplerate);
		setModulationDepth(Depth);
		setModulationFrequency(ModulationSignal_Frequency);
	}

	double process_sample(double sample) {
		double output = sample * ModSignal();
		return output;
	}

	double ModSignal() {
		increment_phase();
		this->phase = fmod(this->phase, 2 * M_PI);
		double Modded_signal = (1 - this->Depth) + this->Depth * (sin(this->phase));
		return Modded_signal;
	}

	void increment_phase() {
		this->phase = this->phase + (2 * M_PI * this->ModulationSignal_Frequency / this->samplerate);
		if (this->phase > 3.14) {
			this->phase = (-1)*this->phase;
		}
	}

	void setSampleRate(double samplerate) {
		this->samplerate = samplerate;
	}

	void setModulationDepth(double Depth) {
		this->Depth = Depth;
	}

	void setModulationFrequency(double ModulationSignal_Frequency) {
		this->ModulationSignal_Frequency = ModulationSignal_Frequency;
	}
};

#endif
