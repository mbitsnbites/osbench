#include "common/time.h"

#include <stdio.h>
#include <stdlib.h>

static const double BENCHMARK_TIME = 5.0;
static const int NUM_ALLOCS = 1000000;

int main(int argc, const char** argv) {
  printf("Benchmark: Allocate/free %d memory chunks (4-128 bytes)...\n", NUM_ALLOCS);
  fflush(stdout);

  double best_time = 1e9;
  const double start_t = get_time();
  while (get_time() - start_t < BENCHMARK_TIME) {
    const double t0 = get_time();

    void* addresses[NUM_ALLOCS];

    for (int i = 0; i < NUM_ALLOCS; ++i) {
      const int memory_size = ((i % 32) + 1) * 4;
      addresses[i] = malloc(memory_size);
      ((char*)addresses[i])[0] = 1;
    }

    for (int i = 0; i < NUM_ALLOCS; ++i) {
      free(addresses[i]);
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

