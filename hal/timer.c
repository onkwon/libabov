#include "abov/hal/timer.h"

#include <stddef.h>

#include "abov/irq.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"

static void set_frequency(periph_t timer, const struct timer_cfg *cfg)
{
	uint32_t tclk = clk_get_peripheral_clock_source_frequency(timer);
	uint32_t fclk = timer_ll_get_frequency(timer, tclk);
	uint32_t tick = fclk / cfg->frequency;

	timer_ll_set_prescaler(timer, tick - 1);
}

bool timer_init(periph_t timer, const struct timer_cfg *cfg)
{
	if (cfg == NULL) {
		return false;
	}

	pwr_enable_peripheral(timer);
	clk_enable_peripheral(timer);

	timer_ll_reset(timer);

	if (cfg->set_clock_source != NULL) {
		cfg->set_clock_source();
	}

	timer_ll_set_mode(timer, cfg->mode);
	if (cfg->mode != TIMER_MODE_CAPTURE) {
		set_frequency(timer, cfg);
	}

	if (cfg->irq != TIMER_EVENT_NONE) {
		timer_ll_enable_irq(timer, cfg->irq);

		irq_set_priority(PERIPH_TO_IRQ(timer), cfg->irq_priority);
		irq_enable(PERIPH_TO_IRQ(timer));
	}

	return true;
}

void timer_deinit(periph_t timer)
{
	clk_disable_peripheral(timer);
	pwr_disable_peripheral(timer);
}

void timer_start(periph_t peri)
{
	timer_ll_start(peri);
}

void timer_stop(periph_t peri)
{
	timer_ll_stop(peri);
}

void timer_set_reload(periph_t peri, uint32_t value)
{
	timer_ll_set_reload(peri, value);
}

uint32_t timer_get_reload(periph_t peri)
{
	return timer_ll_get_reload(peri);
}

void timer_set_cc(periph_t peri, uint32_t cc, uint32_t value)
{
	timer_ll_set_cc(peri, cc, value);
}

uint32_t timer_get_cc(periph_t peri, uint32_t cc)
{
	return timer_ll_get_cc(peri, cc);
}

void timer_clear_event(periph_t peri, timer_event_t events)
{
	timer_ll_clear_event(peri, events);
}

timer_event_t timer_get_event(periph_t peri)
{
	return timer_ll_get_event(peri);
}

void timer_set_edge(periph_t peri, timer_edge_t edge)
{
	timer_ll_set_edge(peri, edge);
}

void timer_set_polarity(periph_t peri, uint32_t level)
{
	timer_ll_set_polarity(peri, level);
}
