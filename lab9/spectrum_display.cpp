/*****************************************************************//**
 * @file spectrum_display.cpp
 *
 * @Generating rainbow like spectrum using the potentiometer and XADC core
 *
 *********************************************************************/

// #define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "xadc_core.h"
#include "sseg_core.h"
#include "spi_core.h"
#include "i2c_core.h"
#include "ps2_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"


GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
XadcCore adc(get_slot_addr(BRIDGE_BASE, S5_XDAC));
PwmCore pwm(get_slot_addr(BRIDGE_BASE, S6_PWM));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));

// function prototypes
void pwm_3color_led_disp(PwmCore *pwm_p, double adc_value);
void sseg_disp(SsegCore *sseg_p, double adc_value);


enum RGB_Colors {
	blue = 0 ,
	green,
	red
};



int main()
{
	double adcValue = 0;

	while(1)
	{
		adcValue = adc.read_adc_in(0);

		pwm_3color_led_disp(&pwm, adcValue);

		sseg_disp(&sseg, adcValue);
	}

	sleep_ms(100);
}


void pwm_3color_led_disp(PwmCore *pwm_p, double adc_value)
{
	// declaring variables for duty cycle calculation
	double redDutyCycle, greenDutyCycle, blueDutyCycle;
	// value of each segment in the spectrum
	double segmentValue = 0.167;
	double slope = 1/segmentValue;


	// setting the duty cycle based on the adc value
	if (adc_value <= segmentValue)
	{
		redDutyCycle = 1.0;
		greenDutyCycle = adc_value * slope;
		blueDutyCycle = 0.0;
	}
	else if (adc_value > segmentValue && adc_value <= segmentValue*2)
	{
		redDutyCycle = 2 - (adc_value *slope) ;
		greenDutyCycle = 1.0;
		blueDutyCycle = 0.0;
	}
	else if (adc_value > segmentValue*2 && adc_value <= segmentValue*3)
	{
		redDutyCycle = 0.0;
		greenDutyCycle = 1.0;
		blueDutyCycle = -2 + (adc_value *slope) ;
	}
	else if (adc_value > segmentValue*3 && adc_value <= segmentValue*4)
	{
		redDutyCycle = 0.0;
		greenDutyCycle = 4 - (adc_value *slope) ;
		blueDutyCycle = 1.0;
	}
	else if (adc_value > segmentValue*4 && adc_value <= segmentValue*5)
	{
		redDutyCycle = -4 + (adc_value *slope) ;
		greenDutyCycle = 0.0;
		blueDutyCycle = 1.0;
	}
	else
	{
		redDutyCycle = 1.0;
		greenDutyCycle = 0.0;
		blueDutyCycle = 6 - (adc_value *slope) ;
	}

	// setting the duty cycle
	pwm_p->set_duty(redDutyCycle, red);
	pwm_p->set_duty(greenDutyCycle, green);
	pwm_p->set_duty(blueDutyCycle, blue);


}

void sseg_disp(SsegCore *sseg_p, double adc_value)
{
		// setting the decimal point
		sseg_p->set_dp(0x04);

		// multiplying adc value by 100 for easy calculation
		int adcValue_int = adc_value *100;

		// setting the most significant digit to zero
	    sseg_p->write_1ptn(sseg_p->h2s(0), 2);

	    // separating the second digit
		int second_digit = (int) ((adcValue_int / 10.0));
		uint8_t second_digit_Convert = sseg_p->h2s(second_digit);
		sseg_p->write_1ptn(second_digit_Convert, 1);

		// separating the least significant bit
		int lower_DispVal = ((int) adcValue_int) % 10;
		uint8_t lowerDigit_Convert = sseg_p->h2s(lower_DispVal);
		sseg_p->write_1ptn(lowerDigit_Convert, 0);


}
