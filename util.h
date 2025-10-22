#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

/* --- Generic Macros --- */
#define MIN(A, B)   ((A) < (B) ? (A) : (B))
#define MAX(A, B)   ((A) > (B) ? (A) : (B))

/* --- Data Structs --- */
/*
 * Rule_t defines the birth and survival rules for cells in the simulation.
 */
typedef struct {
    const char *rule_name;
    const int *birth_rule;
    const int *survival_rule;
} Rule_t;

/*
 * World_t holds the entire state of the Game of Life simulation.
 * It uses two grids (current and next) to calculate the state updates without
 * interfering with the current generation.
 */
typedef struct {
    int rows;
    int cols;
    int **current_grid; // The grid representing the current state
    int **next_grid; // The temporary grid for calculating the next state
    bool is_running;
    long generation;
    long alive_cell_count;
    long min_threshold_count; // The actual cell count for MIN_THRESHOLD (%)
    long max_threshold_count; // The actual cell count for MAX_THRESHOLD (%)
    const Rule_t *current_rule;
} World_t;

/*
 * Pattern_t defines a specific arrangement of alive cells.
 * Used internally by util.c for random seeding logic.
 */
typedef struct {
    int rows;
    int cols;
    // The pattern is stored as a flat array. 1 = alive, 0 = dead. must be ordered row-by-row.
    const int *cells;
} Pattern_t;

/* --- Core Logic Prototypes --- */
World_t* world_init(int rows, int cols, int density);
void world_free(World_t *world);
void world_resize(World_t *world, int new_rows, int new_cols);
void world_seed_random(World_t *world, int density);
void world_update(World_t *world);

#endif
