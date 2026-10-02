# ESP-IDF build port

This component builds the portable FlameClock application, graphics and assets
for ESP32-C3. Its explicit source list excludes the PIC24 implementations in
`../peripferals/pic24_periferals`, `../app/interrupts_pic24.c` and
`../app/delays.c`. Shared HAL headers remain in `../peripferals`.
The portable `../log/log_HAL.c` is included.

`../app/interrupts_esp.c` scans the display from an ESP-IDF GPTimer-driven
task using double-buffered frames. Sound uses LEDC PWM and an `esp_timer`
one-shot for its duration. `hardware_stubs.c` now supplies only the temporary
software RTC. Its date is initialized from build time and stored in RAM; it
does not advance or persist across resets.
Dates use decimal fields and years 00–99 (2000–2099).

`delays_esp.c` provides FreeRTOS millisecond delays (rounded up to a tick) and
ESP ROM microsecond delays. Log output uses the default ESP-IDF console.

`../peripferals/esp32_c3_periferals/uart_console_esp.c` implements the legacy UART interfaces using the ESP-IDF
UART driver. UART_CH1 maps to the configured console, currently UART0 at
115200 baud, 8 data bits, no parity, 1 stop bit, no hardware flow control.
The default ESP32-C3 pins are TX GPIO21 and RX GPIO20. Connect a USB-to-UART
adapter with 3.3 V logic: adapter RX to GPIO21, adapter TX to GPIO20, and GND
to GND. An onboard USB-to-UART bridge normally already provides this wiring.
Native USB Serial/JTAG is a different interface.

Application logging lives in `../log/log.c` and `../log/log.h`, included as
`log/log.h`. It is compiled through clock_port to avoid replacing ESP-IDF's
own component named `log`. printf/DebugMsg use the UART VFS driver; getch is
non-blocking (zero means no character, as in the original interface). Only
the legacy UART_CH1 is supported. The PIC24 UART implementation is excluded.

After flashing, run `idf.py -p COMx monitor` (replace COMx with the adapter's
port). The monitor baud rate is 115200. The startup message identifies the
active USB-UART logging backend.

From an activated ESP-IDF terminal in the project root:

```text
idf.py build
```

The configured target is ESP32-C3. For a fresh configuration, first run
`idf.py set-target esp32c3`. Build outputs are `build/flame_clock.elf` and
`build/flame_clock.bin`. Building does not flash a device.

The RTC remains a software stub. Do not add the PIC24 register drivers to the
ESP-IDF build.
