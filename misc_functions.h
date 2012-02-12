#ifndef _MISC_H_
#define _MISC_H_

#include "stdafx.h"

inline int rrand(int M, int N)
{
	return M + rand() / (RAND_MAX / (N - M + 1) + 1);
}

inline float rrandf(int M, int N)
{
	return M + (float(rand()) / float(RAND_MAX)) * (N-M);
}

struct rgb_sunset
{
	float r, g, b;
	float ro, go, bo;

	rgb_sunset(float _r, float _g, float _b)
		: r(_r), g(_g), b(_b), ro(_r), go(_g), bo(_b)
	{}

	void assert()
	{
		if(r < 0) r =0;
		if(g < 0) g =0;
		if(b < 0) b =0;

		if(r >255) r =255;
		if(g > 255) g =255;
		if(b > 255) b =255;
	}
	void process(float diff)
	{
		r -= diff;
		g -= diff;
		b -= diff;

		assert();
	}
	void process(float rd, float gd, float bd)
	{
		r -= rd;
		g -= gd;
		b -= bd;

		assert();
	}
	int makecolor()
	{
		return makecol((int)r, (int)g, (int)b);
	}
	void reset()
	{
		r = ro;
		g = go;
		b = bo;
	}
};

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