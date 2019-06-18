# see LICENSE file for copyright and license details.

include config.mk

BIN := xkey
HDR := config.h
SRC := xkey.c
OBJ := ${SRC:.c=.o}
MAN := xkey.1

all: ${BIN}

.c.o:
	${CC} -c ${CFLAGS} ${CPPFLAGS} $<

${OBJ}: ${HDR} config.mk

${BIN}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f ${BIN} ${OBJ} ${BIN}-${VERSION}.tar.gz

dist: clean
	mkdir -p ${BIN}-${VERSION}
	cp -R LICENSE makefile README ${HDR} config.mk ${MAN} ${SRC} ${BIN}-${VERSION}
	tar -cf ${BIN}-${VERSION}.tar ${BIN}-${VERSION}
	gzip ${BIN}-${VERSION}.tar
	rm -rf ${BIN}-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ${BIN} ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/${BIN}
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	cp -f ${MAN} ${DESTDIR}${MANPREFIX}/man1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/${MAN}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${BIN}
	rm -f ${DESTDIR}${MANPREFIX}/man1/${MAN}

.PHONY: all clean dist install uninstall

