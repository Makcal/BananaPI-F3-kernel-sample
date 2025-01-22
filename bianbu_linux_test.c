#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define GPIO_BASE 0xD4019000
#define MFPR_96 (0xD401E000 + 0x1E0)
#define PAGE_SIZE (4 * 1024)

typedef uint8_t uint8;
typedef uint32_t uint32;

union mfpr {
    uint32 i;
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

volatile uint8* get_io(off_t offset, int mem_fd) {
    off_t al_off = offset - (offset) % PAGE_SIZE;
    uint8* al_mp = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, mem_fd, al_off);
    if ((long long)al_mp == -1)
        err(EXIT_FAILURE, "mmap");

    volatile uint8* mp = al_mp + (offset) % PAGE_SIZE;
    return mp;
}

inline volatile uint32_t* shift_pointer(volatile uint32_t* p, int offset) {
    return (uint32_t*)((uint8_t*)p + offset);
}

int main() {
    int mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (mem_fd == -1)
        err(EXIT_FAILURE, "open mem");

    volatile uint32* gpio = (uint32*)get_io(GPIO_BASE + 0x100, mem_fd);
    volatile uint32* mfpr = (uint32*)get_io(MFPR_96, mem_fd);

    union mfpr gpio_init = {.pull_sel = 0, .af_sel = 1};
    uint32 x = *mfpr;
    *mfpr = gpio_init.i;
    for (int i = 0; i < 32; i++) {
        printf("%u", x >> (31 - i) & 1);
    }
    printf("\n");
    // sleep(5);

    volatile uint32_t* gpio_direction_set = shift_pointer(gpio, 0x54);
    volatile uint32_t* gpio_set = shift_pointer(gpio, 0x18);
    volatile uint32_t* gpio_clear = shift_pointer(gpio, 0x24);
    *gpio_direction_set = 1;

    uint32 sw = 0;
    while (1) {
        sw = 1 - sw;
        //*gpio = *gpio | 1;
        // printf("\x1b[2J\x1b[H");
        // for (int i = 0; i < 1024; i++)
        // printf("%u:%u ", i, *(gpio + i));
        *gpio_set = sw;
        *gpio_clear = 1 - sw;
        usleep(10000);
        printf("%u ", *gpio & 1);
        printf("\n");
    }

    return 0;
}

