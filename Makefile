.PHONY: run clean

CXX = g++
INCLUDE = -Isrc
LDFLAGS = -lcurses

TARGET = Runaway

$(TARGET): $(wildcard src/*) build/
	$(CXX) -g src/main.cpp ${INCLUDE} ${LDFLAGS} -o $@

build/:
	mkdir -p build

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
