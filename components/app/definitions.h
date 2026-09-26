#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>    
#include <stdint.h>

#define	DEVICE_NAME "FlameClock"
#define	DEVICE_VERSION "HW:01 SW:1.0.1" 
#define	BUILD_TIME  __TIME__
#define	BUILD_DATE  __DATE__

    //-------

#define HORIZONTAL_BYTES_MAX 13
#define HORIZONTAL_PIXEL_MAX 100
#define VERTICAL_LINES_MAX 100      

    typedef struct {
        bool bcdFormat;
        uint8_t year;
        uint8_t month;
        uint8_t day;
        uint8_t weekday;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    } RTCC_DATETIME;

    typedef struct {
        uint8_t data[VERTICAL_LINES_MAX][HORIZONTAL_BYTES_MAX];
    } DisplayFrame_t;

    typedef enum {
        AJUST_MODE__OFF = 0,
        AJUST_MODE__HOURS,
        AJUST_MODE__MINUTES,
        AJUST_MODE__SECONDS,
        AJUST_MODE__WEEKS,
        AJUST_MODE__DAYS,
        AJUST_MODE__MONTHS,
        AJUST_MODE__YEARS,
        AJUST_MODE__THE_LAST,
    } AdjustingMode_t;

    typedef struct {
        const int16_t* dataList;
        int16_t sizeOfList;
    } AdjustingList_t;

#ifdef	__cplusplus
}
#endif

#endif //_DEFINITIONS_H_


