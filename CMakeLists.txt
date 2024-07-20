cmake_minimum_required(VERSION 3.0)
project(rsh)

# Set the version variable
set(VERSION "v0.0.1-dev")

# Add compile options
add_compile_options(-Wall -O2 -DVERSION="${VERSION}")

# Add your source files
set(SOURCES
    src/main.c
)

# Create executable
add_executable(rsh ${SOURCES})

# Link ncurses library
target_link_libraries(rsh ${CURSES_LIBRARIES})
