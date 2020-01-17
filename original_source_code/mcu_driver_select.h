/* Select the Correctly Driver for your MicroControllerUnit */
#ifndef MC68HC908AP64_h
	#define MC68HC908AP64_h
	#include <MC68HC908AP64.h>	/* Include peripheral declarations */
#endif

/* Choose the waiting Base Time for a Timer Interrupt */
#define gTimeBaseInterrupteachus 50

/* Define the kind of the MCU */
#define MC908   /* In case of HC08 family		 */
//#define MCS08 	/* In case of HCS08 family 	 */






/*++++++++++++++++++++++++++++++ DON´T MODIFY +++++++++++++++++++++++++++++++*/

/* Waiting Base Time equivalent to a 1ms */
#define gTimeBaseInterruptperms 1000/gTimeBaseInterrupteachus								

/* Define a shortname for your variable type */ 
typedef unsigned char				UINT8;  /*  8 bits */
typedef unsigned short int	UINT16; /* 16 bits */
typedef unsigned long int		UINT32; /* 32 bits */

typedef signed char					INT8;   /*  8 bits */
typedef signed short int		INT16;  /* 16 bits */
typedef signed long int			INT32;  /* 32 bits */

typedef volatile UINT8			VUINT8;  /*  8 bits */
typedef volatile UINT16			VUINT16; /* 16 bits */
typedef volatile UINT32			VUINT32; /* 32 bits */