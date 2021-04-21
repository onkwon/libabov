#include "abov/ll/timer.h"
#include "abov/bitop.h"

#include "abov/asm/arm/cmsis.h"
#include "a33g.h"
#include "abov/compiler.h"

ABOV_STATIC_ASSERT(TIMER_MODE_NORMAL == 0, "");
ABOV_STATIC_ASSERT(TIMER_MODE_PWM == 1, "");
ABOV_STATIC_ASSERT(TIMER_MODE_ONESHOT == 2, "");
ABOV_STATIC_ASSERT(TIMER_MODE_CAPTURE == 3, "");

static TIMER_Type *get_timer_from_peripheral(periph_t peri)
{
	uint32_t timer_index = peri - PERIPH_TIMER0;
	return (TIMER_Type *)(T0_BASE + (timer_index * 2 << 4));
}

void timer_set_prescaler(periph_t peri, uint32_t div_factor)
{
	bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->PRS,
			0, 0x3ffU, div_factor);
}

void timer_set_clock_divider(periph_t peri, uint32_t div_factor)
{
	bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
			4, 7U, div_factor); /* TCS */
}

void timer_set_counter(periph_t peri, uint32_t value)
{
	get_timer_from_peripheral(peri)->CNT = value;
}

uint32_t timer_get_counter(periph_t peri)
{
	return get_timer_from_peripheral(peri)->CNT;
}

void timer_set_reload(periph_t peri, uint32_t value)
{
	get_timer_from_peripheral(peri)->GRA = value;
}

uint32_t timer_get_reload(periph_t peri)
{
	return get_timer_from_peripheral(peri)->GRA;
}

void timer_set_mode(periph_t peri, timer_mode_t mode)
{
	bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
			0, 3, mode);
}

void timer_enable_irq(periph_t peri, timer_event_t events)
{
	if (events & TIMER_EVENT_OVERFLOW) {
		bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
				10, 1U, 1); /* TOVE */
	}
	if (events & TIMER_EVENT_CC_0) {
		bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
				8, 1U, 1); /* TIE0 */
	}
	if (events & TIMER_EVENT_CC_1) {
		bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
				9, 1U, 1); /* TIE1 */
	}
}

void timer_disable_irq(periph_t peri, timer_event_t events)
{
	if (events & TIMER_EVENT_OVERFLOW) {
		bitop_clear(&get_timer_from_peripheral(peri)->CON, 10); /* TOVE */
	}
	if (events & TIMER_EVENT_CC_0) {
		bitop_clear(&get_timer_from_peripheral(peri)->CON, 8); /* TIE0 */
	}
	if (events & TIMER_EVENT_CC_1) {
		bitop_clear(&get_timer_from_peripheral(peri)->CON, 9); /* TIE1 */
	}
}

void timer_clear_event(periph_t peri, timer_event_t events)
{
	if (events & TIMER_EVENT_OVERFLOW) {
		bitop_set(&get_timer_from_peripheral(peri)->CON, 14); /* IOVF */
	}
	if (events & TIMER_EVENT_CC_0) {
		bitop_set(&get_timer_from_peripheral(peri)->CON, 12); /* TIF0 */
	}
	if (events & TIMER_EVENT_CC_1) {
		bitop_set(&get_timer_from_peripheral(peri)->CON, 13); /* TIF1 */
	}
}

void timer_start(periph_t peri)
{
	bitop_set(&get_timer_from_peripheral(peri)->CMD, 1); /* TCLR */
	bitop_set(&get_timer_from_peripheral(peri)->CMD, 0); /* TEN */
}

void timer_stop(periph_t peri)
{
	bitop_clear(&get_timer_from_peripheral(peri)->CMD, 0); /* TEN */
}

void timer_set_cc(periph_t peri, uint32_t cc, uint32_t value)
{
	if (cc == 0) {
		get_timer_from_peripheral(peri)->GRA = value;
	} else if (cc == 1) {
		get_timer_from_peripheral(peri)->GRB = value;
	}
}

uint32_t timer_get_cc(periph_t peri, uint32_t cc)
{
	if (cc == 0) {
		return get_timer_from_peripheral(peri)->GRA;
	} else if (cc == 1) {
		return get_timer_from_peripheral(peri)->GRB;
	}
	return 0;
}

timer_event_t timer_get_event(periph_t peri)
{
	uint32_t flags = get_timer_from_peripheral(peri)->CON >> 12;
	uint32_t rc = 0;

	if (flags & 1) {
		rc |= TIMER_EVENT_CC_0;
	}
	if (flags & 2) {
		rc |= TIMER_EVENT_CC_1;
	}
	if (flags & 4) {
		rc |= TIMER_EVENT_OVERFLOW;
	}

	return (timer_event_t)rc;
}

void timer_reset(periph_t peri)
{
	TIMER_Type *tim = get_timer_from_peripheral(peri);
	bitop_set(&tim->CMD, 1); /* TCLR */
	tim->CMD = 0;
	tim->CON = 0;
	tim->CON = 0x7000; /* clear interrupt flags */
	tim->GRA = 0;
	tim->GRB = 0;
	tim->PRS = 0;
	tim->CNT = 0;
}

void timer_set_polarity(periph_t peri, uint32_t level)
{
	bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
			7, 1U, level); /* TSTRT */
}

void timer_set_edge(periph_t peri, timer_edge_t edge)
{
	bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
			3, 1U, edge); /* CAPM */
}

uint32_t timer_get_frequency(periph_t peri, uint32_t tclk)
{
	const TIMER_Type *tim = get_timer_from_peripheral(peri);
	uint32_t tcs = (tim->CON >> 4) & 0x7;

	switch (tcs) {
	case 0:
		return tclk / 2;
	case 1:
		return tclk / 4;
	case 2:
		return tclk / 16;
	case 3:
		return tclk / 64;
	default:
		break;
	}

	if (tcs >= 6) {
		return 0;
	}

	return tclk;
}
