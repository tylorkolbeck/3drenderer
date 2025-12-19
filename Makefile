build:
	gcc -Wall -std=c99 ./src/*.c -o ./dist/renderer

run:
	./dist/renderer
all: $(TARGET)

clean:
	rm ./dist/renderer
