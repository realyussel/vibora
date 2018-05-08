default: build

build:
	gcc -o file commander.c browser.c interface.c copy_file.c -lncurses

run: build
	./file

clean:
	find . -perm +100 -type f -delete