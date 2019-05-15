//POT1
//VOLUME
void ADC1_ISR(){
    VOLUME = ADC->ADC_CDR[7];                // read data from ADC7    
    return LEVEL;                           // VOLUME/BOOST
}
//POT2
//DISTORTION: LOW             ECHO: FEEDBACK         REVERB: TONE        TREMOLO: RATE      
void ADC2_ISR(){
    POT1=ADC->ADC_CDR[8];                // read data from ADC8        
    return POT1;  
}

//POT3
//DISTORTION: HIGH            ECHO: DECAY            REVERB: DECAY       TREMOLO: WAVE  
void ADC3_ISR(){
    POT2=ADC->ADC_CDR[9];                // read data from ADC9        
    return POT2;    
}

//POT4
//DISTORTION: DISTORTION       ECHO: BLEND            REVERB: LEVEL-USE POT1      TREMOLO: DEPTH   
void ADC4_ISR(){
    POT3=ADC->ADC_CDR[10];                // read data from ADC10       
    return POT3;    
}
