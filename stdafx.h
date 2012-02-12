// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

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
#include <tchar.h>



// TODO: reference additional headers your program requires here
