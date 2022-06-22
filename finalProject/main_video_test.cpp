/*****************************************************************//**
 * @file main_video_test.cpp
 *
 * @brief Basic test of 4 basic i/o cores
 *
 * @author p chu
 * @version v1.0: initial release
 *********************************************************************/

//#define _DEBUG
#include "chu_init.h"
#include "gpio_cores.h"
#include "vga_core.h"
#include "sseg_core.h"
#include "ps2_core.h"
#include "spi_core.h"
//#include "uart_core.h"

//#include <stdio.h>
//#include <stdlib.h>
static int x=0,y=0;
static int win=0;
static int reset=0;
static int grid []={0, 0, 0, 0, 0, 0, 0, 0, 0};
static int grid_play[]{0, 0, 0, 0, 0, 0, 0, 0, 0};

static int title[11] = {84,73,67,32,84,65,67,32,84,79,69};
static int player_x[10] = {80,108,97,121,101,114,32,88,58,48};
static int player_y[10] = {80,108,97,121,101,114,32,79,58,48};

static int player_x_inst[19] = {80,108,97,121,101,114,32,88,58,76,101,102,116,32,99,108,105,99,107};
static int player_o_inst[20] = {80,108,97,121,101,114,32,79,58,82,105,103,104,116,32,99,108,105,99,107};
static int tab_to_reset[12] = {84,97,112,32,116,111,32,114,101,115,101,116};

static int no_win[11]={78, 111, 32, 79, 110, 101, 32, 87, 105, 110, 115};

static int lclick=0;
static int rclick=0;

void test_start(GpoCore *led_p) {
   int i;

   for (i = 0; i < 20; i++) {
      led_p->write(0xff00);
      sleep_ms(50);
      led_p->write(0x0000);
      sleep_ms(50);
   }
}

/**
 * check bar generator core
 * @param bar_p pointer to Gpv instance
 */
void bar_check(GpvCore *bar_p) {
   bar_p->bypass(0);
   sleep_ms(3000);
}

/**
 * check color-to-grayscale core
 * @param gray_p pointer to Gpv instance
 */
void gray_check(GpvCore *gray_p) {
   gray_p->bypass(0);
   sleep_ms(3000);
   gray_p->bypass(1);
}

/**
 * check osd core
 * @param osd_p pointer to osd instance
 */
void osd_check(OsdCore *osd_p) {
	 osd_p->set_color(0x001, 0x000); // dark gray/green
	       //osd_p->bypass(0);
	       // osd_p->clr_screen();

	       // tic-tac-toe writing
	       for (int i = 0 ; i<11 ; i++)
	       {
	           osd_p->wr_char(34+i, 1, title[i]);
	       }

	       // player 1 score
	       for (int i = 0 ; i<10 ; i++)
	       {
	              osd_p->wr_char(1+i, 1, player_x[i]);
	       }

	       // player 2 score
	       for (int i = 0 ; i<10 ; i++)
	       {
	           osd_p->wr_char(68+i, 1, player_y[i]);
	       }
	       //player x turns
	       if(rclick==1)
	       {
			   for (int i = 0 ; i<10 ; i++)
			   {
				   osd_p->wr_char(1+i, 2, 45);
				   osd_p->wr_char(68+i, 2, 32);
			   }
	       }
	       //player x turns
	       if(lclick==1)
	       {
			   for (int i = 0 ; i<10 ; i++)
			   {
				   osd_p->wr_char(1+i, 2, 32);
				   osd_p->wr_char(68+i, 2, 45);
			   }
	       }
	      // grid setup
	       for (int i = 0; i < 51; i++)
	       {
	          if (i != 15 && i !=35)
	          {
	              osd_p->wr_char(14 + i , 44, 45);
	              osd_p->wr_char(14 + i , 21, 45);
	          }
	       }
	       for (int i = 0; i< 22 ; i++)
	       {
	          osd_p->wr_char(29, 6 + i , 124);
	          osd_p->wr_char(49, 6 + i , 124);
	        }


	       // instruction
	       for (int i = 0; i<19 ; i++)
	       {
	           osd_p->wr_char(1+i,28, player_x_inst[i]);
	       }
	       for (int i = 0; i<20 ; i++)
	       {
	           osd_p->wr_char(59+i,28, player_o_inst[i]);
	       }
	       for (int i = 0; i<12 ; i++)
	       {
	           osd_p->wr_char(34+i,28, tab_to_reset[i]);
	       }


	    }

/**
 * test frame buffer core
 * @param frame_p pointer to frame buffer instance
 */
void frame_check(FrameCore *frame_p) {
	frame_p->clr_screen(0xfff);
}

/**
 * test ghost sprite
 * @param ghost_p pointer to mouse sprite instance
 */
bool mouse_reset(Ps2Core *ps2_p)
{
	int lbtn, rbtn;
	int xmov, ymov;

	 ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov);
	 if(lbtn ==1 && rbtn==1)
		 return true;
	else
		return false;
}
void init(Ps2Core *ps2_p){
    ps2_p -> init();
}

void mouse_check(SpriteCore *mouse_p, Ps2Core *ps2_p) {
   //int x, y;
   //int xmax =639;
   //int ymax = 479;

   int lbtn,rbtn,xmov,ymov;

   // slowly move mouse pointer
   if(ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov)== 1){

       ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov);

          x = x + xmov;//add mouse distance to current cursor location
          y = y - ymov;

          //reset cursor location if out reaching out bounds
          if (x < 0)
          {
              x=0;
          }
          if (x > 625 )
          {
              x=625;
          }
          if (y < 0)
          {
              y=0;
          }
          if (y > 460)
          {
              y=460;
          }

          mouse_p->move_xy(x, y);//move within boundary

      }// if activity


}
void check_win(OsdCore *osd_p)
{
    int playerx[14] = {80,108,97,121,101,114,32, 88,32, 87, 105, 110, 115, 33}; //2
    int playery[14] =  {80,108,97,121,101,114,32,79 ,32, 87, 105, 110, 115, 33}; //3

    int playerxwinsgame[19] =  {80,108,97,121,101,114,32, 88,32, 87, 105, 110, 115,32,71,97,109,101,  33}; //2
    int playerywinsgame[19] =  {80,108,97,121,101,114,32,79 ,32, 87, 105, 110, 115,32,71,97,109,101,  33}; //3
    int noWin=0;

	if(grid_play[0]==2 && grid_play[1]==2 && grid_play[2]==2) // 3 across top
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playerx[i]);
		   win=1;

	   }
	   player_x[9]=player_x[9]+1;
	}
	if(grid_play[0]==3 && grid_play[1]==3 && grid_play[2]==3)
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playery[i]);
		   win=1;
	   }
	   player_y[9]=player_y[9]+1;
	}
	if(grid_play[3]==2 && grid_play[4]==2 && grid_play[5]==2)// 3 across middle
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playerx[i]);
		   win=1;
	   }
	   player_x[9]=player_x[9]+1;
	}
	if(grid_play[3]==3 && grid_play[4]==3 && grid_play[5]==3)
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playery[i]);
		   win=1;
	   }
	   player_y[9]=player_y[9]+1;
	}
	if(grid_play[6]==2 && grid_play[7]==2 && grid_play[8]==2)// 3 across bottom
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playerx[i]);
		   win=1;
	   }
	   player_x[9]=player_x[9]+1;
	}
	if(grid_play[6]==3 && grid_play[7]==3 && grid_play[8]==3)
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playery[i]);
		   win=1;
	   }
	   player_y[9]=player_y[9]+1;
	}
	if(grid_play[0]==2 && grid_play[3]==2 && grid_play[6]==2)// 3 down left
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playerx[i]);
		   win=1;
	   }
	   player_x[9]=player_x[9]+1;
	}
	if(grid_play[0]==3 && grid_play[3]==3 && grid_play[6]==3)
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playery[i]);
		   win=1;
	   }
	   player_y[9]=player_y[9]+1;
	}
	if(grid_play[1]==2 && grid_play[4]==2 && grid_play[7]==2)// 3 down middle
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playerx[i]);
		   win=1;
	   }
	   player_x[9]=player_x[9]+1;
	}
	if(grid_play[1]==3 && grid_play[4]==3 && grid_play[7]==3)
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playery[i]);
		   win=1;
	   }
	   player_y[9]=player_y[9]+1;
	}
	if(grid_play[2]==2 && grid_play[5]==2 && grid_play[8]==2)// 3 down right
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playerx[i]);
		   win=1;
	   }
	   player_x[9]=player_x[9]+1;
	}
	if(grid_play[2]==3 && grid_play[5]==3 && grid_play[8]==3)
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playery[i]);
		   win=1;
	   }
	   player_y[9]=player_y[9]+1;
	}
	if(grid_play[0]==2 && grid_play[4]==2 && grid_play[8]==2)// 3 down diagonal
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playerx[i]);
		   win=1;
	   }
	   player_x[9]=player_x[9]+1;
	}
	if(grid_play[0]==3 && grid_play[4]==3 && grid_play[8]==3)
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playery[i]);
		   win=1;
	   }
	   player_y[9]=player_y[9]+1;
	}
	if(grid_play[2]==2 && grid_play[4]==2 && grid_play[6]==2)// 3 up diagonal
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playerx[i]);
		   win=1;
	   }
	   player_x[9]=player_x[9]+1;
	}
	if(grid_play[2]==3 && grid_play[4]==3 && grid_play[6]==3)
	{
	   for (int i = 0 ; i<14 ; i++)
	   {
		   osd_p->wr_char(33+i, 3, playery[i]);
		   win=1;
	   }
	   player_y[9]=player_y[9]+1;
	}
	if(player_x[9]==57)
	{
	   for (int i = 0 ; i<19 ; i++)
	   {
		   osd_p->wr_char(31+i, 3, playerxwinsgame[i]);
		   win=1;
	   }

	}
	if(player_y[9]==57)
	{
	   for (int i = 0 ; i<19 ; i++)
	   {
		   osd_p->wr_char(31+i, 3, playerywinsgame[i]);
		   win=1;
	   }

	}
	if (win==0)
	{
		for(int i = 0; i<9 ; i++)
		{
			if (grid_play[i] != 0)
				noWin++;
		}
		if (noWin== 9)
			for(int i = 0; i<11 ; i++)
			{
				osd_p->wr_char(34+i, 3, no_win[i]);
			}
	}
}
void play(OsdCore *osd_p, Ps2Core *ps2_p)
{
	int lbtn, rbtn, xmov, ymov;

	ps2_p->get_mouse_activity(&lbtn, &rbtn, &xmov, &ymov);

	if (x<=233 && x>=110 && y<=190 && y>=94)
	    {
	        if (lbtn == 1 && grid[0]==0 && rclick==1)
	        {
	            osd_p->wr_char(21, 9 , 88);
	            grid[0]=1;
	            grid_play[0]=2;
	            lclick=1;
	            rclick=0;
	        }

	        if (rbtn == 1 && grid[0]==0 && lclick==1)
	        {
	            osd_p->wr_char(21, 9 , 79);
	            grid[0]=1;
	            grid_play[0]=3;
	            lclick=0;
	            rclick=1;
	        }
	    }

	    // square 2
	    if (x<=390 && x>=240 && y<=190 && y>=94)
	    {
	        if (lbtn == 1 && grid[1]==0 && rclick==1)
	        {
	            osd_p->wr_char(39, 9 , 88);
	            grid[1]=1;
	            grid_play[1]=2;
	            lclick=1;
	            rclick=0;
	        }
	        if (rbtn == 1 && grid[1]==0 && lclick==1)
	        {
	            osd_p->wr_char(39, 9 , 79);
	            grid[1]=1;
	            grid_play[1]=3;
	            lclick=0;
	            rclick=1;

	        }
	     }

	    // square 3
	    if (x<=518 && x>=401 && y<=190 && y>=94)
	    {
	        if (lbtn == 1 && grid[2]==0 && rclick==1 )
	        {
	            osd_p->wr_char(57, 9 , 88);
	            grid[2]=1;
	            grid_play[2]=2;
	            lclick=1;
	            rclick=0;
	        }
	        if (rbtn == 1 && grid[2]==0 && lclick==1)
	        {
	            osd_p->wr_char(57, 9 , 79);
	            grid[2]=1;
	            grid_play[2]=3;
	            lclick=0;
	            rclick=1;
	        }
	    }

	    // square 4
	    if (x<=223 && x>=110 && y<=334 && y>=207)
	    {
	        if (lbtn == 1 && grid[3]==0 && rclick==1)
	        {
	            osd_p->wr_char(21, 17 , 88);
	            grid[3]=1;
	            grid_play[3]=2;
	            lclick=1;
	            rclick=0;
	        }
	        if (rbtn == 1 && grid[3]==0 && lclick==1)
	        {
	            osd_p->wr_char(21, 17 , 79);
	            grid[3]=1;
	            grid_play[3]=3;
	            lclick=0;
	            rclick=1;
	        }
	    }

	    // square 5
	    if (x<=390 && x>=240 && y<=334 && y>=207)
	    {
	        if (lbtn == 1 && grid[4]==0 && rclick==1)
	        {
	            osd_p->wr_char(39, 17 , 88);
	            grid[4]=1;
	            grid_play[4]=2;
	            lclick=1;
	            rclick=0;
	        }
	        if (rbtn == 1 && grid[4]==0 && lclick==1)
	        {
	            osd_p->wr_char(39, 17 , 79);
	            grid[4]=1;
	            grid_play[4]=3;
	            lclick=0;
	            rclick=1;
	        }
	    }

	    // square 6
	    if (x<=518 && x>=401 && y<=334 && y>=207)
	    {
	        if (lbtn == 1 && grid[5]==0 && rclick==1)
	        {
	            osd_p->wr_char(57, 17 , 88);
	            grid[5]=1;
	            grid_play[5]=2;
	            lclick=1;
	            rclick=0;
	        }
	        if (rbtn == 1 && grid[5]==0 && lclick==1)
	        {
	            osd_p->wr_char(57, 17 , 79);
	            grid[5]=1;
	            grid_play[5]=3;
	            lclick=0;
	            rclick=1;
	        }
	    }

	    // square 7
	    if (x<=223 && x>=110 && y<=445 && y>=351)
	    {
	        if (lbtn == 1 && grid[6]==0 && rclick==1)
	        {
	            osd_p->wr_char(21, 24 , 88);
	            grid[6]=1;
	            grid_play[6]=2;
	            lclick=1;
	            rclick=0;
	        }
	        if (rbtn == 1 && grid[6]==0 && lclick==1)
	        {
	            osd_p->wr_char(21, 24 , 79);
	            grid[6]=1;
	            grid_play[6]=3;
	            lclick=0;
	            rclick=1;
	        }
	    }

	    // square 8
	    if (x<=390 && x>=240 && y<=445 && y>=351)
	    {
	        if (lbtn == 1 && grid[7]==0 && rclick==1)
	        {
	            osd_p->wr_char(39, 24 , 88);
	            grid[7]=1;
	            grid_play[7]=2;
	            lclick=1;
	            rclick=0;
	        }
	        if (rbtn == 1 && grid[7]==0 && lclick==1)
	        {
	            osd_p->wr_char(39, 24 , 79);
	            grid[7]=1;
	            grid_play[7]=3;
	            lclick=0;
	            rclick=1;
	        }
	    }

	    // square 9
	    if (x<=518 && x>=401 && y<=445 && y>=351)
	    {
	        if (lbtn == 1 && grid[8]==0 && rclick==1)
	        {
	            osd_p->wr_char(57, 24 , 88);
	            grid[8]=1;
	            grid_play[8]=2;
	            lclick=1;
	            rclick=0;
	        }
	        if (rbtn == 1 && grid[8]==0 && lclick==1)
	        {
	            osd_p->wr_char(57, 24 , 79);
	            grid[8]=1;
	            grid_play[8]=3;
	            lclick=0;
	            rclick=1;
	        }
	    }

	    check_win(osd_p);
}
void reset_screen(OsdCore *osd_p)
{
	   osd_p->set_color(0x001, 0x000); // dark gray/green
	   osd_p->bypass(0);
	   osd_p->clr_screen();

	   static int title[11] = {84,73,67,32,84,65,67,32,84,79,69};
	   static int playerx[9] = {80,108,97,121,101,114,32,88,58};
	   static int playery[9] = {80,108,97,121,101,114,32,79,58};

	   // tic-tac-toe writing
	   for (int i = 0 ; i<11 ; i++)
	   {
	       osd_p->wr_char(34+i, 1, title[i]);
	   }

	   // player 1 score
	   for (int i = 0 ; i<9 ; i++)
	   {
	          osd_p->wr_char(1+i, 1, playerx[i]);
	   }

	   // player 2 score
	   for (int i = 0 ; i<9 ; i++)
	   {
	       osd_p->wr_char(68+i, 1, playery[i]);
	   }

	  // grid setup
	   for (int i = 0; i < 51; i++)
	   {
	      osd_p->wr_char(14 + i , 44, 32);
	      osd_p->wr_char(14 + i , 21, 32);
	   }
	   for (int i = 0; i< 22 ; i++)
	   {
	      osd_p->wr_char(29, 6 + i , 32);
	      osd_p->wr_char(49, 6 + i , 32);
	    }
}
void check_first(GpiCore *sw_p)
{
    int first = sw_p->read(0);
	if (first ==1)
	{
		lclick=0;
		rclick=1;
	}
	else
	{
		lclick=1;
		rclick=0;
	}
}

/**
 * test ghost sprite
 * @param ghost_p pointer to ghost sprite instance
 */
void ghost_check(SpriteCore *ghost_p) {
   int x, y;

   // slowly move mouse pointer
   ghost_p->bypass(0);
   ghost_p->wr_ctrl(0x1c);  //animation; blue ghost
   x = 0;
   y = 100;
   for (int i = 0; i < 156; i++) {
      ghost_p->move_xy(x, y);
      sleep_ms(100);
      x = x + 4;
      if (i == 80) {
         // change to red ghost half way
         ghost_p->wr_ctrl(0x04);
      }
   }
   sleep_ms(3000);
}
int gsensor_set(SpiCore *spi_p) {
   const uint8_t RD_CMD = 0x0b;
   const uint8_t PART_ID_REG = 0x02;
   const uint8_t DATA_REG = 0x08;
   const float raw_max = 127.0 / 2.0;  			//128 max 8-bit reading for +/-2g

   int8_t xraw1, xraw2, yraw1, yraw2, zraw1, zraw2;
   double x1, x2, y1, y2, z1, z2;
   double distance1, distance2;
   int id;

   spi_p->set_freq(400000);
   spi_p->set_mode(0, 0);
   // check part id
   spi_p->assert_ss(0);    						// activate
   spi_p->transfer(RD_CMD);  					// for read operation
   spi_p->transfer(PART_ID_REG);  				// part id address
   id = (int) spi_p->transfer(0x00);
   spi_p->deassert_ss(0);

   spi_p->assert_ss(0);    						// activate
   spi_p->transfer(RD_CMD);  					// for read operation
   spi_p->transfer(DATA_REG);
   xraw1 = spi_p->transfer(0x00);
   yraw1 = spi_p->transfer(0x00);
   zraw1 = spi_p->transfer(0x00);
   spi_p->deassert_ss(0);
   x1 = (double) (xraw1 / raw_max);
   y1 = (double) (yraw1 / raw_max);
   z1 = (double) (zraw1 / raw_max);

   distance1 = abs(x1)+abs(y1)+abs(z1); 		//city block distance instead of magnitude equation
   return distance1;
}



// external core instantiation
GpoCore led(get_slot_addr(BRIDGE_BASE, S2_LED));
GpiCore sw(get_slot_addr(BRIDGE_BASE, S3_SW));
DebounceCore btn(get_slot_addr(BRIDGE_BASE, S7_BTN));
FrameCore frame(FRAME_BASE);
GpvCore bar(get_sprite_addr(BRIDGE_BASE, V7_BAR));
GpvCore gray(get_sprite_addr(BRIDGE_BASE, V6_GRAY));
SpriteCore ghost(get_sprite_addr(BRIDGE_BASE, V3_GHOST), 1024);
SpriteCore mouse(get_sprite_addr(BRIDGE_BASE, V1_MOUSE), 1024);
OsdCore osd(get_sprite_addr(BRIDGE_BASE, V2_OSD));
SsegCore sseg(get_slot_addr(BRIDGE_BASE, S8_SSEG));
Ps2Core ps2(get_slot_addr(BRIDGE_BASE, S11_PS2));
SpiCore spi(get_slot_addr(BRIDGE_BASE, S9_SPI));

int main() {
        init(&ps2);
        bar.bypass(1);
        gray.bypass(0);
        frame.bypass(0);
        ghost.bypass(1);
        double distance1 = gsensor_set(&spi);


        frame_check(&frame);
        check_first(&sw);
        while (1)
        {
            osd_check(&osd);


            mouse_check(&mouse, &ps2);
            if(!win)
            {

            	play(&osd, &ps2);
            }
            double distance2 = gsensor_set(&spi);
            double difference = abs(distance2-distance1)*1000; // multiplied by 1000 so it's easier to see the difference
		    if(difference>=600)
			    reset=1;
		    int complete_reset =btn.read(4);

		    if(complete_reset)
		    {
            	reset_screen(&osd);
            	reset=0;
            	win=0;
            	check_first(&sw);
            	for(int i=0; i<9; i++)
            		grid[i]=0;
            	for(int i=0; i<9; i++)
            	    grid_play[i]=0;

				player_x[9]=48;
				player_y[9]=48;
		    }

            if(reset)
            {
            	reset_screen(&osd);
            	reset=0;
            	win=0;
            	check_first(&sw);
            	for(int i=0; i<9; i++)
            		grid[i]=0;
            	for(int i=0; i<9; i++)
            	    grid_play[i]=0;
            	if(player_x[9]==57)
            	{
            		player_x[9]=48;
            		player_y[9]=48;
            	}
				if(player_y[9]==57)
				{
					player_x[9]=48;
					player_y[9]=48;
				}

            }
        }


} //main
