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

