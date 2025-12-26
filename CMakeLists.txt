cmake_minimum_required(VERSION 3.16)
project(jetson_task_scheduler VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(task_scheduler_core STATIC
    src/task.cpp
    src/task_registry.cpp
    src/scheduler.cpp
    src/cpu_utils.cpp
    src/thread_pool.cpp
    src/metrics.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(task_scheduler_core PUBLIC Threads::Threads)

add_executable(task_demo src/main.cpp)
target_link_libraries(task_demo PRIVATE task_scheduler_core)


# Python bindings
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG)

if(pybind11_FOUND)
    pybind11_add_module(jetson_scheduler python/bindings.cpp)
    target_link_libraries(jetson_scheduler PRIVATE task_scheduler_core)
    message(STATUS "Python bindings enabled")
else()
    message(STATUS "pybind11 not found, Python bindings disabled")
endif()