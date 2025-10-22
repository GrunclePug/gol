#ifndef CONFIG_H
#define CONFIG_H

#include "util.h"

// --- CELL APPEARANCE ---
static const char CELL_ALIVE = '#';
static const char CELL_DEAD  = ' ';

// --- DIMENSIONS & SPEED ---
static const int MIN_COLS         = 10;
static const int MIN_ROWS         = 10;
static const int DEFAULT_DENSITY  = 2;
static const int DEFAULT_SPEED_MS = 50;

// --- DYNAMIC RULE THRESHOLDS ---
static const int MIN_THRESHOLD = 5; // %
static const int MAX_THRESHOLD = 34; // %

// --- KEYBINDS ---
#define KEY_QUIT 'q'
#define KEY_PAUSE ' '
#define KEY_STEP 's'

// --- RULES ---
static const int RULE_GROWTH_B[] = {0, 0, 0, 1, 0, 0, 1, 0, 0}; // Default: B36
static const int RULE_GROWTH_S[] = {0, 0, 1, 1, 0, 0, 1, 0, 0}; // Default: S236

static const int RULE_DECAY_B[]  = {0, 0, 0, 1, 0, 0, 0, 0, 0}; // Default: B3
static const int RULE_DECAY_S[]  = {0, 0, 1, 1, 0, 0, 0, 0, 0}; // Default: S23

static const Rule_t GROWTH_RULE = {
    .rule_name = "Grow (B36/S236)",
    .birth_rule = RULE_GROWTH_B,
    .survival_rule = RULE_GROWTH_S
};

static const Rule_t DECAY_RULE = {
    .rule_name = "Decay (B3/S23)",
    .birth_rule = RULE_DECAY_B,
    .survival_rule = RULE_DECAY_S
};

#endif
