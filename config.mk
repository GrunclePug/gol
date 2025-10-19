# --- Version ---
VERSION = 0.1

# --- Paths ---
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man
# OpenBSD (uncomment)
#MANPREFIX = ${PREFIX}/man

# --- Includes / Libs ---
LIBS = -lncurses

# --- Flags ---
CPPFLAGS = -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=700L -DVERSION=\"${VERSION}\"
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Os ${CPPFLAGS}
LDFLAGS  = ${LIBS}

# --- Compiler / Linker ---
CC = cc
