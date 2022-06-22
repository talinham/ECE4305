//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "xadc_core.h"
#include "sseg_core.h"
#include "spi_core.h"
#include "i2c_core.h"
#include "ps2_core.h"
#include "ddfs_core.h"
#include "adsr_core.h"


// instantiate switch, led
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
XadcCore adc(get_slot_addr(BRIDGE_BASE, S5_XDAC));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));

uint16_t ledReset(GpoCore *led_p, GpiCore *sw_p);	  // SW0 reset LED to rightmost position
uint32_t ledSpeed(XadcCore *adc_p, GpoCore *led_p);
void sseg_check(SsegCore *sseg_p);


int main()
{
	// variable declaration
	bool moveLeft = true;
	uint16_t ledIterate;
	uint32_t Speed, SpeedPrev;

	// initial state
	ledIterate = ledReset(&led, &sw);

	while (1) {
		//check for initialization and LED speed
		if(sw.read(0) == 1)
			ledIterate = ledReset(&led, &sw);

		SpeedPrev = Speed;
		Speed = ledSpeed(&adc, &led);

		//determine when LED should change directions
		if(ledIterate == 0x8000) moveLeft = false;
		if(ledIterate == 0x0001) moveLeft = true;

		led.write(ledIterate);

		//LED direction
		ledIterate = (moveLeft) ? ledIterate << 1 : ledIterate >> 1;

		//display delay time on console
		if(Speed != SpeedPrev)
		{
			uart.disp("Current Speed: ");
			uart.disp(Speed, 10, 3);
			uart.disp("\n\r");
		}
		sleep_ms(Speed);
	}

}

uint16_t ledReset(GpoCore *led_p, GpiCore *sw_p)
{
	led_p->write(0x0001);
	return 0x0001;
}

uint32_t ledSpeed(XadcCore *adc_p, GpoCore *led_p)
{
	double ledSpeed;

	ledSpeed = (adc_p->read_adc_in(0))*1000;
	uart.disp("value: ");
	uart.disp(ledSpeed, 3);
	uart.disp("\n\r");


	return ledSpeed;
}

void sseg_check(SsegCore *sseg_p) {
   int i, n;
   uint8_t dp;

   //turn off led
   for (i = 0; i < 8; i++) {
      sseg_p->write_1ptn(0xff, i);
   }
   //turn off all decimal points
   sseg_p->set_dp(0x00);

   // display 0x0 to 0xf in 4 epochs
   // upper 4  digits mirror the lower 4
   for (n = 0; n < 4; n++) {
      for (i = 0; i < 4; i++) {
         sseg_p->write_1ptn(sseg_p->h2s(i + n * 4), 3 - i);
         sseg_p->write_1ptn(sseg_p->h2s(i + n * 4), 7 - i);
         sleep_ms(300);
      } // for i
   }  // for n
      // shift a decimal point 4 times
   for (i = 0; i < 4; i++) {
      bit_set(dp, 3 - i);
      sseg_p->set_dp(1 << (3 - i));
      sleep_ms(300);
   }
   //turn off led
   for (i = 0; i < 8; i++) {
      sseg_p->write_1ptn(0xff, i);
   }
   //turn off all decimal points
   sseg_p->set_dp(0x00);

}

