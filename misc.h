#ifndef _MISC_H_
#define _MISC_H_


#define ALLEGRO_USE_CONSOLE
#define ALLEGRO_STATICLINK
#define ALLEGRO_NO_ASM
#if defined(ALLEGRO_STATICLINK)
	#if defined(DEBUG) || defined(_DEBUG)
		#pragma comment(lib, "alld_s")
	#else
		#pragma comment(lib, "alleg_s")
	#endif
#else
	#if defined(DEBUG) || defined(_DEBUG)
		#pragma comment(lib, "alld")
	#else
		#pragma comment(lib, "alleg")
	#endif
#endif


#include <allegro.h>
#include <stdio.h>
#include <math.h>

int rrand(int M, int N);
float rrandf(int M, int N);

struct rgb_color
{
	float r, g, b;
	float ro, go, bo;
};

void assert_color(struct rgb_color * r);
void process_color(struct rgb_color * r, float diff);
void process_color_r(struct rgb_color * r, float rd, float gd, float bd);

int make_color(struct rgb_color * r);
void reset(struct rgb_color *r);
int setup();
const char * os_name(int);

void render_object(
				   MATRIX_f *cam, 
				   BITMAP *bmp, BITMAP *sprite, 
				   float x, float y, float z
				   );

void darken_pixel(BITMAP * bit, int r, int g, int b);
void darken_pixel16(BITMAP * bit, int r, int g, int b);

extern volatile int ticks;
extern volatile int game_time;

void ticker();
void game_time_ticker();

#endif
