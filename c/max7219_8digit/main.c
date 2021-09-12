#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "max7219_8digit.h"


//SPI pins for MAX
const uint CS_MAX   = 5;
const uint SCK_MAX  = 2;
const uint MOSI_MAX = 3;
const uint MISO_MAX = 4;

#define LED 0

int main() {
    
    spi_inst_t *spi = spi0;
    spi_init(spi, 1000 * 1000);

    // Set SPI format
    spi_set_format( spi0,   // SPI instance
                    8,      // Number of bits per transfer
                    1,      // Polarity (CPOL)
                    0,      // Phase (CPHA)
                    SPI_MSB_FIRST);

    // Initialize SPI pins
    gpio_set_function(SCK_MAX, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_MAX, GPIO_FUNC_SPI);
    gpio_set_function(MISO_MAX, GPIO_FUNC_SPI);

    bi_decl(bi_3pins_with_func(MISO_MAX, MOSI_MAX, SCK_MAX, GPIO_FUNC_SPI));

    max7219 *max = max7219__new(spi, CS_MAX, SCK_MAX, MOSI_MAX, 7);

    max7219__write_to_buffer(max, "12345678");
    max7219__display(max);

    while(1) {
        for (uint i = 0; i < 16; i++) {
            max7219__set_intensity(max, i);
            max7219__update_intensity(max);
            sleep_ms(250);
        }
    }

    return 0;
}