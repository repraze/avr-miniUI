/*  revRotDriver.c

    Author: Thomas Dubosc            May 2014 
            using components written by
                Steve Gunn and Peter Dannegger

 */

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
//#include "miniUI.h"
#include <avr/interrupt.h>



#define STDIO_BAUD  9600
#define PRNTBUF 200


#define D0_L  DDRD |=  _BV(PD0); PORTD &= ~_BV(PD0);
#define D0_H  DDRD |=  _BV(PD0); PORTD |=  _BV(PD0);
#define D0_R  DDRD &= ~_BV(PD0); PORTD |=  _BV(PD0);
#define D0_Z  DDRD &= ~_BV(PD0); PORTD &= ~_BV(PD0);

#define D1_L  DDRD |=  _BV(PD1); PORTD &= ~_BV(PD1);
#define D1_H  DDRD |=  _BV(PD1); PORTD |=  _BV(PD1);
#define D1_R  DDRD &= ~_BV(PD1); PORTD |=  _BV(PD1);
#define D1_Z  DDRD &= ~_BV(PD1); PORTD &= ~_BV(PD1);


#define C2_L  DDRC |=  _BV(PC2); PORTC &= ~_BV(PC2);
#define C2_H  DDRC |=  _BV(PC2); PORTC |=  _BV(PC2);
#define C2_R  DDRC &= ~_BV(PC2); PORTC |=  _BV(PC2);
#define C2_Z  DDRC &= ~_BV(PC2); PORTC &= ~_BV(PC2);

#define C3_L  DDRC |=  _BV(PC3); PORTC &= ~_BV(PC3);
#define C3_H  DDRC |=  _BV(PC3); PORTC |=  _BV(PC3);
#define C3_R  DDRC &= ~_BV(PC3); PORTC |=  _BV(PC3);
#define C3_Z  DDRC &= ~_BV(PC3); PORTC &= ~_BV(PC3);

#define C4_L  DDRC |=  _BV(PC4); PORTC &= ~_BV(PC4);
#define C4_H  DDRC |=  _BV(PC4); PORTC |=  _BV(PC4);
#define C4_R  DDRC &= ~_BV(PC4); PORTC |=  _BV(PC4);
#define C4_Z  DDRC &= ~_BV(PC4); PORTC &= ~_BV(PC4);

#define C5_L  DDRC |=  _BV(PC5); PORTC &= ~_BV(PC5);
#define C5_H  DDRC |=  _BV(PC5); PORTC |=  _BV(PC5);
#define C5_R  DDRC &= ~_BV(PC5); PORTC |=  _BV(PC5);
#define C5_Z  DDRC &= ~_BV(PC5); PORTC &= ~_BV(PC5);


/* #define DZ DDRD=0; PORTD=0; */
#define CZ DDRD=0; PORTD=0;



#define LED_GREEN_ON   DDRD |= _BV(PD6); PORTD |= _BV(PD6); 
#define LED_GREEN_OFF  DDRD |= _BV(PD6); PORTD &= ~_BV(PD6); 

#define LED_YELLOW_ON  DDRD |= _BV(PD4); PORTD |= _BV(PD4); 
#define LED_YELLOW_OFF DDRD |= _BV(PD4); PORTD &= ~_BV(PD4); 

#define LED_RED_ON     DDRB |= _BV(PB7); PORTB |= _BV(PB7); 
#define LED_RED_OFF    DDRB |= _BV(PB7); PORTB &= ~_BV(PB7); 

void init_stdio_uart1( void );
void scanswitch_init( void );


/* volatile int8_t delta;    // -128 ... 127 */
/* int8_t iob_delta(void); */
int16_t iob_delta(void);
volatile int16_t delta;

//move rectangle
void move_rectangle(rectangle *r,int x, int y);

//rectangle border
void stroke_rectangle(rectangle r, uint16_t col);

//simple collision
int rectangle_collide(rectangle a,rectangle b);

typedef struct {
	int left, right, up, down;
	int center;
	int wheel_d;
	int has_pressed_button;
} button_map;
//button listener
void listen_to_button(button_map *bmap);

//display text at a x y coordinate
void display_string_at(char *str,int x, int y);
//disaplay a menu at
int display_menu_at(char *menu, int choice, int x,int y, int padding);

int rectangle_inside(rectangle a,rectangle b);
int rectangle_collide(rectangle a,rectangle b);

typedef struct {
	uint16_t menu_background;
	uint16_t button_foreground;
	uint16_t button_background;
	uint16_t button_selected_foreground;
	uint16_t button_selected_background;
} miniUI;

miniUI default_UI;

void init_miniUI();

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

void init_miniUI(){
	default_UI.menu_background = WHITE;
	default_UI.button_foreground = BLACK;
	default_UI.button_background = 0xCCCC;
	default_UI.button_selected_foreground = WHITE;
	default_UI.button_selected_background = BLUE;
}

void listen_to_button(button_map *bmap){
	int has_pressed_button = 0;
	uint8_t ddrc, portc;
	
	portc  = PORTC;  /* Store display configuration of Port C */
	ddrc   = DDRC;
	
	bmap->wheel_d = iob_delta();
	has_pressed_button += bmap->wheel_d;
	
	D1_Z

	D0_H
		D0_R

			C2_H C3_H C4_H C5_L
		_delay_ms(1);//wait for value to change on the io board
		bmap->up = (!(PIND & _BV(PD0)));
		has_pressed_button += bmap->up;

			C2_H C3_H C4_L C5_H
		_delay_ms(1);//wait for value to change on the io board
		bmap->right = (!(PIND & _BV(PD0)));
		has_pressed_button += bmap->right;


			C2_H C3_L C4_H C5_H
		_delay_ms(1);//wait for value to change on the io board
		bmap->down = (!(PIND & _BV(PD0)));
		has_pressed_button += bmap->down;

			C2_L C3_H C4_H C5_H
		_delay_ms(1);//wait for value to change on the io board
		bmap->left = (!(PIND & _BV(PD0)));
		has_pressed_button += bmap->left;

		D0_Z

		D1_H 
	D1_R
	 
	C2_Z C3_Z C4_L C5_Z
	_delay_ms(1);//wait for value to change on the io board
	bmap->center = (!(PIND & _BV(PD1)));
	has_pressed_button += bmap->center;


	D1_L

	C2_R C3_R
		
	//if (!(PINC & _BV(PC2))) { pushed("A"); } else {printf("\n");}
	//if (!(PINC & _BV(PC3))) { pushed("B "); } else {printf("\n");}
	
	DDRC    = ddrc;  /* Restore display configuration of Port C */
	PORTC   = portc;
	
	bmap->has_pressed_button = has_pressed_button;
}

//new drawing feature, rectangle "border"
void stroke_rectangle(rectangle r, uint16_t col){
	rectangle pr;
	//top
	pr.left		= r.left;
	pr.right	= r.right;
	pr.top		= r.top;
	pr.bottom	= r.top;
	fill_rectangle(pr, col);
	//bottom
	pr.left		= r.left;
	pr.right	= r.right;
	pr.top		= r.bottom;
	pr.bottom	= r.bottom;
	fill_rectangle(pr, col);
	//left
	pr.left		= r.left;
	pr.right	= r.left;
	pr.top		= r.top;
	pr.bottom	= r.bottom;
	fill_rectangle(pr, col);
	//right
	pr.left		= r.right;
	pr.right	= r.right;
	pr.top		= r.top;
	pr.bottom	= r.bottom;
	fill_rectangle(pr, col);
}

int rectangle_collide(rectangle a,rectangle b){
	if( a.bottom <= b.top ){
		return 0;
	}
	if( a.top >= b.bottom ){
		return 0;
	}
	if( a.right <= b.left ){
		return 0;
	}
	if( a.left >= b.right ){
		return 0;
	}
	
	return 1;
}

//check if rectangle a is inside b
int rectangle_inside(rectangle a,rectangle b){
	if( a.top >= b.top && 
		a.bottom <= b.bottom &&
		a.left >= b.left &&
		a.right <= b.right ){
		return 1;
	}
	
	return 0;
}

//move the x and y of a rectangle
void move_rectangle(rectangle *r,int x, int y){
	r->left+=x;
	r->right+=x;
	r->top+=y;
	r->bottom+=y;
}

void display_string_at(char *str,int x, int y){
	display.x = x;
	display.y = y;
	
	display_string(str);
}

int count_chars(const char* string, char ch){
    int count = 0;
    int i;

    int length = strlen(string);

    for (i = 0; i < length; i++){
        if (string[i] == ch)
        {
            count++;
        }
    }
    return count;
}

int display_menu_at(char *menu, int choice, int x,int y, int padding){
	int menu_height = padding*2+7;
	int menu_width = +strlen(menu)*6;
	
	char sep = ';';
	char *seps = ";";
	int item_nb=count_chars(menu,sep)+1;
	
	menu_width += padding*(item_nb+1)-(item_nb-1)*6;
	
	char *buttons[item_nb];
	
	char *token = strtok (menu,seps);
	
	int i=0;
	while (token != NULL){
		buttons[i++] = token;
		token = strtok (NULL, seps);
	}

	if(choice<0){
		choice = 0;
	}
	else if(choice>item_nb-1){
		choice = item_nb-1;
	}
	
	button_map bmap;
	bmap.left 	= 0;
	bmap.right 	= 0;
	bmap.up 	= 0;
	bmap.down 	= 0;
	bmap.center = 0;
	bmap.wheel_d= 0;
	bmap.has_pressed_button=0;
	
	int menuloop = 1;
	
	uint16_t foreground_save = display.foreground;
	uint16_t background_save = display.background;
	
	//wait for user to release the buttons
	listen_to_button(&bmap);
	while(bmap.has_pressed_button){
		listen_to_button(&bmap);
		_delay_ms(10);
	}
	
	rectangle menur;
	menur.top=y;
	menur.left=x;
	menur.right=x+menu_width;
	menur.bottom=y+menu_height;
	
	fill_rectangle(menur,default_UI.menu_background);
	
	while(menuloop){
		//listen to button and restore
		listen_to_button(&bmap);
		
		choice = (choice+bmap.wheel_d) % item_nb;
		if(choice<0){
			choice+=item_nb;
		}
		
		int position_x=x+padding;
		int position_y=y+padding;
		for (i=0;i<item_nb; ++i){
			if(i!=choice){
				display.foreground = default_UI.button_foreground;
				display.background = default_UI.button_background;
			}
			else{
				display.foreground = default_UI.button_selected_foreground;
				display.background = default_UI.button_selected_background;
			}
			display_string_at(buttons[i],position_x,position_y);
			position_x+=strlen(buttons[i])*6;
			position_x+=padding;
		}
		
		if(bmap.center){
			menuloop=0;
		}
		
		_delay_ms(100);
	}
	
	display.foreground = foreground_save;
	display.background = background_save;
	
	return choice;
}

void scanswitch_init( void ) {

   // Timer 0 for switch scan interrupt:

   TCCR0A = _BV(WGM01);
   TCCR0B = _BV(CS01)
          | _BV(CS00); 	 /* F_CPU / 64 */

   /* 1ms for manual movement of rotary encoder: */
   OCR0A = (uint8_t)(F_CPU / (64.0 * 1000) - 0.5);

   TIMSK0 |= _BV(OCIE0A);  /* Enable timer interrupt */

   sei();
 }




 ISR( TIMER0_COMPA_vect )
 {
     static int8_t last;
     uint8_t ddrc, portc;
     uint8_t ddrd, portd;
     int8_t new, diff;
     uint8_t wheel;

     /* Save state of Port C & D */
     ddrc   = DDRC; portc  = PORTC;
     ddrd   = DDRD; portd  = PORTD;


     CZ C2_H C3_H C2_R C3_R
     D0_Z D1_L
     _delay_us(3);
     wheel = PINC;


     /*
        Scan rotary encoder
        ===================
        This is adapted from Peter Dannegger's code available at:
        http://www.mikrocontroller.net/attachment/40597/ENCODE.C
     */

     new = 0;
     if( wheel  & _BV(PC3) ) new = 3;
     if( wheel  & _BV(PC2) )
	 new ^= 1;			// convert gray to binary
     diff = last - new;			// difference last - new
     if( diff & 1 ){			// bit 0 = value (1)
	 last = new;		       	// store new as next last
	 delta += (diff & 2) - 1;	// bit 1 = direction (+/-)
     }


    /* Restore state of Port C */
    DDRC    = ddrc; PORTC   = portc;
    DDRD    = ddrd; PORTD   = portd;

    PORTA &= ~_BV(PA0);  /* ISR Timing End */
    sei();
}


// read two step encoder
int16_t iob_delta(){
    int16_t val;

    cli();
    val = delta;
    delta &= 1;
    sei();

    return val >> 1;
}





/*  Author: Steve Gunn
  * Licence: This work is licensed under the Creative Commons Attribution License.
  *          View this license at http://creativecommons.org/about/licenses/
  *   Usage: Include in your main file and call init_stdio_uart0() (or init_stdio_uart1())
  *          from the beginning of main to initialise redirection of stdout, stderr
  *          and stdin to UART0 (or UART1).
  */

 int uputchar1(char c, FILE *stream)
 {
	 if (c == '\n') uputchar1('\r', stream);
	 while (!(UCSR1A & _BV(UDRE1)));
	 UDR1 = c;
	 return c;
 }

 int ugetchar1(FILE *stream)
 {
	 while(!(UCSR1A & _BV(RXC1)));
	 return UDR1;
 }

 void init_stdio_uart1(void)
 {
	 /* Configure UART1 baud rate, one start bit, 8-bit, no parity and one stop bit */
	 UBRR1H = (F_CPU/(STDIO_BAUD*16L)-1) >> 8;
	 UBRR1L = (F_CPU/(STDIO_BAUD*16L)-1);
	 UCSR1B = _BV(RXEN1) | _BV(TXEN1);
	 UCSR1C = _BV(UCSZ10) | _BV(UCSZ11);

	 /* Setup new streams for input and output */
	 static FILE uout = FDEV_SETUP_STREAM(uputchar1, NULL, _FDEV_SETUP_WRITE);
	 static FILE uin = FDEV_SETUP_STREAM(NULL, ugetchar1, _FDEV_SETUP_READ);

	 /* Redirect all standard streams to UART0 */
	 stdout = &uout;
	 stderr = &uout;
	 stdin = &uin;
 }
