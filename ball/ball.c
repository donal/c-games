#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define DELAY 50000

int main() {

	WINDOW *mainwin;
	int ch;

	int x = 1, y = 1;
	int w_width = 0, w_height = 0;
	int next_x = 0;
	// int next_y = 0;
	int direction_x = 1;

	if ( (mainwin = initscr()) == NULL ) {
		fprintf(stderr, "Error initialising ncurses\n");
		exit(EXIT_FAILURE);
	}
	noecho();
	curs_set(FALSE);
	keypad(mainwin, TRUE);
	nodelay(mainwin, TRUE);
	box(mainwin, 0, 0);
	refresh();

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
			switch ( ch ) {
				case 'z':
					x = 1;
					break;
				case 'd':
					direction_x *= -1;
					break;
				case KEY_RIGHT:
					if (x + 1 < w_width / 2) {
						x += 1;
					}
					break;
				case KEY_LEFT:
					if (x - 1 > 1) {
						x -= 1;
					}
					break;
				case KEY_DOWN:
					if (y + 1 < w_height / 2) {
						y += 1;
					}
					break;
				case KEY_UP:
					if (y - 1 > 0) {
						y -= 1;
					}
					break;
			}
		}
		if (ch == 'p') {
			paused = (paused == 0) ? 1 : 0;
		}

		getmaxyx(mainwin, w_height, w_width);
		clear();
		mvprintw(y, x, "o");
		box(mainwin, 0, 0);
		refresh();
		usleep(DELAY);

		if (paused == 1) {
			count_paused++;
			continue;
		}
		count_unpaused++;

		next_x = x + direction_x;

		if (next_x >= (w_width / 2) || next_x < 1) {
			direction_x *= -1;
		} else {
			x += direction_x;
		}
	}

	endwin();
	printf("count of unpaused iterations = %d\n", count_unpaused);
	printf("count of paused iterations = %d\n", count_paused);
}
