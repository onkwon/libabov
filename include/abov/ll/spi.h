#ifndef ABOV_SPI_LL_H
#define ABOV_SPI_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "abov/periph.h"

typedef enum {
	SPI_MODE_MASTER,
	SPI_MODE_SLAVE,
} spi_mode_t;

typedef enum {
	SPI_IRQ_NONE				= 0,
	SPI_IRQ_RX				= (1U << 0),
	SPI_IRQ_TX				= (1U << 1),
	SPI_IRQ_EDGE_CHAGNE			= (1U << 2),
	SPI_IRQ_OVERRUN				= (1U << 3),
	SPI_IRQ_FRAME_ERROR			= (1U << 4),
	SPI_IRQ_MASK				= 0x1F,
} spi_irq_t;

typedef enum {
	SPI_EVENT_NONE				= 0,
	SPI_EVENT_TX_COMPLETE			= (1U << 0),
	SPI_EVENT_RX				= (1U << 1),
	SPI_EVENT_BUSY				= (1U << 2),
	SPI_EVENT_OVERRUN			= (1U << 3),
	SPI_EVENT_UNDERRUN			= (1U << 4),
	SPI_EVENT_CHIP_SELECTED			= (1U << 5),
	SPI_EVENT_CHIP_DESELECTED		= (1U << 6),
	SPI_EVENT_MASK				= 0x7F,
} spi_event_t;

/**
 * Reset SPI
 *
 * This function makes the given SPI the reset default state.
 *
 * :param spi: a peripheral enumerated in :c:type:`periph_t`
 */
void spi_reset(periph_t spi);
/***/
uint32_t spi_get_rxd(periph_t spi);
/***/
void spi_set_txd(periph_t spi, uint32_t value);
/***/
void spi_clear_rx_buffer(periph_t spi);
/***/
void spi_clear_tx_buffer(periph_t spi);
/***/
void spi_enable_irq(periph_t spi, spi_irq_t irqs);
/***/
void spi_disable_irq(periph_t spi, spi_irq_t irqs);
/***/
void spi_enable_chip_select(periph_t spi);
/***/
void spi_disable_chip_select(periph_t spi);
/***/
void spi_set_chip_select_mode(periph_t spi, bool manual);
/***/
void spi_set_chip_select_level(periph_t spi, int level);
/***/
void spi_set_chip_select_polarity(periph_t spi, int level);
/***/
void spi_set_loopback(periph_t spi, bool enable);
/***/
void spi_set_mode(periph_t spi, spi_mode_t mode);
/***/
void spi_start(periph_t spi);
/***/
void spi_stop(periph_t spi);
/***/
spi_event_t spi_get_event(periph_t spi);
/***/
void spi_clear_event(periph_t spi, spi_event_t events);
/**
 * Set clock phase
 *
 * :param spi: a peripheral enumerated in :c:type:`periph_t`
 * :param cpha: 0 for the first clock edge or 1 for the second clock edge
 */
void spi_set_clock_phase(periph_t spi, int cpha);
/**
 * Set clock polarity
 *
 * :param spi: a peripheral enumerated in :c:type:`periph_t`
 * :param cpol: 0 for low, 1 for high
 */
void spi_set_clock_polarity(periph_t spi, int cpol);
/***/
void spi_set_data_width(periph_t spi, uint32_t data_width);
/***/
void spi_set_bitorder(periph_t spi, bool lsb_first);
/***/
void spi_set_frequency(periph_t spi, uint32_t hz, uint32_t pclk);
/***/
void spi_set_start_delay(periph_t spi, uint32_t nsck);
/***/
void spi_set_stop_delay(periph_t spi, uint32_t nsck);
/***/
void spi_set_burst_delay(periph_t spi, uint32_t nsck);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_SPI_LL_H */
