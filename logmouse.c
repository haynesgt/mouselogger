/*
 * Records where the mouse clicks, recording which buttons were pressed and released, and when.
 *
 * Compile with:
	gcc -o logmouse -I/usr/X11R6/include -L/usr/X11R6/lib logmouse.c -lX11 -lXtst
 *
 * Requirements: libxtst-dev libx11-dev
 * Made by Gavin Haynes, 2012.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

double gettime()
{
	static double t0 = 0;
	struct timeval tim;
	gettimeofday(&tim, NULL);
	double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	if (t0 == 0) t0 = t1;
	return t1 - t0;
}

struct MData;
struct MData
{
	struct MData *next;
	int x;
	int y;
};

int main(int argc, char *argv[])
{
	Display *display;
	Window w;
	Window nw; // Null window for making XQueryPointer work ...
	unsigned int mouse_return;
	unsigned int mouse_last;
	struct MData firstM;
	struct MData *lastM;
	display = XOpenDisplay(NULL);
	w = DefaultRootWindow(display);
	mouse_return = 0;
	lastM = &firstM;
	lastM->next = NULL;
	lastM->x = 0;
	lastM->y = 0;
	while (1) {
		int mx, my;
		int nn; // Null number to make XQueryPointer work.
		int i;
		mouse_last = mouse_return;
//		This function gets the pointer position and state:
		XQueryPointer(display, w, &nw, &nw, &mx, &my, &nn, &nn, 
								&mouse_return);
//		We only need bits 8 to 10. The others are modifiers or something
		for (i = 8; i <= 10; i++) {
			if ((mouse_return & (1<<i)) && !(mouse_last & (1<<i))) {
				printf ("%.20f: +%d (%d, %d)\n", gettime(),
								i, mx, my);
				if (i != 8) goto replay;
				lastM->x = mx;
				lastM->y = my;
				lastM->next = malloc(sizeof(struct MData));
				lastM = lastM->next;
				lastM->next = NULL;
			}
			if (!(mouse_return & (1<<i)) && (mouse_last & (1<<i))) {
				printf ("%.20f: -%d (%d, %d)\n", gettime(),
								i, mx, my);
			}
		}
		usleep(30e3);
	}
	replay:
#if 0	
	//usleep(3e6);
	struct MData *iM;
	iM = &firstM;
	while (iM->next != NULL) {
		XTestFakeMotionEvent(display, -1, iM->x, iM->y, 0);
		XFlush(display);
		usleep(30e3);
		iM = iM->next;
	}
#endif
	XCloseDisplay(display);
}
