cmake_minimum_required(VERSION 3.6)
find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)

# set(LLVM_LINK_COMPONENTS support)
# if clang add libc++ stdlib flag
if(CMAKE_CXX_COMPILER MATCHES "[cC]lang\\+\\+")
    message(STATUS "FIXATING clang++")
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libstdc++ -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS")
endif()

file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME}
    ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    clangTooling
    clangBasic
    clangASTMatchers)

