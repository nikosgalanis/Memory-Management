#pragma once

typedef enum {
  LRU, WS
}algorithm;

int page_faults;
int succesfull_searches;
int writes;
int reads;
int dirty_pages;

void simulator(int alg, int n_frames, int set_length, int max, int window_size);
