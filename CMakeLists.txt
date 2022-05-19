cmake_minimum_required(VERSION 3.21)
project(myCompiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(myCompiler main.cpp compiler.cpp compiler.h lexical_analyzer.cpp lexical_analyzer.h
        syntax_analyzer.cpp syntax_analyzer.h word.h symbol_table.cpp symbol_table.h node.h quaternary.h quaternary.cpp)
