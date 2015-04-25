/* Send output from logmouse to this file to get it to replay the mouse events.
 * Gavin Haynes. 2012.
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>
#include <time.h>

void sleepd(double seconds)
{
	struct timespec x, y;
	x.tv_sec = floor(seconds);
	x.tv_nsec = (seconds-x.tv_sec)*1e9;
	while (x.tv_nsec + x.tv_sec > 0) {
		nanosleep(&x, &y);
		x = y;
	}
}

int main(int argc, char *argv[])
{
	Display *display;
	Window w;
	float time;
	int i, mx, my;
	char c;
	float t = -1;
	display = XOpenDisplay(NULL);
	w = DefaultRootWindow(display);
	c = EOF+1;
	while (c != EOF) {
		if (scanf("%f: ", &time) == EOF) break;
		c = getc(stdin);
		if (c == EOF) break;
		if (scanf("%d (%d, %d)", &i, &mx, &my) == EOF) break;
		if (t != -1) {
			sleepd((time - t)/1.0);
			printf("Sleeping %f\n", (time - t)/1.0);
		}
		t = time;
		XTestFakeMotionEvent(display, -1, mx, my, 0);
		if (c == '+') {
			XTestFakeButtonEvent(display, 1, True, CurrentTime);
		} else {
			XTestFakeButtonEvent(display, 1, False, CurrentTime);
		}
		XFlush(display);
		printf ("%.20f: %c%d (%d, %d)\n", time, c, i, mx, my);
		c = getc(stdin);
	}
	return 0;
}
