#include <stdint.h>

#define GPIO_BASE 0xD4019000
#define MFPR_96 (0xD401E000 + 0x1E0)

union mfpr {
    uint32_t i;
    struct {
        unsigned int af_sel : 3;
        unsigned int spu : 1;
        unsigned int edge_rise_en : 1;
        unsigned int edge_fall_en : 1;
        unsigned int edge_clear : 1;
        unsigned int sle : 1;
        unsigned int st : 2;
        unsigned int drive : 3;
        unsigned int pulldn_en : 1;
        unsigned int pullup_en : 1;
        unsigned int pull_sel : 1;
        unsigned int : 16;
    };
};

void sleep(void) {
    for (long long i = 0; i < 10000000; i++);
}

void kernel_start(void) {
    volatile uint32_t* gpio = (uint32_t*)(GPIO_BASE + 0x100);
    volatile uint32_t* mfpr = (uint32_t*)MFPR_96;

    union mfpr gpio_init = {.pull_sel = 0, .af_sel = 1};
    *mfpr = gpio_init.i;

    volatile uint32_t* gpio_direction_set = (uint32_t*)((uint8_t*)gpio + 0x54);
    volatile uint32_t* gpio_set = (uint32_t*)((uint8_t*)gpio + 0x18);
    volatile uint32_t* gpio_clear = (uint32_t*)((uint8_t*)gpio + 0x24);
    *gpio_direction_set = 1;
    while (1) {
        *gpio_set = 1;
        sleep();
        *gpio_clear = 1;
        sleep();
    }
}

