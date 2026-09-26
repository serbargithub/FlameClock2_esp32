
#ifndef _FONT_H_
#define _FONT_H_

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {
        FONT_ANTONIO_REGULAR_SIZE16,
        FONT_BEBAS_SIZE12,
        FONT_BEBAS_BOLD_SIZE16,
        FONT_M12_SIZE16,
        FONT_M12_SIZE20,
    } FontList_t;

    typedef struct {
        const uint8_t* data;
        uint8_t stride;
        uint8_t width;
        uint8_t height;
    } SymbolData_t;

    void Font_SetCurrentFont(FontList_t fontList);
    SymbolData_t* Font_GetNewSymbolData(char symbol);
    SymbolData_t* Font_GetCurrentSymbolData();

#ifdef	__cplusplus
}
#endif

#endif //_FONT_H_



