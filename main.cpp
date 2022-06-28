#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char **argv) {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_RED);

	for (int y = 0; y < LINES; y++)
		for (int x = 0; x < COLS; x++)
			mvaddch(y, x, '+');
	move(10, 10);
	attron(COLOR_PAIR(1));
	addch('#');
	attroff(COLOR_PAIR(1));
	refresh();

	for (int ch = getch(); ch != 'q'; ch = getch()) {
		move(getcury(stdscr), getcurx(stdscr) - 1);
		addch('+');
		switch (ch) {
			case KEY_LEFT:
			case '4':
				move(getcury(stdscr), getcurx(stdscr) - 2);
				break;
			case KEY_RIGHT:
			case '6':
				break;
			case KEY_UP:
			case '8':
				move(getcury(stdscr) - 1, getcurx(stdscr) - 1);
				break;
			case KEY_DOWN:
			case '2':
				move(getcury(stdscr) + 1, getcurx(stdscr) - 1);
				break;
			case KEY_A1:
//			case '7':
				move(getcury(stdscr) - 1, getcurx(stdscr) - 2);
				break;
			case KEY_A3:
//			case '9':
				move(getcury(stdscr) - 1, getcurx(stdscr));
				break;
			case KEY_C1:
//			case '1':
				move(getcury(stdscr) + 1, getcurx(stdscr) - 2);
				break;
			case KEY_C3:
//			case '3':
				move(getcury(stdscr) + 1, getcurx(stdscr));
				break;
			default:
				continue;
		}
		attron(COLOR_PAIR(1));
		addch('#');
		attroff(COLOR_PAIR(1));
//		move(getcury(stdscr), getcurx(stdscr) - 1);
	}

	endwin();
}
