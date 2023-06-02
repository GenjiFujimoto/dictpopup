include config.mk

SRC = popup.c format_output.c
OBJ = $(SRC:.c=.o)

all: options dictpopup

options:
	@echo popup build options:
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "CC       = $(CC)"

config.h:
	cp config.def.h config.h

.c.o:
	$(CC) -c $(CFLAGS) `pkg-config --cflags gtk+-3.0` $<

popup.o: config.h

$(OBJ): config.h config.mk

dictpopup: $(OBJ)
	$(CC) -o format_output format_output.o 
	$(CC) -o popup `pkg-config --libs gtk+-3.0` popup.o 

clean:
	rm -f popup $(OBJ) format_output

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f popup ${DESTDIR}${PREFIX}/bin
	cp -f dictpopup ${DESTDIR}${PREFIX}/bin
	cp -f format_output ${DESTDIR}${PREFIX}/bin

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/popup
	rm -f ${DESTDIR}${PREFIX}/bin/dictpopup
	rm -f ${DESTDIR}${PREFIX}/bin/format_output


.PHONY: all options clean install uninstall
