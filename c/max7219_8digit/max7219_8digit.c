#include "max7219_8digit.h"

#define REG_NO_OP           0x00
#define REG_DIGIT_BASE      0x01
#define REG_DECODE_MODE     0x09
#define REG_INTENSITY       0x0a
#define REG_SCAN_LIMIT      0x0b
#define REG_SHUTDOWN        0x0c
#define REG_DISPLAY_TEST    0x0f


static const struct {
	char   ascii;
	char   segs;
} MAX7219_Font[] = {
  {' ', 0x00},
  {'0', 0x7e},
  {'1', 0x30},
  {'2', 0x6d},
  {'3', 0x79},
  {'4', 0x33},
  {'5', 0x5b},
  {'6', 0x5f},
  {'7', 0x70},
  {'8', 0x7f},
  {'9', 0x7b},
  {'A', 0x77},
  {'B', 0x1f},
  {'C', 0x4e},
  {'D', 0x3d},
  {'E', 0x4f},
  {'F', 0x47},
  {'\0', 0x00}
};

struct _max7219 {
    spi_inst_t *spi;
    uint cs;
    uint sck;
    uint mosi;
    uint intensity;
    char buffer[8];
};

max7219* max7219__new(spi_inst_t *spi_arg, uint cs_arg, uint sck_arg, uint mosi_arg, uint intensity_arg) {

    max7219* me = (max7219*)malloc(sizeof(struct _max7219));

    me->spi = spi_arg;
    me->cs = cs_arg;
    me->sck = sck_arg;
    me->mosi = mosi_arg;
    me->intensity = intensity_arg;

    gpio_init(me->cs);
    gpio_put(me->cs, 1);
    gpio_set_dir(me->cs, GPIO_OUT);

    max7219__reset(me);

    return me;
}

void max7219__reset(max7219 *me) {
    max7219__set_register(me, REG_DECODE_MODE, 0);
    max7219__set_register(me, REG_INTENSITY, me->intensity);
    max7219__set_register(me, REG_SCAN_LIMIT, 7);
    max7219__set_register(me, REG_DISPLAY_TEST, 0);
    max7219__set_register(me, REG_SHUTDOWN, 1);
}

void max7219__set_register(max7219 *me, uint8_t register_arg, uint8_t value_arg) {
    uint8_t args[2];
    args[0] = register_arg;
    args[1] = value_arg;
    gpio_put(me->cs, 1);
    gpio_put(me->cs, 0);
    spi_write_blocking(me->spi, args, 2);
    gpio_put(me->cs, 1);

    /*
    LOAD_1();                                           // take LOAD high to begin
    MAX7219_SendByte(reg_number);                       // write register number to MAX7219
    MAX7219_SendByte(dataout);                          // write data to MAX7219
    LOAD_0();                                           // take LOAD low to latch in data
    LOAD_1();                                           // take LOAD high to end
    */
}

char max7219__decode_char(max7219 *me, char c) {
    for (uint8_t i = 0; MAX7219_Font[i].ascii; i++) {            // scan font table for ascii code
        if (c == MAX7219_Font[i].ascii) {
            return MAX7219_Font[i].segs;
        }
    }
    //Blank character
    return 0x00;
}

void max7219__write_to_buffer(max7219 *me, const char *str) {
    char str_temp[9]; //9 because null char
    strncpy(str_temp, str, 9);

    //Write 8 digits
    for (uint8_t i = 0; i < 8; i++) {
        me->buffer[7-i] = max7219__decode_char(me, str_temp[i]);
    }
    

}

void max7219__display(max7219 *me) {
    for (uint8_t i = 0; i < 8; i++) {
        max7219__set_register(me, REG_DIGIT_BASE + i, me->buffer[i]);
        
        // Debug
        //max7219__set_register(me, REG_DIGIT_BASE + i, MAX7219_Font[i+1].segs);
    }
}

void max7219__set_intensity(max7219 *me, uint intensity_arg) {
    me->intensity = intensity_arg;
}

void max7219__update_intensity(max7219 *me) {
    max7219__set_register(me, REG_INTENSITY, me->intensity);
}
