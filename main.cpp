#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include "cpps.h"
#include "retf.h"

#define PLAYER_PAIR 1
#define PLAYER_G_PAIR 2
#define SPC_PAIR 3
#define BORDER_PAIR 4
#define WIN_PAIR 5
#define LOSE_PAIR 6

#define NSPC_0 3
#define NSPC_S 10
#define NSPC_U 20
#define NSPC_W 1000
#define NSPC_ZER 40000
#define NSPC NSPC_W

int main(int argc, char * const *argv) {
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	init_pair(PLAYER_PAIR, COLOR_WHITE, COLOR_RED);
	init_pair(PLAYER_G_PAIR, COLOR_WHITE, COLOR_GREEN);
	init_pair(SPC_PAIR, COLOR_CYAN, COLOR_YELLOW);
	init_pair(BORDER_PAIR, COLOR_BLACK, COLOR_CYAN);
	init_pair(WIN_PAIR, COLOR_WHITE, COLOR_GREEN);
	init_pair(LOSE_PAIR, COLOR_WHITE, COLOR_RED);
	int cury, curx, gp, isns;

	int vtime = 45;
	int opt;
	struct option long_options[] {
		{"help",0,NULL,'h'},
		{"timeout",1,NULL,'t'},
		{"no-timeout",0,NULL,'n'},
		{"disabled-timeout",0,NULL,'n'},
		{NULL,0,NULL,0}
	};
	while ((opt = getopt_long(argc, argv, "ht:n", long_options, NULL)) != -1) {
		const char *arg = optarg;
		switch (opt) {
			case 'h':
				endwin();
				printf("Usage: %s [options]\n\n", argv[0]);
				printf("Available options:\n");
				printf("\t--help, -h                           - display this help and exit\n");
				printf("\t--timeout, -t timeout                - player loses if timeout is up, 0 for no timeout.\n");
				printf("\t--no-timeout, --disabled-timeout, -n - same as --timeout=0\n");
				exit(0);
				break;
			case 'n':
				arg = "0";
			case 't':
				vtime = atoi(arg);
				break;
		}
	}

	srand((argc - optind) ? (!strcmp(argv[optind], "x") ? time(NULL) : atoi(argv[optind])) : 0);

	gp = 0;
	for (int y = 1; y < LINES - 1; y++)
		for (int x = 1; x < COLS - 1; x++) {
			isns = rand() % NSPC;
			if (!isns) {
				gp++;
				attron(COLOR_PAIR(SPC_PAIR));
			}
			mvaddch(y, x, isns ? '+' : '*');
			attroff(COLOR_PAIR(SPC_PAIR));
		}
	attron(COLOR_PAIR(BORDER_PAIR));
	border(0, 0, 0, 0, 0, 0, 0, 0);
	attroff(COLOR_PAIR(BORDER_PAIR));
	move(10, 10);
	attron(COLOR_PAIR(1));
	addch('#');
	attroff(COLOR_PAIR(1));
	refresh();

	time_t starttime;
	(vtime && time(&starttime));
	char timend = 0;

	for (int ch = getch(); ch != 'q'; ch = getch()) {
		move(getcury(stdscr), getcurx(stdscr) - 1);
		addch('+');
		getyx(stdscr, cury, curx);
		switch (ch) {
			case KEY_LEFT:
			case '4':
				move(cury, curx - ((curx > 2) ? 2 : 1));
				break;
			case KEY_RIGHT:
			case '6':
				if (curx >= (COLS - 1))
					move(cury, curx - 1);
				break;
			case KEY_UP:
			case '8':
				move(cury - ((cury > 1) ? 1 : 0), curx - 1);
				break;
			case KEY_DOWN:
			case '2':
				move(cury + ((cury < (LINES - 2)) ? 1 : 0), curx - 1);
				break;
			default:
				continue;
		}
		if (vtime) {
			if (time(NULL) - starttime > vtime) {
				timend = 1;
				break;
			}
			getyx(stdscr, cury, curx);
			mvaddstr(0, 0, to_chptr_as(vtime - (time(NULL) - starttime), "  "));
			move(cury, curx);
		}
		if ((inch() & A_CHARTEXT) == '*') {
			gp--;
			attron(COLOR_PAIR(PLAYER_G_PAIR));
		}
		else
			attron(COLOR_PAIR(PLAYER_PAIR));
		addch('#');
		attroff(COLOR_PAIR(PLAYER_PAIR));
		attroff(COLOR_PAIR(PLAYER_G_PAIR));
		if (gp <= 0)
			break;
//		move(getcury(stdscr), getcurx(stdscr) - 1);
	}

	if (!gp || timend) {
		int nlines = 12;
		int ncols = 40;
		int win_lines = (LINES / 2) - (nlines / 2);
		int win_cols = (COLS / 2) - (ncols / 2);
		WINDOW *win = newwin(nlines, ncols, win_lines, win_cols);
		wattron(win, COLOR_PAIR(BORDER_PAIR));
		wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
		wattroff(win, COLOR_PAIR(BORDER_PAIR));
		const char *wintext = gp ? "You Lose!" : "You Win!";
//		const char *sltext = (retfc<const char *, pair<bool, const char *>>("", MKRCP(timend, "Time out.")));
		const char *sltext = NULL;
		STS(sltext, timend, "Time out.");
		const int winlen = strlen(wintext);
//		const int sllen = strlen(sltext);
		wattron(win, COLOR_PAIR(timend ? LOSE_PAIR : WIN_PAIR));
		mvwaddstr(win, (nlines / 2) - 1, (ncols / 2) - (winlen / 2), wintext);
		if (sltext)
			mvwaddstr(win, (nlines / 2), (ncols / 2) - (winlen / 2), sltext);
		wattroff(win, COLOR_PAIR(WIN_PAIR));
		wattroff(win, COLOR_PAIR(LOSE_PAIR));
		wrefresh(win);
		while (1) {
			switch (getch()) {
				case KEY_LEFT:
				case KEY_RIGHT:
				case KEY_UP:
				case KEY_DOWN:
					continue;
			}
			break;
		}
	}

	endwin();
	return 0;
}
