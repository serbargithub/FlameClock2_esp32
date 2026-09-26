
#ifndef _IO_PORTS_NAMES_H_
#define _IO_PORTS_NAMES_H_

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <p24Fxxxx.h>

#define BUZZER_OUT       _RB5		//>
#define BUCK_UP1_OUT     _RB6		//>
#define BUCK_UP2_OUT     _RB7		//>
#define POWER_SENSOR_IN  _RB12		//<

#define TX_UART1		_RD4		//>[RP25]
#define RX_UART1		_RD5		//<[RP20]
#define DISPLAY_SDO		_RD9		//>[RP4]
#define DISPLAY_SCK		_RD10		//<[RP3]
#define DISPLAY_LATCH	_RD11		//>       

#define USB_DATA_PLUS	_RG2		//
#define USB_DATA_MINUS	_RG3		//

#define BUTTON1_IN      _RE0		//<
#define BUTTON2_IN      _RE1		//<
#define BUTTON3_IN      _RE2		//<
#define BUTTON4_IN      _RE3		//<   
#define INFORM_LED_OUT  _RE5		//>



#define  MASK_PORTB 	0b1111111100011111
#define  MASK_PORTC    	0xFFFF  
#define  MASK_PORTD    	0b1111010111101111  
#define  MASK_PORTE    	0b1111111111011111  
#define  MASK_PORTG    	0xFFFF  
#define  MASK_PORTF    	0xFFFF 
    
#define  MASK_OFF    	0xFFFF 



#ifdef	__cplusplus
}
#endif

#endif //_IO_PORTS_NAMES_H_
