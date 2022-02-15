

CXX = g++
INCLUDE = -Isrc
LDFLAGS = -lcurses

Runaway: $(wildcard src/*)
	$(CXX) -g src/main.cpp ${INCLUDE} ${LDFLAGS} -o $@