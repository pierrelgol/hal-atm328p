/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hal.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:00:56 by pollivie          #+#    #+#             */
/*   Updated: 2025/03/03 21:00:56 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HAL_H
#define HAL_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint_least8_t  u8;
typedef int_least8_t   i8;
typedef uint_least16_t u16;
typedef int_least16_t  i16;
typedef uint_least32_t u32;
typedef int_least32_t  i32;
typedef uint_least64_t u64;
typedef int_least64_t  i64;
typedef uint_least16_t usize;
typedef int_least16_t  isize;

typedef volatile u8   *ptr8;
typedef volatile u16  *ptr16;
typedef volatile void *opaque;

typedef enum {
        LOW  = 0,
        HIGH = 1,
        ON   = 0,
        OFF  = 1
} State;

#define hal_mask8(bit)                   (1U << (bit))
#define hal_mask16(bit)                  (1UL << (bit))

#define hal_set_bit8(reg, bit)           ((reg) |= hal_mask8(bit))
#define hal_clear_bit8(reg, bit)         ((reg) &= ~hal_mask8(bit))
#define hal_toggle_bit8(reg, bit)        ((reg) ^= hal_mask8(bit))
#define hal_read_bit8(reg, bit)          (((reg) & hal_mask8(bit)) ? HIGH : LOW)
#define hal_write_bit8(reg, bit, state)  ((state) == HIGH ? hal_set_bit8(reg, bit) : hal_clear_bit8(reg, bit))
#define hal_is_bit8_set(reg, bit)        (((reg) & hal_mask8(bit)) != 0)
#define hal_is_bit8_unset(reg, bit)      (((reg) & hal_mask8(bit)) == 0)

#define hal_set_bit16(reg, bit)          ((reg) |= hal_mask16(bit))
#define hal_clear_bit16(reg, bit)        ((reg) &= ~hal_mask16(bit))
#define hal_toggle_bit16(reg, bit)       ((reg) ^= hal_mask16(bit))
#define hal_read_bit16(reg, bit)         (((reg) & hal_mask16(bit)) ? HIGH : LOW)
#define hal_write_bit16(reg, bit, state) ((state) == HIGH ? hal_set_bit16(reg, bit) : hal_clear_bit16(reg, bit))
#define hal_is_bit16_set(reg, bit)       (((reg) & hal_mask16(bit)) != 0)
#define hal_is_bit16_unset(reg, bit)     (((reg) & hal_mask16(bit)) == 0)

#define hal_or_reg8(reg, mask)           ((reg) |= (mask))
#define hal_and_reg8(reg, mask)          ((reg) &= (mask))
#define hal_xor_reg8(reg, mask)          ((reg) ^= (mask))
#define hal_not_reg8(reg)                ((reg) = ~(reg))

#define hal_or_reg16(reg, mask)          ((reg) |= (mask))
#define hal_and_reg16(reg, mask)         ((reg) &= (mask))
#define hal_xor_reg16(reg, mask)         ((reg) ^= (mask))
#define hal_not_reg16(reg)               ((reg) = ~(reg))

#define EXT_INT_LOW_LEVEL                0b00
#define EXT_INT_ANY_CHANGE               0b01
#define EXT_INT_FALLING_EDGE             0b10
#define EXT_INT_RISING_EDGE              0b11

typedef enum {
        GPIO_PORTB = (uint16_t)&PORTB,
        GPIO_PORTC = (uint16_t)&PORTC,
        GPIO_PORTD = (uint16_t)&PORTD
} GpioPort;

typedef struct {
        volatile uint8_t *port;
        volatile uint8_t *ddr;
        volatile uint8_t *pin;
        u8                bit;
} GpioPin;

#define gpio_init(gpio, port_val, bit_val)                                                                                                                     \
        do {                                                                                                                                                   \
                (gpio)->port = (volatile uint8_t *)(port_val);                                                                                                 \
                (gpio)->ddr  = (gpio)->port - 1;                                                                                                               \
                (gpio)->pin  = (gpio)->port - 2;                                                                                                               \
                (gpio)->bit  = (bit_val);                                                                                                                      \
        } while (0)

#define gpio_set_output(gpio)   hal_set_bit8(*(gpio)->ddr, (gpio)->bit)
#define gpio_set_input(gpio)    hal_clear_bit8(*(gpio)->ddr, (gpio)->bit)
#define gpio_set(gpio)          hal_set_bit8(*(gpio)->port, (gpio)->bit)
#define gpio_clear(gpio)        hal_clear_bit8(*(gpio)->port, (gpio)->bit)
#define gpio_toggle(gpio)       hal_toggle_bit8(*(gpio)->port, (gpio)->bit)
#define gpio_read(gpio)         hal_read_bit8(*(gpio)->pin, (gpio)->bit)
#define gpio_write(gpio, state) hal_write_bit8(*(gpio)->port, (gpio)->bit, state)

static inline void hal_mem_set(void *dst, u8 value, usize size) {
        u8 *ptr = (u8 *)dst;
        while (size--) {
                *ptr++ = value;
        }
}

static inline void hal_mem_clear(void *dst, usize size) {
        u8 *ptr = (u8 *)dst;
        while (size--) {
                *ptr++ = 0;
        }
}

static inline void hal_mem_copy(void *dst, const void *src, usize size) {
        u8       *d = (u8 *)dst;
        const u8 *s = (const u8 *)src;
        while (size--) {
                *d++ = *s++;
        }
}

static inline void hal_mem_move(void *dst, const void *src, usize size) {
        u8       *d = (u8 *)dst;
        const u8 *s = (const u8 *)src;

        if (d < s) {
                while (size--) {
                        *d++ = *s++;
                }
        } else if (d > s) {
                d += size;
                s += size;
                while (size--) {
                        *(--d) = *(--s);
                }
        }
}

static inline int hal_mem_compare(const void *ptr1, const void *ptr2, usize size) {
        const u8 *p1 = (const u8 *)ptr1;
        const u8 *p2 = (const u8 *)ptr2;

        while (size--) {
                if (*p1 != *p2) {
                        return (*p1 - *p2);
                }
                p1++;
                p2++;
        }
        return 0;
}

#endif // HAL_H
