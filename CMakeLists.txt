cmake_minimum_required(VERSION 3.0.0)
project(FluidGamelan VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -pthread")
find_library(LIB_FLUID fluidsynth curses)

add_executable(FluidGamelan main.cpp)

target_link_libraries(${PROJECT_NAME} ${LIB_FLUID})
target_include_directories( FluidGamelan PRIVATE ${CMAKE_BUILD_DIR}/sf2list )

install(TARGETS FluidGamelan DESTINATION BIN)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
