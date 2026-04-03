dependencies = [glad, glfw, opengl]

chapters = [
    '2_3_helloWindow',
    '2_4_helloTriangle',
    '2_4_helloTriangle_e1',
    '2_4_helloTriangle_e2',
    '2_4_helloTriangle_e3',
]

foreach chapter : chapters
    sources = ['@0@.cpp'.format(chapter)]
    exe = executable(
        chapter,
        sources,
        cpp_args: ['-DNAME="@0@"'.format(chapter)],
        dependencies: dependencies,
        install: true,
    )
endforeach
