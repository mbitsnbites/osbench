// This is free and unencumbered software released into the public domain.
// For more information, see UNLICENSE.

#include "common/time.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)

// Windows implementation.
#define WIN32_LEAN_AND_MEAN
#include <string.h>
#include <windows.h>

typedef struct {
  HANDLE hProcess;
  HANDLE hThread;
} process_t;

static process_t create_process(const char* arg0, int process_no) {
  // The program name is the first argument to the parent process.
  const char* prg_name = arg0;

  // Construct a valid command line.
  // TODO(m): We could do this outside of the benchmark loop.
  size_t prg_name_len = strlen(prg_name);
  char* cmd_line = (char*)malloc(prg_name_len + 5);
  if (!cmd_line) {
    fprintf(stderr, "*** Out of memory (process #%d)\n", process_no);
    exit(1);
  }
  strncpy(&cmd_line[1], prg_name, prg_name_len);
  cmd_line[0] = '"';
  cmd_line[prg_name_len + 1] = '"';
  cmd_line[prg_name_len + 2] = ' ';
  cmd_line[prg_name_len + 3] = '-';
  cmd_line[prg_name_len + 4] = 0;

  // Start the child process.
  STARTUPINFO startup_info;
  memset(&startup_info, 0, sizeof(STARTUPINFO));
  startup_info.cb = sizeof(STARTUPINFO);
  PROCESS_INFORMATION process_info;
  if (!CreateProcess(prg_name, cmd_line, NULL, NULL, TRUE, 0, NULL, NULL, &startup_info, &process_info)) {
    fprintf(stderr, "*** Unable to create process no. %d\n", process_no);
    exit(1);
  }

  free((void*)cmd_line);
  return (process_t){ process_info.hProcess, process_info.hThread };
}

static void wait_process(process_t pid) {
  WaitForSingleObject(pid.hProcess, INFINITE);
  CloseHandle(pid.hProcess);
  CloseHandle(pid.hThread);
}

#else

// Unix implementation.
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef pid_t process_t;

static process_t create_process(const char* arg0, int process_no) {
  pid_t pid = fork();
  if (pid == 0) {
    execlp(arg0, arg0, "-", (char*)0);
  } else if (pid < 0) {
    fprintf(stderr, "*** Unable to create process no. %d\n", process_no);
    exit(1);
  }
  return (process_t)pid;
}

static void wait_process(process_t pid) {
  waitpid(pid, (int*)0, 0);
}

#endif

#define NUM_PROGRAMS 100

static const double BENCHMARK_TIME = 5.0;

int main(int argc, char** argv) {
  // When called as a child-process, the first argument is defined.
  if (argc > 1) {
    exit(0);
  }

  printf("Benchmark: Launch %d programs...\n", NUM_PROGRAMS);
  fflush(stdout);

  double best_time = 1e9;
  const double start_t = get_time();
  while (get_time() - start_t < BENCHMARK_TIME) {
    process_t processes[NUM_PROGRAMS];
    const double t0 = get_time();

    // Create all the processes.
    for (int i = 0; i < NUM_PROGRAMS; ++i) {
      processes[i] = create_process(argv[0], i);
    }

    // Wait for all child processes to terminate.
    for (int i = 0; i < NUM_PROGRAMS; ++i) {
      wait_process(processes[i]);
    }

    double dt = get_time() - t0;
    if (dt < best_time) {
      best_time = dt;
    }
  }

  printf("%f us / program\n", (best_time / (double)NUM_PROGRAMS) * 1000000.0);
  fflush(stdout);

  return 0;
}

