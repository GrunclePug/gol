# gol - Game of Life TUI application
# See LICENSE file for copyright and license details.

include config.mk

# --- Project-Specific Variables ---
TARGET = gol
SRC = gol.c drw.c util.c
HDR = drw.h util.h
OBJ = ${SRC:.c=.o}

# --- Targets ---
all: ${TARGET}

.c.o:
	${CC} -c ${CPPFLAGS} ${CFLAGS} $<

${OBJ}: config.h config.mk

${TARGET}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

config.h:
	cp config.def.h $@

# --- Installation Targets ---
install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${TARGET} ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/${TARGET}
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < gol.1 > ${DESTDIR}${MANPREFIX}/man1/gol.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/gol.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${TARGET}\
		${DESTDIR}${MANPREFIX}/man1/gol.1

# --- Cleanup Targets ---
clean:
	rm -f ${TARGET} ${OBJ} ${TARGET}-${VERSION}.tar.gz

# Distribution target: creates a source code tarball
dist: clean
	mkdir -p ${TARGET}-${VERSION}
	cp -R LICENSE Makefile README.md config.def.h config.mk gol.1 ${SRC} ${HDR} ${TARGET}-${VERSION}
	tar -cf ${TARGET}-${VERSION}.tar ${TARGET}-${VERSION}
	gzip ${TARGET}-${VERSION}.tar
	rm -rf ${TARGET}-${VERSION}

.PHONY: all clean dist install uninstall
