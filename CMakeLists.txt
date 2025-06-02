cmake_minimum_required(VERSION 3.22.1)
project(gameboy LANGUAGES CXX)



set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
)

FetchContent_MakeAvailable(SFML)



include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)


file(GLOB_RECURSE MAIN_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.h)
list(FILTER MAIN_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")
# Build executable
add_executable(gameboy src/main.cpp ${MAIN_SOURCES})
target_compile_features(gameboy PUBLIC cxx_std_17)
target_link_libraries(gameboy PRIVATE SFML::Graphics SFML::Window SFML::System)



file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp src/*.h)
add_executable(tests ${MAIN_SOURCES} ${TEST_SOURCES})
target_compile_features(tests PUBLIC cxx_std_17)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

target_compile_options(gameboy PRIVATE -Wall -Wextra -Wshadow -Wconversion -Wno-sign-conversion -Werror)
target_compile_options(tests PRIVATE -Wall -Wextra -Wshadow -Wconversion -Wno-sign-conversion -Werror)