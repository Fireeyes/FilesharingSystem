IDIR = include
SDIR = src
ODIR = obj
LDIR =../libs

CC=gcc
CFLAGS=-I$(IDIR) -I$(LDIR)
EXEC = server_exec

_SHARED = command.h utils.h user.h
SHARED = $(patsubst %,$(LDIR)/%,$(_SHARED))

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = server.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS) $(SHARED)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ)
	gcc -o $(EXEC) $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(EXEC)
