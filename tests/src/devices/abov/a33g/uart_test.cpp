#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/ll/uart.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"
#include "a33g.h"

extern "C" {
static struct uart_stub reg0, reg1, reg2, reg3;

struct uart_stub * const UART0 = &reg0;
struct uart_stub * const UART1 = &reg1;
struct uart_stub * const UART2 = &reg2;
struct uart_stub * const UART3 = &reg3;
}

uint32_t clk_get_pclk_frequency(void)
{
	return mock().actualCall(__func__)
		.returnUnsignedIntValueOrDefault(8000000);
}

void pwr_enable_peripheral(peripheral_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}
void clk_enable_peripheral(peripheral_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}
void pwr_disable_peripheral(peripheral_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}
void clk_disable_peripheral(peripheral_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

TEST_GROUP(uart) {
	void setup(void) {
		mock().ignoreOtherCalls();

		set_initial_state();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}

	void set_initial_state(void) {
		UART0->LSR = UART1->LSR = UART2->LSR = UART3->LSR = 0x60;
		UART0->IIR = UART1->IIR = UART2->IIR = UART3->IIR = 1;
		uart_reset(UART_PORT_0);
		uart_reset(UART_PORT_1);
		uart_reset(UART_PORT_2);
		uart_reset(UART_PORT_3);
	}
};

TEST(uart, get_event_ShouldReturnLRS) {
	uint32_t expected = 0xA55A;
	UART0->LSR = expected >> 8;
	UART0->IIR = expected & 0xff;
	LONGS_EQUAL(expected | UART_EVENT_TX_READY, uart_get_event(UART_PORT_0));
}

TEST(uart, enable_irq_ShouldSetDrieInIER_WhenRxGiven) {
	uart_enable_irq(UART_PORT_0, UART_EVENT_RX);
	LONGS_EQUAL(1, UART0->IER);
}

TEST(uart, disable_irq_ShouldClearDrieInIER_WhenRxGiven) {
	UART0->IER = 1;
	uart_disable_irq(UART_PORT_0, UART_EVENT_RX);
	LONGS_EQUAL(0, UART0->IER);
}

TEST(uart, enable_irq_ShouldSetDrieInIER_WhenTxGiven) {
	uart_enable_irq(UART_PORT_0, UART_EVENT_TX_READY);
	LONGS_EQUAL(2, UART0->IER);
}

TEST(uart, disable_irq_ShouldClearDrieInIER_WhenTxGiven) {
	UART0->IER = 2;
	uart_disable_irq(UART_PORT_0, UART_EVENT_TX_READY);
	LONGS_EQUAL(0, UART0->IER);
}

TEST(uart, enable_irq_ShouldSetMultiIntr_WhenMixedGiven) {
	uart_enable_irq(UART_PORT_0, (uart_event_t)(UART_EVENT_RX | UART_EVENT_TX_READY));
	LONGS_EQUAL(3, UART0->IER);
}

TEST(uart, disable_irq_ShouldClearMultiIntr_WhenMixedGiven) {
	UART0->IER = 3;
	uart_disable_irq(UART_PORT_0, (uart_event_t)(UART_EVENT_RX | UART_EVENT_TX_READY));
	LONGS_EQUAL(0, UART0->IER);
}

TEST(uart, set_baudrate_ShouldSetBaudrate_When38400With8MhzPclkGiven) {
	mock().expectOneCall("clk_get_pclk_frequency").andReturnValue(8000000);
	uart_set_baudrate(UART_PORT_0, 38400);
	LONGS_EQUAL(0x00, UART0->DLM);
	LONGS_EQUAL(0x06, UART0->DLL);
	LONGS_EQUAL(0x82, UART0->BFR);
}

TEST(uart, set_baudrate_ShouldSetBaudrate_When38400With75MhzPclkGiven) {
	mock().expectOneCall("clk_get_pclk_frequency").andReturnValue(75000000);
	uart_set_baudrate(UART_PORT_0, 38400);
	LONGS_EQUAL(0x00, UART0->DLM);
	LONGS_EQUAL(0x3d, UART0->DLL);
	LONGS_EQUAL(0x09, UART0->BFR);
}

TEST(uart, set_baudrate_ShouldSetBaudrate_When115200With75MhzPclkGiven) {
	mock().expectOneCall("clk_get_pclk_frequency").andReturnValue(75000000);
	uart_set_baudrate(UART_PORT_0, 115200);
	LONGS_EQUAL(0x00, UART0->DLM);
	LONGS_EQUAL(0x14, UART0->DLL);
	LONGS_EQUAL(0x58, UART0->BFR);
}

TEST(uart, set_baudrate_ShouldSetBaudrate_When9600With75MhzPclkGiven) {
	mock().expectOneCall("clk_get_pclk_frequency").andReturnValue(75000000);
	uart_set_baudrate(UART_PORT_0, 9600);
	LONGS_EQUAL(0x00, UART0->DLM);
	LONGS_EQUAL(0xf4, UART0->DLL);
	LONGS_EQUAL(0x24, UART0->BFR);
}

TEST(uart, read_byte_ShouldReturnReceivedByte) {
	UART0->RBR = 0xA5;
	UART0->LSR |= 1;
	LONGS_EQUAL(0xA5, uart_read_byte(UART_PORT_0));
}

TEST(uart, read_byte_nonblock_ShouldReturnNegativeOne_WhenNonReceived) {
	LONGS_EQUAL(-1, uart_read_byte_nonblock(UART_PORT_0));
}

TEST(uart, write_byte_ShouldWriteByteToTHR) {
	uart_write_byte(UART_PORT_0, 0xA5);
	LONGS_EQUAL(0xA5, UART0->THR);
}

TEST(uart, set_parity_ShoudSetParity_WhenParityOddGiven) {
	uart_set_parity(UART_PORT_0, UART_PARITY_ODD);
	LONGS_EQUAL(0x8, UART0->LCR);
}

TEST(uart, set_parity_ShoudSetParity_WhenParityEvenGiven) {
	uart_set_parity(UART_PORT_0, UART_PARITY_EVEN);
	LONGS_EQUAL(0x18, UART0->LCR);
}

TEST(uart, set_parity_ShoudDisableParity_WhenParityNoneGiven) {
	UART0->LCR = 0x18;
	uart_set_parity(UART_PORT_0, UART_PARITY_NONE);
	LONGS_EQUAL(0x10, UART0->LCR);
}

TEST(uart, set_stopbits_ShouldSetStopbit_When1StopGiven) {
	UART0->LCR = 0x4;
	uart_set_stopbits(UART_PORT_0, UART_STOPBIT_1);
	LONGS_EQUAL(0, UART0->LCR);
}

TEST(uart, set_stopbits_ShouldSetStopbit_When1_5StopGiven) {
	uart_set_stopbits(UART_PORT_0, UART_STOPBIT_1_5);
	LONGS_EQUAL(4, UART0->LCR);
}

TEST(uart, set_stopbits_ShouldSetStopbit_When2StopGiven) {
	uart_set_stopbits(UART_PORT_0, UART_STOPBIT_2);
	LONGS_EQUAL(4, UART0->LCR);
}

TEST(uart, set_wordsize_ShouldSetWordsize_When8Given) {
	uart_set_wordsize(UART_PORT_0, UART_WORDSIZE_8);
	LONGS_EQUAL(3, UART0->LCR);
}

TEST(uart, set_wordsize_ShouldSetWordsize_When7Given) {
	uart_set_wordsize(UART_PORT_0, UART_WORDSIZE_7);
	LONGS_EQUAL(2, UART0->LCR);
}

TEST(uart, set_wordsize_ShouldSetWordsize_When6Given) {
	uart_set_wordsize(UART_PORT_0, UART_WORDSIZE_6);
	LONGS_EQUAL(1, UART0->LCR);
}

TEST(uart, set_wordsize_ShouldSetWordsize_When5Given) {
	UART0->LCR = 1;
	uart_set_wordsize(UART_PORT_0, UART_WORDSIZE_5);
	LONGS_EQUAL(0, UART0->LCR);
}

TEST(uart, enable_ShouldCallPwrAndClk_WhenUart0Given) {
	mock().expectOneCall("pwr_enable_peripheral")
		.withParameter("peri", PERIPHERAL_UART0);
	mock().expectOneCall("clk_enable_peripheral")
		.withParameter("peri", PERIPHERAL_UART0);
	uart_enable(UART_PORT_0);
}

TEST(uart, enable_ShouldCallPwrAndClk_WhenUart3Given) {
	mock().expectOneCall("pwr_enable_peripheral")
		.withParameter("peri", PERIPHERAL_UART3);
	mock().expectOneCall("clk_enable_peripheral")
		.withParameter("peri", PERIPHERAL_UART3);
	uart_enable(UART_PORT_3);
}

TEST(uart, disable_ShouldCallPwrAndClk_WhenUart1Given) {
	mock().expectOneCall("clk_disable_peripheral")
		.withParameter("peri", PERIPHERAL_UART1);
	mock().expectOneCall("pwr_disable_peripheral")
		.withParameter("peri", PERIPHERAL_UART1);
	uart_disable(UART_PORT_1);
}

TEST(uart, disable_ShouldCallPwrAndClk_WhenUart2Given) {
	mock().expectOneCall("clk_disable_peripheral")
		.withParameter("peri", PERIPHERAL_UART2);
	mock().expectOneCall("pwr_disable_peripheral")
		.withParameter("peri", PERIPHERAL_UART2);
	uart_disable(UART_PORT_2);
}
