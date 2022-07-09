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
#define TIME_PAIR 7
#define TIME_LOW_PAIR 8
#define ANTI_TOUCHER_PAIR 9

#define NSPC_0 3
#define NSPC_S 10
#define NSPC_U 20
#define NSPC_W 1000
#define NSPC_ZER 40000
#define NSPC NSPC_W

#ifndef DEFAULT_ALERT
#define DEFAULT_ALERT beep
#endif

int main(int argc, char * const *argv) {
	initscr();
	cbreak();
	noecho();
	timeout(250);
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	init_pair(PLAYER_PAIR, COLOR_WHITE, COLOR_RED);
	init_pair(PLAYER_G_PAIR, COLOR_WHITE, COLOR_GREEN);
	init_pair(SPC_PAIR, COLOR_CYAN, COLOR_YELLOW);
	init_pair(BORDER_PAIR, COLOR_BLACK, COLOR_CYAN);
	init_pair(WIN_PAIR, COLOR_WHITE, COLOR_GREEN);
	init_pair(LOSE_PAIR, COLOR_WHITE, COLOR_RED);
	init_pair(TIME_PAIR, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(TIME_LOW_PAIR, COLOR_WHITE, COLOR_RED);
	init_pair(ANTI_TOUCHER_PAIR, COLOR_BLUE, COLOR_WHITE);
	int cury, curx, gp, isns, isns_extra, incht, lose_attc, timend, attc;
	isns_extra = attc = lose_attc = timend = 0;

	time_t vtime = 45;
	int opt;
	struct option long_options[] {
		{"help",0,NULL,'h'},
		{"timeout",1,NULL,'t'},
		{"no-timeout",0,NULL,'n'},
		{"disabled-timeout",0,NULL,'n'},
		{"anti-touchers",0,NULL,'a'},
		{"advanced",1,NULL,'d'},
		{NULL,0,NULL,0}
	};
	chtype bv[8] = {0,0,0,0,0,0,0,0};
	while ((opt = getopt_long(argc, argv, "ht:nad:", long_options, NULL)) != -1) {
		const char *arg = optarg;
		switch (opt) {
			case 'h':
				endwin();
				printf("Usage: %s [options] [map]\n\n", argv[0]);
				printf("The default map is 0.\n\n");
				printf("Available options:\n");
				printf("\t-h, --help                           - display this help and exit.\n");
				printf("\t-t, --timeout=timeout                - player loses if timeout is up, 0 for no timeout.\n");
				printf("\t-n, --no-timeout, --disabled-timeout - same as --timeout=0.\n");
				printf("\t-a, --anti-touchers                  - play with anti-touchers.\n");
				printf("\t-d, --advanced=advanced-option       - parse advanced-option as Movement-ZD advanced option.\n");
				exit(0);
				break;
			case 'n':
				arg = "0";
			case 't':
				vtime = atoi(arg);
				break;
			case 'a':
				arg = "a1";
			case 'd':
				char aopt = *arg;
				arg++;
				if (aopt == 'a')
					attc = atoi(arg);
				else if (aopt == 'm')
					for (int i = 0; i < 8; i++)
						bv[i] = arg[i];
				else if (aopt == 'x')
					isns_extra = atoi(arg);
				else {
					endwin();
					fprintf(stderr, "%s: unrecognized anvanced option: /%c/%s/\n", argv[0], aopt, arg);
					exit(1);
				}
				break;
		}
	}

	srand((argc > optind) ? (!strcmp(argv[optind], "x") ? time(NULL) : atoi(argv[optind])) : 0);

	gp = 0;
	for (int y = 1; y < LINES - 1; y++)
		for (int x = 1; x < COLS - 1; x++) {
			isns = rand() % NSPC;
			if (isns <= isns_extra) {
				gp++;
				attron(COLOR_PAIR(SPC_PAIR));
			}
			else if (attc && isns <= attc)
				attron(COLOR_PAIR(ANTI_TOUCHER_PAIR));
			mvaddch(y, x, (isns > isns_extra) ? ((attc && isns <= attc) ? '^' : '+') : '*');
			attroff(COLOR_PAIR(SPC_PAIR));
		}
	attron(COLOR_PAIR(BORDER_PAIR));
//	border(0, 0, 0, 0, 0, 0, 0, 0);
	border(bv[0], bv[1], bv[2], bv[3], bv[4], bv[5], bv[6], bv[7]);
	attroff(COLOR_PAIR(BORDER_PAIR));
	move(10, 10);
	attron(COLOR_PAIR(1));
	addch('#');
	attroff(COLOR_PAIR(1));
	refresh();

	time_t starttime;
	time(&starttime);

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
			case 'q':
			default:
				DEFAULT_ALERT();
			case ERR:
				move(cury, curx - 1);
		}
		const time_t tns = time(NULL) - starttime;
		const time_t vts = vtime ? (vtime - tns) : tns;
		getyx(stdscr, cury, curx);
		if (vtime && tns > vtime) {
			timend = 1;
			break;
		}
		const char *retim = to_chptr(vts);
		const int retimln = strlen(retim);
		attron(COLOR_PAIR((!vtime || vts > 5) ? TIME_PAIR : TIME_LOW_PAIR));
		mvaddstr(0, 0, retim);
		attroff(COLOR_PAIR(TIME_PAIR));
		attroff(COLOR_PAIR(TIME_LOW_PAIR));
		attron(COLOR_PAIR(BORDER_PAIR));
		mvhline(0, retimln, ACS_HLINE, COLS - 1 - retimln);
		attroff(COLOR_PAIR(BORDER_PAIR));
		move(cury, curx);
		incht = inch() & A_CHARTEXT;
		if (incht == '*') {
			gp--;
			attron(COLOR_PAIR(PLAYER_G_PAIR));
		}
		else if (incht == '^') {
			lose_attc = 1;
			break;
		}
		else
			attron(COLOR_PAIR(PLAYER_PAIR));
		addch('#');
		attroff(COLOR_PAIR(PLAYER_PAIR));
		attroff(COLOR_PAIR(PLAYER_G_PAIR));
		refresh();
		if (gp <= 0)
			break;
//		move(getcury(stdscr), getcurx(stdscr) - 1);
	}

	if (!gp || timend || lose_attc) {
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
		const char *tltext = gp ? "Press q to quit" : NULL;
		STS(sltext, timend, "Time out.");
		TS(sltext, lose_attc, "Anti-toucher.");
		ETS(sltext, "Press q to quit");
		const int winlen = strlen(wintext);
		const int sllen = sltext ? strlen(sltext) : 0;
		const int tllen = tltext ? strlen(tltext) : 0;
		wattron(win, COLOR_PAIR(gp ? LOSE_PAIR : WIN_PAIR));
		mvwaddstr(win, (nlines / 2) - 1, (ncols / 2) - (winlen / 2), wintext);
		if (sltext)
			mvwaddstr(win, (nlines / 2), (ncols / 2) - (sllen / 2), sltext);
		if (tltext)
			mvwaddstr(win, (nlines / 2) + 1, (ncols / 2) - (tllen / 2), tltext);
		wattroff(win, COLOR_PAIR(WIN_PAIR));
		wattroff(win, COLOR_PAIR(LOSE_PAIR));
		wrefresh(win);
		while (getch() != 'q');
	}

	endwin();
	return 0;
}
