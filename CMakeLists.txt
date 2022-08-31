cmake_minimum_required(VERSION 3.22)
project(cpp_csv)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_csv main.cpp settings/settings.cpp settings/settings.h parser/cmd_parser.cpp parser/cmd_parser.h logger/logger.cpp logger/logger.h)
