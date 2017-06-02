# OSBench

This is a collection of benchmarks that aim to measure the performance of operating system primitives, such as process and thread creation.

## Building

To build the benchmarks you need a C compiler, [meson](http://mesonbuild.com/) and [ninja](https://ninja-build.org/).

```bash
mkdir out
cd out
meson --buildtype=release ../src
ninja
```

## Micro benchmarks

All benchmarks run their work for five seconds, and the fastest pass is reported.

### create_threads

Create 100 threads and wait for them to finish. Each thread does nothing (just return).

* POSIX: `pthread_create()`, `pthread_join()`
* WIN32: `_beginthreadex()`, `WaitForSingleObject()`, `CloseHandle()`

### create_processes

Create 100 processes and wait for them to finish. Each process does nothing (just exit).

* Unix: `fork()`, `waitpid()`
* WIN32: Not implemented (Windows lacks the corresponding functionality).

### launch_programs

Launch 100 programs and wait for them to finish. Each program does nothing (just exit).

* Unix: `fork()`, `execlp()`, `waitpid()`
* WIN32: `CreateProcess()`, `WaitForSingleObject()`, `CloseHandle()`

### create_files

Create 65,534 files, write 32 byts of data to each file, and then delete them.

* `fopen()`, `fwrite()`, `fclose()`, `remove()`

NOTE: To measure filesystem/kernel performance rather than storage medium performance, consider using a RAM disk.

### mem_alloc

Allocate 1,000,000 small chunks of memory, and then free them. Each chunk is 4-128 bytes in size.

* `malloc()`, `free()`

