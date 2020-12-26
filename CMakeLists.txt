cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(conditions)

# JSON
# Typically you don't care so much for a third party library's tests to be
# run from your own project's code.
set(JSON_BuildTests OFF CACHE INTERNAL "")
# If you only include this third party in PRIVATE source files, you do not
# need to install it when your main project gets installed.
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(3dparty/json/nlohmann_json)


add_executable(conditions main.cpp)

target_include_directories(conditions PUBLIC src)
target_sources(conditions PUBLIC src/organization.cpp)
target_compile_features(conditions PUBLIC cxx_std_17)
target_link_libraries(conditions PRIVATE nlohmann_json::nlohmann_json)
