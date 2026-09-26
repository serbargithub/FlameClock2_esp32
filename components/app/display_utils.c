#include <p24Fxxxx.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h> 
#include "definitions.h"
#include "fonts/fonts.h"
#include "display_utils.h"


static uint8_t g_CurrentPositionX, g_CurrentPositionY;
static uint8_t aLineData[HORIZONTAL_BYTES_MAX];
static uint8_t aLineMask[HORIZONTAL_BYTES_MAX];

const char *WeekDay[] = {"", "Mon", "Tue", "Wen", "Thu", "Fri", "Sat", "Sun"};
const char *Month[] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

uint8_t ShiftRightByBytes(uint8_t shiftValue) {

    uint8_t shiftBytes = shiftValue / 8;
    if (shiftBytes == 0) {
        return (shiftValue - shiftBytes * 8);
    }
    for (uint8_t pos = HORIZONTAL_BYTES_MAX; pos > 0; pos--) {

        if (pos <= (HORIZONTAL_BYTES_MAX - shiftBytes)) {
            aLineData[pos - 1 + shiftBytes] = aLineData[pos - 1];
            aLineData[pos - 1] = 0xFF;
            aLineMask[pos - 1 + shiftBytes] = aLineMask[pos - 1];
            aLineMask[pos - 1] = 0x00;
        } else {
            aLineData[pos - 1] = 0xFF;
            aLineMask[pos - 1] = 0x00;
        }
    }
    return (shiftValue - shiftBytes * 8);
}

void ShifRightByBites(uint8_t shiftValue) {
    for (uint8_t i = 0; i < shiftValue; i++) {
        for (uint8_t pos = HORIZONTAL_BYTES_MAX; pos > 0; pos--) {
            aLineData[pos - 1] = aLineData[pos - 1] >> 1;
            aLineMask[pos - 1] = aLineMask[pos - 1] >> 1;
            if ((pos - 1) == 0) {
                aLineData[pos - 1] |= 0x80;
            } else {
                aLineData[pos - 1] |= (aLineData[pos - 2] & 0x01) ? 0x80 : 0x00;
                aLineMask[pos - 1] |= (aLineMask[pos - 2] & 0x01) ? 0x80 : 0x00;
            }
        }
    }
}

void ShiftRightPreparedLine(uint8_t shiftValue) {
    shiftValue = ShiftRightByBytes(shiftValue);
    ShifRightByBites(shiftValue);
}

void AddPreparedLineToScreen(DisplayFrame_t* displayFrame, uint8_t currentLine) {
    for (uint8_t pos = 0; pos < HORIZONTAL_BYTES_MAX; pos++) {
        displayFrame->data[currentLine][pos] |= aLineMask[pos];
        displayFrame->data[currentLine][pos] &= aLineData[pos];
    }
}

bool Display_PutSymbol(DisplayFrame_t* displayFrame, uint8_t screenX, uint8_t screenY, char symbol) {

    if ((screenX > HORIZONTAL_PIXEL_MAX) || (screenX > VERTICAL_LINES_MAX)) {
        return false;
    }
    SymbolData_t* symbolData = Font_GetNewSymbolData(symbol);
    g_CurrentPositionX = screenX;
    g_CurrentPositionY = screenY;
    uint8_t symbolX = 0, symbolY = 0;
    for (symbolY = 0; symbolY < symbolData->height; symbolY++) {
        memset(aLineData, 0xFF, sizeof (aLineData));
        memset(aLineMask, 0, sizeof (aLineData));
        for (symbolX = 0; symbolX < symbolData->stride; symbolX++) {
            uint8_t byteOut = *(symbolData->data + symbolX + symbolY * symbolData->stride);
            aLineData[symbolX] = (byteOut^0xFF);
            aLineMask[symbolX] = 0xFF;
        }
        ShiftRightPreparedLine(screenX);
        AddPreparedLineToScreen(displayFrame, screenY + symbolY);
    }
    return true;
}

bool Display_PutNextSymbol(DisplayFrame_t* displayFrame, char symbol) {
    SymbolData_t* symbolData = Font_GetCurrentSymbolData();
    uint8_t nextX = g_CurrentPositionX + symbolData->width;
    uint8_t nextY = g_CurrentPositionY;

    return Display_PutSymbol(displayFrame, nextX, nextY, symbol);
}

#define PRINT_STRING_MAX 128

bool Display_Printf(DisplayFrame_t* displayFrame, uint8_t screenX, uint8_t screenY, char* strToPrint, ...) {
    char formatted_message[PRINT_STRING_MAX] = "";
    va_list args;
    va_start(args, strToPrint);
    vsprintf(formatted_message, strToPrint, args);
    va_end(args);
    uint8_t symbol = *(formatted_message);
    if (symbol == 0) {
        return false;
    }
    Display_PutSymbol(displayFrame, screenX, screenY, symbol);

    for (uint8_t i = 1; i < PRINT_STRING_MAX; i++) {
        uint8_t symbol = *(formatted_message + i);
        if (symbol == 0) {
            break;
        }
        symbol = *(formatted_message + i);
        Display_PutNextSymbol(displayFrame, symbol);
    }
    return true;
}

bool Display_PutPixel(DisplayFrame_t* displayFrame, uint8_t screenX, uint8_t screenY, uint8_t color) {

    if ((screenX > HORIZONTAL_PIXEL_MAX) || (screenX > VERTICAL_LINES_MAX)) {
        return false;
    }
    memset(aLineData, 0xFF, sizeof (aLineData));
    memset(aLineMask, 0, sizeof (aLineData));

    if (color) {
        aLineData[0] = 0x7F;
    }
    aLineMask[0] = 0x80;

    ShiftRightPreparedLine(screenX);
    AddPreparedLineToScreen(displayFrame, screenY);
    return true;
}

bool Display_SetImage(DisplayFrame_t* displayFrame, const uint8_t* imageData, uint16_t imageSize) {

    if (imageSize > sizeof (DisplayFrame_t)) {
        imageSize = sizeof (DisplayFrame_t);
    }
    memcpy(displayFrame->data, imageData, imageSize);

    return true;
}
