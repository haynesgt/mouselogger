#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <math.h>

void sleepd(double seconds)
{
	struct timespec x, y;
	x.tv_sec = 0;
	x.tv_nsec = floor(seconds * 1e9);
	nanosleep(&x, &y);
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
		if (c = getc(stdin) == EOF) break;
		if (scanf("%d (%d, %d)", &i, &mx, &my) == EOF) break;
		printf ("%.20f: %c%d (%d, %d)\n", time, c, i, mx, my);
		if (t != -1) sleepd((time - t)/2.0);
		t = time;
		XTestFakeMotionEvent(display, -1, mx, my, 0);
		if (c == '+') {
			XTestFakeButtonEvent(display, 1, True, CurrentTime);
		} else {
			XTestFakeButtonEvent(display, 1, False, CurrentTime);
		}
		XFlush(display);
		c = getc(stdin);
	}
	return 0;
}
