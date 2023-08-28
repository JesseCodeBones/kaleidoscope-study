cmake_minimum_required(VERSION 3.0.0)
project(kaleidoscope-study VERSION 0.1.0 LANGUAGES C CXX)
set(LLVM_REQUIRED_CXX_STANDARD 17)
include(CTest)
enable_testing()
set(LLVM_DIR /usr/local/lib/cmake/llvm/)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

aux_source_directory(src sources)
add_executable(kaleidoscope-study ${sources})

llvm_map_components_to_libnames(llvm_libs Core
ExecutionEngine
Object
OrcJIT
Support
native)
target_link_libraries(kaleidoscope-study ${llvm_libs})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
