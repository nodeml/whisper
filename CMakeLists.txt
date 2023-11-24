cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

include(${CMAKE_SOURCE_DIR}/utils.cmake)

cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

project(nodeml_whisper VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DNAPI_VERSION=4)

include_directories(${CMAKE_JS_INC})

set(WHISPER_CPP_VERSION "1.5.0" CACHE STRING "Whisper.cpp Version")

set(WHISPER_CPP_DIR ${CMAKE_SOURCE_DIR}/deps/whisper)

DownloadWhisperCpp(${WHISPER_CPP_VERSION} ${CMAKE_SOURCE_DIR}/deps)

set(WHISPER_BUILD_TESTS OFF)
set(WHISPER_BUILD_EXAMPLES OFF)

find_package(CUDAToolkit)

if (CUDAToolkit_FOUND)
    set(WHISPER_CUBLAS ON)
endif()


add_subdirectory(${CMAKE_SOURCE_DIR}/deps/whisper whisper)

# Build a shared library named after the project from the files in `src/`
file(GLOB_RECURSE SOURCE_FILES "src/*.cc" "src/*.h")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

target_link_libraries(${PROJECT_NAME} whisper)

target_include_directories(${PROJECT_NAME} PRIVATE ${WHISPER_CPP_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

GenerateNodeLIB()

add_custom_command(TARGET ${PROJECT_NAME}
                    POST_BUILD
                    COMMAND node ${CMAKE_SOURCE_DIR}/scripts/dlls.js)