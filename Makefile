PROGNAME = code
INCLUDES = -I/usr/local/include -I../include
LDFLAGS = -L./
OBJECTS = main.o
CFLAGS = -Wall -Wextra -O0 -ggdb -std=c++11 -pedantic

all: $(OBJECTS)
	@mkdir -p bin
	g++ $(CFLAGS) -o bin/$(PROGNAME) build/*.o $(LIBS) $(INCLUDES) $(LDFLAGS)

build/%.o: src/%.cpp include/%.h
	@mkdir -p build
	g++ -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY: clean
clean:
	rm -f ../Python/*.pyc ../build/*.o core