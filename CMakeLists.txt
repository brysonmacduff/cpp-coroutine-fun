cmake_minimum_required(VERSION 3.22)
project(coroutine_fun C CXX)
set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "*.h" "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


