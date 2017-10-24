// This is free and unencumbered software released into the public domain.
// For more information, see UNLICENSE.

#include "common/time.h"

#include <stdio.h>
#include <stdlib.h>

static const double BENCHMARK_TIME = 5.0;
#define NUM_ALLOCS 1000000

static void* s_addresses[NUM_ALLOCS];

int main(int argc, const char** argv) {
  (void)argc;
  (void)argv;
  printf("Benchmark: Allocate/free %d memory chunks (4-128 bytes)...\n", NUM_ALLOCS);
  fflush(stdout);

  double best_time = 1e9;
  const double start_t = get_time();
  while (get_time() - start_t < BENCHMARK_TIME) {
    const double t0 = get_time();

    for (int i = 0; i < NUM_ALLOCS; ++i) {
      const size_t memory_size = ((i % 32) + 1) * 4;
      s_addresses[i] = malloc(memory_size);
      ((char*)s_addresses[i])[0] = 1;
    }

    for (int i = 0; i < NUM_ALLOCS; ++i) {
      free(s_addresses[i]);
    }

    double dt = get_time() - t0;
    if (dt < best_time) {
      best_time = dt;
    }
  }

  printf("%f ns / alloc\n", (best_time / (double)NUM_ALLOCS) * 1000000000.0);
  fflush(stdout);

  return 0;
}

