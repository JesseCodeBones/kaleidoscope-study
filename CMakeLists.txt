cmake_minimum_required(VERSION 3.0.0)
project(kaleidoscope-study VERSION 0.1.0 LANGUAGES C CXX)
set(LLVM_REQUIRED_CXX_STANDARD 17)
include(CTest)
enable_testing()

set(LLVM_DIR "/home/jesse/workspace/llvm-project/llvm/build")
find_package(LLVM REQUIRED CONFIG)

# 获取目标列表
get_target_property(target_list LLVM INTERFACE_LINK_LIBRARIES)

# 输出目标列表
message("Targets of ExamplePackage:")
foreach(target IN LISTS target_list)
    message("  ${target}")
endforeach()

aux_source_directory(src sources)
add_executable(kaleidoscope-study ${sources})
target_link_libraries(kaleidoscope-study)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
