#include "../_config/_config.h"
#if COMPONENT_TOOLS_RP
#include "tools_rp.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/flash.h"
#include "hardware/gpio.h"
#include "hardware/watchdog.h"

#if __has_include("hardware/psram.h")
    #define USE_PSRAM 1
    #include "hardware/psram.h"
#endif

extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __StackTop;
extern char __bss_end__;

namespace tools_rp {

void reset_cmd(int argc, char * argv[]) {
    print_ok();
	watchdog_reboot(0, 0, 0);
}

void mem_cmd(int argc, char * argv[]) {
	uint32_t flash_size = PICO_FLASH_SIZE_BYTES;
    uint32_t flash_used = uint32_t(&__flash_binary_end) - uint32_t(&__flash_binary_start);
    uint32_t flash_free = flash_size - flash_used;

    uint32_t ram_size = uint32_t(&__StackTop) - SRAM_BASE;
    uint32_t ram_used = uint32_t(&__bss_end__) - SRAM_BASE;
    uint32_t ram_free = ram_size - ram_used;

    #if USE_PSRAM
        uint32_t psram_size = psram_get_size();
    #else
        uint32_t psram_size = 0;
    #endif

    printf("           FLASH        RAM      PSRAM\n");
    printf("Size: %10lu %10lu %10lu\n", flash_size, ram_size, psram_size);
    printf("Used: %10lu %10lu\n",       flash_used, ram_used);
    printf("Free: %10lu %10lu\n",       flash_free, ram_free);
}

void fcpu_cmd(int argc, char * argv[]) {
    if(argc == 2) {
        uint32_t fcpu_mhz;
        if(parse_uint(argv[1], &fcpu_mhz)) return;
        bool success = set_sys_clock_khz(fcpu_mhz * 1000, true);
        if(success) {
            stdio_init_all();
            print_ok();
        }
    }
    else {
        uint32_t fcpu_mhz = clock_get_hz(clk_sys) / 1000000;
        printf("%lu MHz\n", fcpu_mhz);
    }
}

void gpio_dir_cmd(int argc, char * argv[]) {
    if(argc != 3) {
        printf("num[DEC] dir[in=0, out=1]\n");
        return;
    }

    uint8_t pin_num;
    if(parse_uint(argv[1], &pin_num)) return;
    printf("1\n");

    uint8_t pin_dir;
    if(parse_uint(argv[2], &pin_dir)) return;
    printf("2\n");

    printf("3\n");
    gpio_init(pin_num);
    printf("4\n");
    gpio_set_dir(pin_num, pin_dir);
    printf("5\n");
    printf("pin_num: %u, pin_dir: %u\n", pin_num, pin_dir);
    print_ok();
}

void gpio_out_cmd(int argc, char * argv[]) {
    if(argc != 3) {
        printf("num[DEC] state[0/1]\n");
        return;
    }

    uint8_t pin_num;
    if(parse_uint(argv[1], &pin_num)) return;
    printf("1\n");

    uint8_t pin_state;
    if(parse_uint(argv[2], &pin_state, 10, uint8_t(1))) return;
    printf("2\n");

    gpio_put(pin_num, bool(pin_state));
    printf("3\n");
    printf("pin_num: %u, pin_state: %u\n", pin_num, pin_state);
    print_ok();
}

void gpio_in_cmd(int argc, char * argv[]) {
    if(argc != 2) {
        printf("num[DEC]\n");
        return;
    }

    uint8_t pin_num;
    if(parse_uint(argv[1], &pin_num)) return;

    bool pin_state = gpio_get(pin_num);

    printf("Pin %u state: %u\n", pin_num, pin_state);
}

void test_cmd(int argc, char * argv[]) {
    static bool state = false;

    gpio_init(25);                    // Inicjalizacja pinu (ustawia domyślny multiplexer)
    gpio_set_dir(25, GPIO_OUT);       // Ustawienie kierunku: WYJŚCIE
    gpio_put(25, state);
    
    state = !state;
    print_ok();
}

}

#endif
