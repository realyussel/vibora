// Librerias
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdbool.h>
#include <curses.h>

/*
Se puede dar un nombre nuevo a cualquier tipo de datos mediante typedef.
http://sopa.dis.ulpgc.es/fso/cpp/intro_c/introc57.htm
*/

// Definición de variables
typedef enum Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA } Direccion;
typedef enum Estado { CONTINUAR, TERMINAR } Estado;

// Estructuras de datos
typedef struct Coordenada {
  int x;
  int y;
  struct Coordenada* next;
} Punto;

typedef struct Elementos {
  Punto* vibora;
  Punto* alimentos;
  int xmax;
  int ymax;
} Escena; // Se declara el tipo Escena

// Definición de funciones
bool es_el_mismo_punto(Punto* punto_1, Punto* punto_2);
Estado mover(Escena* board, Direccion dir);
Punto* proximo_movimiento(Escena* board, Direccion dir);
Punto* create_cell(int x, int y);
Punto* punto_aleatorio(int xmax, int ymax);
Punto* create_vibora();
Escena* create_board(Punto* alimentos, Punto* vibora, int xmax, int ymax);
bool list_contains(Punto* cell, Punto* list);
bool remove_from_list(Punto* elt, Punto** list);
void nuevo_alimento(Escena* board);
// FRONTEND
Direccion siguiente_movimiento(Direccion previous);
void display_points(Punto* vibora, const chtype symbol);

void display_points(Punto* vibora, const chtype symbol) {
  while(vibora) {
    mvaddch(vibora->y, vibora->x, symbol);
    vibora = vibora->next;
  }
}

Direccion siguiente_movimiento(Direccion previous) {
  int ch = getch();
  switch (ch) {
    case KEY_LEFT:
      if (previous != DERECHA) return IZQUIERDA;
    case KEY_RIGHT:
      if (previous != IZQUIERDA) return DERECHA;
    case KEY_DOWN:
      if (previous != ARRIBA) return ABAJO;
    case KEY_UP:
      if (previous != ABAJO) return ARRIBA;
    default:
      return previous;
  }
}
//BACKEND
bool remove_from_list(Punto* elt, Punto** list) {
  Punto *currP, *prevP;
  prevP = NULL;

  for (currP = *list;
      currP != NULL;
      prevP = currP, currP = currP->next) {
    if (es_el_mismo_punto(currP, elt)) {
      if (prevP == NULL) {
        *list = currP->next;
      } else {
        prevP->next = currP->next;
      }
      free(currP);
      return true;
    }
  }
  return false;
}

Estado mover(Escena* board, Direccion dir) {
  // Create a new beginning. Check boundaries.
  Punto* beginning = proximo_movimiento(board, dir);
  if (beginning == NULL) {
    return TERMINAR;
  }

  // If we've gone backwards, don't do anything
  if (board->vibora->next && es_el_mismo_punto(beginning, board->vibora->next)) {
    beginning->next = NULL;
    free(beginning);
    return CONTINUAR;
  }

  // Check for collisions
  if (list_contains(beginning, board->vibora)) {
    return TERMINAR;
  }

  // Check for food
  if (list_contains(beginning, board->alimentos)) {
    // Attach the beginning to the rest of the vibora;
    beginning->next = board->vibora;
    board->vibora = beginning;
    remove_from_list(beginning, &(board->alimentos));
    nuevo_alimento(board);

    return CONTINUAR;
  }

  // Attach the beginning to the rest of the vibora
  beginning->next = board->vibora;
  board->vibora = beginning;


  // Cut off the end
  Punto* end = board->vibora;
  while(end->next->next) {
    end = end->next;
  }
  free(end->next);
  end->next = NULL;

  return CONTINUAR;
}

bool es_el_mismo_punto(Punto* punto_1, Punto* punto_2) {
  return punto_1->x == punto_2->x && punto_1->y == punto_2->y;
}


Punto* proximo_movimiento(Escena* board, Direccion dir) {
  Punto* vibora = board->vibora;
  int new_x = vibora->x;
  int new_y = vibora->y;
  switch(dir) {
    case ARRIBA:
      new_y = vibora->y - 1;
      break;
    case ABAJO:
      new_y = vibora->y + 1;
      break;
    case IZQUIERDA:
      new_x = vibora->x - 1;
      break;
    case DERECHA:
      new_x = vibora->x + 1;
      break;
  }
  if (new_x < 0 || new_y < 0 || new_x >= board->xmax || new_y >= board->ymax) {
    return NULL;
  } else {
    return create_cell(new_x, new_y);
  }
}

Punto* punto_aleatorio(int xmax, int ymax) {
  return create_cell(rand() % xmax, rand() % ymax);
}

void nuevo_alimento(Escena* board) {
  Punto* new_food;
  do {
    new_food = punto_aleatorio(board->xmax, board->ymax);
  } while(list_contains(new_food, board->alimentos) || list_contains(new_food, board->vibora));
  new_food->next = board->alimentos;
  board->alimentos = new_food;
}

bool list_contains(Punto* cell, Punto* list) {
  Punto* s = list;
  while (s) {
    if (es_el_mismo_punto(s, cell)) {
      return true;
    }
    s = s->next;
  }
  return false;
}

Punto* create_cell(int x, int y) {
  Punto* cell = malloc(sizeof(*cell));
  cell->x = x;
  cell->y = y;
  cell->next = NULL;
  return cell;
}

Escena* create_board(Punto* vibora, Punto* alimentos, int xmax, int ymax) {
  Escena* board = malloc(sizeof(*board));
  board->alimentos = alimentos;
  board->vibora = vibora;
  board->xmax = xmax;
  board->ymax = ymax;
  return board;
}

Punto* create_vibora() {
  Punto* a = create_cell(2,3);
  Punto* b = create_cell(2,2);
  a->next = b;
  return a;
}

int main() {

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE); // teclas de dirección
  curs_set(0); // ocultar el cursor
  timeout(100);

  int xmax;
  int ymax;
  getmaxyx(stdscr, ymax, xmax); // obtiene las coordenadas X y Y max de la pantalla
  Direccion dir = DERECHA; // variable dir, inicia derecha

  Escena* board = create_board(create_vibora(), NULL, xmax, ymax);
  int i; // contador
  for (i = 0; i < 2; i++) { // Añadir 2 Alimentos al escenario
    nuevo_alimento(board); // Función para añadir un alimento
  }

  while(true) {
    clear();
    display_points(board->vibora, ACS_BLOCK);
    display_points(board->alimentos, ACS_DIAMOND);
    refresh();
    dir = siguiente_movimiento(dir);
    Estado estado = mover(board, dir);
    if (estado == TERMINAR) break;
  }
  endwin();

  return 0;
}