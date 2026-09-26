
#ifndef _SCREENS_STATIC_H_
#define _SCREENS_STATIC_H_

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "../definitions.h"

extern const unsigned char BlankScreen1[VERTICAL_LINES_MAX][HORIZONTAL_BYTES_MAX];
extern const unsigned char BlankScreen2[VERTICAL_LINES_MAX][HORIZONTAL_BYTES_MAX];

extern const unsigned char HelloWorld[VERTICAL_LINES_MAX][HORIZONTAL_BYTES_MAX];
extern const unsigned char Pepsi[VERTICAL_LINES_MAX][HORIZONTAL_BYTES_MAX];
extern const unsigned char ArtsStrade[VERTICAL_LINES_MAX][HORIZONTAL_BYTES_MAX];
extern const unsigned char Triden[VERTICAL_LINES_MAX][HORIZONTAL_BYTES_MAX];
extern const unsigned char AnalogClock[VERTICAL_LINES_MAX][HORIZONTAL_BYTES_MAX];

extern const unsigned char FlameAnimation[VERTICAL_LINES_MAX * 5][HORIZONTAL_BYTES_MAX];


#ifdef	__cplusplus
}
#endif

#endif //_SCREENS_STATIC_H_



