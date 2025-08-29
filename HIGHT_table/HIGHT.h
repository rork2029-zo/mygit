#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static inline uint8_t f0(uint8_t tmp) {
    return ((tmp << 1) | (tmp >> 7)) ^ ((tmp << 2) | (tmp >> 6)) ^ ((tmp << 7) | (tmp >> 1));
};
static inline uint8_t f1(uint8_t tmp) {
    return ((tmp << 3) | (tmp >> 5)) ^ ((tmp << 4) | (tmp >> 4)) ^ ((tmp << 6) | (tmp >> 2));
};

void enc(uint8_t CT[], uint8_t PT[], uint8_t MK[]);
void dec(uint8_t DT[], uint8_t CT[], uint8_t MK[]);

