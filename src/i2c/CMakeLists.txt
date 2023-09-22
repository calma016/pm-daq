FILE(GLOB *.c *.h)

add_Library(i2c 
    i2c.c 
    i2c.h
)
target_link_libraries(i2c
    pico_stdlib
    hardware_i2c
)
target_include_directories(i2c PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(i2c PUBLIC 
I2C_PROG_SCL = 6 
I2C_PROG_SDA = 7
)