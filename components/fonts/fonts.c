#include <p24Fxxxx.h>
#include <stdint.h>
#include "fonts.h"

#define WINDTHS_DATA_SIZE 128
#define METRIC_BLOCK_SIZE 148

typedef struct {
    const uint8_t* widths;
    const uint32_t* format;
    const uint32_t* stride;
    const uint32_t* maxWidth;
    const uint32_t* maxHeight;
    const uint8_t* rawData;
} FontParam_t;

extern const uint8_t Antonio_Regular_16_L1[];
extern const uint8_t BebasNeue_Bold_16_L1[];
extern const uint8_t BebasNeueRegular_12_L1[];
extern const uint8_t m12_16_L1[];
extern const uint8_t m12_20_L1[];

static FontParam_t g_CurrenFont;
static SymbolData_t g_CurrentSymbol;

void Font_GetFontParam(const uint8_t* fontData) {
    g_CurrenFont.widths = fontData;
    g_CurrenFont.format = (uint32_t*) (fontData + WINDTHS_DATA_SIZE);
    g_CurrenFont.stride = (uint32_t*) (fontData + WINDTHS_DATA_SIZE + sizeof (uint32_t));
    g_CurrenFont.maxWidth = (uint32_t*) (fontData + WINDTHS_DATA_SIZE + sizeof (uint32_t) * 2);
    g_CurrenFont.maxHeight = (uint32_t*) (fontData + WINDTHS_DATA_SIZE + sizeof (uint32_t) * 3);
    g_CurrenFont.rawData = (uint8_t*) (fontData + WINDTHS_DATA_SIZE + sizeof (uint32_t) * 5);
}

void Font_SetCurrentFont(FontList_t fontList) {

    switch (fontList) {
        case FONT_ANTONIO_REGULAR_SIZE16:
            Font_GetFontParam(Antonio_Regular_16_L1);
            break;
        case FONT_BEBAS_SIZE12:
            Font_GetFontParam(BebasNeueRegular_12_L1);
            break;
        case FONT_BEBAS_BOLD_SIZE16:
            Font_GetFontParam(BebasNeue_Bold_16_L1);
            break;
        case FONT_M12_SIZE16:
            Font_GetFontParam(m12_16_L1);
            break;
        case FONT_M12_SIZE20:
            Font_GetFontParam(m12_20_L1);
            break;
        default:
            break;
    }
}

SymbolData_t* Font_GetNewSymbolData(char symbol) {

    if ((symbol > 127) || (symbol < 0)) {
        symbol = 0;
    }
    g_CurrentSymbol.width = *(g_CurrenFont.widths + symbol);
    g_CurrentSymbol.stride = *(g_CurrenFont.stride);
    g_CurrentSymbol.height = *(g_CurrenFont.maxHeight);

    uint16_t dataOffset = 0;
    for (uint16_t i = 0; i < symbol; i++) {
        int16_t width = *(g_CurrenFont.widths + i);
        if (width > 0) {
            dataOffset += (g_CurrentSymbol.height * g_CurrentSymbol.stride);
        }
    }
    g_CurrentSymbol.data = g_CurrenFont.rawData + dataOffset;
    return &g_CurrentSymbol;
}

SymbolData_t* Font_GetCurrentSymbolData() {
    return &g_CurrentSymbol;
}