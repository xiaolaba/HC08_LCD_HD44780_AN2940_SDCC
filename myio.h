// by xiaolaba

//    PTA =  0bxx543210  // HC908QT4, PTA0 to PTA5, 8 pin IC, i/o pin for user, bit0 to bit5
#define LED0 0b00000001  // PTA0
#define LED1 0b00000010  // PTA1
#define LED2 0b00000100  // PTA2
#define LED3 0b00001000  // PTA3
#define LED4 0b00010000  // PTA4
#define LED5 0b00100000  // PTA5

#define myLED LED3



// asm, CPU direct instruction define
#define NOP __asm nop __endasm;
#define GLOBAL_INTERRUPT_ENABLE  __asm cli __endasm;
#define GLOBAL_INTERRUPT_DISABLE __asm sei __endasm;
#define CPU_STOP __asm stop __endasm;  // STOP mode, I bit cleared, enabled INT
                                       // KBI interrupt wake up, then go next step
                                       
#define COP_disable     0b00000001
#define STOP_enable     0b00000010
#define LVIPWRD_disable 0b00010000  // set LVIPWRD ¡X LVI Power Disable Bit
                                    // want save more power but no effect, 
                                    //still 10uA in STOP mode                                       