// This is free and unencumbered software released into the public domain.
// For more information, see UNLICENSE.

#include "common/time.h"

#include <stdio.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <process.h>
#include <windows.h>
#else
#include <pthread.h>
#endif

#if defined(_WIN32)

// WIN32 thread implementation.
typedef HANDLE thread_t;

static unsigned WINAPI thread_fun(void* arg) {
  // We do nothing here...
  (void)arg;
  return 0u;
}

static thread_t create_thread() {
  return (HANDLE)_beginthreadex((void*)0, 0, thread_fun, (void*)0, 0, (unsigned*)0);
}

static void join_thread(thread_t thread) {
  if (WaitForSingleObject(thread, INFINITE) != WAIT_FAILED) {
    CloseHandle(thread);
  }
}

#else

// POSIX thread implementation.
typedef pthread_t thread_t;

static void* thread_fun(void* arg) {
  // We do nothing here...
  (void)arg;
  return (void*)0;
}

static thread_t create_thread() {
  thread_t result;
  pthread_create(&result, (const pthread_attr_t*)0, thread_fun, (void*)0);
  return result;
}

static void join_thread(thread_t thread) {
  pthread_join(thread, (void**)0);
}

#endif  // WIN32

#define NUM_THREADS 100

static const double BENCHMARK_TIME = 5.0;

int main() {
  printf("Benchmark: Create/teardown of %d threads...\n", NUM_THREADS);
  fflush(stdout);

  double best_time = 1e9;
  const double start_t = get_time();
  while (get_time() - start_t < BENCHMARK_TIME) {
    thread_t threads[NUM_THREADS];
    const double t0 = get_time();

    // Create all the child threads.
    for (int i = 0; i < NUM_THREADS; ++i) {
      threads[i] = create_thread();
    }

    // Wait for all the child threads to finish.
    for (int i = 0; i < NUM_THREADS; ++i) {
      join_thread(threads[i]);
    }

    double dt = get_time() - t0;
    if (dt < best_time) {
      best_time = dt;
    }
  }

  printf("%f us / thread\n", (best_time / (double)NUM_THREADS) * 1000000.0);
  fflush(stdout);

  return 0;
}

