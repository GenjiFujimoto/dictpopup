include config.mk

SRC = main.c popup.c util.c anki.c xlib.c deinflector.c
OBJ = $(SRC:.c=.o)

all: options dictpopup

options:
	@echo popup build options:
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo "CC       = $(CC)"

.c.o:
	$(CC) -c $(CFLAGS) $<

$(OBJ): config.h config.mk

config.h:
	cp config.def.h $@

dictpopup: ${OBJ}
	$(CC) -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f dictpopup ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f dictpopup ${DESTDIR}${PREFIX}/bin
	mkdir -p /usr/share/dictpopup
	cp -f textview.css /usr/share/dictpopup

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dictpopup
	rm -f /usr/share/dictpopup/textview.css

.PHONY: all options clean install uninstall
