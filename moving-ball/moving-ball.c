#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define DELAY 30000
#define LEFT_BOUNDARY 1
#define TOP_BOUNDARY  0
#define COLOR_PAIR_DEFAULT 1
#define COLOR_PAIR_TOP_LEFT 2
#define COLOR_PAIR_TOP_RIGHT 3
#define COLOR_PAIR_BOTTOM_LEFT 4
#define COLOR_PAIR_BOTTOM_RIGHT 5

int main() {

	WINDOW *mainwin, *ballwin;
	int ch;

	int x = 1, y = 1;
	int w_width = 0, w_height = 0;
	int next_x = 0, next_y = 0;
	int direction = 0; // 0, 1, 2, 3

	if ( (mainwin = initscr()) == NULL ) {
		fprintf(stderr, "Error initialising ncurses\n");
		exit(EXIT_FAILURE);
	}

	if (has_colors() == FALSE) {
		fprintf(stderr, "Error initialising ncurses\n");
		exit(EXIT_FAILURE);
	}

	noecho();
	curs_set(FALSE);
	keypad(mainwin, TRUE);
	nodelay(mainwin, TRUE);
	start_color();
	init_pair(COLOR_PAIR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLOR_PAIR_TOP_LEFT, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLOR_PAIR_TOP_RIGHT, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(COLOR_PAIR_BOTTOM_LEFT, COLOR_CYAN, COLOR_BLACK);
	init_pair(COLOR_PAIR_BOTTOM_RIGHT, COLOR_YELLOW, COLOR_BLACK);

	refresh();

	getmaxyx(mainwin, w_height, w_width);
	ballwin = subwin(mainwin, w_height /2 + 1, w_width/2 + 1, 0, 0);
	box(ballwin, 0, 0);
	refresh();
	wrefresh(ballwin);

	int paused = 0;

	while(1) {
		ch = getch();
		if (ch == 'q') {
			break;
		}

		switch ( ch ) {
			case 'z':
				x = LEFT_BOUNDARY;
				y = TOP_BOUNDARY + 1;
				break;
			case 't':
				y = TOP_BOUNDARY + 1;
				break;
			case 'b':
				y = w_height / 2 - 1;
				break;
			case 's':
				x = LEFT_BOUNDARY;
				break;
			case 'e':
				x = w_width / 2 - 1;
				break;
			case KEY_RIGHT:
			case 'l':
				direction = 0;
				break;
			case KEY_LEFT:
			case 'h':
				direction = 1;
				break;
			case KEY_DOWN:
			case 'j':
				direction = 2;
				break;
			case KEY_UP:
			case 'k':
				direction = 3;
				break;
		}
		if (ch == 'p') {
			paused = (paused == 0) ? 1 : 0;
		}

		clear();
		if (x < w_width / 4 && y < w_height / 4) {
			attron(COLOR_PAIR(COLOR_PAIR_TOP_LEFT));
		} else if (x >= w_width / 4 && y < w_height / 4) {
			attron(COLOR_PAIR(COLOR_PAIR_TOP_RIGHT));
		} else if (x < w_width / 4 && y >= w_height / 4) {
			attron(COLOR_PAIR(COLOR_PAIR_BOTTOM_LEFT));
		} else if (x >= w_width / 4 && y >= w_height / 4) {
			attron(COLOR_PAIR(COLOR_PAIR_BOTTOM_RIGHT));
		} else {
			attron(COLOR_PAIR(COLOR_PAIR_DEFAULT));
		}
		mvaddch(y, x, ' '|A_REVERSE);
		if (x < w_width / 4 && y < w_height / 4) {
			attroff(COLOR_PAIR(COLOR_PAIR_TOP_LEFT));
		} else if (x >= w_width / 4 && y < w_height / 4) {
			attroff(COLOR_PAIR(COLOR_PAIR_TOP_RIGHT));
		} else if (x < w_width / 4 && y >= w_height / 4) {
			attroff(COLOR_PAIR(COLOR_PAIR_BOTTOM_LEFT));
		} else if (x >= w_width / 4 && y >= w_height / 4) {
			attroff(COLOR_PAIR(COLOR_PAIR_BOTTOM_RIGHT));
		} else {
			attroff(COLOR_PAIR(COLOR_PAIR_DEFAULT));
		}
		box(ballwin, 0, 0);
		wrefresh(ballwin);
		refresh();
		if (direction == 0 || direction == 1) {
			usleep(DELAY);
		} else if (direction == 2 || direction == 3) {
			usleep(DELAY * 2);
		}

		if (paused == 1) {
			continue;
		}

		switch (direction) {
			case 0:
				next_x = x + 1;

				if (next_x >= (w_width / 2)) {
					direction = 1;
				} else {
					x = next_x;
				}
				break;
			case 1:
				next_x = x - 1;

				if (next_x < LEFT_BOUNDARY) {
					direction = 0;
				} else {
					x = next_x;
				}
				break;
			case 2:
				next_y = y + 1;

				if (next_y >= (w_height / 2)) {
					direction = 3;
				} else {
					y = next_y;
				}
				break;
			case 3:
				next_y = y - 1;

				if (next_y <= TOP_BOUNDARY) {
					direction = 2;
				} else {
					y = next_y;
				}
				break;
		}
	}

	delwin(ballwin);
	delwin(mainwin);
	endwin();
	refresh();
}
