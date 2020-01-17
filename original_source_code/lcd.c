#include "lcd.h"        /* Include LCD driver definitions                    */

#ifdef lcd4bit
  #define OUTNUMBER(character)  (character<<lcdDataPins) /* Shift the data   */
                        /*    bits until match the 4 consecutive output      */
                        /*    pins                                           */
  #define PORTMASK              (~(0x0F<<lcdDataPins))   /* To locate the 4  */
                        /*    consecutive output pins of the port, which     */
                        /*    will transmit the data                         */
#endif

#define cActive     1
#define cInactive   0

/* Variables used with the main program */
UINT8   timerLCD;       /* Variable for delay times                          */

/* Local variables */
UINT8    LCDInternalStatus = 0; /* Variable for internal status              */
UINT8   howmany;        /* Temporal variable that have the number of         */
                        /*      characters pending to print                  */
UINT8   *nextchr;       /* Pointer to next character to print                */

/* Local functions */
void   LCDPrintNext(void); /* Function that print the next character and     */
                        /*      increment the pointer "nextchr" and          */
                        /*      decrement the counter "howmany"              */

/*+++++++++++++++++++++++++++++++ LCD Driver ++++++++++++++++++++++++++++++++*/
#ifdef lcdExists

  /* Indicate to LCD "read data" through fallflank */
  void LCDSend(void) {
  
    lcdE = 1;
    lcdE = 0;  
    
  }

  /* Initialize LCD */
  void LCDInit(void) {

    static UINT8 Local_LCDStatusInit; /* Used for internal of function status*/

    if (LCDInternalStatus == lcdStatusWaitingInit) {
    
      /* Configure Pins to output and data sended */
      lcdEDD =  1;                   /* LCD Enable pin as output             */
      lcdE =    0;                   /* LCD Enable pin is clear              */
      lcdRSDD = 1;                   /* Data/Instruction pin as output       */
      lcdRS =   0;                   /* Data/Instruction is clear            */
      #ifdef lcd4bit
        lcdPortDD |= (0x0F << lcdDataPins); /* LCD port pins as output       */
        lcdPort &= PORTMASK;        /* Taking the 4 pins of the port         */
        lcdPort |= OUTNUMBER(0x00); /* Nibble sended                         */
      #else
        lcdPortDD = 0xFF;
        lcdPort   = 0x00;
      #endif
    
      LCDInternalStatus = lcdStatusInit; /* Internal control identify the    */
                                    /*      status of initialization for     */
                                    /*      base time                        */
      Local_LCDStatusInit = 1;      /* Internal of function status in step   */
                                    /*      one                              */
      timerLCD = (UINT8) (gTimeBaseInterruptperms * 45); /* Sets the delay   */
                                    /*      for of 45ms                      */
    }
    else switch (Local_LCDStatusInit) {
      case 1: /* Function set */
          #ifdef lcd4bit
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x03);
          #else
            lcdPort  = 0x03;
          #endif
          LCDSend();
          timerLCD = gTimeBaseInterruptperms * 5;
          Local_LCDStatusInit = 2;
          break;
      case 2: /* Function set */
          #ifdef lcd4bit
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x03);
          #else
            lcdPort = 0x03;
          #endif
          LCDSend();
          timerLCD = gTimeBaseInterruptperms;
          Local_LCDStatusInit = 3;
          break;
      case 3: /* Function set */
          #ifdef lcd4bit
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x03);
          #else
            lcdPort = 0x03;
          #endif
          LCDSend();
          timerLCD = gTimeBaseInterruptperms;
          Local_LCDStatusInit = 4;
          break;
      case 4: /* Function set */
          #ifdef lcd4bit
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x02);                /* 4 bit mode         */
          #else
            lcdPort = 0x3C;                            /* 8 bit mode         */
          #endif
          LCDSend();
          timerLCD = gTimeBaseInterruptperms;
          Local_LCDStatusInit = 5;
          break;
      case 5: /* Function set */
          #ifdef lcd4bit
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x02);
            LCDSend();
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x08);                /* 2 line; 5x10 dots  */
            LCDSend();
          #else
            lcdPort = 0x38;                          
            LCDSend();
          #endif
          timerLCD = gTimeBaseInterruptperms;
          Local_LCDStatusInit = 6;
          break;
       case 6: /* Display ON/OFF Control */
           #ifdef lcd4bit
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x00);
            LCDSend();
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x0C);  /* Disply on; cursor off; blink off */
            LCDSend();
          #else
            lcdPort = 0x0C;              
            LCDSend();
          #endif
          timerLCD = gTimeBaseInterruptperms;
          Local_LCDStatusInit = 7;
          break;
      case 7: /* Entry mode set */
          #ifdef lcd4bit
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x00);
            LCDSend();
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x06);   /* Increment mode; Entire shift on */
            LCDSend();
          #else
            lcdPort = 0x06;               /* Increment mode; Entire shift on */
            LCDSend();
          #endif
          timerLCD = gTimeBaseInterruptperms;
          Local_LCDStatusInit = 8;
          break;
      case 8: /* Clear display */
          #ifdef lcd4bit
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x00);
            LCDSend();
            lcdPort &= PORTMASK;
            lcdPort |= OUTNUMBER(0x01);
            LCDSend();
          #else
            lcdPort = 0x01;
            LCDSend();
          #endif
          timerLCD = gTimeBaseInterruptperms;
          Local_LCDStatusInit = 9;
          break;
      case 9:
          LCDInternalStatus = lcdStatusReady;
          Local_LCDStatusInit = 0;
          break;
      
    }
    
  }

  /* Clear display */
  void LCDClear(void) {

    lcdRS = 0;
    #ifdef lcd4bit
      lcdPort &= PORTMASK;
      lcdPort |= OUTNUMBER(0x00);
      LCDSend();
      lcdPort &= PORTMASK;
      lcdPort |= OUTNUMBER(0x01);
      LCDSend();
    #else
      lcdPort = 0x01;
      LCDSend();
    #endif
    timerLCD = gTimeBaseInterruptperms * 2;
    LCDInternalStatus = lcdStatusWaiting;

  }
  
  /* Set the cursor on the second line */
  void LCD2L(void) {

    LCDCursor(0x40);

  }

  /* Print the next character and increment the pointer "nextchr" and        */
  /*      decrement the counter "howmany"                                    */
  void LCDPrintNext(void) {
  
    if (howmany-- > 0) {            
      lcdRS = 1;
      #ifdef lcd4bit
        lcdPort &= PORTMASK;
        lcdPort |= OUTNUMBER(((*nextchr)>>0x04));
        LCDSend();
        lcdPort &= PORTMASK;
        lcdPort |= OUTNUMBER(((*nextchr) & 0x0F));
        LCDSend();          
      #else
        lcdPort = (*nextchr);
        LCDSend();
      #endif
      nextchr++;
      timerLCD = gTimeBaseInterruptperms;
      LCDInternalStatus = lcdStatusPrinting;
    }
    else {
      LCDInternalStatus = lcdStatusReady;
    }
    
  }
  
  /* Display a string with a specific length */
  void LCDPrint(UINT8 *where, UINT8 length) {

    if (LCDInternalStatus == lcdStatusReady) {
      if (length > 0) {
        howmany = length;
        nextchr = where;
        LCDPrintNext();
      }
      else { // ignore
      }
    }
    else {
      LCDInternalStatus = lcdStatusError;
    }
    
  }

  /* This must be called each time that timerLCD is equal to 0 */
  void LCDTimeBase(void) {

    if (LCDInternalStatus == lcdStatusInit) {
      LCDInit();
    } else if (LCDInternalStatus == lcdStatusPrinting) {
      LCDPrintNext();
    } else if (LCDInternalStatus == lcdStatusWaiting) {
      LCDInternalStatus = lcdStatusReady;
    }
    
  }
  
  /* Return the status of LCD Drive */
  UINT8 LCDStatus(void) {
  
    return LCDInternalStatus;
    
  }
  
  /* Sets the cursor in the display ddramAddress */
  void LCDCursor(UINT8 ddramAddress) {
  
    if (LCDInternalStatus == lcdStatusReady) {
      lcdRS = 0;
      ddramAddress |= 0x80;
      #ifdef lcd4bit        
        lcdPort &= PORTMASK;
        lcdPort |= OUTNUMBER(ddramAddress>>4);
        LCDSend();
        lcdPort &= PORTMASK;
        lcdPort |= OUTNUMBER(ddramAddress & 0x0F);
        LCDSend();
      #else
        lcdPort = ddramAddress;
        LCDSend();
      #endif
      timerLCD = gTimeBaseInterruptperms;
      LCDInternalStatus = lcdStatusWaiting;
    }
    else {
      LCDInternalStatus = lcdStatusError;
    }
    
  }
  
#endif

/*++++++++++++++++++++++++++++++ END LCD Driver +++++++++++++++++++++++++++++*/