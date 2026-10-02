#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "definitions.h" 
   
void Interrupt__Setup(void);
bool Interrupt__DisableAll(void);
void Interrupt__ShowFrame(DisplayFrame_t* displayFrame);
bool Interrupt__IsFrameEnd(void);
char Interrupt__GetUART1RX(void);
void Interrupt__PlaySound(uint16_t freq_Hz, uint16_t time_Ms);

#ifdef	__cplusplus
}
#endif

#endif //_INTERRUPTS_H_

