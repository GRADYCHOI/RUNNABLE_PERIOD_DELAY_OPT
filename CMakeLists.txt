cmake_minimum_required(VERSION 3.10)
project(dag-opt)
include_directories(.)

add_executable(opt opt.c) 
target_link_libraries(opt PUBLIC m)
install(TARGETS opt DESTINATION bin)
