#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "drw.h"

/* Initialize ncurses environment. */
void drw_init(void) {
    initscr(); // Initialize the terminal
    raw(); // Immediate character input (no need to press Enter)
    noecho(); // Do not display input characters
    curs_set(0); // Hide the cursor
    keypad(stdscr, TRUE); // Enable special keys (like arrow keys, although not used here)
    timeout(0); // Non-blocking input by default
}

/* Restore terminal to normal state. */
void drw_shutdown(void) {
    endwin();
}

/* Draw current state of world grid. */
void drw_world(World_t *world) {
    // Ensure the grid fits within the visible terminal area
    int draw_rows = MIN(world->rows, LINES - 2); // Reserve 2 lines for status/border
    int draw_cols = MIN(world->cols, COLS);

    // Draw cells
    for (int i = 0; i < draw_rows; i++) {
        for (int j = 0; j < draw_cols; j++) {
            mvaddch(i, j, world->current_grid[i][j] ? CELL_ALIVE : CELL_DEAD);
        }
    }
}

/* Draw status bar [ G: 12345 | STATE | QUIT: q, PAUSE: SPACE, STEP: s ]. */
void drw_status(World_t *world) {
    const char *state = world->is_running ? "RUNNING" : "PAUSED";
    char status_bar[COLS];

    // snprintf is used to safely format the string without buffer overflow
    int len = snprintf(status_bar, COLS, " G: %-10ld | %-8s | Q: %c, P: %c, S: %c ",
        world->generation, state, KEY_QUIT, KEY_PAUSE, KEY_STEP);

    // Ensure status bar is padded with spaces to clear the old content
    if (len < COLS) {
        memset(status_bar + len, ' ', COLS - len);
    }

    // Ensure status buffer is always null-terminated at the end of the buffer
    status_bar[COLS - 1] = '\0';

    // Draw the status bar on the second-to-last row (LINES - 1)
    mvprintw(LINES - 1, 0, "%s", status_bar);
}
