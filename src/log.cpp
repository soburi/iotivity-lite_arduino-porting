#include <Arduino.h>
#include <stdarg.h>
#include <Stream.h>
#include "port/oc_log.h"
#define MAX_LOG_BUFFER_SIZE 80

class NullStream : public Stream {
public:
  size_t write(uint8_t) {return 1; }
  int available() { return 0; }
  int read() { return 0; }
  int peek() { return 0; }
};

static NullStream nullstream;
static Stream* serial_ref = &nullstream;

void oc_arduino_set_logstream(Stream& strm)
{
  serial_ref = &strm;
}

#if defined(__AVR__)
void avr_log(PROGMEM const char *format, ...) {
#else
void arm_log(const char *format, ...) {
#endif
  do {
    va_list ap;
    va_start(ap, format);
#if defined(__AVR__)
    uint16_t formatLength = strlen_P((PGM_P)format) ; // cast it to PGM_P , which is const char *
#else
    uint16_t formatLength = strlen(format) ; // cast it to PGM_P , which is const char *
#endif
    if(formatLength == 0 ) return;
    char print_buffer[MAX_LOG_BUFFER_SIZE];
#if defined(__AVR__)
    vsnprintf_P(print_buffer, sizeof(print_buffer), (const char *)format, ap);
#else
    vsnprintf(print_buffer, sizeof(print_buffer), (const char *)format, ap);
#endif
    for (char *p = &print_buffer[0]; *p; p++)
    {
      // emulate cooked mode for newlines
      if (*p == '\n')
      {
        serial_ref->write('\r');
      }
      serial_ref->write(*p);
    }
    va_end(ap);
  } while (0);
}
