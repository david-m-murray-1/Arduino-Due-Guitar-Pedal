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

extern double InputSignal_f32_1kHz_15kHz[1280];

std::string current_working_directory()
{
	char* cwd = _getcwd(0, 0); // **** microsoft specific ****
	std::string working_directory(cwd);
	std::free(cwd);
	cout << working_directory << endl;
	return working_directory;
}


const double PI = 3.141592653589793238460;
typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;
void fft(CArray &x);
void ifft(CArray &x);
void fft_conquer(CArray &x);

int main(int argc, const char* argv[]) {
	size_t bufptr = 1;
	/*current_working_directory();
	ofstream fs;
	string filename = "InputSignal_data_320.csv";
	fs.open(filename);
	*/
	complex<double> *dft_x = new complex<double>[1345];
	complex<double> *conj_dft_x = new complex<double>[1345];
	complex<double> *conj_sdft_x = new complex<double>[1345];
	complex<double> *inputbuffer_complex = new complex<double>[1345];
	complex<double> *ifft_data = new complex<double>[1345];
	double *inputbuffer = new double[1345];
	double *outputbuffer = new double[1345];


	//double *data = new double[1320];


	// Use double precision arithmetic and a 320-length DFT
	static SlidingDFT<double, 320> sdft;
	static SlidingDFT<double, 320> isdft;
	circular_buffer<double> circle(345);

	/*
	for (int i = 0; i <= 320; i++) {
		data[i] = InputSignal_f32_1kHz_15kHz[i];
		data[i] = static_cast<double>(data[i]);
		fs << data[i] << endl;
	}
	fs.close();
	*/
	while (bufptr < 380) {
		/* load data and update dft*/
		cout << "index: " << bufptr << endl;
		circle.put(InputSignal_f32_1kHz_15kHz[bufptr]);		// load buffer
		inputbuffer[bufptr] = circle.get_head();
		copy(inputbuffer, inputbuffer+bufptr, inputbuffer_complex);
		cout << "complex input buffer: " << inputbuffer_complex[bufptr] << endl;
		sdft.update(inputbuffer[bufptr]);

		cout << "real: " << sdft.dft[bufptr].real() << endl;

		CArray fft_data(inputbuffer_complex, 320);
		CArray ifft_data_array(ifft_data, 320);
		cout << "time domain original: " << inputbuffer[bufptr] << endl;
		dft_x[bufptr] = sdft.dft[bufptr];
		cout << "complex vector output: " << dft_x[bufptr] << endl;

		fft_conquer(fft_data);
		cout << "fft algorithm output" << fft_data[bufptr] << endl;


		ifft_data_array[bufptr] = (dft_x[bufptr].real(), dft_x[bufptr].imag());
		ifft(ifft_data_array);
		ifft(fft_data);
		cout << "time domain output of fft: " << fft_data[bufptr] << endl;
		cout << "time domain output of sdft: " << ifft_data_array[bufptr] << endl;

		cout << "time domain: fft: " << ifft_data_array[bufptr].real() * cos(ifft_data_array[bufptr].imag()) << endl;
		cout << "time domain: sdft: " << fft_data[bufptr].real() * cos(fft_data[bufptr].imag()) << endl << endl;

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
	// Normalize (This section make it not working correctly)
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

void fft_conquer(CArray& x)
{
	const size_t N = x.size();
	if (N <= 1) return;

	// divide
	CArray even = x[std::slice(0, N / 2, 2)];
	CArray  odd = x[std::slice(1, N / 2, 2)];

	// conquer
	fft(even);
	fft(odd);

	// combine
	for (size_t k = 0; k < N / 2; ++k)
	{
		Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k + N / 2] = even[k] - t;
	}
}
