FILE(GLOB *.c *.h)

add_Library(spi
    spi.c 
    spi.h
)
target_link_libraries(spi
    pico_stdlib
    hardware_spi
   
)
target_include_directories(spi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_definitions(spi PUBLIC 
    SPI_PROG_SCK = 2
    SPI_PROG_CSN = 9
    SPI_PROG_MOSI = 3
    SPI_PROG_MISO = 4
)