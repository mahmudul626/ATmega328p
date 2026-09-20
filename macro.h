#ifndef MACRO_H
#define MACRO_H

#define SET_BIT(reg, bit) (reg |= (1 << (bit)))
#define CLR_BIT(reg, bit) (reg &= ~(1 << (bit)))
#define FLIP_BIT(reg, bit) (reg ^= (1 << (bit)))
#define READ_BIT(reg, bit) (((reg) >> (bit)) & 0x01)

#endif
