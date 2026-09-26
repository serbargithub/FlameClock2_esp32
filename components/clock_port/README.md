# ESP-IDF build port

This component builds the portable FlameClock application, graphics and assets
for ESP32-C3. Its explicit source list excludes the PIC24 implementations in
`../peripferals`, `../app/interrupts.c` and `../app/delays.c`; `log_HAL.c` is
portable and is included.

`hardware_stubs.c` supplies the existing hardware interfaces for this initial
build stage. It does not configure GPIO, ADC, SPI, UART, display scanning or
sound. Power is reported as present, buttons as released, UART input as empty,
and display submission returns immediately. RTC is a fixed date initialized
from build time, stored in RAM; it does not advance or persist across resets.
Dates use decimal fields and years 00–99 (2000–2099).

`delays_esp.c` provides FreeRTOS millisecond delays (rounded up to a tick) and
ESP ROM microsecond delays. Log output uses the default ESP-IDF console.

From an activated ESP-IDF terminal in the project root:

```text
idf.py build
```

The configured target is ESP32-C3. For a fresh configuration, first run
`idf.py set-target esp32c3`. Build outputs are `build/flame_clock.elf` and
`build/flame_clock.bin`. Building does not flash a device.

When adding real hardware support, replace the corresponding stub definitions
and update this component's source list. Do not add the PIC24 register drivers
to the ESP-IDF build.
