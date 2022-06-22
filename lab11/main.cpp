#include "chu_init.h"
#include "gpio_cores.h"
#include "sseg_core.h"
#include "ps2_core.h"


#define LEFT true;
#define RIGHT false;



GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));

// function prototypes
void ledChase(GpoCore *led_p, Ps2Core *ps2_p, uint8_t* pauseLed, int *LedSpeed);
void chaserMouseControl(Ps2Core *ps2_p, uint8_t *pause, int *LedSpeed);
void DispSseg(SsegCore *sseg_p, int *LedSpeed, uint8_t *pause);

int main()
{
	int id = -1;         // initial state (-1=no response, 1=keyboard, 2=mouse)
	int LedSpeed = 0;
	uint8_t pauseLed = 0;


	// initializing mouse / 2 mouse id
	while (id != 2)
	{
		id = ps2.init();
	}

	while (1)
	{
		chaserMouseControl(&ps2, &pauseLed, &LedSpeed);
		ledChase(&led, &ps2, &pauseLed, &LedSpeed);
		DispSseg(&sseg, &LedSpeed, &pauseLed);

	}
}

void ledChase(GpoCore *led_p, Ps2Core *ps2_p, uint8_t *pauseLed, int *currLedSpeed)
{
	static bool moveDirection = LEFT;
	static uint8_t currLedPos = 0;
	static uint8_t prevLedPos = 0;
	unsigned long last;

	//setting the minimum speed
	if (*currLedSpeed == 0)
		*currLedSpeed = 50;

	// setting pause
	if (*pauseLed)
		return;

	// led write based on position
	led_p->write(0, prevLedPos);
	led_p->write(1, currLedPos);
	prevLedPos = currLedPos;

	// led position
	if(currLedPos == 0)
		moveDirection = LEFT;
	if(currLedPos == 15)
		moveDirection = RIGHT;

	// led direction logic
	if(moveDirection)
		currLedPos++;
	else
		currLedPos--;

	// checking inputs during delays
	last = now_ms();

	while (now_ms() - last < ((long unsigned) *currLedSpeed))
	{
		chaserMouseControl(ps2_p, pauseLed, currLedSpeed);
	}

}

void chaserMouseControl(Ps2Core *ps2_p, uint8_t *pause, int *LedSpeed)
{
	int lbtn, rbtn, xmov, ymov;
	int rClickToggle = 0;

	if (ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov))
	{
		if (lbtn == 1)
		{
			uart.disp("Left button clicked ");
			uart.disp("\n\r");
			uart.disp("\n\r");

			*pause = (*pause == 1) ? 0 : 1;
			 return;
		}
		if (rbtn == 1)
		{
			rbtn = 0;
			rClickToggle = 1;

			uart.disp("Right button clicked ");
			uart.disp(rClickToggle);
			uart.disp("\n\r");

			uart.disp("Current Rate: ");
			uart.disp(*LedSpeed);
			uart.disp("\n\r");

			do
			{
				if (ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov))
				{
					if (rbtn)
						rClickToggle = 0;
					else
					{
						*LedSpeed += (int)(((double) xmov) * 0.15);	// 0.15 sensitivity
						if (*LedSpeed > 999)
							*LedSpeed = 999;
						if (*LedSpeed < 50)
							*LedSpeed = 50;
						DispSseg(&sseg, LedSpeed, pause);
					}
				}

			} while (rClickToggle);

			//setting minimum and maximum limits



			uart.disp("Updated Rate: ");
			uart.disp(*LedSpeed);
			uart.disp("\n\r");
			uart.disp("\n\r");

			return;
		}
	}
}

void DispSseg(SsegCore *sseg_p, int *LedSpeed, uint8_t *pause)
{
	sseg.set_dp(0x00);
	int dispLedSpeed = *LedSpeed;

	// displaying P for pause

	if (*pause)
	{
		const uint8_t PT_PTN[]={0xff,0xff,0xff,0xb7,0x8c,0x92,0xff,0x8c};
		sseg_p->write_8ptn((uint8_t*) PT_PTN);
	}
	else
	{
		const uint8_t P_PTN[]={0xff,0xff,0xff,0xb7,0x8c,0x92,0xff,0xff};
		sseg_p->write_8ptn((uint8_t*) P_PTN);
	}

	// separating speed value digits

	int lower_Digit = (int) (dispLedSpeed  % 10);
	uint8_t lowerDigit_Convert = sseg_p->h2s(lower_Digit);
	sseg_p->write_1ptn(lowerDigit_Convert, 0);

	int second_digit = (int) ((dispLedSpeed / 10) %10);
	uint8_t second_digit_Convert = sseg_p->h2s(second_digit);
	sseg_p->write_1ptn(second_digit_Convert, 1);

	int upper_digit = (int) ((dispLedSpeed / 100));
	uint8_t upper_digit_Convert = sseg_p->h2s(upper_digit);
	sseg_p->write_1ptn(upper_digit_Convert, 2);


}


