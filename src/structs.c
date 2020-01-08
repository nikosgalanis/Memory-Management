#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/structs.h"

/* Create the inverted page table needed for the simulation */
ipt_entry* create_ipt(int size) {
  ipt_entry* new;
  new = malloc(size * sizeof(ipt_entry));
  if (new == NULL) {
    printf("Allocation error. Exiting...\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < size; ++i) {
    new[i].pid = EMPTY;
    new[i].p_hash = EMPTY;
  }

  return new;
}

/* Create the temporary set to store traces from the files */
set_entry* create_temp_set(int size) {
  set_entry* new;
  new = malloc(size * sizeof(set_entry));
  if (new == NULL) {
    printf("Allocation error. Exiting...\n");
    exit(EXIT_FAILURE);
  }

  return new;
}

/* Create the virtual main memory needed for the simulation */
main_memory_entry* create_main_memory(int size) {
  main_memory_entry* new;
  new = malloc(size * sizeof(main_memory_entry));
  if (new == NULL) {
    printf("Allocation error. Exiting...\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < size; ++i) {
    new[i].curr_offset = EMPTY;
    new[i].modified = False;
    new[i].time_signature = EMPTY;
  }

  return new;
}

/* Fill the temporary set by reading from the file given as an argument */
void fill_set(set_entry* set, FILE* file, int size) {
  for (size_t i = 0; i < size; ++i) {
    fscanf(file, "%X %c\n", &set[i].address, &set[i].operation);
  }
}

/* Given a hex number, seperate it to [p# | offset], and return a struct
  containing that info */
logical_adress* create_logical_adress (int x) {
  logical_adress* addr = malloc(sizeof(logical_adress));
  int p_hash = x >> 12;
  int offset = x & MASK;
  addr->p_hash = p_hash;
  addr->offset = offset;

  return addr;
}

/* Given an ipt, search for free space, or for the existance of the page.
  If we find the page, return imediatelly FOUND_PAGE, and pass the position value
  to the argument. If we do not find the page, return FOUND_EMPTY_SPACE or
  FAILURE accordingly */
int find_space_ipt(ipt_entry* ipt, int size, int pid, int p_hash, int* pos) {
  /* We want to return the result of the search: aka if we've found the page,
     or if not, if we've found empty space in the ipt to insert it */
    int to_return = FAILURE;
    /* Scan the ipt to find an empty space to insert the new entry */
    for (size_t i = 0; i < size; ++i) {
      if (ipt[i].p_hash == EMPTY) {
        to_return = FOUND_EMPTY_SPACE;
        *pos = i;
      }
      else if (ipt[i].p_hash == p_hash && ipt[i].pid == pid) {
        /*imediatelly when you find the page in the table, return */
        to_return = FOUND_PAGE;
        *pos = i;
        return to_return;
      }
    }

    return to_return;
}

Queue* initialize_queue(int size) {
    Queue* Q = malloc(sizeof(Queue));
    Q->Front = NULL;
    Q->Rear = NULL;
    Q->curr_size = 0;
    Q->max_size = size;
    return Q;
}

void insert(Queue* Q, int X) {
  if (Q->curr_size == Q->max_size) {
    Remove(Q);
  }

   QueueNode *Temp;

   Temp = malloc(sizeof(QueueNode));

   if (Temp==NULL) {
      printf("System storage is exhausted");
      exit(EXIT_FAILURE);
   } else {
     Q->curr_size++;
      Temp->Item = X;
      Temp->Link = NULL;
      if (Q->Rear == NULL) {
         Q->Front = Temp;
         Q->Rear = Temp;
      } else {
         Q->Rear->Link = Temp;
         Q->Rear = Temp;
      }
   }
}

void Remove(Queue *Q) {
   QueueNode* Temp;

   if (Q->Front == NULL) {
      printf("Attempt to remove item from an empty queue");
      exit(EXIT_FAILURE);
   } else {
      Temp = Q->Front;
      Q->Front = Temp->Link;
      free(Temp);
      if (Q->Front == NULL)
        Q->Rear = NULL;
   }
   Q->curr_size--;
}

int* initialize_set(int size) {
  int* new =  malloc(size * sizeof(int));
  for (size_t i = 0; i < size; ++i) {
    new[i] = EMPTY;
  }
  return new;
}

int in_set(int* ws, int x, int size) {
  for (size_t i = 0; i < size; ++i) {
    if (x == ws[i])
    return 1;
  }
  return 0;
}

int* fill_working_set(int* ws, Queue* queue, int size) {
  int i = 0;
  QueueNode* Temp;
  Temp = queue->Front;
  while (Temp != NULL) {
    if(in_set(ws, Temp->Item, size) == 0) {
      ws[i] = Temp->Item;
      i++;
    }
      Temp = Temp->Link;
  }
  return ws;
}
