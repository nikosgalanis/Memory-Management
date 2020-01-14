#pragma once
#include "structs.h"

typedef enum {
  LRU, WS
}algorithm;

Queue* active_pr1;
Queue* active_pr2;

int page_faults;
int succesfull_searches;
int writes;
int reads;
int dirty_pages;
int ws_failures;

void simulator(int alg, int n_frames, int set_length, int max, int window_size);
