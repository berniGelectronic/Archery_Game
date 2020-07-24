/* graphics_lib.h defines functions which hide
   complexities of allegro and allow
   the C programming labscripts to be used
   almost as is. This is done so that many of the complexities
   of C are hidden from the beginner.
   graphics_lib.h does not update the display after each drawing
   function, instead it expects the programmer to do that
   (using update_display).
   This will allow the ability to simulate multiple events.
   Author: Julian Miller, Department of Electronics
   University of York, Jan 2013.

   Updates: Bitmap handling added on 29/4/2013
*/


#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <conio.h>
#endif
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#ifdef __APPLE__
#include <termios.h>//Used for getch() on Unix-based systems
#include <unistd.h> //used for getch() on Unix-based systems
#endif


typedef struct colour_info
{
  unsigned r;
  unsigned g;
  unsigned b;
} COLOUR;

#define  PI  3.141592653589793

/* globals */
COLOUR        RGB, BACKGROUND_RGB;
ALLEGRO_DISPLAY     *DISPLAY;
int                 GLOBAL_X, GLOBAL_Y;
ALLEGRO_FONT        *GLOBAL_FONT; /*global font pointer */
ALLEGRO_PATH        *path;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_EVENT       event;
int                 XMOUSE, YMOUSE, ZMOUSE;

#define BITMAP          ALLEGRO_BITMAP*
#define BLACK           0
#define BLUE            1
#define GREEN           2
#define CYAN            3
#define RED             4
#define MAGENTA         5
#define BROWN           6
#define LIGHTGRAY       7
#define DARKGRAY        8
#define LIGHTBLUE       9
#define LIGHTGREEN      10
#define LIGHTCYAN       11
#define LIGHTRED        12
#define LIGHTMAGENTA    13
#define YELLOW          14
#define WHITE           15


/* prototypes */
int rand_number(int lower_range, int upper_range);

void pausefor(int time_in_milliseconds);

void initwindow(int x, int y);

void initfont(void);

void closegraph(void);

COLOUR make_RGB(unsigned colour);

void setcolor(unsigned x);

void update_display(void);

void filled_circle(int x, int y, int radius, unsigned fillcolour);

void circle(int x, int y, int radius, int thickness);


void filled_rectangle(int upper_left_x, int upper_left_y,
               int lower_right_x, int lower_right_y,
               unsigned fillcolour);

void rectangle(int upper_left_x, int upper_left_y,
               int lower_right_x, int lower_right_y,
               int thickness);

void filled_ellipse(int centre_x, int centre_y,
               int radius_x, int radius_y,
               unsigned fillcolour);

void ellipse(int centre_x, int centre_y,
               int radius_x, int radius_y,
               int thickness);

void arc(int centre_x, int centre_y, int radius,
         double angle_start, double angle_end, int thickness);

void line(int xstart, int ystart, int xend, int yend, int thickness);

void lineto(int x, int y, int thickness);


void setbkcolor(unsigned colour);

void cleardevice(void);

void moveto(int x, int y);

void outtextxy(int x, int y, char text[]);

void create_event_queue(void);

void reg_display_events(void);

void reg_mouse_events(void);

void hide_mouse_cursor(void);

int check_if_event(void);

void wait_for_event(void);

int event_close_display(void);

int event_mouse_position_changed(void);

void get_mouse_coordinates(void);

int event_mouse_button_down(void);

int event_mouse_left_button_down(void);

int event_mouse_right_button_down(void);

int event_mouse_scroll_button_down(void);

void initmouse(void);

void closemouse(void);

/*Additional Functions*/

void initkeyboard(void);

void reg_keyboard_events(void);

void closekeyboard(void);

int event_key_down(void);

int event_key_up(void);

int event_key_up_arrow(void);

int event_key_down_arrow(void);

int event_key_left_arrow(void);

int event_key_right_arrow(void);

int event_key(char letter);

/* Bitmap routines */
BITMAP init_bitmap(void);

BITMAP load_bitmap(char* name);

void make_image_background_transparent(BITMAP image, int red, int green, int blue);

void draw_bitmap(BITMAP image, int x, int y);

void destroy_bitmap(BITMAP image);

#ifdef __APPLE__
int getch(); //getch() alternative for Unix based systems
#endif

