#include "mcu_driver_select.h"     /* Include generic driver definitions     */


/***  Declarations of ports of a specified MCU  ***/
/* Choose the output control pins accord your MCU */
#define lcdExists      1           /* If LCD does not exist, do not declare  */
                                   /*        this #define                    */
#ifdef lcdExists
  #define lcd4bit      1           /* 4 bit interface; comment this line if  */
                                   /*        is 8 bit interface              */
  #define lcdE        PTD_PTD6     /* Enabled pin of LCD                     */
  #define lcdEDD      DDRD_DDRD6   /* Data Direction of Enabled Pin          */
  #define lcdRS       PTD_PTD7     /* RS pin of LCD (Data/Instruction Select)*/
  #define lcdRSDD     DDRD_DDRD7   /* Data Direction of RS Pin               */
  #define lcdPort     PTA          /* Port of 4 data bits to lcd connection  */
  #define lcdPortDD   DDRA         /* Data direction for 4 data pins         */
  #ifdef lcd4bit
    #define  lcdDataPins  0        /* Number of pin of the port where begin  */
                                   /*        the data pins (4 pins). These   */
                                   /*        pins must be consecutive. Only  */
                                   /*        in case of 4 bit interface      */
  #endif
#endif

/*++++++++++++++++++++++++++++++ DON´T MODIFY +++++++++++++++++++++++++++++++*/

/* Possible status of LCD */
#define lcdStatusReady       1 /* LCD is ready to use                        */
#define lcdStatusError       2 /* Error ocurred when you are using the LCD   */
                               /*      and you try to execute other          */
                               /*      operation. You can change the state   */
                               /*      using LCDClear() function             */
#define lcdStatusPrinting    3 /* LCD is printing string                     */
#define lcdStatusInit        4 /* LCD is initializing                        */
#define lcdStatusWaiting     5 /* LCD status are waiting for ready mode      */
#define lcdStatusWaitingInit 0 /* LCD is waiting for initialization          */

/* Function declaration */
void  LCDInit(void);           /* Initialize the LCD                         */
void  LCDClear(void);          /* Clear the LCD                              */
void  LCD2L(void);             /* Go to second line of LCD                   */
void  LCDPrint(UINT8 *where, UINT8 length); /* Print from [where] memory     */
                               /*      address [length] characters           */
void  LCDTimeBase(void);       /* Time Base of LCD. This must be called      */
                               /*     each time that timerLCD is equal to 0  */
UINT8 LCDStatus(void);         /* Return the status of the LCD               */
void  LCDCursor(UINT8 ddramAddress); /* Send the address that you can put    */
                                     /*        the cursor                    */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/