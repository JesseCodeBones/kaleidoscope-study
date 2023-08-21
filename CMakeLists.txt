cmake_minimum_required(VERSION 3.0.0)
project(kaleidoscope-study VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()
aux_source_directory(src sources)
add_executable(kaleidoscope-study ${sources})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
