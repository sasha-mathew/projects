CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=gnu99 -I/local/courses/csse2310/include -g
LDFLAGS= -lm -L/local/courses/csse2310/lib -lcsse2310a1


SRCS = uqunscramble.c
OBJS = $(SRCS:.c=.o)

all: uqunscramble

uqunscramble: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f uqunscramble $(OBJS)
