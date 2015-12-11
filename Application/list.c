#include "list.h"

void list_add_time(int which, unsigned int time) {

  pthread_mutex_lock(&mutex[which]);
  time_elem* new;

  new = (time_elem*)malloc(sizeof(time_elem));
  new->next= NULL;
  new->time = time;
  if(!time_list_heads[which]) {
    new->prev = NULL;
    time_list_heads[which] = new;
    time_list_tails[which] = time_list_heads[which];
  }
  else {
    time_list_tails[which]->next = new;
    new->prev = time_list_tails[which];
  }
  time_list_tails[which] = new;
  pthread_mutex_unlock(&mutex[which]);
}

unsigned int list_get_time(int which, int* valid) {

  time_elem* act;
  unsigned int ret;

  pthread_mutex_lock(&mutex[which]);
  if(!time_list_heads[which]) {
    *valid = FALSE;
    return 0;
  }
  act = time_list_heads[which];
  time_list_heads[which] = time_list_heads[which]->next;
  if(!time_list_heads[which])
    time_list_tails[which] = NULL;
  ret = act->time;
  free((void*)act);
  pthread_mutex_unlock(&mutex[which]);
  *valid = TRUE;
  return ret;
}

int list_is_valid(int which) {

  int ret;
  pthread_mutex_lock(&mutex[which]);
  if (time_list_heads[which])
    ret = TRUE;
  else
    ret = FALSE;
  pthread_mutex_unlock(&mutex[which]);
  return ret;
}

void list_init() {

  int i;
  for(i = 0; i < LIST_NUM; i++){
    time_list_heads[i] = 0;
    time_list_tails[i] = 0;
  }
}

void clear_lists(){

  int which;
  time_elem *act, *next;

  for (which = 0; which < LIST_NUM; which++) {
     pthread_mutex_lock(&mutex[which]);
    act = time_list_heads[which];
    while(act){
      next = act->next;
      free((void*)act);
      act = next;
    }
    time_list_tails[which] = NULL;
    time_list_heads[which] = NULL;
    pthread_mutex_unlock(&mutex[which]);
  }
}
