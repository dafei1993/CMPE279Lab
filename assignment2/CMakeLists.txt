CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

PROJECT(assignment2)


SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

add_executable(server server.c)

add_executable(client client.c)


