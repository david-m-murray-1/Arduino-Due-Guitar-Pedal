/* ARDUINO DUE GUITAR PEDAL
 * DAVID MURRAY
 */
#include <asf.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>
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

void TC4_Handler();                                 // setup clock for tremolo/ringmodulator.
codecTxReadyInterrupt(HiFiChannelID_t channel);
codecRxReadyInterrupt(HiFiChannelID_t channel);
void switchTo_DISTORTION();
void switchTo_RINGMODULATOR{ 
void switchTo_REVERB(); 
void switchTo_TREMOLO():

static uint32_t left_in = 0;
static uint32_t right_in = 0;
static uint32_t left_out = 0;
static uint32_t right_out = 0;

volatile char EFFECT;

volatile int POT0, POT1, POT2, POT3;

void setup() {
  ////////////////////////// enable LED outputs  ////////////////
  PIOB->PIO_PER = PIO_PB25;  //Enable PIO
  PIOC->PIO_PER = (PIO_PC24 | PIO_PC25 | PIO_PC26 | PIO_PC28);  //Enable PIO

  PIOB->PIO_OER = PIO_PB25;  //Set to OUTPUT
  PIOC->PIO_OER = (PIO_PC24 | PIO_PB25 | PIO_PC25 | PIO_PC26 | PIO_PC28);  //Set to OUTPUT

  PIOB->PIO_PUDR = PIO_PB25; //Disable the pull up resistor
  PIOC->PIO_PUDR = (PIO_PC24 | PIO_PC25 | PIO_PC26 | PIO_PC28); //Disable the pull up resistor

  ////////////////////////// External Interrupts ////////////////
  enable_NVIC_interrupts();

  //////////////////////////      I2C        ////////////////////
  Serial.begin(9600);
  display.display();
  display.clearDisplay();
  
  //////////////////////////  SET UP TIMER:  ////////////////////
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC4);    // ID_TC4: TIMER CHANNEL 1
 
  //we want wavesel 01 with RC 
  TC_Configure(TC1,1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK2);
  TC_SetRC(TC1, 1, 238); // sets <> 44.1 Khz interrupt rate
  TC_Start(TC1, 1);
 
  // enable timer interrupts on the timer
  TC1->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;
  TC1->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;
 
  NVIC_EnableIRQ(TC4_IRQn);       // ID_TC4: TIMER CHANNEL 1

  ///////////////////////     I2S COMMUNICATION      //////////////////
  HiFi.begin();
  // set codec into reset. turn on led power indicator
  PIOC -> PIO_CODR = PIO_PC21;     
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
  PIOC -> PIO_SODR = PIO_PC21;     // digitalWrite(7,HIGH);
  // Enable both receiver and transmitter.
  HiFi.enableRx(true);
  HiFi.enableTx(true);

  ///////////////////////     ADC POTENTIOMETERS      //////////////////
  //ADC Configuration
  ADC->ADC_MR |= 0x80;       // adc: free running mode
  ADC->ADC_CR= 0x02;         // start adc conversion
  ADC->ADC_CHER= 0xF0;       // Enable ADC channels ch7-A0, ch6-A1, ch5-A2, ch4-A3  

  ///////////////////////             MAIN            //////////////////
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
      setFs(pot2);
      setFc(pot3);                                                  // carrier frequency
      left_out = RINGMODULATOR_process_pamples(*left_in);
      right_out = RINGMODULATOR_process_samples(*right_in);
      
      //adjust the volume with POT2
      left_out=map(left_in,0,4095,1,POT1);
      right_out=map(right_in,0,4095,1,POT1);
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

void int_DISTORTION{ 
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("DISTORTION"));
  LED1ON;
  LED2OFF;
  LED3OFF;
  LED4OFF;
  EFFECT = DISTORTION;
  return EFFECT;
}

void int_RINGMODULATOR{
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("RINGMODULATOR"));
  LED1OFF;
  LED2ON;
  LED3OFF;
  LED4OFF;
  EFFECT = RINGMODULATOR;
  return EFFECT;
}

void int_REVERB{
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("REVERB"));
  LED1OFF;
  LED2OFF;
  LED3ON;
  LED4OFF;
  EFFECT = REVERB;
  return EFFECT;
}

void int_TREMOLO{
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("TREMOLO"));
  LED1OFF;
  LED2OFF;
  LED3OFF;
  LED4ON;
  EFFECT = TREMOLO;
  return EFFECT;
}

void enable_NVIC_interrupts(){
// PORT A NVIC
  pmc_enable_periph_clk(ID_PIOA);
  pio_set_input(PIOA, PIO_PA29, PIO_PULLUP;
  pio_handler_set(PIOA, ID_PIOC, PIO_PA29, PIO_IT_EDGE, int_RINGMODULATOR);
  pio_enable_interrupt(PIOA, PIO_PA29);
  NVIC_EnableIRQ(PIOA_IRQn);

// PORT B NVIC
  pmc_enable_periph_clk(ID_PIOB);
  pio_set_input(PIOB, PIO_PB25, PIO_PULLUP;
  pio_handler_set(PIOB, ID_PIOB, PIO_PB25, PIO_IT_EDGE, int_TREMOLO);
  pio_enable_interrupt(PIOB, PIO_PB25);
  NVIC_EnableIRQ(PIOB_IRQn);

// PORT C NVIC
  pmc_enable_periph_clk(ID_PIOC);
  pio_set_input(PIOC, PIO_PC22, PIO_PULLUP;
  pio_handler_set(PIOC, ID_PIOC, PIO_PC22, PIO_IT_EDGE, int_DISTORTION);
  pio_enable_interrupt(PIOC, PIO_PC22);
  NVIC_EnableIRQ(PIOC_IRQn);

// PORT D NVIC
  pmc_enable_periph_clk(ID_PIOD);
  pio_set_input(PIOD, PIO_PD7, PIO_PULLUP;
  pio_handler_set(PIOD, ID_PIOD, PIO_PD7, PIO_IT_EDGE, int_REVERB);
  pio_enable_interrupt(PIOD, PIO_PD7);
  NVIC_EnableIRQ(PIOD_IRQn);
}  
