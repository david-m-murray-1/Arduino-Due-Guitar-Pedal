// Signal Mean Algorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include <cmath>
#include "waveforms.cpp"
#define ENABLE_DEBUG
#define SIG_LENGTH 320

using namespace std;

int matrix[5][4] = { 0, 1, 1, 0, -1, 0, 0, -1, 1, 0, 0, -1, 0, 1, -1, 0 };
// 0, 1, 1, 0, -1, 0, 0, -1, 1, 0, 0, -1, 0, 1, -1, 0
int decaylines[4] = { 887, 1279, 2089, 3167 };

struct Signal {
	int signal[signal_length];
	double level;
	double sampling_freq;
	int decaylines;
	int outDecays;
	int inDecays;
	double decay_lo;
	double decay_hi;
	int post_lp_cut;
	int pre_lp_cut;
};

int ones_array[4] = { 1 };
r_lo = 1 - 

extern double InputSignal_f32_1kHz_15kHz[SIG_LENGTH];

double calc_signal_mean(double *sig_src_arr, int sig_length);

double calc_signal_variance(double *sig_src_arr, double sig_mean, int sig_length);

double signal_mean;
double signal_variance;

int main()
{
#ifdef ENABLE_DEBUG
	/*signal_mean = calc_signal_mean(&InputSignal_f32_1kHz_15kHz[0], SIG_LENGTH);
	cout << &InputSignal_f32_1kHz_15kHz[0];
	cout << "\n\nMean =" << signal_mean << endl;
	signal_variance = calc_signal_variance(&InputSignal_f32_1kHz_15kHz[0], signal_mean, SIG_LENGTH);
	cout << "\nVariance =" << signal_variance << endl;
	*/
	cout << matrix[1][0] << endl;
	cout << size(matrix[1]) << endl;
#endif
	return 0;
}

double calc_signal_mean(double *sig_src_arr, int sig_length) {
	double _mean = 0.0;
	cout << sig_src_arr << endl;
	for (int i = 0; i < sig_length; i++) {
		_mean = _mean + sig_src_arr[i];
		cout << _mean << endl;
	}
	cout << sig_src_arr;
	_mean = _mean / (double)sig_length;
	cout << _mean;

	return _mean;
}

double calc_signal_variance(double *sig_src_arr, double sig_mean, int sig_length) {
	double _variance = 0.0;
	cout << sig_src_arr << endl;
	for (int i = 0; i < sig_length; i++) {
		_variance = _variance * pow((sig_src_arr[i] - sig_mean), 2);
	}

	_variance = _variance / (sig_length - 1);

	return _variance;
}


