#pragma once
#include <complex>
#include <valarray>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <direct.h>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include "circle.h"
#include "SlidingDFT.h"

using namespace std;

int bufptr = 1;
std::string current_working_directory()
{
	char* cwd = _getcwd(0, 0); // **** microsoft specific ****
	std::string working_directory(cwd);
	std::free(cwd);
	cout << working_directory << endl;
	return working_directory;
}


extern double InputSignal_f32_1kHz_15kHz[1280];

const double PI = 3.141592653589793238460;
typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
void fft(CArray &x);
void ifft(CArray &x);

int main() {
	current_working_directory();
	ofstream fs;
	string filename = "InputSignal_data_320.csv";
	fs.open(filename);

	complex<double> dft_x[345];
	complex<double> conj_dft_x[345];
	complex<double> conj_sdft_x[345];
	complex<double> inputbuffer_complex[345];
	double inputbuffer[345];
	double outputbuffer[345];
	double data[320];

	// Use double precision arithmetic and a 320-length DFT
	static SlidingDFT<double, 100> sdft;
	static SlidingDFT<double, 100> isdft;
	circular_buffer<double> circle(345);

	/*
	for (int i = 0; i <= 320; i++) {
		data[i] = InputSignal_f32_1kHz_15kHz[i];
		data[i] = static_cast<double>(data[i]);
		cout << data[i] << endl;
		fs << data[i] << endl;
	}
	fs.close();
	*/
	while (bufptr < 1245) {
		/* load data and update dft*/
		circle.put(InputSignal_f32_1kHz_15kHz[bufptr]);		// load buffer
		inputbuffer[bufptr] = circle.get_head();
		sdft.update(inputbuffer[bufptr]);
		cout << "time domain original: " << inputbuffer[bufptr] << endl;
		dft_x[bufptr] = sdft.dft[bufptr];
		cout << "complex vector output: " << dft_x[bufptr] << endl;

		inputbuffer_complex[bufptr] = inputbuffer[bufptr];
		CArray fft_data(inputbuffer_complex, 320);
		fft(fft_data);
		cout << "fft algorithm output" << fft_data[bufptr] << endl;


		ifft(fft_data);
		cout << "time domain output" << fft_data[bufptr] << endl << endl;


		//conj_isdft_x[bufptr] = conj(isdft.dft[bufptr]);
		//cout << "time domain" << conj_isdft_x[bufptr] << endl;
		
		/* ifft ==> conjugate -> fft -> conjugate -> out */

		
		/* apply allpass filter */

		/* apply 90 degree phase shift */

		/* f_osc x sin */

		/* f_osc x cos */

		/* add sin and cos signals */

		/* convert back to time domain ==> isdft */
		outputbuffer[bufptr] = circle.get();


		/* save sample to file stream */

		bufptr++;
	}
}

void fft(CArray &x)
{
	// DFT
	unsigned int N = x.size(), k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				Complex t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	Complex f = 1.0 / sqrt(N);
	for (unsigned int i = 0; i < N; i++)
		x[i] *= f;
}

// inverse fft (in-place)
void ifft(CArray& x)
{
	// conjugate the complex numbers
	x = x.apply(std::conj);

	// forward fft
	fft(x);

	// conjugate the complex numbers again
	x = x.apply(std::conj);

	// scale the numbers
	x /= x.size();
}