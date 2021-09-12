#ifndef MAX7219_8DIGIT_H
#define MAX7219_8DIGIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/types.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"

typedef struct _max7219 max7219;

max7219* max7219__new(spi_inst_t *spi_arg, uint cs_arg, uint sck_arg, uint mosi_arg, uint intensity_arg);

void max7219__reset(max7219 *me);

void max7219__set_register(max7219 *me, uint8_t register_arg, uint8_t value_arg);

//Does this even need to be in the interface file?
char max7219__decode_char(max7219 *me, char c);

void max7219__write_to_buffer(max7219 *me, const char *str);

void max7219__display(max7219 *me);

void max7219__set_intensity(max7219 *me, uint intensity_arg);

void max7219__update_intensity(max7219 *me);


#endif /* MAX7219_8DIGIT_H */