CC = gcc
SRCDIR = ./src
NAME = main

CFILES := $(wildcard $(SRCDIR)/*.c)
LINUX_OFILES = $(patsubst $(SRCDIR)/%.c, $(SRCDIR)/%.o, $(CFILES))

OFLAGS = -O0
CFLAGS = -Wall -g -I$(SRCDIR) $(OFLAGS) 

linux: $(NAME)

$(NAME): $(LINUX_OFILES)
	$(CC) -o $(NAME) $^ $(LINUX_SDLFLAGS)

$(SRCDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(SRCDIR)/*.o
