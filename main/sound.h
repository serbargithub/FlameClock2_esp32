#ifndef _SOUND_H_
#define _SOUND_H_

#ifdef	__cplusplus
extern "C" {
#endif

void Sound__KeyBeep();
void Sound__Beep(uint16_t freq_Hz, uint16_t time_ms);

#ifdef	__cplusplus
}
#endif

#endif //_SOUND_H_
