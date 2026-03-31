project('glad', 'c', version: '0.1')

glad_inc = include_directories('include')

sources = ['src/gl.c']

glad_lib = library(meson.project_name(), sources, include_directories: glad_inc)

glad_dep = declare_dependency(include_directories: glad_inc, link_with: glad_lib)
