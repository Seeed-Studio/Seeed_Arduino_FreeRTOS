#include <stdio.h>
#include "Arduino.h"
extern "C"{
void freertos_printf(const char *format, ...) {
    char print_buf[512] = { 0 };

    va_list args;
    va_start(args, format);
    int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
    va_end(args);

    if (r > 0) {
        SERIAL_PORT_MONITOR.write(print_buf);
    }
}
}
