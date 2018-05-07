default: build

build:
	gcc vibora.c -o vibora -lncurses

run: build
	./vibora