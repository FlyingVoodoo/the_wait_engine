#!/usr/bin/env python3

from SCons.Script import Default, SConscript

env = SConscript("cpp_libs/godot-cpp/SConstruct")

sources = env.Glob("src/*.cpp")

lib_name = "the_wait_cpp"

env.Append(CPPPATH=["src/"])

library = env.SharedLibrary(
    "bin/{}{}{}".format(lib_name, env["suffix"], '.so'),
    source=sources,
)

Default(library)