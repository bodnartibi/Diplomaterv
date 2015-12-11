#ifndef LIST_H
#define LIST_H

#define TRUE 1
#define FALSE 0

#define LIST_NUM 3

#include <pthread.h>
#include <stdlib.h>


typedef struct time_elem_t {
  struct time_elem_t* next;
  struct time_elem_t* prev;
  unsigned int time;
} time_elem;

time_elem* time_list_heads[LIST_NUM];
time_elem* time_list_tails[LIST_NUM];
pthread_mutex_t mutex[LIST_NUM];

/*
 * Idopont hozzaadasa a listahoz
 * Parameterkent megadando, hogy melyik listahoz
 */
void list_add_time(int which, unsigned int time);

/*
 * Kovetkezo adat lekerese egy listabol
 * Visszateresi ertek az idobelyeg
 * Hiba eseten a valid erteket FALSE-ra allitjuk, egyebkent TRUE
 */
unsigned int list_get_time(int which, int* valid);

/*
 * Van-e ervenyes adat a listaban
 * Visszateresei ertek TRUE, ha van, egyebkent FALSE
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
