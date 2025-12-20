build:
	gcc -Wall -std=c99 ./src/*.c -lSDL3 -o ./dist/renderer

run:
	./dist/renderer

all: $(TARGET)

clean:
	rm ./dist/renderer
