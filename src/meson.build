project('osbench', ['c'])

# All includes are relative to the root directory.
root_inc = include_directories('.')

# Common functions for all benchmarks.
subdir('common')

# We need a threading library.
thread_dep = dependency('threads', required: true)

# Threading benchmark.
create_threads_exe = executable(
    'create_threads',
    ['create_threads.c'],
    include_directories: [root_inc],
    dependencies: [
        common_dep,
        thread_dep
        ]
    )

# Process creation benchmark.
if target_machine.system() != 'windows'
  create_processes_exe = executable(
      'create_processes',
      ['create_processes.c'],
      include_directories: [root_inc],
      dependencies: [
          common_dep,
          ]
      )
endif

# File creation benchmark.
create_files_exe = executable(
    'create_files',
    ['create_files.c'],
    include_directories: [root_inc],
    dependencies: [
        common_dep,
        ]
    )

# Memory allocation benchmark.
mem_alloc_exe = executable(
    'mem_alloc',
    ['mem_alloc.c'],
    include_directories: [root_inc],
    dependencies: [
        common_dep,
        ]
    )

