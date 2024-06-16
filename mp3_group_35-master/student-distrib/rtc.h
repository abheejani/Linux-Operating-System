#ifndef _RTC_H
#define _RTC_H

#include "types.h"

static volatile int read_flag = 0;
void init_RTC(); // initialize RTC 

void rtc_interrupt(void);
int rtc_write(int32_t fd, void* buf, int32_t nbytes);
int rtc_open(uint8_t* filename);
int rtc_read(int32_t fd, void* buf, int32_t nbytes);
int rtc_close(int32_t fd);


#endif /* _RTC_H */
