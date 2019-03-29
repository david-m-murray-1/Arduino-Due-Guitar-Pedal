/* 8 BIT RESOLUTION SIGNAL
 * See: www.electrosmash.com/pedalshield for original effects code.  This script conjoins two of their effects.  2 buttons are added to switch between echo and distortion.
 * 
 * Licensed under a Creative Commons Attribution 3.0 Unported License.
   Based on rcarduino.blogspot.com previous work.
   www.electrosmash.com/pedalshield
*/

int in_ADC0, in_ADC1;  //variables for 2 ADCs values (ADC0, ADC1)
int POT0, POT1, POT2, out_DAC0, out_DAC1; //variables for 3 pots (ADC8, ADC9, ADC10)
int LED = 3;
int FOOTSWITCH = 7; 
int TOGGLE = 2; 

#define MAX_DELAY 40000
uint16_t sDelayBuffer0[MAX_DELAY];
uint16_t sDelayBuffer1[MAX_DELAY];

unsigned int Delay_Depth, DelayCounter = 0;
int ECHO_enable = 0, DISTORTION_enable = 0;
int upper_threshold, lower_threshold;
int DISTORTION_pin = 51, ECHO_pin = 53;

void setup()
{
  attachInterrupt(digitalPinToInterrupt(DISTORTION_pin),switchTo_DISTORTION,LOW);
  attachInterrupt(digitalPinToInterrupt(ECHO_pin),switchTo_ECHO,LOW);

  //ADC Configuration
  ADC->ADC_MR |= 0x80;   // DAC in free running mode.
  ADC->ADC_CR=2;         // Starts ADC conversion.
  ADC->ADC_CHER=0x1CC0;  // Enable ADC channels 0,1,8,9 and 10  
 
  //DAC Configuration
  analogWrite(DAC0,0);  // Enables DAC0
  analogWrite(DAC1,0);  // Enables DAC1
  
  //turn on the timer clock in the power management controller
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC4);
  
  //TC4_IRQn where 4 is the timer number * timer channels (3) + the channel number 
  //(=(1*3)+1) for timer1 channel1 
  //we want wavesel 01 with RC 
  TC_Configure(TC1,1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK2);
  TC_SetRC(TC1, 1, 238); // sets <> 44.1 Khz interrupt rate
  TC_Start(TC1, 1);
  // enable timer interrupts on the timer
  TC1->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;
  TC1->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS; 
  }  

void loop()
{
  //Read the ADCs
  while((ADC->ADC_ISR & 0x1CC0)!=0x1CC0);// wait for ADC 0, 1, 8, 9, 10 conversion complete.
  in_ADC0=ADC->ADC_CDR[7];           // read data from ADC0
  in_ADC1=ADC->ADC_CDR[6];           // read data from ADC1  
  POT0=ADC->ADC_CDR[10];                // read data from ADC8        
  POT1=ADC->ADC_CDR[11];                // read data from ADC9   
  POT2=ADC->ADC_CDR[12];                // read data from ADC10
  
  if (DISTORTION_enable == 1) 
  {   
      noInterrupts();
      attachInterrupt(ECHO_pin,switchTo_ECHO,LOW);//recommended for arduino board
      attachInterrupt(DISTORTION_pin,switchTo_DISTORTION,LOW);//recommended for arduino board

      // DISTORTION parameters
      upper_threshold=map(POT0,0,4095,4095,2047);
      lower_threshold=map(POT0,0,4095,0000,2047);

      // apply DISTORTION threshold
      if(in_ADC0>=upper_threshold) in_ADC0=upper_threshold; 
      else if(in_ADC0<lower_threshold)  in_ADC0=lower_threshold;
        
      if(in_ADC1>=upper_threshold) in_ADC1=upper_threshold;
      else if(in_ADC1<lower_threshold)  in_ADC1=lower_threshold;   

      //adjust the volume with POT2
      out_DAC0=map(in_ADC0,0,4095,1,POT2);
      out_DAC1=map(in_ADC1,0,4095,1,POT2);
      
      dacc_set_channel_selection(DACC_INTERFACE, 0);          //select DAC channel 0
      dacc_write_conversion_data(DACC_INTERFACE, out_DAC0);   //write on DAC
      dacc_set_channel_selection(DACC_INTERFACE, 1);          //select DAC channel 1
      dacc_write_conversion_data(DACC_INTERFACE, out_DAC1);   //write on DAC 
  }
}

void switchTo_ECHO()
{
  delay(0.3);  // wait for debounce
  NVIC_EnableIRQ(TC4_IRQn);
  attachInterrupt(ECHO_pin,switchTo_ECHO,LOW);//recommended for arduino board
  attachInterrupt(DISTORTION_pin,switchTo_DISTORTION,LOW);//recommended for arduino board
}

void switchTo_DISTORTION()
{
  delay(0.3);  // wait for debounce
  noInterrupts();
  attachInterrupt(ECHO_pin,switchTo_ECHO,LOW);
  attachInterrupt(DISTORTION_pin,switchTo_DISTORTION,LOW);
  DISTORTION_enable = 1;
}

//Interrupt at 44.1KHz rate (every 22.6us)
void TC4_Handler()
{
  //Clear status allowing the interrupt to be fired again.
  TC_GetStatus(TC1, 1);
 
  //Store current readings  
  sDelayBuffer0[DelayCounter]  = (in_ADC0 + (sDelayBuffer0[DelayCounter]))>>1;
  sDelayBuffer1[DelayCounter]  = (in_ADC1 + (sDelayBuffer1[DelayCounter]))>>1;
 
  //Adjust Delay Depth based in pot0 position.
  Delay_Depth =map(POT0>>2,0,2047,1,MAX_DELAY);
 
  //Increse/reset delay counter.   
  DelayCounter++;
  if(DelayCounter >= Delay_Depth) DelayCounter = 0; 
  out_DAC0 = ((sDelayBuffer0[DelayCounter]));
  out_DAC1 = ((sDelayBuffer1[DelayCounter]));
 
  //Add volume feature based in POT2 position.
  out_DAC0=map(out_DAC0,0,4095,1,POT2);
  out_DAC1=map(out_DAC1,0,4095,1,POT2);
 
  //Write the DACs
  dacc_set_channel_selection(DACC_INTERFACE, 0);          //select DAC channel 0
  dacc_write_conversion_data(DACC_INTERFACE, out_DAC0);   //write on DAC
  dacc_set_channel_selection(DACC_INTERFACE, 1);          //select DAC channel 1
  dacc_write_conversion_data(DACC_INTERFACE, out_DAC1);          //write on DAC
}
