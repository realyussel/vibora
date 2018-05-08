/*
 * cedit.c
 *
 * Description: Ncurses Text Editor Example
 * Copyright (c) 2017 Aleksander Djuric. All rights reserved.
 * Distributed under the GNU Lesser General Public License (LGPL).
 * The complete text of the license can be found in the LICENSE
 * file included in the distribution.
 *
 */

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STR 1024
#define MENU_ITEMS 10

void ncstart();
void ncfinish();
void draw_menubar(WINDOW *menu, int size);
void draw_statbar(WINDOW *status, const char *fmt, ...);
int mvwtoeol(WINDOW *p, int y, int x);
char *wstr(char *str, int size);

int main(int argc, char* argv[]) {
	FILE *fp = NULL;
	WINDOW *p, *status, *menu;
	int i, key, x, y, width, height, start, maxrows, maxcols;
	char fname[MAX_STR];
	char buffer[MAX_STR];
	bool exitflag = false;

	fname[0] = '\0';
	if (argc > 1) {
		strncpy(fname, argv[1], MAX_STR-1);

		fp = fopen(fname, "rw");
		if (fp == NULL) {
			printf("Couldn't open file: %s\n", fname);
			return -1;
		}
	}
	
	ncstart();

	maxrows = 1;
	maxcols = MAX_STR;
	start = 0;
	width = COLS - 1;
	height = LINES - 2;
	x = 0;
	y = 0;

	// Status bar
	status = newwin(1, COLS, 0, 0);
	wbkgd(status, COLOR_PAIR(2));
	draw_statbar(status, "%s", fname);

	// Bottom bar
	menu = newwin(1, COLS, LINES-1, 0);
	draw_menubar(menu, COLS);

	// Editor window
	p = newpad(maxrows, maxcols);
	wbkgd(p, COLOR_PAIR(1));
	keypad(p, true);
	scrollok(p, TRUE);

	if (fp) {
		while(!feof(fp)) {
			if (!fgets(buffer, MAX_STR, fp)) break;
			wresize(p, maxrows + 1, maxcols);
			waddstr(p, buffer);
			maxrows++;
		}
		maxrows--;
		fclose(fp);
	}

	// Refresh
	refresh();
	wrefresh(status);
	wrefresh(menu);

	do {
		prefresh(p, start, 0, 1, 0, height, width);
		wmove(p, y, x);
		key = getch();

		switch(key) {
		case ERR:
		case KEY_F(1):
		case KEY_F(3):
		case KEY_F(4):
		case KEY_F(5):
		case KEY_F(6):
		case KEY_F(7):
		case KEY_F(9):
		case KEY_F(11):
		case KEY_F(12):
			break;
		case KEY_F(2):
			if (*fname == '\0') {
				draw_statbar(status, "Enter file name: ");
				echo();
				wrefresh(status);
				wgetstr(status, fname);
				draw_statbar(status, "%s", fname);
				noecho();
				wrefresh(status);
				if (*fname == '\0') break;
			} else {
				draw_statbar(status, "Saving to file: %s", fname);
				wrefresh(status);
				key = wgetch(status);
				draw_statbar(status, "%s", fname);
				wrefresh(status);
				if (key == 27) {
					break;
				}
			}
			fp = fopen(fname, "w+");
			if (!fp) break;
			for (i = 0; i < maxrows; i++) {
				mvwinstr(p, i, 0, buffer);
				if (fputs(wstr(buffer, maxcols), fp) == EOF)
					break;
			}
			fclose(fp);
			break;
		case KEY_F(8):
		case 25: // ctrl-y
			x = 0;
			wmove(p, y, x);
			wdeleteln(p);
			if (maxrows > 1) maxrows--;
			break;
		case KEY_F(10):
			exitflag = true;
			break;
		case KEY_RESIZE:
			clear();

			wresize(status, 1, COLS);
			draw_statbar(status, "%s", fname);
			wresize(menu, 1, COLS);
			mvwin(menu, LINES-1, 0);
			draw_menubar(menu, COLS);

			refresh();
			wrefresh(status);
			wrefresh(menu);

			width = COLS - 1;
			height = LINES - 2;
			prefresh(p, start, 0, 1, 0, height, width);
			break;
		case KEY_HOME:
			x = 0;
			break;
		case KEY_END:
			x = mvwtoeol(p, y, maxcols);
			break;
		case KEY_PPAGE:
		case KEY_A3:
			i = start;
			if (start > height) {
				start -= height;
				y -= height;
			} else {
				start = 0;
				y -= i;
			}
			break;
		case KEY_NPAGE:
		case KEY_C3: // numpad pgdn
			i = start;
			if (maxrows - start > height) {
				start += height;
				y += height;
			} else {
				start = maxrows - height;
				y += (start - i);
			}
			break;
		case KEY_LEFT:
			if (x > 0) x--;
			else if (y > 0) {
				y--;
				x = mvwtoeol(p, y, maxcols);
			}
			break;
		case KEY_RIGHT:
			if (x == mvwtoeol(p, y, maxcols)) {
				if (y < maxrows - 1) {
					x = 0, y++;
					if ((y - start) == height) start++;
				}
			} else x++;
			break;
		case KEY_UP:
			if (y > start) y--;
			else if (start > 0) y = 0, start--;
			x = mvwtoeol(p, y, x);
			break;
		case KEY_DOWN:
			if (y < maxrows - 1) {
				y++;
				if ((y - start) == height) start++;
			}
			x = mvwtoeol(p, y, x);
			break;
		case KEY_DC: // delete
			if (0 == mvwtoeol(p, y, maxcols) &&
				maxrows > 1) {
				wdeleteln(p);
				maxrows--;
			} else mvwdelch(p, y, x);
			break;
		case 127: // backspace
		case KEY_BACKSPACE:
			if (x > 0) {
				x--;
				wmove(p, y, x);
				wdelch(p);
			} else if (y > 0) {
				winstr(p, buffer);
				wclrtoeol(p);	// wdeleteln is not work here
				y--, maxrows--;
				x = mvwtoeol(p, y, maxcols);
				mvwaddstr(p, y, x, buffer);
			}
			break;
		case 10: // return
		case KEY_ENTER:
			winstr(p, buffer);
			wclrtoeol(p);
			maxrows++;
			wresize(p, maxrows + 1, maxcols);
			x = 0, y++;
			wmove(p, y, x);
			winsertln(p);
			mvwaddstr(p, y, x, buffer);
			if (y >= height) start++;
			break;
		case 27: // esc
			if ((key = getch()) == 27)
				exitflag = true;
			break;
		default:
			if (x == maxcols - 1) {
				if (y == maxrows - 1) {
					maxrows++;
					wresize(p, maxrows + 1, maxcols);
					winsertln(p);
				}
				x = 0, y++;
				if (y >= height) start++;
			}
			mvwaddch(p, y, x, key);
			x++;
			break;
		}
	} while (!exitflag);

	ncfinish();

	return 0;
}

void ncstart() {
	initscr();
	keypad(stdscr, true);
	noecho();
	cbreak();

	if (has_colors()) {
		start_color();

		init_pair(1, COLOR_WHITE, COLOR_BLUE);	// edit screen
		init_pair(2, COLOR_BLACK, COLOR_CYAN);	// menus
		init_pair(3, COLOR_CYAN, COLOR_BLACK);	// menu label
		init_pair(4, COLOR_WHITE, COLOR_RED);	// errors
		init_pair(5, COLOR_BLACK, COLOR_CYAN);	// other windows
	}

	wbkgd(stdscr, COLOR_PAIR(1));
	refresh();
	timeout(0);
	curs_set(1);
}

void ncfinish() {
	endwin();
	exit(0);
}

void draw_menubar(WINDOW *menu, int size) {
	const char* buttons[MENU_ITEMS] = { "", "Save", "", "", "", "", "", "Delete", "", "Exit" };
	int bs = size / MENU_ITEMS;
	int br = size % MENU_ITEMS;
	int i, pos;

	wbkgd(menu, COLOR_PAIR(2));
	wclear(menu);

	for (pos = bs, i = 0; i < MENU_ITEMS; i++, pos += bs) {
		wattron(menu, COLOR_PAIR(3));
		wprintw(menu, "%2d", i+1);
		wattroff(menu, COLOR_PAIR(3));
		waddstr(menu, buttons[i]);
		if (br) { pos++; br--; }
		wmove(menu, 0, pos);	
	}
}

void draw_statbar(WINDOW *status, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	wclear(status);
	vwprintw(status, fmt, args);
	va_end(args);
}

int mvwtoeol(WINDOW *p, int y, int x) {
	char c;

	for (--x; x >= 0; x--) {
		c = mvwinch(p, y, x) & A_CHARTEXT;
		if (!isspace(c)) break;
	}

	return ++x;
}

char *wstr(char *str, int size) {
	char *e = str + size - 1;
	int i, s = 0;

	// replace leading spaces with tabs
	for (s = 0; isspace((unsigned char) *str); str++) s++;
	for (i = s % 8; i > 0; i--) *(--str) = ' '; 
	for (i = s / 8; i > 0; i--) *(--str) = '\t';

	// trim trailing space
	if (*str == '\0') return str;
	while (e > str && isspace((unsigned char) *e)) e--;

	*(++e) = '\n';
	*(++e) = '\0';

	return str;
}
