# HC08_LCD_HD44780_AN2940_SDCC
copy and rewrite the code, port to sdcc 3.9.0 and QT device

notes:
1) removed dependency hidef.h, rewrite EnableInterrupt, inline asm or uses macro of myio.h, see main.c
```
  //EnableInterrupts;         // Enable interrupts
  __asm CLI; __endasm;        // sdcc 3.9.0
//  GLOBAL_INTERRUPT_ENABLE;  // same as above, uses macro
```
2) rewrite code to fit for QT device & sdcc 3.9.0, NXP/Freescale and original code was for AP device, see mcu_driver_select.h
```
/* Select the Correctly Driver for your MicroControllerUnit */
#ifndef MC68HC908AP64_h
	#define MC68HC908AP64_h
	//#include <MC68HC908AP64.h>	/* Include peripheral declarations */ for Codeworrier
    //#include <mc68hc908apxx.h>  /* include for sdcc 3.9.0, by xiaolaba
    #include <mc68hc908qy.h>

#endif
```


The original source code and application note. it has been designed for propritary software CodeWarrior® for Microcontrollers (Classic IDE) - RS08/HC(S)08, ColdFire V1 v6.3

https://www.nxp.com/webapp/sps/download/license.jsp?colCode=AN2940SW  
https://www.nxp.com/docs/en/application-note/AN2940.pdf

