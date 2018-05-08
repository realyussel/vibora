#include <ncurses.h>
#include <unistd.h>

//basic grapher
//
//possible improvements:
//           -only animate/draw graph when it's in our visible range
//           -some kind of interface to take in a function pointer
//           -animate several graphs at a time


const size_t DELAY=60000;

int x_o;
int y_o;

int t_x(int x){
    return  x-x_o;
}

int t_y(int y){
    return y_o - y;
}

int parabola(int x){
    return (x*x / 4);
}

int main(int argc, char** argv){
    initscr();
    keypad(stdscr, TRUE);
    noecho();

    y_o = LINES / 2;
    x_o = COLS / 2;

    //draw axes
    for (int i=0; i<COLS; i++){
        mvaddch(y_o, i, '#');
    }
    for (int i=0; i < LINES; i++){
        mvaddch(i, x_o, '#');
    }
    refresh();

    //graph stuff over x
    for (int x=0; x<COLS; x++){
        mvaddch(t_y(parabola(t_x(x))), x, 'x');
        refresh();
        usleep(DELAY);
    }
    endwin();
}
