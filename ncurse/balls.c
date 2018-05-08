#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DELAY 30000
#define NUMBALLS 3
#define MAX_X_VEL 1.2
#define MAX_Y_VEL 1.2

//----UTILITY FUNCTIONS----
//returns within [0,1]
float rfloat(){
    return ((float)rand() / (float)RAND_MAX);
} //C doesn't have a built-in random float func
//
//rounds float to int
int roundfl(float in){
    return (in + 0.5);
}

//----MAIN SECTION STARTS HERE----
//
//our central type
typedef struct{
    float x_pos, y_pos;
    float x_vel, y_vel;
} ball_t;

//global object pool
ball_t* ball_pool[NUMBALLS] = {0};

//used to initialize
ball_t* spawn_random_ball(int max_x, int max_y){
    ball_t* raw_ball = malloc(sizeof(ball_t));

    raw_ball->x_pos = rfloat() * max_x;
    raw_ball->y_pos = rfloat() * max_y;
    raw_ball->x_vel = rfloat() * MAX_X_VEL;
    raw_ball->y_vel = rfloat() * MAX_Y_VEL;

    return raw_ball;
}

//----LOOP LOGIC----
void draw_balls(){
    int i, x_round=0, y_round=0;
    for (i=0; i<NUMBALLS;i++){
        x_round = roundfl(ball_pool[i]->x_pos);
        y_round = roundfl(ball_pool[i]->y_pos);
        mvprintw(y_round, x_round, "o");
    }
}

void check_collisions(float max_x, float max_y){
    //will I hit the wall?
    int i;
    for (i=0; i<NUMBALLS;i++){
        float next_x=0, next_y=0;
        next_x = ball_pool[i]->x_pos + ball_pool[i]->x_vel;
        next_y = ball_pool[i]->y_pos + ball_pool[i]->y_vel;

        //collide with the wall and add the remaining distance,
        //then move it a bit back for the update() step
        if (next_x < 0){
            ball_pool[i]->x_vel *= -1;
            ball_pool[i]->x_pos = next_x*-1.0 - ball_pool[i]->x_vel;
        }
        else if (next_x > max_x){
            ball_pool[i]->x_vel *= -1;
            ball_pool[i]->x_pos = max_x - (next_x - max_x) - ball_pool[i]->x_vel;
        }
        if (next_y < 0){
            ball_pool[i]->y_vel *= -1;
            ball_pool[i]->y_pos = next_y*-1.0 - ball_pool[i]->y_vel;
        }
        else if (next_y > max_y){
            ball_pool[i]->y_vel *= -1;
            ball_pool[i]->y_pos = max_y - (next_y - max_y) - ball_pool[i]->y_vel;
        }
    }
}

void step(){
    int i;
    for (i=0; i<NUMBALLS; i++){
        ball_pool[i]->x_pos += ball_pool[i]->x_vel;
        ball_pool[i]->y_pos += ball_pool[i]->y_vel;
    }
}

//----MAIN PROGRAM----
int main(int argc, char *argv[]) {
    //init random seed
    srand(time(NULL));

    //ncurses related stuff
    int max_y=0, max_x=0;
    initscr(); //initialize scren
    noecho();  //don't echo input
    curs_set(FALSE); //don't display cursor
    //get rows and columns, init global "standard screen" var
    getmaxyx(stdscr, max_y, max_x); 

    //create balls
    int i;
    for (i=0; i<NUMBALLS; i++){
        ball_pool[i] = spawn_random_ball(max_x,max_y);
    }
    //game loop
    while(1) {
        clear();
        draw_balls();
        refresh(); //draws the screen's current state
        usleep(DELAY);
        check_collisions(max_x, max_y);
        step();
    }

    endwin();
}

//POTENTIAL TODO: Use good collission data structure, add different collission modes(e.g. no wall, softer collissions)
//                variable sized balls, ball-on-ball action
