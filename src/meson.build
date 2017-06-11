# This is free and unencumbered software released into the public domain.
# For more information, see UNLICENSE.

project('osbench', ['c'])

# All includes are relative to the root directory.
root_inc = include_directories('.')

# Common functions for all benchmarks.
subdir('common')

# We need a threading library.
thread_dep = dependency('threads', required: true)

# We need the math library on some systems.
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

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
          common_dep
          ]
      )
endif

# Launch programs benchmark.
launch_programs_exe = executable(
    'launch_programs',
    ['launch_programs.c'],
    include_directories: [root_inc],
    dependencies: [
        common_dep
        ]
    )

# File creation benchmark.
create_files_exe = executable(
    'create_files',
    ['create_files.c'],
    include_directories: [root_inc],
    dependencies: [
        common_dep,
        m_dep
        ]
    )

# Memory allocation benchmark.
mem_alloc_exe = executable(
    'mem_alloc',
    ['mem_alloc.c'],
    include_directories: [root_inc],
    dependencies: [
        common_dep
        ]
    )

