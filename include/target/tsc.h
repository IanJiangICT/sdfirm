#ifndef __TSC_H_INCLUDE__
#define __TSC_H_INCLUDE__

#include <target/config.h>
#include <target/generic.h>
#include <driver/tsc.h>

#define TICKS_TO_MICROSECONDS	(TSC_FREQ / 1000)
#define TICKS_TO_MILISECONDS	(TICKS_TO_MICROSECONDS * 1000)

#define tsc_read_counter()	tsc_hw_read_counter()

#endif /* __TSC_H_INCLUDE__ */
