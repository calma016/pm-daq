FILE(GLOB *.c *.h)

add_Library(gps
    air530.h
    air530.c

)
target_link_libraries(gps
    pico_stdlib
    
)
target_include_directories(gps PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")