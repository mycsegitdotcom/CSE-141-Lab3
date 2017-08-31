
CC=g++
CFLAGS =-Wall -g

SOURCES=main.cpp predictor.cpp

TEST=predictor

test: $(TEST)

all: test

predictor: $(SOURCES)
	$(CC) $(FLAGS) main.cpp predictor.cpp -o predictor

clean:
	rm -f $(TEST)
