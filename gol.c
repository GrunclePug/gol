#include <errno.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "config.h"
#include "drw.h"
#include "util.h"

int main(int argc, char *argv[]) {
    World_t *world = NULL;
    int ch; // Variable to hold input character
    int density = DEFAULT_DENSITY;

    // Process command-line arguments for density
    if (argc > 1) {
        char *endptr;
        long arg_density = strtol(argv[1], &endptr, 10);

        // Check for conversion errors or invalid input
        if (argv[1] == endptr || *endptr != '\0' || errno == ERANGE) {
            fprintf(stderr, "Warning: Invalid density argument '%s'. Using default (%d).\n", argv[1], DEFAULT_DENSITY);
        } else if (arg_density < 0 || arg_density > 1000) {
            fprintf(stderr, "Warning: Density must be between 0 and 1000. Using default (%d).\n", DEFAULT_DENSITY);
        } else {
            density = (int)arg_density;
        }
    }

    // Setup Ncurses Environment and Game World
    drw_init();

    // Check if terminal size is sufficient, otherwise use config defaults
    int rows = MAX(MIN_ROWS, LINES - 2); // Reserve 2 lines for status bar
    int cols = MAX(MIN_COLS, COLS);

    // Ensure minimum size
    if (rows < MIN_ROWS || cols < MIN_COLS) {
        drw_shutdown();
        fprintf(stderr, "Error: Terminal too small (%dx%d). Minimum size is %dx%d.\n", rows, cols, MIN_ROWS, MIN_COLS);
        return EXIT_FAILURE;
    }

    world = world_init(rows, cols, density);

    // Main Game Loop
    while (true) {
        // --- Input Handling (Non-blocking) ---
        ch = getch();

        switch (ch) {
            case KEY_QUIT:
                goto cleanup; // Use goto to jump to cleanup section
            case KEY_PAUSE:
                // Toggle the running state
                world->is_running = !world->is_running;
                break;
            case KEY_STEP:
                // If paused, advance one generation
                if (!world->is_running) {
                    world_update(world);
                }
                break;
            case KEY_RESIZE:
                // Handle terminal resize if detected by ncurses
                endwin();
                refresh();

                // Recalculate size based on new LINES/COLS
                int new_rows = MAX(MIN_ROWS, LINES - 2);
                int new_cols = MAX(MIN_COLS, COLS);

                // Only resize world if dimensions have actually changed
                if (new_rows != world->rows || new_cols != world->cols) {
                    if (new_rows < MIN_ROWS || new_cols < MIN_COLS) {
                        break; // If too small, skip the resize.
                    }
                    world_resize(world, new_rows, new_cols);
                }
                break;
        }

        // --- Simulation Update ---
        if (world->is_running) {
            world_update(world);
        }

        // --- Drawing ---
        erase(); // Clear the screen buffer before redrawing
        drw_world(world);
        drw_status(world);
        refresh();

        // --- Timing ---
        // Sleep for the defined interval (in microseconds)
        usleep(DEFAULT_SPEED_MS * 1000);
    }

cleanup:
    // Cleanup and Exit
    world_free(world);
    drw_shutdown();

    return EXIT_SUCCESS;
}
