#include "ld26.h"

long get_time() {
	struct timeval actual;
	gettimeofday(&actual, NULL);
	
	return ((actual.tv_sec * 1000) + (actual.tv_usec / 1000));
}

Timer::Timer() {
	last = 0;
	last_tick = 0;
}

void Timer::start() {
	last = get_time();
	last_tick = last;
}

void Timer::stop() {
	
}

long Timer::get_tick() {
	last_tick = get_time();
	return (get_time() - last);
}

long Timer::get_elapsed_last_call() {
	long temp = last_tick;
	last_tick = get_time();
	return (get_time() - temp);
}
