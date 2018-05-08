#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#define DELAY 65000


//the snake is a doubly-linked list
typedef struct node{
    int x;
    int y;
    struct node* next;
    struct node* prev;
} node_t;

node_t * snek_front = NULL;
node_t * snek_back  = NULL;


//snek treat
int treat_x;
int treat_y;
int snek_alive=1;


int max_y=0;
int max_x=0;     //screen limits


void grow_snek(int x, int y){
    node_t * new_head = malloc(sizeof(node_t));
    new_head->x = x;
    new_head->y = y;
    new_head->next = snek_front;
    new_head->prev = NULL;
    snek_front->prev = new_head;
    snek_front = new_head;
}

void push_position(int x, int y, node_t* snek){
    if (snek != NULL){
        push_position(snek->x, snek->y, snek->next);
        snek->x = x;
        snek->y = y;
    }
}

int in_snek(int x, int y){
    node_t * cur_node = snek_front;
    while (cur_node != NULL){
        if (x == cur_node->x && y == cur_node->y)
            return 1;
        cur_node = cur_node->next;
    }
    return 0;
}

void spawn_treat(){
    //make sure: treat isn't where the snek is 
    treat_x = rand() % max_x;
    treat_y = rand() % max_y;

    while (in_snek(treat_x, treat_y)){
        treat_x = rand() % max_x;
        treat_y = rand() % max_y;
    }
}

void move_snek(int dx, int dy){
    int new_x = snek_front->x + dx;
    int new_y = snek_front->y + dy;
    node_t  * temp_ptr = snek_front;
    if (new_x < 0 || new_x >max_x || new_y < 0 || new_y > max_y) {
        snek_alive = 0;
    } else if (new_x == treat_x && new_y == treat_y){ //treat
        grow_snek(new_x, new_y);
        spawn_treat(); //change x&y coordinates of treat
    } else {
        push_position(new_x, new_y, snek_front);
    }
}


int draw_snek(){
    node_t * cur_node = snek_front;
    while (cur_node != NULL){
        mvprintw(cur_node->y, cur_node->x, "s");
        cur_node = cur_node->next;
    }
    return 0;
}

void draw_treat(){
    mvprintw(treat_y, treat_x,"t");
}

int main(){
    //ncurses init stuff
	initscr();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);
    getmaxyx(stdscr, max_y, max_x);

    int last_key = KEY_LEFT;

    //init
    snek_front =  malloc(sizeof(node_t));
    snek_back  =  malloc(sizeof(node_t));
    snek_front->x = max_x/2;
    snek_front->y = max_y/2;
    snek_back ->x = max_x/2 + 1;
    snek_back ->y = max_x/2;
    snek_front->prev = NULL; snek_back->next = NULL;
    //point to each other
    snek_front->next = snek_back; snek_back->prev = snek_front;


    //init treat somewhere to the left of snek
    treat_x= max_x/4;
    treat_y= rand()%max_y;

	int input;

    //our loop is fairly simple, we could have handles input a number
    //of different ways, such as a separate function to handle a global flag
    //or just a handle_input that returns a struct with the dx, dy vals
	while(snek_alive) {

		input = getch();
		if (input != ERR)   //ERR is returned if we don't provide any input
			last_key = input;  //if the user hits any other key it becomes last_key!

        switch(last_key){
            case KEY_UP:    //KEY_DIRECTION globals are provided by ncurses
                move_snek(0,-1);
                break;
            case KEY_LEFT:
                move_snek(-1,0);
                break;
            case KEY_RIGHT:
                move_snek(1, 0);
                break;
            case KEY_DOWN:
                move_snek(0,1);
                break;
        }

        clear();    //draws screen
        draw_snek();
        draw_treat();
        refresh();

		usleep(DELAY);  //u = mu = sleep for microseconds

	}
    endwin();
}

