default: build

build:
	gcc vibora.c -o vibora -lncurses

run: build
	./vibora

clean:
	find . -perm +100 -type f -delete