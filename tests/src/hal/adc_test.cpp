#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/hal/adc.h"

TEST_GROUP(ADC) {
	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(ADC, enable_ShouldEnablePeripheralAndClock) {
	mock().expectOneCall("pwr_ll_enable_peripheral")
		.withParameter("peri", PERIPH_ADC1);
	mock().expectOneCall("clk_ll_enable_peripheral")
		.withParameter("peri", PERIPH_ADC1);
	mock().expectOneCall("adc_ll_enable")
		.withParameter("adc", PERIPH_ADC1);
	adc_enable(PERIPH_ADC1);
}

TEST(ADC, disable_ShouldDisablePeripheralAndClock) {
	mock().expectOneCall("pwr_ll_disable_peripheral")
		.withParameter("peri", PERIPH_ADC1);
	mock().expectOneCall("clk_ll_disable_peripheral")
		.withParameter("peri", PERIPH_ADC1);
	mock().expectOneCall("adc_ll_disable")
		.withParameter("adc", PERIPH_ADC1);
	adc_disable(PERIPH_ADC1);
}
