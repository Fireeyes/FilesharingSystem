IDIR = include
SDIR = src
ODIR = obj
LDIR =../libs

CC=gcc
CFLAGS=-I$(IDIR) -I$(LDIR)
EXEC = client_exec

_SHARED = command.h
SHARED = $(patsubst %,$(LDIR)/%,$(_SHARED))

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = client.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) $(SHARED)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ)
	gcc -o $(EXEC) $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(EXEC)
