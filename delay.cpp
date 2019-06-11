#include "delay.h"
#include <cmath>
#define MAX_BUF_SIZE 1

/*****************************************************************************
*				Fractional delay line implementation in Cpp
*
*                    ---------[d_mix]->-------------------------
*                    |                                         |
*                    |										   |
*                    |x1                                       v
*     xin ------>[+]----->[z^-M]--[interp.]----[d_fw]-------->[+]-----> yout
*                 ^                         |
*                 |                         |
*                 |----------[d_fb]<--------|
*******************************************************************************/

class Delay {
	private:
		static struct fract_delay del;
	public:
		void init(double delay_samples, double dfb, double dfw, double dmix) {
			set_delay(delay_samples);
			set_fb(dfb);
			set_fw(dfw);
			set_mix(dmix);
			del.wrtPtr = &d_buffer[MAX_BUF_SIZE-1];
		}
		void set_fb(double val) {
			del.dfb = val;
		}
		void set_fw(double val) {
			del.d_fw = val;
		}
		void set_mix(double val) {
			del.d_mix = val;
		}
		double task(double *inputbuffer, double *outputbuffer, int bufptr) {
			double yout;
			double * y0;
			double * y1;
			double x1;
			double x_est;

			/*Calculates current read pointer position*/
			del.rdPtr = del.wrtPtr - (short)del.d_samples;

			/* wraps read pointer */
			if (del.rdPtr < d_buffer) {
				del.rdPtr += MAX_BUF_SIZE - 1;
			}
			y0 = del.rdPtr - 1;
			y1 += MAX_BUF_SIZE - 1;
			if (y0 < d_buffer) {
				y0 += MAX_BUF_SIZE - 1;
			}

			x_est = (*(y0)-*(y1)) * del.n_fract + *(y1);

			/* calculate next value to store in buffer */
			x1 = inputbuffer[bufptr] + x_est * del.d_fb;
			/* store value in buffer */
			*(del.wrtPtr) = x1;

			/* output value calculation*/
			outputbuffer[bufptr] = x1 * del.d_mix + x_est * del.d_fw;
			del.wrtPtr++;

			/*wrap delay write pointer*/
			if (del.wrtPtr - &d_buffer[0] > MAX_BUF_SIZE - 1) {
				del.wrtPtr = &d_buffer[0];
			}
			return *outputbuffer;
		}
};