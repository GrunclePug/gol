#ifndef DRW_H
#define DRW_H

#include "util.h"

/* --- ncurses Setup / Teardown --- */
void drw_init(void);
void drw_shutdown(void);

/* --- Drawing Functions --- */
void drw_world(World_t *world);
void drw_status(World_t *world);

#endif
