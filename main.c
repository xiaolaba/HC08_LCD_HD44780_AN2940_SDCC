//#include <hidef.h>     // For EnableInterrupts macro

#include "lcd.h"       //Include LCD Driver definitions
#include "myio.h"      //my macro and some def, GLOBAL_INTERRUPT_ENABLE

void TimerInit(void);  /* Initialization of timer                            */

extern UINT8 timerLCD; /* Variable in the driver used for delays             */

UINT8 current_function_flag; /* Variable used for state of print in LCD      */

UINT8 cycles;          /* Variables to demonstrate the functionality of LCD  */
                       /*       Driver                                       */
UINT8 cycles_100;
UINT8 cycles_10000;

UINT8 temp;            /* Temporal variable                                  */

/******************************** Main Routine *******************************/
void main(void)
 {
  /* Disable the Cop and the LVI module power */
//  CONFIG1_COPD =    1;
//  CONFIG1_LVIPWRD =  1;
  CONFIG1 = COP_disable | STOP_enable | LVIPWRD_disable ;  //one time write after reset

  /* Using the External Crystal Oscilator. Wich frecuency is Fxtal=9.8304MHz */
//  CONFIG2_OSCCLK0=0;
//  CONFIG2_OSCCLK1=1;
  
  TimerInit();              /* Initialization of time                        */
  LCDInit();                /* Initialization of LCD Driver                  */

  //EnableInterrupts;         // Enable interrupts, original CW code, not working for sdcc 3.9.0
  __asm CLI; __endasm;        // sdcc 3.9.0
//  GLOBAL_INTERRUPT_ENABLE;  // same as above, uses macro, see myio.h
 
  cycles =         0;
  cycles_100 =     0;
  cycles_10000 =   0;
  
  for(;;) 
  {

    if (timerLCD == 0) LCDTimeBase(); /* LCD Driver time base                */
          
    /* State routine for print in LCD */
    if(LCDStatus() == lcdStatusReady)
    {
      if(current_function_flag == 0)
      {
        current_function_flag = 1;
        LCDCursor(0x00);                /* Set the cursor at home            */
      }      
      else if(current_function_flag == 1)
      {
        current_function_flag = 2;
        LCDPrint("Line one",8);     /* Print the text of a specific length   */
      }
      else if(current_function_flag == 2)
      {
        current_function_flag = 3;
        LCDCursor(0x40);
      } 
      else if(current_function_flag == 3)
      {
        current_function_flag = 4;
        LCDPrint("Line two",8); 
      }
      else if(current_function_flag == 4)
      {
        current_function_flag = 5;
        LCDCursor(0x0A);
      } 
      else if(current_function_flag == 5)
      {
        current_function_flag = 6;
        LCDPrint("Cycles",6); 
      }
      else if(current_function_flag == 6)
      {
        current_function_flag = 7;
        LCDCursor(0x4A);
      } 
      else if(current_function_flag == 7)
      {
        current_function_flag = 8;
        LCDPrint("x64*",4);
      }
      else if(current_function_flag == 8)
      {
        current_function_flag = 9;
        temp = (((cycles_100)>>4) & 0x0F) + '0';
        if (temp > '9') temp += 7;
        LCDPrint(&temp,1);
      }
      else if(current_function_flag == 9)
      {
        current_function_flag = 0;
        temp = ((cycles_100) & 0x0F) + '0';
        if (temp > '9') temp += 7;
        LCDPrint(&temp,1);
        cycles =         0;
        cycles_100 =     0;
        cycles_10000 =   0;
      }
    }  
    
    cycles++;
    if (cycles == 100) {
      cycles_100++;
      cycles = 0;
      if (cycles_100 == 100) {
        cycles_10000++;
        cycles_100 = 0;
      }
    }
            
  }  /* Loop forever */
}


/* Initialization of timer */
void TimerInit(void)
{

//  T1SC_TOIE = 1;     /* Enable overflow interrupt                            */
//  T1SC_PS0 =  0;     /* Select prescale divisor. Preescaler = 1              */
//  T1SC_PS1 =  0;                   
//  T1SC_PS2 =  0;                     

//  T1MOD = 0x00f6;    /* For an overflow interrupt of aprox 100 us, this need */
//                     /*        246 counts because each count take 1 Tbus, so */
//                     /*        (246 * 1Tbus) = 100.09us                      */
  
  // for QT4, xiaolaba
  TSTOP = 0;    /* For normal operation                                 */

  TOIE = 1;     /* Enable overflow interrupt                            */
  PS0 =  0;     /* Select prescale divisor. Preescaler = 1              */
  PS1 =  0;                   
  PS2 =  0;                     

  TMOD = 0x00f6;    /* For an overflow interrupt of aprox 100 us, this need */
                    /*        246 counts because each count take 1 Tbus, so */
                    /*        (246 * 1Tbus) = 100.09us                      */
  
  TSTOP = 0;    /* For normal operation                                 */
  
}



/* Timer Overflow Interrupt */
//void interrupt 7 timeOverFlowInterrupt(void) 
void timeOverFlowInterrupt(void) __critical __interrupt 7 __naked     //xiaolaba,  for sdcc 3.9.0
{

//    T1SC &= 0x7F;                 /* Clear flag                              */
    TSC &= 0x7F;                 // Clear flag, for QT4 & sdcc 3.9.0

    if (timerLCD > 0) timerLCD--; /* Decrement one time the timerLCD         */
}


