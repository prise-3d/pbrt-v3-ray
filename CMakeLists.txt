cmake_minimum_required (VERSION 2.6)
project (topbrt)
set(SRCS
    ./src/topbrt.cpp
    ./src/Chemin.cpp
    )

set(HEADERS
    ./src/Chemin.hpp
   )

add_definitions(-std=c++17)

add_executable(topbrt ${SRCS} ${HEADERS})



target_link_libraries(topbrt )
