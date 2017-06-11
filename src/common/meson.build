# This is free and unencumbered software released into the public domain.
# For more information, see UNLICENSE.

common_src = [
    'time.c',
    'time.h'
    ]
common_lib = static_library(
    'common',
    common_src,
    include_directories: [root_inc]
    )
common_dep = declare_dependency(link_with: common_lib)

