//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"

// instantiate switch, led
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));

uint16_t ledLit(GpoCore *led_p, GpiCore *sw_p);	  // SW0 reset LED to rightmost position
uint32_t ledSpeed(GpoCore *led_p, GpiCore *sw_p);  	  // SW1-SW5 determine and return speed increments

int main()
{
	// variable declaration
	bool moveLeft = true;
	uint16_t ledIterate;
	uint32_t Speed, SpeedPrev;

	// initial state
	ledIterate = ledLit(&led, &sw);

	while (1) {
		//check for initialization and LED speed
		if(sw.read(0) == 1)
			ledIterate = ledLit(&led, &sw);

		SpeedPrev = Speed;
		Speed = ledSpeed(&led, &sw);

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
			uart.disp(Speed, 10, 4);
			uart.disp("\n\r");
		}
		sleep_ms(Speed);
	}

}

uint16_t ledLit(GpoCore *led_p, GpiCore *sw_p)
{
	led_p->write(0x0001);
	return 0x0001;
}

uint32_t ledSpeed(GpoCore *led_p, GpiCore *sw_p)
{
	uint32_t ledSpeed;

	ledSpeed = (sw_p->read() == 0x0000) ? 100 : ((300 * sw_p->read(5)) +
			   (250 * sw_p->read(4)) + (200 * sw_p->read(3)) + (130 * sw_p->read(2)) + (120 * sw_p->read(1)));

	return ledSpeed;
}
