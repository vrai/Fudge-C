CC      = gcc
RM      = rm -f
CFLAGS  = -Wall -pedantic -std=c99

OBJS    = $(patsubst %.c,%.o,$(wildcard *.c))
TEST    = test

.PHONY: all clean

all: $(TEST)

$(TEST): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	@ $(RM) $(OBJS)
	@ $(RM) $(TEST)
