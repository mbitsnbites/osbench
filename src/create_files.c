// This is free and unencumbered software released into the public domain.
// For more information, see UNLICENSE.

#include "common/time.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const double BENCHMARK_TIME = 5.0;

// Note: The maximum number of files in a folder for different file systems:
//  - 65534 (FAT32)
//  - 4294967295 (NTFS, ext4)
static const int NUM_FILES = 65534;

static double my_log2(double x) {
  static const double LOG2SCALE = 1.442695040888963;  // 1.0 / log(2.0);
  return log(x) * LOG2SCALE;
}

static int num_hex_chars(int max_int) {
  int num_bits = (int)ceil(my_log2((double)max_int));
  return (num_bits + 3) / 4;
}

static char path_separator() {
#if defined(_WIN32)
  return '\\';
#else
  return '/';
#endif
}

static void to_hex(int x, char* str, const int str_len) {
  static const char TOHEX[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
  };

  str += str_len - 1;
  for (int i = 0; i < str_len; ++i) {
    *str-- = TOHEX[x & 15];
    x = x >> 4;
  }
}

static void create_file(const char* file_name) {
  static const char FILE_DATA[32] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
    26, 27, 28, 29, 30, 31
  };

  FILE *f = fopen(file_name, "wb");
  if (!f) {
    fprintf(stderr, "*** Unable to create file \"%s\"\n", file_name);
    exit(1);
  }
  fwrite(FILE_DATA, 1, sizeof(FILE_DATA), f);
  fclose(f);
}

static void delete_file(const char* file_name) {
  remove(file_name);
}

int main(int argc, const char** argv) {
  if (argc != 2) {
    printf("Usage: %s root-folder\n", argv[0]);
    exit(1);
  }

  printf("Benchmark: Create/delete %d files...\n", NUM_FILES);
  fflush(stdout);

  // Create a path string.
  int hex_len = num_hex_chars(NUM_FILES - 1);
  size_t root_path_len = strlen(argv[1]);
  size_t path_len = root_path_len + 1 + hex_len;
  char* file_name = (char*)malloc(path_len + 1);
  if (!file_name) {
    fprintf(stderr, "*** Out of memory!\n");
    exit(1);
  }
  strncpy(file_name, argv[1], root_path_len);
  file_name[root_path_len] = path_separator();
  file_name[path_len] = 0;

  double best_time = 1e9;
  const double start_t = get_time();
  while (get_time() - start_t < BENCHMARK_TIME) {
    const double t0 = get_time();

    for (int file_no = 0; file_no < NUM_FILES; ++file_no) {
      // Construct the file name for this file.
      to_hex(file_no, &file_name[root_path_len + 1], hex_len);

      // Create the file.
      create_file(file_name);
    }

    for (int file_no = 0; file_no < NUM_FILES; ++file_no) {
      // Construct the file name for this file.
      to_hex(file_no, &file_name[root_path_len + 1], hex_len);

      // Delete the file.
      delete_file(file_name);
    }

    double dt = get_time() - t0;
    if (dt < best_time) {
      best_time = dt;
    }
  }

  printf("%f us / file\n", (best_time / (double)NUM_FILES) * 1000000.0);
  fflush(stdout);

  free((void*)file_name);

  return 0;
}

