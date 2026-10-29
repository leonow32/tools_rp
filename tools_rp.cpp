#include "../_config/_config.h"
#if COMPONENT_TOOLS_RP
#include "tools_rp.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/flash.h"
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

}

#endif
