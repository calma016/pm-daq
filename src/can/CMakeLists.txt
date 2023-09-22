FILE(GLOB *.c *.h)

add_Library(can
    haltech.h 
    haltech.c
    mcp2515.c
    mcp2515.h
    registers.h
)
target_link_libraries(can
    pico_stdlib
    hardware_spi
)
target_include_directories(can PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")