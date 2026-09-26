#include <math.h>
#include <stdlib.h> // Provides exit
#include <ctype.h>
#include "draw.h"
#include "../display_utils.h"

typedef struct {
    int x0;
    int y0; 
    int x1; 
    int y1;
    int color;
} Line_t;


void Draw_Line(char color, DisplayFrame_t* displayFrame, int x0, int y0, int x1, int y1) {

int mirror_on;
int a0, b0, a1, b1;

int deltax = abs(x1 - x0);
int deltay = abs(y1 - y0);
if((deltax == 0) && (deltay == 0)) {
    return;
}
    if (deltax > deltay) {
        mirror_on = 0;
        a0 = x0;
        b0 = y0;
        a1 = x1;
        b1 = y1;
    }
    else {
        mirror_on = 1;
        a0 = y0;
        b0 = x0;
        a1 = y1;
        b1 = x1;
    }
    int delta_a = abs(a1 - a0);
    int delta_b = abs(b1 - b0);

    int error = 0;
    int deltaerr = (delta_b + 1);
    
    int a = a0;
    int direction_a = a1 - a0;
    if (direction_a > 0) {
        direction_a = 1;
    }
    if (direction_a < 0) {
        direction_a = -1;
    }

    int b = b0;
    int direction_b = b1 - b0;
    if (direction_b > 0) {
        direction_b = 1;
    }
    if (direction_b < 0) {
        direction_b = -1;
    }

    for (a = a0; (direction_a>0)?(a <= a1): (a >= a1); a += direction_a) {

        if (!mirror_on) {
            Display_PutPixel(displayFrame, a, b, color);
        }
        else {
            Display_PutPixel(displayFrame, b, a, color);
        }
        error = error + deltaerr;
        if (error >= (delta_a + 1)) {
            b = b + direction_b;
            error = error - (delta_a + 1);
        }
    }
}

void SegmentDrawing(DisplayFrame_t* displayFrame, Line_t* showline) {
    Draw_Line(showline->color, displayFrame, showline->x0, showline->y0, showline->x1, showline->y1);
}

void  SegmentSetPosition(Line_t* line, int pos_x, int pos_y) {
    line->x0 += pos_x;
    line->x1 += pos_x;
    line->y0 += pos_y;
    line->y1 += pos_y;
}

void SegmentRotate(Line_t* line, int angle) {

#define PI 3.14159265
    double angle_rad = angle * (PI / 180);
    Line_t new_line;

    new_line.x0 = round(line->x0 * cos(angle_rad) - line->y0 * sin(angle_rad));
    new_line.y0 = round(line->x0 * sin(angle_rad) + line->y0 * cos(angle_rad));
    new_line.x1 = round(line->x1 * cos(angle_rad) - line->y1 * sin(angle_rad));
    new_line.y1 = round(line->x1 * sin(angle_rad) + line->y1 * cos(angle_rad));

    line->x0 = new_line.x0;
    line->x1 = new_line.x1;
    line->y0 = new_line.y0;
    line->y1 = new_line.y1;
}

void Draw__Object(DisplayFrame_t* displayFrame, DrawObj_t Obj, int pos_x, int pos_y, int angle) {
    
    for (int i = 0; i < Obj.size - 3; i+=3) {

        Line_t SegmentLine;
        SegmentLine.x0 = *(Obj.data + i);
        SegmentLine.y0 = *(Obj.data + i + 1);
        SegmentLine.x1 = *(Obj.data + i +3);
        SegmentLine.y1 = *(Obj.data + i +4);
        SegmentLine.color = *(Obj.data + i +5);
        
        SegmentRotate(&SegmentLine, angle);
        SegmentSetPosition(&SegmentLine, pos_x, pos_y);
        SegmentDrawing(displayFrame, &SegmentLine);
    }
}