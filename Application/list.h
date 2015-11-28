#ifndef LIST_H
#define LIST_H

#define TRUE 1
#define FALSE 0

#include <pthread.h>
#include <sys/select.h>
#include <stdlib.h>
//#include <stdio.h>

typedef struct time_elem_t {
  struct time_elem_t* next;
  struct time_elem_t* prev;
  unsigned int time;
} time_elem;

time_elem* time_list_heads[3];
time_elem* time_list_tails[3];
pthread_mutex_t mutex[3];

/*
 * Idopont hozzaadasa a listahoz
 * Parameterkent megadando, hogy melyik listahoz
 */
void list_add_time(int which, unsigned int time);

/*
 * Kovetkezo adat lekerese egy listabol
 */

unsigned int list_get_time(int which, int* valid);

/*
 * Van-e ervenyes adat a listaban
 */

int list_is_valid(int which);

/*
 * Listak inicializalasa
 */
void list_init();

/*
 * Listak uritese
 */

void clear_lists();

#endif //LIST_H
