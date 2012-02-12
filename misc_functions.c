#include "misc_functions.h"


volatile int ticks = 0;
volatile int game_time = 0;


int setup()
{
	srand(time(NULL));
	allegro_init();
	set_window_title("pixel valentine");

	return install_keyboard() & install_timer();
}


const char * os_name(int number)
{
	switch (number)
	{
	case OSTYPE_UNKNOWN: return "Unknown or MS-DOS";
	case OSTYPE_WIN3:    return "Windows";
	case OSTYPE_WIN95:   return "Windows 95";
	case OSTYPE_WIN98:   return "Windows 98";
	case OSTYPE_WINME:   return "Windows ME";
	case OSTYPE_WINNT:   return "Windows NT";
	case OSTYPE_WIN2000: return "Windows 2000";
	case OSTYPE_WINXP:   return "Windows XP";
	case OSTYPE_OS2:     return "OS/2";
	case OSTYPE_WARP:    return "OS/2 Warp 3";
	case OSTYPE_DOSEMU:  return "Linux DOSEMU";
	case OSTYPE_OPENDOS: return "Caldera OpenDOS";
	case OSTYPE_LINUX:   return "Linux";
	case OSTYPE_FREEBSD: return "FreeBSD";
	case OSTYPE_QNX:     return "QNX";
	case OSTYPE_UNIX:    return "Unix variant";
	case OSTYPE_BEOS:    return "BeOS";
	case OSTYPE_MACOS:   return "MacOS";
	default:             return "";
	}
}



void render_object(
				   MATRIX_f *cam, 
				   BITMAP *bmp, BITMAP *sprite, 
				   float x, float y, float z
				   ) 
{     
	float r_x=0, r_y=0, r_z=0;
	float t_x=0, t_y=0, s_x=0, s_y=0;

	// Transport world view to camera view
	apply_matrix_f(cam, x, y, z, &r_x, &r_y, &r_z);

	// Project top-right, bottom left to 2-D
	persp_project_f(r_x, r_y, r_z, &t_x, &t_y);   
	persp_project_f(sprite->w, sprite->h, r_z, &s_x, &s_y);

	stretch_sprite(bmp, sprite, (int)t_x, (int)t_y, (int)s_x/10, (int)s_y/10);
}



void darken_pixel(BITMAP * bit, int r, int g, int b)
{				
	for(int i = 0; i < bit->w; i++) 
	{
		for(int j = 0; j < bit->h; j++) 
		{
			int p = getpixel(bit, i, j);
			int lol = p;
			lol &= 0x0000FF00;
			lol >>= 8;

			if(lol > 80) lol--;

			lol <<= 8;
			p &= 0xFFFF00FF;
			p |= lol;
			putpixel(bit, i, j, p);
		}
	}
}

void darken_pixel16(BITMAP * bit, int r, int g, int b)
{
	for(int i = 0; i < bit->w; i++) {
		for(int j = 0; j < bit->h; j++) {
			int c = _getpixel16(bit, i, j);
			int cr = c & 31;
			int cg = (c >> 5) & 63;
			int cb = (c >> 11) & 31;
			c = ((cr | (cb << 10) | (cg << 20)) * 50) >> 6;
			cr = c & 31;
			cb = (c >> 10) & 31;
			cg = (c >> 20) & 63;
			_putpixel16(bit, i, j, cr | (cg << 5) | (cb << 11));
		}
	}
}

void ticker() { ticks++; }
END_OF_FUNCTION(ticker)

void game_time_ticker() { game_time++; }
END_OF_FUNCTION(game_time_ticker)









