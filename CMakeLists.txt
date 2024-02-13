cmake_minimum_required(VERSION 3.25)
project(movement-zd)
set(version v2.0.0)
message("=> Movement ZD ${version} <=")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Curses REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

file(GLOB_RECURSE sources src/main/*.cpp src/main/*.h)

add_executable(movement-zd ${sources})

target_link_libraries(movement-zd ${CURSES_LIBRARIES})

file(COPY man/man6/ DESTINATION man)

install(TARGETS movement-zd DESTINATION bin)
install(DIRECTORY man/man6/ DESTINATION share/man/man6 FILES_MATCHING PATTERN "*.6")
