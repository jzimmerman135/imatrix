CC := gcc
CFLAGS := -Wall -Werror -pedantic
OFLAGS := -O3

all: imatrix.o

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

release: CFLAGS += $(OFLAGS)
release: all
