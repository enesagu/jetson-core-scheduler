cmake_minimum_required(VERSION 3.16)
project(jetson_task_scheduler VERSION 0.1.0 LANGUAGES CXX)

# C++17 standardı
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler uyarıları
add_compile_options(-Wall -Wextra -Wpedantic)

# Include dizini
include_directories(${CMAKE_SOURCE_DIR}/include)

# Kütüphane (static)
add_library(task_scheduler_core STATIC
    src/task.cpp
)

# Ana executable (demo)
add_executable(task_demo
    src/main.cpp
)

target_link_libraries(task_demo PRIVATE task_scheduler_core)

# Install hedefleri (opsiyonel)
install(TARGETS task_scheduler_core DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
