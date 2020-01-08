#pragma once

/* Defines */
#define EMPTY -1
#define MASK 0b00000000000000000000111111111111

/* Enumerations */
typedef enum {
  False, True
} boolean;

/* Declaration of structs */
typedef struct {
  int p_hash;
  int offset;
} logical_adress;

typedef struct {
  int pid;
  int p_hash;
} ipt_entry;

typedef struct {
  int address;
  char operation;
} set_entry;

typedef struct {
  int curr_offset;
  boolean modified;
  int time_signature;

} main_memory_entry;

typedef struct QueueNodeTag {
     int Item;
     struct QueueNodeTag *Link;
} QueueNode;

typedef struct {
    QueueNode *Front;
    QueueNode *Rear;
    int max_size;
    int curr_size;
} Queue;

/* Declaration of global variables and defines */
int time;

#define FAILURE -1
#define FOUND_PAGE 1
#define FOUND_EMPTY_SPACE 2

/* Declaration of functions */
ipt_entry* create_ipt(int size);
set_entry* create_temp_set(int size);
main_memory_entry* create_main_memory(int size);
logical_adress* create_logical_adress(int x);

void fill_set(set_entry* set, FILE* file, int size);
int find_space_ipt(ipt_entry* ipt, int size, int pid, int p_hash, int* pos);

Queue* initialize_queue(int size);
int Empty(Queue*);
void insert(Queue* Q, int x);
void Remove(Queue* Q);
int* initialize_set(int size);
int in_set(int* ws, int x, int size);
int* fill_working_set(int* ws, Queue* queue, int size);
