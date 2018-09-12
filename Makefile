PROGNAME = code
INCLUDES = -I/usr/local/include -I./include
LDFLAGS = -L./
ODIR = build
SDIR = src
IDIR = include
_OBJECTS = main.o Game.o Search.o
OBJECTS = $(patsubst %, $(ODIR)/%,$(_OBJECTS))
CFLAGS = -Wall -Wextra -O0 -ggdb -std=c++11 -pedantic

all: $(OBJECTS)
	@mkdir -p bin
	g++ $(CFLAGS) -o bin/$(PROGNAME) $(OBJECTS) $(LIBS) $(INCLUDES) $(LDFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp $(IDIR)/*.h
	@mkdir -p $(ODIR)
	g++ -c $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY: clean
clean:
	rm -f Python/*.pyc build/*.o core