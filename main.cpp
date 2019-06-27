/* ARDUINO DUE GUITAR PEDAL
 * DAVID MURRAY
 * Include .hpp and .cpp files used when compiling with Arduino IDE.
 */
#include "pch.h"
#include <iostream>
#include <cstdio>
#include <asf.h>
#include <HiFi.h>
#include <ssc.h>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include "circle.h"
#include "Effect.h"

#define LED1ON (PIOC -> PIO_SODR = PIO_PC24)        // set output data register SODR
#define LED2ON (PIOC -> PIO_SODR = PIO_PC25)
#define LED3ON (PIOC -> PIO_SODR = PIO_PC26)     
#define LED4ON (PIOC -> PIO_SODR = PIO_PC28)
#define LED5ON (PIOB -> PIO_SODR = PIO_PB25)
#define LED1OFF (PIOC -> PIO_CODR = PIO_PC24)       // clear output data register CODR
#define LED2OFF (PIOC -> PIO_CODR = PIO_PC25)
#define LED3OFF (PIOC -> PIO_CODR = PIO_PC26)
#define LED4OFF (PIOC -> PIO_CODR = PIO_PC28)
#define LED5OFF (PIOB -> PIO_CODR = PIO_PB25)
#define PWM_LRCK IOPORT_CREATE_PIN(PIOB, 27)

codecTxReadyInterrupt(HiFiChannelID_t channel);
codecRxReadyInterrupt(HiFiChannelID_t channel);

void int_DISTORTION();
void int_RINGMODULATOR(); 
void int_REVERB(); 
void int_TREMOLO():

typedef struct {
	/** Frequency of clock A in Hz (set 0 to turn it off) */
	uint32_t ul_clka;
	/** Frequency of clock B in Hz (set 0 to turn it off) */
	uint32_t ul_clkb;
	/** Frequency of master clock in Hz */
	uint32_t ul_mck;
} pwm_clock_t;  
  

volatile int EFFECT;

volatile int POT0, POT1;

/////// Effects vars ///////////////////////////////////////////////////////////////////////////////
  int left_buff_ptr = 0;
  int right_buff_ptr = 0;
  double inputbuffer_left[345];
  double inputbuffer_right[345];
  double outputbuffer_left[345];			EFFECTS
  double outputbuffer_right[345];
  double min_amplitude_left;
  double min_amplitude_right;	
  int bit_depth = 32;
 ////////////////////////////////////////////////////////////////////////////////////

int main(){

  ////////////////////////// INITIALIZE BUFFERS  ////////////////
  STEREO stereo_left;
  STEREO stereo_right;

  Distortion Distortion;
  Distortion.setDepth(1);
  Distortion.setTimbre(1);
  RingModulator RingModulation;
  RingModulation.setFc(440);
  RingModulation.setFs(300);
  Tremolo Tremolo;
  Flanger Flanger;

  //////////////////////////  init ring buffers  ////////////////
  circular_buffer<double> circle_left(bit_depth);
  circular_buffer<double> circle_right(bit_depth);
		
  ////////////////////////// enable LED outputs  ////////////////
  PIOB->PIO_PER = PIO_PB25;  //Enable PIO_B
  PIOC->PIO_PER = (PIO_PC24 | PIO_PC25 | PIO_PC26 | PIO_PC28);  //Enable PIO_C

  PIOB->PIO_OER = PIO_PB25;  //Set to OUTPUT
  PIOC->PIO_OER = (PIO_PC24 | PIO_PB25 | PIO_PC25 | PIO_PC26 | PIO_PC28);  //Set to OUTPUT

  PIOB->PIO_PUDR = PIO_PB25; //Disable the pull up resistor
  PIOC->PIO_PUDR = (PIO_PC24 | PIO_PC25 | PIO_PC26 | PIO_PC28); //Disable the pull up resistor

  ////////////////////////// External Interrupts ////////////////
  enable_NVIC_interrupts();
  
  //////////////////////////  Left-Right Clock (LRCK)  //////////
  pio_configure_pin(PWM_LRCK, PIO_TYPE_PIO_PERIPH_B);
  pmc_enable_periph_clk(ID_PWM);
  pwm_channel_disable(PWM, PWM_CHANNEL_0);
   pwm_clock_t PWM_LRCK_clock_config = 
  {
	.ul_clka = 96000,                   // set to 96 kHz
	.ul_clkb = 0,
	.ul_mck = sysclk_get_cpu_hz()
  }; 
  
  pwm_init(PWM, &PWM_LRCK_clock_config);
	pwm_channel_instance.channel = PWM_CHANNEL_0;
	pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
	pwm_channel_instance.polarity = PWM_HIGH;
	pwm_channel_instance.alignment = PWM_ALIGN_LEFT;
	pwm_channel_instance.ul_period = 20;
	pwm_channel_instance.ul_duty = 10;
	//apply the channel configuration
	pwm_channel_init(PWM, &pwm_channel_instance);
	//configuration is complete, so enable the channel
	pwm_channel_enable(PWM, PWM_CHANNEL_0);
  
  ///////////////////////     I2S COMMUNICATION      //////////////////
  HiFi.begin();
  // set codec into reset. turn on led power indicator
  PIOC -> PIO_CODR = PIO_PC23;     
  LED5;
  // Configure transmitter for 2 channels, external TK/TF clocks, 32 bit per
  // channel (data less than 32-bit is left justified in the 32 bit word, but
  // codec config needs 32 clocks per channel).
  HiFi.configureTx(HIFI_AUDIO_MODE_STEREO, HIFI_CLK_MODE_USE_EXT_CLKS, 32);

  // Same config as above, except sync the receiver to transmitter (RK/RF
  // clock signals not needed)
  HiFi.configureRx(HIFI_AUDIO_MODE_STEREO, HIFI_CLK_MODE_USE_TK_RK_CLK, 32);
  HiFi.onTxReady(codecTxReadyInterrupt);
  HiFi.onRxReady(codecRxReadyInterrupt);

  // release codec from reset
  PIOC -> PIO_SODR = PIO_PC23;     
  // Enable both receiver and transmitter.
  HiFi.enableRx(true);
  HiFi.enableTx(true);

  ///////////////////////     ADC POTENTIOMETERS      //////////////////
  //ADC Configuration
  ADC->ADC_CR = ADC_CR_SWRST;                           // Reset ADC


  ADC->ADC_MR |= 0x80;       // adc: free running mode
  ADC->ADC_CR= 0x02;         // start adc conversion
  ADC->ADC_CHER = ADC_CHER_CH6; 
  ADC->ADC_CHER = ADC_CHER_CH7; 

  ///////////////////////             MAIN            //////////////////
while(1){
  POT0=ADC->ADC_CDR[6];      // read effect parameters from POTs        
  POT1=ADC->ADC_CDR[7];                   
}
}
  
void codecTxReadyInterrupt(HiFiChannelID_t channel)
{
  if (channel == HIFI_CHANNEL_ID_1) {
	switch (Effect) {
		case 1:
			Distortion.setDepth(POT0);
			Distortion.setTimbre(POT1);
			Distortion.process_samples(&inputbuffer_left[0], &outputbuffer_left[0], left_buff_ptr);
			Distortion.process_samples(&inputbuffer_right[0], &outputbuffer_right[0], right_buff_ptr);
			break;
		case 2:
			RingModulation.setFc(POT0);
			RingModulation.setFs(POT1);
			RingModulation.process_samples(&inputbuffer_left[0], &outputbuffer_right[0], left_buff_ptr);
			RingModulation.process_samples(&inputbuffer_right[0], &outputbuffer_left[0], right_buff_ptr);
			break;
		case 3:
			Tremolo.setRate(POT0);
			Tremolo.setDepth(POT1);
			Tremolo.process_samples(&inputbuffer_left[0], &outputbuffer_left[0], left_buff_ptr);
			Tremolo.process_samples(&inputbuffer_right[0], &outputbuffer_right[0], right_buff_ptr);
			break;
		case 4:
			Flanger.process_samples(&inputbuffer_left[0], &outputbuffer_left[0], left_buff_ptr);
			Flanger.process_samples(&inputbuffer_right[0], &outputbuffer_right[0], right_buff_ptr);
			break;
		case 5:																								//////////////////// BYPASS ////////////////////////
			outputbuffer_left[left_buff_ptr] = inputbuffer_left[left_buff_ptr];
			outputbuffer_right[right_buff_ptr] = inputbuffer_right[right_buff_ptr];
			break;
		default:
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
	  	// stereo left
		stereo_left.calc_rms_amplitude(&outputbuffer_left[0], left_buff_ptr, stereo_left.rms_width, stereo_left.rms_amplitude);
		stereo_left.calc_rms_dB(stereo_left.rms_amplitude, stereo_left.rms_dB);
		// positve comp slope
		stereo_left.calc_comp_scale(stereo_left.comp_slope, stereo_left.comp_threshold, stereo_left.rms_dB);
		// negative comp slope
		stereo_left.calc_exp_scale(stereo_left.exp_slope, stereo_left.exp_threshold, stereo_left.rms_dB);
		min_amplitude_left = *min_element(begin(outputbuffer_left), end(outputbuffer_left));;

		if (stereo_left.calc_scaling_factor(min_amplitude_left) < stereo_left.gain) {
			stereo_left.gain = (1 - stereo_left.attack) * stereo_left.gain + (stereo_left.attack * stereo_left.calc_scaling_factor(min_amplitude));
		} else {
			stereo_left.gain = (1 - stereo_left.release) * stereo_left.gain + (stereo_left.release * stereo_left.calc_scaling_factor(min_amplitude));
		}
	  		stereo_left.calc_rms_amplitude(&outputbuffer_left[0], bufptr, stereo_left.rms_width, stereo_left.rms_amplitude);
		
		// stereo right
	  	stereo_right.calc_rms_amplitude(&outputbuffer_right[0], right_buff_ptr, stereo_right.rms_width, stereo_right.rms_amplitude);
		stereo_right.calc_rms_dB(stereo_right.rms_amplitude, stereo_right.rms_dB);
		// positve comp slope
		stereo_right.calc_comp_scale(stereo_right.comp_slope, stereo_right.comp_threshold, stereo_right.rms_dB);
		// negative comp slope
		stereo_right.calc_exp_scale(stereo_right.exp_slope, stereo_right.exp_threshold, stereo_right.rms_dB);
		min_amplitude_right = *min_element(begin(outputbuffer_right), end(outputbuffer_right);;

		if (stereo_right.calc_scaling_factor(min_amplitude_right) < stereo_right.gain) {
			stereo_right.gain = (1 - stereo_right.attack) * stereo_right.gain + (stereo_right.attack * stereo_right.calc_scaling_factor(min_amplitude));
		}
		else {
		stereo_right.gain = (1 - stereo_right.release) * stereo_right.gain + (stereo_right.release * stereo_right.calc_scaling_factor(min_amplitude));
		}
						   
		outputbuffer_left[left_buff_ptr] = stereo_left.gain * outputbuffer_left[bufptr];
		outputbuffer_right[right_buff_ptr] = stereo_right.gain * outputbuffer_right[bufptr];
		circle_left.put_back(outputbuffer_right[left_buff_ptr]);	   
		circle_right.put_back(outputbuffer_left[right_buff_ptr]);
	  
     		HiFi.write(circle_left.get()); //output next sample
      		HiFi.write(circle_right.get()); //output next sample
}

void codecRxReadyInterrupt(HiFiChannelID_t channel)
{
  if (channel == HIFI_CHANNEL_ID_1)
  {
    // Left channel
    inputbuffer_left[left_buff_ptr++] = circle_left.put(HiFi.read());
    if (circle_left.full() == 1){
	    circle_left.reset();
	    left_buff_ptr = 0;
    }
  }
  else
  {
    // Right channel
    inputbuffer_right[right_buff_ptr++] = circle_right.put(HiFi.read());
    right_buff_ptr++;
    if (circle_left.full() == 1){
	    circle_right.reset();
	    right_buff_ptr = 0;
    }
  }
}

void int_DISTORTION(){ 
  LED1ON;
  LED2OFF;
  LED3OFF;
  LED4OFF;
  EFFECT = 1;
  return EFFECT;
}

void int_RingModulator(){
  LED1OFF;
  LED2ON;
  LED3OFF;
  LED4OFF;
  EFFECT = 2;
  return EFFECT;
}

void int_TREMOLO(){
  LED1OFF;
  LED2OFF;
  LED3OFF;
  LED4ON;
  EFFECT = 3;
  return EFFECT;
}
	
void int_Flanger(){
  LED1OFF;
  LED2OFF;
  LED3ON;
  LED4OFF;
  EFFECT = 4;
  return EFFECT;
}
						   
void int_BYPASS(){
  LED1OFF;
  LED2OFF;
  LED3OFF;
  LED4OFF;
  EFFECT = 5;
  return EFFECT;
}	
						  					   
void enable_NVIC_interrupts(){

  // PORT D NVIC
  pmc_enable_periph_clk(ID_PIOD);
  pio_set_input(PIOD, PIO_PD8, PIO_PULLUP);
  pio_handler_set(PIOD, ID_PIOD, PIO_PD8, PIO_IT_EDGE, int_BYPASS);
  pio_enable_interrupt(PIOC, PIO_PD8);
  NVIC_EnableIRQ(PIOD_IRQn);
  
  pio_set_input(PIOD, PIO_PD7, PIO_PULLUP);
  pio_handler_set(PIOD, ID_PIOD, PIO_PD7, PIO_IT_EDGE, int_TREMOLO);
  pio_enable_interrupt(PIOD, PIO_PD7);
  NVIC_EnableIRQ(PIOD_IRQn);
  
  // PORT C NVIC
  pmc_enable_periph_clk(ID_PIOC);
  pio_set_input(PIOC, PIO_PC29, PIO_PULLUP);
  pio_handler_set(PIOC, ID_PIOC, PIO_PC29, PIO_IT_EDGE, int_RINGMODULATOR);
  pio_enable_interrupt(PIOC, PIO_PC29);
  NVIC_EnableIRQ(PIOC_IRQn);

  pio_set_input(PIOC, PIO_PC21, PIO_PULLUP);
  pio_handler_set(PIOC, ID_PIOC, PIO_PC21, PIO_IT_EDGE, int_DISTORTION);
  pio_enable_interrupt(PIOC, PIO_PC21);
  NVIC_EnableIRQ(PIOC_IRQn);

  pio_set_input(PIOC, PIO_PC22, PIO_PULLUP);
  pio_handler_set(PIOC, ID_PIOC, PIO_PC22, PIO_IT_EDGE, int_REVERB);
  pio_enable_interrupt(PIOC, PIO_PC22);
  NVIC_EnableIRQ(PIOC_IRQn);
}  
		
