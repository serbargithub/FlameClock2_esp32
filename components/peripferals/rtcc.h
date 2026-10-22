/*******************************************************************************
Copyright 2016 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"

#ifndef _RTCC_H
#define _RTCC_H


/* ESP32 DS3231M backend: task context, years 00..99 mean 2000..2099,
 * weekdays are Monday=1 through Sunday=7. Start preserves retained time.
 * Initialize accepts decimal or BCD according to value->bcdFormat;
 * TimeGet requires that flag to be initialized and returns the same format.
 * BuildTimeGet always returns decimal on ESP32 and initializes the flag.
 * Invalid RTC contents/OSF return zero date fields for startup recovery.
 * I2C failures are logged; TimeGet leaves the caller's value unchanged.
 */
void RTCC_Start(void);
void RTCC_Initialize (RTCC_DATETIME * value);
void RTCC_TimeGet (RTCC_DATETIME * value);
void RTCC_BuildTimeGet( RTCC_DATETIME *dateTime);

#endif // _RTCC_H
