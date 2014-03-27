TARGET = 3600dns

$(TARGET): $(TARGET).c
	gcc -std=c99 -O0 -g -lm -Wall -Wextra -o $@ $<

all: $(TARGET)

test: all
	./test

clean:
	rm $(TARGET)

