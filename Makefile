

INCLUDE = -Isrc
LDFLAGS = -lcurses

Runaway: $(wildcard src/*)
	g++ src/main.cpp ${INCLUDE} ${LDFLAGS} -o Runaway -g