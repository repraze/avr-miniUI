/*  Author: Thomas Dubosc
 * Licence: This work is licensed under the Creative Commons Attribution License.
 *           View this license at http://creativecommons.org/licenses/
 */
 
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include <avr/interrupt.h>

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
