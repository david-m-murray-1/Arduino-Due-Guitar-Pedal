#pragma once
#ifndef _DELAY_H_
#define _DELAY_H_

#define MAX_BUF_SIZE 1

void Delay_set_dfb(double val);
void Delay_set_dfw(double val);
void Delay_set_dmix(double val);
void Delay_set_delay(double n_delay);
void Delay_set_dfb(double dfb);
void Delay_set_dfw(double dfw);
void Delay_set_dmix(double d_mix);
double Delay_get_dfb(void);
double Delay_get_dfw(void);
double Delay_get_dmix(void);
double Delay_task(double *inputbuffer, double *outputbuffer, int bufptr);
void Delay_init(double delay_samples, double dfb, double dfw, double dmix);

#endif
