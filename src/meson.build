dependencies = [glad, glfw, opengl]

chapters = [
    '2_3_helloWindow',
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
