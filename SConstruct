#!/usr/bin/env python3
import os
from SCons.Script import Default, Exit, SConscript

godot_cpp_dir = os.environ.get("GODOT_CPP_DIR", "../cpp_libs/godot-cpp")
godot_cpp_sconstruct = os.path.join(godot_cpp_dir, "SConstruct")

if not os.path.exists(godot_cpp_sconstruct):
    print(f"Error: godot-cpp not found at {godot_cpp_dir}")
    print("Please set GODOT_CPP_DIR environment variable or check your paths.")
    Exit(1)

env = SConscript(godot_cpp_sconstruct)

if env["platform"] == "linux":
    env['MAXLINELENGTH'] = 8192

lib_name = "the_wait_cpp"
sources = env.Glob("src/*.cpp")
env.Append(CPPPATH=["src/"])

target_path = "../bin/{}{}".format(lib_name, env["suffix"])

library = env.SharedLibrary(
    target=target_path,
    source=sources,
)

Default(library)