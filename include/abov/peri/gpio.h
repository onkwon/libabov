#ifndef ABOV_GPIO_H
#define ABOV_GPIO_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "abov/peripheral.h"

/** GPIO mode */
typedef enum {
	/** Output only mode */
	GPIO_MODE_PUSHPULL,
	/** Input only mode */
	GPIO_MODE_INPUT,
	/** Input only mode with pull-up function */
	GPIO_MODE_INPUT_PULLUP,
	/** Input only mode with pull-down function */
	GPIO_MODE_INPUT_PULLDOWN,
	/** Input/output open-drain mode */
	GPIO_MODE_OPENDRAIN,
	/** Input/output open-drain mode with pull-up function */
	GPIO_MODE_OPENDRAIN_PULLUP,
	/** Input/output open-drain mode with pull-down function */
	GPIO_MODE_OPENDRAIN_PULLDOWN,
	/** Analog function */
	GPIO_MODE_ANALOG,
} gpio_mode_t;

/** GPIO interrupt type */
typedef enum {
	/** Rising edge interrupt */
	GPIO_IRQ_EDGE_RISING,
	/** Falling edge interrupt */
	GPIO_IRQ_EDGE_FALLING,
	/** Both rising and falling edge interrupts */
	GPIO_IRQ_EDGE_ANY,
	/** Logic level high interrupt */
	GPIO_IRQ_LEVEL_HIGH,
	/** Logic level low interrupt */
	GPIO_IRQ_LEVEL_LOW,
} gpio_irq_t;

/**
 * Reset GPIO port
 *
 * This function makes GPIO port the reset state.
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 */
void gpio_reset(peripheral_t port);
/**
 * Initialize the given GPIO pin to the specified mode
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 * :param mode: sets gpio operation mode
 * :return: true on success
 */
void gpio_open(peripheral_t port, uint32_t pin, gpio_mode_t mode);
/**
 * Deinitialize the given GPIO pin
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 * :return: true on success
 */
void gpio_close(peripheral_t port, uint32_t pin);
/**
 * Select GPIO alternate function
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 * :param altfunc: alternate function number
 * :return: true on success
 */
void gpio_set_altfunc(peripheral_t port, uint32_t pin, int altfunc);
/**
 * Write output level to the given GPIO pin
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 * :param value: logic output level. Only 0(low) and 1(high) are possible
 */
void gpio_write(peripheral_t port, uint32_t pin, int value);
/**
 * Read the current input level from the given GPIO pin
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 * :return: 0 when the logic level is low. 1 when the logic level is high
 */
int gpio_read(peripheral_t port, uint32_t pin);
/**
 * Write a value to the given GPIO port
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param value: to be written to the GPIO port
 */
void gpio_write_port(peripheral_t port, int value);
/**
 * Read the current value of the given GPIO port
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :return: the value read from the specified GPIO port
 */
int gpio_read_port(peripheral_t port);
/**
 * Enable interrupt on the given GPIO pin
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 * :param irq_type: sets interrupt trigger type
 */
void gpio_enable_irq(peripheral_t port, uint32_t pin, gpio_irq_t irq_type);
/**
 * Disable interrupt on the given GPIO pin
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 */
void gpio_disable_irq(peripheral_t port, uint32_t pin);
/**
 * Clear interrupt flag on the given GPIO pin
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 */
void gpio_clear_event(peripheral_t port, uint32_t pin);
/**
 * Enable the given port
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 */
void gpio_enable_port(peripheral_t port);
/**
 * Disable the given port
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 */
void gpio_disable_port(peripheral_t port);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_GPIO_H */