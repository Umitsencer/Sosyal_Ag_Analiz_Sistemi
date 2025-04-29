CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lm

all: sosyal_ag

sosyal_ag: main.o social_network.o
	$(CC) $(CFLAGS) -o sosyal_ag main.o social_network.o $(LDFLAGS)

main.o: main.c social_network.h
	$(CC) $(CFLAGS) -c main.c

social_network.o: social_network.c social_network.h
	$(CC) $(CFLAGS) -c social_network.c

clean:
	rm -f *.o sosyal_ag 