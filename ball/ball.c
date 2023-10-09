#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define DELAY 50000
#define LEFT_BOUNDARY 1
#define TOP_BOUNDARY  0

int main() {

	WINDOW *mainwin, *ballwin;
	int ch;

	int x = 1, y = 1;
	int w_width = 0, w_height = 0;
	int next_x = 0, next_y = 0;
	int direction_x = 1, direction_y = 1;
	int direction = 1;

	if ( (mainwin = initscr()) == NULL ) {
		fprintf(stderr, "Error initialising ncurses\n");
		exit(EXIT_FAILURE);
	}
	noecho();
	curs_set(FALSE);
	keypad(mainwin, TRUE);
	nodelay(mainwin, TRUE);
	refresh();

	getmaxyx(mainwin, w_height, w_width);
	ballwin = subwin(mainwin, w_height /2 + 1, w_width/2 + 1, 0, 0);
	box(ballwin, 0, 0);
	refresh();
	wrefresh(ballwin);

	int paused = 0;

	int count_unpaused = 0;
	int count_paused = 0;
	while(1) {
		ch = getch();
		if (ch == 'q') {
			break;
		}

		// controlling ball when it's stopped
		if (paused == 1) {
			// TODO t for top of line, b for bottom
			// s for start of line, e for end
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
				case 'd':
					direction = (direction == 0) ? 1 : 0;
					break;
				case 'x':
					direction_x *= -1;
					break;
				case 'y':
					direction_y *= -1;
					break;
				case KEY_RIGHT:
					if (x + 1 < w_width / 2) {
						x += 1;
					}
					break;
				case KEY_LEFT:
					if (x - 1 >= LEFT_BOUNDARY) {
						x -= 1;
					}
					break;
				case KEY_DOWN:
					if (y + 1 < w_height / 2) {
						y += 1;
					}
					break;
				case KEY_UP:
					if (y - 1 > TOP_BOUNDARY) {
						y -= 1;
					}
					break;
			}
		}
		if (ch == 'p') {
			paused = (paused == 0) ? 1 : 0;
		}

		clear();
		mvprintw(y, x, "o");
		box(ballwin, 0, 0);
		wrefresh(ballwin);
		refresh();
		usleep(DELAY);

		if (paused == 1) {
			count_paused++;
			continue;
		}
		count_unpaused++;

		if (direction == 0) {
			next_y = y + direction_y;

			if (next_y >= (w_height / 2) || next_y < (TOP_BOUNDARY + 1)) {
				direction_y *= -1;
			} else {
				y += direction_y;
			}
		} else if (direction == 1) {
			next_x = x + direction_x;

			if (next_x >= (w_width / 2) || next_x < LEFT_BOUNDARY) {
				direction_x *= -1;
			} else {
				x += direction_x;
			}
		}
	}

	delwin(ballwin);
	delwin(mainwin);
	endwin();
	refresh();
	printf("count of unpaused iterations = %d\n", count_unpaused);
	printf("count of paused iterations = %d\n", count_paused);
}
