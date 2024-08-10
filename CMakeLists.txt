cmake_minimum_required(VERSION 3.12)
set(PostgreSQL_ADDITIONAL_VERSIONS "16")

project(cmakeSQL_libpqxx)

add_subdirectory("C:\\cpp\\libpqxx-7.9.2\\libpqxx-7.9.2" libpqxx-build)

set(SOURCE_EXE main.cpp)
add_executable(cmakeSQL_libpqxx ${SOURCE_EXE} "ClientManager.h" "ClientManager.cpp")

target_compile_features(cmakeSQL_libpqxx PRIVATE cxx_std_17)

target_link_libraries(cmakeSQL_libpqxx PRIVATE pqxx)