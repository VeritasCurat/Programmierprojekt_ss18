#!/usr/bin/make
.SUFFIXES:
SRC = adjazensliste.c Programmierprojekt.c
TAR = adjazensliste Programmierprojekt
PCK = src.zip

CFLAGS = -std=gnu11 -c -g -Os -Wall -Werror

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@

%: %.o
	$(CC) -o $@ $^

all: $(TAR)

run: $(TAR)
	./shell

pack:
	zip $(PCK) $(SRC) Makefile

clean:
	$(RM) $(RMFILES) $(TAR) $(PCK)
