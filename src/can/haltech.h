#ifndef _HALTECH_H
#define _HALTECH_H
#include <stdio.h>
#include <pico/stdlib.h>
#include <mcp2515.h>

uint16_t haltech_can_data(canMessage *message);

#endif