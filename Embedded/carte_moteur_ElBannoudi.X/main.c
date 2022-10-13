/* 
 * File:   main.c
 * Author: TP-EO-1
 *
 * Created on 21 septembre 2022, 15:06
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "timer.h"
#include "IO.h"
#include "PWM.h"
#include "ADC.h"

unsigned int ADCValue0;
unsigned int ADCValue1;
unsigned int ADCValue2;


int main(void) {   
  
    //? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    // I n i t i a l i s a t i o n   de   l ? o s c i l l a t e u r
    //? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    InitOscillator();
    //? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    // C o n f i g u r a t i o n   d e s   � e n t r e s   s o r t i e s
    //? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    InitIO();
    
    InitTimer23();
    InitTimer1();
    
    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;
    
    InitPWM();
    InitADC1();
    
    // Boucle P r i n c i p a l e    
    while(1) {
        if (ADCIsConversionFinished() == 1){
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            ADCValue0 = result[0];
            ADCValue1 = result[1];
            ADCValue2 = result[2];
        }

    } // f i n main
 
    
}