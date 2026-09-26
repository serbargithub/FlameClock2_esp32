#ifndef _DRAW_H_
#define _DRAW_H_

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
#include "../definitions.h"


typedef struct{
    const int *data;
    int16_t size;
} DrawObj_t;

void Draw__Line(char color, DisplayFrame_t* displayFrame, int x0, int y0, int x1, int y1);
void Draw__Object(DisplayFrame_t* displayFrame, DrawObj_t Obj, int pos_x, int pos_y, int angle);

#ifdef	__cplusplus
}
#endif

#endif //_DRAW_H_