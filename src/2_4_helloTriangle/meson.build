src_names = ['example', 'exercise_1', 'exercise_2', 'exercise_3']

foreach src_name: src_names
  name ='@0@_@1@'.format(chapter, src_name)
  src = ['@0@.cpp'.format(src_name)]
  exe = executable(
    src_name,
    src,
    cpp_args: ['-DNAME="@0@"'.format(name)],
    dependencies: dependencies,
    install: true,
  )
endforeach
