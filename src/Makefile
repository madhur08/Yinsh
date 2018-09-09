PROGNAME = code
LIBS = 
INCLUDES = -I/usr/local/include -I../include
LDFLAGS = -L./
OBJECTS = main.o

CFLAGS = -Wall -Wextra -O0 -ggdb -march=native -std=c++11 -pedantic

all: $(PROGNAME)

$(PROGNAME): $(OBJECTS)
	@mkdir -p bin
	g++ -O2 -DNDEBUG -march=native -o bin/$(PROGNAME) build/*.o $(LIBS) $(INCLUDES) $(LDFLAGS)

$(OBJECTS): Makefile

%.o: %.cpp
	@mkdir -p build
	g++ -c $(CFLAGS) $(INCLUDES) -o build/$@ $<

clean:
	find . -name '*.pyc' -delete