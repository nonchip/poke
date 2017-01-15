cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)

# Set some flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")
set(CMAKE_BUILD_TYPE Debug)

# Define our project for future reference
project(poke)

# Define files for libpoke
set(LIBPOKE_SRC
        lib/pk_parse.c
        lib/poke.c)

# Define libpoke library with dependencies
add_library(libpoke STATIC ${LIBPOKE_SRC})

# Add some libraries to libpoke
target_link_libraries(libpoke
        config
        ssh)

# Officially add our file scopes
target_include_directories(libpoke PUBLIC "include")
target_include_directories(libpoke PRIVATE "lib")

#########################################################

# Define files for libpoke
set(POKE_SRC src/test_main.c)

# Then add the executable
add_executable(poke ${POKE_SRC})
target_link_libraries(poke libpoke)