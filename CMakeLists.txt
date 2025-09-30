cmake_minimum_required(VERSION 3.15)
project(DCEPass C CXX)
find_package(LLVM REQUIRED CONFIG)

add_library(DCEPass SHARED
  src/DCEPass.cpp
  src/EliminateDeadAllocas.cpp
  src/EliminateDeadBB.cpp
)

target_compile_features(DCEPass PRIVATE cxx_std_17)

target_include_directories(DCEPass PRIVATE 
  ${LLVM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(DCEPass PRIVATE ${LLVM_DEFINITIONS})

if (NOT MSVC)
  target_compile_options(DCEPass PRIVATE -fvisibility=hidden)
endif()

set_target_properties(DCEPass PROPERTIES OUTPUT_NAME "DCEPass")