CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
TARGET = risk_system
SRCS = main.c input.c validation.c risk_engine.c score_engine.c recommendation.c report.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lm

%.o: %.c merchant.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
