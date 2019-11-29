#include <asf.h>
#include <pwm.h>
#include "pio.h"
#include "conf_board.h"
#include "conf_clock.h"

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


volatile int EFFECT;

void int_DISTORTION(uint32_t, uint32_t);
void int_Flanger(uint32_t, uint32_t);
void int_RingModulator(uint32_t, uint32_t);
void int_TREMOLO(uint32_t, uint32_t);



struct pwm_clock_t{
	/** Frequency of clock A in Hz (set 0 to turn it off) */
	uint32_t ul_clka;
	/** Frequency of clock B in Hz (set 0 to turn it off) */
	uint32_t ul_clkb;
	/** Frequency of master clock in Hz */
	uint32_t ul_mck;
};


////////////////////////////////////////////////////////////////////////////////////

int main(void){
	
		//////////////////////////// Enable Interrupts /////////////////////////////
		// PORT A NVIC
		pmc_enable_periph_clk(ID_PIOA);
		pio_set_input(PIOA, PIO_PA29, PIO_PULLUP);
		pio_handler_set(PIOA, ID_PIOC, PIO_PA29, (PIO_IT_FALL_EDGE), int_RingModulator);
		NVIC_EnableIRQ((IRQn_Type)ID_PIOA);
		pio_enable_interrupt(PIOA, PIO_PA29);

		// PORT B NVIC
		pmc_enable_periph_clk(ID_PIOB);
		pio_set_input(PIOB, PIO_PB25, PIO_PULLUP);
		pio_handler_set(PIOB, ID_PIOB, PIO_PB25, (PIO_IT_FALL_EDGE), int_TREMOLO);
		NVIC_EnableIRQ((IRQn_Type)ID_PIOB);
		pio_enable_interrupt(PIOB, PIO_PB25);

		// PORT C NVIC
		pmc_enable_periph_clk(ID_PIOC);
		pio_set_input(PIOC, PIO_PC22, PIO_PULLUP);
		pio_handler_set(PIOC, ID_PIOC, PIO_PC22, (PIO_IT_FALL_EDGE), int_DISTORTION);
		NVIC_EnableIRQ((IRQn_Type)ID_PIOC);
		pio_enable_interrupt(PIOC, PIO_PC22);

		// PORT D NVIC
		pmc_enable_periph_clk(ID_PIOD);
		pio_set_input(PIOD, PIO_PD7, PIO_PULLUP);
		pio_handler_set(PIOD, ID_PIOD, PIO_PD7, (PIO_IT_FALL_EDGE), int_Flanger);
		NVIC_EnableIRQ((IRQn_Type)ID_PIOD);
		pio_enable_interrupt(PIOD, PIO_PD7);
	
	////////////////////////// enable LED outputs  ////////////////
	PIOB->PIO_PER = PIO_PB25;  //Enable PIO_B
	PIOC->PIO_PER = (PIO_PC24 | PIO_PC25 | PIO_PC26 | PIO_PC28);  //Enable PIO_C

	PIOB->PIO_OER = PIO_PB25;  //Set to OUTPUT
	PIOC->PIO_OER = (PIO_PC24 | PIO_PB25 | PIO_PC25 | PIO_PC26 | PIO_PC28);  //Set to OUTPUT

	PIOB->PIO_PUDR = PIO_PB25; //Disable the pull up resistor
	PIOC->PIO_PUDR = (PIO_PC24 | PIO_PC25 | PIO_PC26 | PIO_PC28); //Disable the pull up resistor

	
	//////////////////////////  Left-Right Clock (LRCK)  //////////
	pwm_channel_t pwm_channel_instance;

	pio_configure_pin(PWM_LRCK, PIO_TYPE_PIO_PERIPH_B);
	pmc_enable_periph_clk(ID_PWM);
	pwm_channel_disable(PWM, PWM_CHANNEL_0);
	pwm_clock_t PWM_LRCK_clock_config =  
	{
		.ul_clka = 96000,                   // set to 48 kHz
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
	
	while(1)
	{
	}
}

void int_DISTORTION(uint32_t a, uint32_t b){
	LED1ON;
	LED2OFF;
	LED3OFF;
	LED4OFF;
	EFFECT = 1;
}

void int_RingModulator(uint32_t a, uint32_t b){
	LED1OFF;
	LED2ON;
	LED3OFF;
	LED4OFF;
	EFFECT = 2;
}

void int_TREMOLO(uint32_t a, uint32_t b){
	LED1OFF;
	LED2OFF;
	LED3OFF;
	LED4ON;
	EFFECT = 3;
}

void int_Flanger(uint32_t a, uint32_t b){
	LED1OFF;
	LED2OFF;
	LED3ON;
	LED4OFF;
	EFFECT = 4;
}
