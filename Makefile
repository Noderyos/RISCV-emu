CC = gcc
LD = gcc
MD = mkdir


SRCDIR = src
OBJDIR = obj
INCDIR = include 

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

EXEC = main

CFLAGS =-Wall -Wextra -I$(INCDIR)
LDFLAGS =


all: $(OBJDIR) $(EXEC)
	
run: all 
	./$(EXEC)

$(OBJDIR):
	$(MD) -p $(OBJDIR)

obj/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXEC) : $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -fr $(OBJ)

mrproper: clean
	rm -fr $(OBJDIR)
	rm -fr $(EXEC)
