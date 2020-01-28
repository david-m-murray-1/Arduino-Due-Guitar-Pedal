#pragma once
#ifndef _TREMOLO_H_
#define _TREMOLO_H_
#define _USE_MATH_DEFINES
#include <cmath>

class Tremolo {
private:
	double phase = 0; // modulation phase index
	double ModulationSignal_Frequency;
	double samplerate;

public:
	Tremolo();
	~Tremolo();

	Tremolo(double samplerate, double Depth, double ModulationSignal_Frequency) {
		setModulationDepth(Depth);
		setModulationFrequency(double ModulationSignal_Frequency);
	}

	double process_sample(double sample) {
		double output = sample * ModSignal();
		return output;
	}

	double ModSignal() {
		increment_phase();
		this->phase = this->phase % 2 * M_PI;
		double Modded_signal = (1 - this->Depth) + this->Depth * (sin(this->phase * this->ModulationSignal_Frequency));
		return Modded_signal;
	}

	void increment_phase() {
		this->phase = this->phase + (2 * M_PI * this->ModulationSignal_Frequency / this->samplerate);
	}

	double setModulationDepth(double Depth) {
		this->Depth = Depth;
	}

	double setModulationFrequency(double ModulationSignal_Frequency) {
		this->ModulationSignal_Frequency = ModulationSignal_Frequency;
	}
};

#endif
