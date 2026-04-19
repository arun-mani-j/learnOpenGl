name = '@0@_example'.format(chapter)
exe = executable('example',
                 ['example.cpp'],
                 cpp_args: ['-DNAME="@0@"'.format(name)],
                 dependencies: dependencies,
                 install: true,
                )
