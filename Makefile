all:	logmouse replaymouse
logmouse:	logmouse.c
	gcc -o bin/logmouse -I/usr/X11R6/include -L/usr/X11R6/lib logmouse.c -lX11 -lXtst

replaymouse:	replaymouse.c
	gcc -o bin/replaymouse -I/usr/X11R6/include -L/usr/X11R6/lib replaymouse.c -lX11 -lXtst -lm
