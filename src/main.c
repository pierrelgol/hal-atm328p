/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pollivie <pollivie.student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 21:49:25 by pollivie          #+#    #+#             */
/*   Updated: 2025/03/03 21:49:26 by pollivie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hal.h"
#include <util/delay.h>

GpioPin led1;
GpioPin led2;
GpioPin button;

void    delay_ms(uint16_t ms) {
        while (ms--) {
                _delay_ms(1);
        }
}

bool is_button_pressed(const GpioPin *button_pin) {
        if (gpio_read(button_pin) == LOW) {
                delay_ms(20);
                return (gpio_read(button_pin) == LOW);
        }
        return false;
}

int main(void) {
        gpio_init(&led1, GPIO_PORTD, PIND3);
        gpio_init(&led2, GPIO_PORTD, PIND5);
        gpio_init(&button, GPIO_PORTD, PIND2);

        gpio_set_output(&led1);
        gpio_set_output(&led2);
        gpio_set_input(&button);
        gpio_set(&button);

        u8 pattern  = 0b10101010;
        u8 position = 0;
        u8 speed    = 100;

        while (1) {
                if (is_button_pressed(&button)) {
                        pattern = ~pattern;

                        while (gpio_read(&button) == LOW) {
                                _delay_ms(10);
                        }
                }

                if (hal_is_bit8_set(pattern, position)) {
                        gpio_set(&led1);
                } else {
                        gpio_clear(&led1);
                }

                if (hal_is_bit8_set(pattern, position)) {
                        gpio_clear(&led2);
                } else {
                        gpio_set(&led2);
                }

                position = (position + 1) % 8;

                delay_ms(speed);
        }
}
