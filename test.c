#include "miniUI.h"
#include "lcd.h"

void main() {
	/* Disable JTAG in software, so that it does not interfere with Port C */
	MCUCR |= _BV(JTD);
	MCUCR |= _BV(JTD);


	init_stdio_uart1();

	init_lcd();
	set_orientation(East);
     


	scanswitch_init();
	
	button_map bmap;
	bmap.left 	= 0;
	bmap.right 	= 0;
	bmap.up 	= 0;
	bmap.down 	= 0;
	bmap.center = 0;
	bmap.wheel_d= 0;
	bmap.has_pressed_button=0;
	
	init_miniUI();
	
	display_menu_at("test;tes2;tes3",1, 10,10,2);
}