// This is free and unencumbered software released into the public domain.
// For more information, see UNLICENSE.

#include "common/time.h"

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/time.h>
#endif

#if defined(_WIN32)
static LARGE_INTEGER s_start;
static double s_period = 0.0;
#endif

double get_time() {
  double result;
#if defined(_WIN32)
  LARGE_INTEGER count;
  if (s_period == 0.0) {
    LARGE_INTEGER frequency;
    if (QueryPerformanceFrequency(&frequency)) {
      s_period = 1.0 / (double)frequency.QuadPart;
      QueryPerformanceCounter(&s_start);
    }
  }
  if (QueryPerformanceCounter(&count)) {
    result = (count.QuadPart - s_start.QuadPart) * s_period;
  } else {
    result = 0.0;
  }
#else
  struct timeval tv;
  if (gettimeofday(&tv, (struct timezone *)0) == 0) {
    result = ((double)tv.tv_sec) + 0.000001 * (double)tv.tv_usec;
  } else {
    result = 0.0;
  }
#endif
  return result;
}

