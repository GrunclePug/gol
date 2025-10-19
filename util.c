#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "util.h"
#include "config.h"

/* --- GoL Pattern Definitions --- */

// Classic Glider (3x3)
const int pattern_glider[] = {
    0, 1, 0,
    0, 0, 1,
    1, 1, 1
};

// Small Exploder (4x4)
const int pattern_exploder[] = {
    0, 1, 0, 0,
    1, 1, 1, 1,
    1, 0, 1, 1,
    0, 1, 0, 0
};

// 10-Cell Row (1x10) - An oscillator that breaks apart
const int pattern_ten_cell_row[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

// Blinker (3x1) - Simplest oscillator
const int pattern_blinker[] = {
    1, 1, 1
};

// R-pentomino (3x3) - A common chaos generator
const int pattern_r_pentomino[] = {
    0, 1, 1,
    1, 1, 0,
    0, 1, 0
};

// Array of all available patterns
const Pattern_t PATTERNS[] = {
    { 3, 3, pattern_glider },
    { 4, 4, pattern_exploder },
    { 1, 10, pattern_ten_cell_row },
    { 3, 1, pattern_blinker },
    { 3, 3, pattern_r_pentomino }
};

#define NUM_PATTERNS (sizeof(PATTERNS) / sizeof(PATTERNS[0]))

/* Helper function to safely apply a pattern to the grid at a given coordinate (r, c). */
static void apply_pattern(World_t *world, int r_start, int c_start, const Pattern_t *pat) {
    // Check if the pattern will fit on the grid boundary
    if (r_start < 0 || c_start < 0 || r_start + pat->rows > world->rows || c_start + pat->cols > world->cols) {
        return; // Pattern too large or out of bounds for the current grid
    }

    // Apply pattern cells
    for (int r = 0; r < pat->rows; r++) {
        for (int c = 0; c < pat->cols; c++) {
            int flat_index = r * pat->cols + c;
            if (pat->cells[flat_index] == 1) {
                // Set alive cells to 1; doesn't clear existing cells, allowing patterns to overlap and interact immediately.
                world->current_grid[r_start + r][c_start + c] = 1;
            }
        }
    }
}

/* Helper function to safely allocate memory and check for failure. */
static void* ecalloc(size_t nmemb, size_t size) {
    void *p = calloc(nmemb, size);
    if (!p) {
        fprintf(stderr, "Fatal: cannot allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

/* Helper function to count the number of alive neighbors for a cell. */
static int count_neighbors(World_t *world, int r, int c) {
    int neighbors = 0;

    // Iterate over the 3x3 grid centered at (r, c)
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // Skip the center cell itself

            // Calculate toroidal (wrapping) coordinates
            int neighbor_r = (r + i + world->rows) % world->rows;
            int neighbor_c = (c + j + world->cols) % world->cols;

            // Add the neighbor's state (1 for alive, 0 for dead)
            neighbors += world->current_grid[neighbor_r][neighbor_c];
        }
    }
    return neighbors;
}

/* Allocates and initializes the World_t structure.
 * The grid is seeded randomly based on the provided density (0-1000). */
World_t* world_init(int rows, int cols, int density) {
    static int seed_set = 0;
    World_t *world = ecalloc(1, sizeof(World_t));
    world->rows = rows;
    world->cols = cols;
    world->is_running = true;
    world->generation = 0;

    // Allocate memory for rows (pointers to columns)
    world->current_grid = ecalloc(rows, sizeof(int *));
    world->next_grid = ecalloc(rows, sizeof(int *));

    // Allocate memory for columns (the actual cells)
    for (int i = 0; i < rows; i++) {
        world->current_grid[i] = ecalloc(cols, sizeof(int));
        world->next_grid[i] = ecalloc(cols, sizeof(int));
    }

    if (seed_set == 0) {
            srand(time(NULL));
            seed_set = 1;
        }
        world_seed_random(world, density);

    return world;
}

/* Frees all memory allocated for the world. */
void world_free(World_t *world) {
    if (world) {
        for (int i = 0; i < world->rows; i++) {
            free(world->current_grid[i]);
            free(world->next_grid[i]);
        }
        free(world->current_grid);
        free(world->next_grid);
        free(world);
    }
}

/*
 * Resize world grid, preserving existing cells and zeroing out new space.
 */
void world_resize(World_t *world, int new_rows, int new_cols) {
    // Store old dimensions to manage the copy/free process
    int old_rows = world->rows;
    int old_cols = world->cols;

    // Allocate memory for new grids
    int **new_current = ecalloc(new_rows, sizeof(int *));
    int **new_next = ecalloc(new_rows, sizeof(int *));

    // Allocate memory for columns (the actual cells) and initialize to 0
    for (int i = 0; i < new_rows; i++) {
        new_current[i] = ecalloc(new_cols, sizeof(int));
        new_next[i] = ecalloc(new_cols, sizeof(int));
    }

    // Copy contents of old current grid to new current grid
    int copy_rows = MIN(old_rows, new_rows);
    int copy_cols = MIN(old_cols, new_cols);

    for (int r = 0; r < copy_rows; r++) {
        for (int c = 0; c < copy_cols; c++) {
            // Copy state from old grid
            new_current[r][c] = world->current_grid[r][c];
        }
    }

    // Free old grid data
    for (int i = 0; i < old_rows; i++) {
        free(world->current_grid[i]);
        free(world->next_grid[i]);
    }
    free(world->current_grid);
    free(world->next_grid);

    // Update World_t structure pointers and dimensions
    world->current_grid = new_current;
    world->next_grid = new_next;
    world->rows = new_rows;
    world->cols = new_cols;
}

/* Fills the world grid with random patterns based on the configured density. */
void world_seed_random(World_t *world, int density) {
    // Iterate over the grid, checking the density chance
    for (int r = 0; r < world->rows; r++) {
        for (int c = 0; c < world->cols; c++) {
            // Check against density (e.g., 35/1000 chance)
            if ((rand() % 1000) < density) {
                // Randomly select a pattern
                int pat_index = rand() % NUM_PATTERNS;
                const Pattern_t *pat = &PATTERNS[pat_index];

                // Attempt to place the pattern at the current cell (r, c)
                apply_pattern(world, r, c, pat);
            }
        }
    }
}

/* Calculate next generation based on GoL rules. */
void world_update(World_t *world) {
    if (!world) return;

    for (int r = 0; r < world->rows; r++) {
        for (int c = 0; c < world->cols; c++) {
            int neighbors = count_neighbors(world, r, c);
            int current_state = world->current_grid[r][c];
            int next_state = current_state; // Assume state remains the same

            // -- CONWAY'S RULES --
            if (current_state == 1) {
                // Rule 1: Underpopulation (Dies if < 2 neighbors)
                // Rule 2: Overpopulation (Dies if > 3 neighbors)
                if (neighbors < 2 || neighbors > 3) {
                    next_state = 0; // Dies
                }
            } else {
                // Rule 4: Reproduction (Becomes alive if exactly 3 neighbors)
                if (neighbors == 3) {
                    next_state = 1; // Becomes alive
                }
            }

            world->next_grid[r][c] = next_state;
        }
    }

    // Swap current and next grids to advance the generation
    int **temp = world->current_grid;
    world->current_grid = world->next_grid;
    world->next_grid = temp;

    world->generation++;
}
