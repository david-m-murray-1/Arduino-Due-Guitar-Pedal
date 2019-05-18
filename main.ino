/* ARDUINO DUE GUITAR PEDAL
 * DAVID MURRAY
 */
#include <HiFi.h>
#include <distortion.h>
#include <reverb.h>
#include <ringModulator.h>
#include <tremolo.h>
#include <effect.h>
#include <ssc.h>

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

void TC4_Handler();
void codecTxReadyInterrupt(HiFiChannelID_t);
void codecRxReadyInterrupt(HiFiChannelID_t);

void Distortion_process_samples(float *inputbuffer);
void REVERB_process_samples(float *inputbuffer);
void RINGMODULATOR_process_samples(float *inputbuffer);
void TREMOLO_process_samples(float *inputbuffer);

static uint32_t left_in = 0;
static uint32_t right_in = 0;
static uint32_t left_out = 0;
static uint32_t right_out = 0;

volatile char EFFECT;

volatile int POT0, POT1, POT2, POT3;

void setup() {
  PIOB->PIO_PER = PIO_PB27;
  PIOB->PIO_PER = PIO_PB27;
  PIOB->PIO_PER = PIO_PB27;
  PIOB->PIO_PER = PIO_PB27;
  PIOB->PIO_PER = PIO_PB27;

  //////////////////////////  SET UP TIMER:  ////////////////////
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC4);
 
  //we want wavesel 01 with RC 
  TC_Configure(TC1,1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK2);
  TC_SetRC(TC1, 1, 238); // sets <> 44.1 Khz interrupt rate
  TC_Start(TC1, 1);
 
  // enable timer interrupts on the timer
  TC1->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;
  TC1->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;
 
  //Enable the interrupt in the nested vector interrupt controller 
  //TC4_IRQn where 4 is the timer number * timer channels (3) + the channel number 
  //(=(1*3)+1) for timer1 channel1 
  NVIC_EnableIRQ(TC4_IRQn);

void configure_ext_int_1(){

void configure_ext_int_1(){

void configure_ext_int_0(){
  PMC->PMC_PCER0 = 1 << ID_PIOC;        // Enable Clock for PIOB - needed for sampling falling edge
  PIOC->PIO_PER = PIO_PC22;             // Enable IO pin control
  PIOC->PIO_ODR = PIO_PC22;             // Disable output (set to High Z)
  PIOC->PIO_PUER = PIO_PC22;            // Enable pull-up
  PIOC->PIO_IFER = PIO_PC22;            // Enable Glitch/Debouncing filter\
  PIOC->PIO_DIFSR = PIO_PC22;           // Select Debouncing filter
  PIOC->PIO_SCDR = 0x4FF;               // Set Debouncing clock divider 
  PIOC->PIO_AIMER = PIO_PC22;           // Select additional detection mode (for single edge detection)
  PIOC->PIO_ESR = PIO_PC22;             // The interrupt source is an Edge detection event.
  PIOC->PIO_FELLSR = PIO_PC22;          // The interrupt source is set to a Falling Edge detection
  PIOC->PIO_IER |= PIO_PC22;         // Enables the Input Change Interrupt on the I/O line.
  NVIC_EnableIRQ(PIOC_IRQn);            // Enable Interrupt Handling in NVIC
}

void configure_ext_int_1(){
  PMC->PMC_PCER1 = 1 << ID_PIOB;         // Enable Clock for PIOB - needed for sampling falling edge
  PIOB->PIO_PER = PIO_PB27;             // Enable IO pin control
  PIOB->PIO_ODR = PIO_PB27;             // Disable output (set to High Z)
  PIOB->PIO_PUER = PIO_PB27;            // Enable pull-up
  PIOB->PIO_IFER = PIO_PB27;            // nable Glitch/Debouncing filter
  PIOB->PIO_DIFSR = PIO_PB27;           // Select Debouncing filter 
  PIOB->PIO_SCDR = 0x4FF;               // Set Debouncing clock divider
  PIOB->PIO_AIMER = PIO_PB27;           // Select additional detection mode (for single edge detection)
  PIOB->PIO_ESR = PIO_PB27;             // The interrupt source is an Edge detection event.
  PIOB->PIO_FELLSR = PIO_PB27;          // The interrupt source is set to a Falling Edge detection
  PIOB->PIO_IER = PIO_PB27;             // Enables the Input Change Interrupt on the I/O line. 
  NVIC_EnableIRQ(PIOB_IRQn);            // Enable Interrupt Handling in NVIC
}

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
  PIOC -> PIO_SODR = PIO_PC23;     // digitalWrite(7,HIGH);
  // Enable both receiver and transmitter.
  HiFi.enableRx(true);
  HiFi.enableTx(true);

  ///////////////////////     ADC POTENTIOMETERS      //////////////////
  //ADC Configuration
  ADC->ADC_MR |= 0x80;       // adc: free running mode
  ADC->ADC_CR= 0x02;         // start adc conversion
  ADC->ADC_CHER= 0xF0;       // Enable ADC channels ch7-A0, ch6-A1, ch5-A2, ch4-A3  
  
void loop() {
  POT0=ADC->ADC_CDR[7];      // read effect parameters from POTs        
  POT1=ADC->ADC_CDR[6];                   
  POT2=ADC->ADC_CDR[5];               
  POT3=ADC->ADC_CDR[4];                  

  switch (EFFECT){
    case DISTORTION:
      left_out = DISTORTION_process_pamples(*left_in);
      right_out = DISTORTION_process_samples(*right_in);
      
      //adjust the volume with POT1 -- 2^24 (input signal bit res.) mapped to 2^12 (adc is 12 bit res.)
      left_out=map(left_out,0,16777215,1,POT1);
      right_out=map(right_out,0,16777215‬,1,POT1);
      break;
      
    case RINGMODULATOR: 
      left_out = RINGMODULATOR_process_pamples(*left_in);
      right_out = RINGMODULATOR_process_samples(*right_in);
      
      //adjust the volume with POT2
      left_out=map(left_in,0,4095,1,POT2);
      right_out=map(right_in,0,4095,1,POT2);
      break;
      
    case REVERB:
      left_out = REVERB_process_pamples(*left_in);
      right_out = REVERB_process_samples(*right_in);
      
      //adjust the volume with POT2
      left_out=map(left_in,0,4095,1,POT2);
      right_out=map(right_in,0,4095,1,POT2);
      break;
      
    case TREMOLO:
      left_out = TREMOLO_process_pamples(*left_in);
      right_out = TREMOLO_process_samples(*right_in);
      
      //adjust the volume with POT2
      left_out=map(left_in,0,4095,1,POT2);
      right_out=map(right_in,0,4095,1,POT2);
      break;
  }
}

void codecTxReadyInterrupt(HiFiChannelID_t channel)
{
  if (channel == HIFI_CHANNEL_ID_1)
  {
    // Left channel
    HiFi.write(left_out);  
  }
  else
  {
    // Right channel
    HiFi.write(right_out);
  }
}

void codecRxReadyInterrupt(HiFiChannelID_t channel)
{
  if (channel == HIFI_CHANNEL_ID_1)
  {
    // Left channel
    left_in = HiFi.read();
  }
  else
  {
    // Right channel
    right_in = HiFi.read();
  }
}

void switchTo_DISTORTION{
  LED1ON;
  LED2OFF;
  LED3OFF;
  LED4OFF;
  EFFECT = DISTORTION;
  return EFFECT;
}

void switchTo_RINGMODULATOR{
  LED1OFF;
  LED2ON;
  LED3OFF;
  LED4OFF;
  EFFECT = RINGMODULATOR;
  return EFFECT;
}

void switchTo_REVERB{
  LED1OFF;
  LED2OFF;
  LED3ON;
  LED4OFF;
  EFFECT = REVERB;
  return EFFECT;
}

void switchTo_TREMOLO{
  LED1OFF;
  LED2OFF;
  LED3OFF;
  LED4ON;
  EFFECT = TREMOLO;
  return EFFECT;
}

 
