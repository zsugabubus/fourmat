#ifndef FOURMAT_H
#define FOURMAT_H
#include <stdint.h>

/* functions write the formatted result to |str| and return the number of
 * written bytes */

/* format |time| seconds */
int
fmt_time(char *str, uint32_t time);

/* format |num| bytes/seconds; use powers of 1000 (SI) */
int
fmt_speed(char *str, uint64_t num);

/* format |num| bytes; use powers of 1024 (IEC) */
int
fmt_space(char *str, uint64_t num);

/* format |num| */
int
fmt_number(char *str, uint64_t num);

/* format |num|/|total| */
int
fmt_percent(char *str, uint64_t num, uint64_t total);
#endif /* FOURMAT_H */
