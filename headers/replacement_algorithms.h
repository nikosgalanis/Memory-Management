#pragma once

#include "../headers/structs.h"
#include "../headers/simulator.h"

int find_page_to_sacrifise(int algorithm, ipt_entry* ipt, main_memory_entry* main_memory, int size, int pid, Queue* active, int window_size);
int lru(main_memory_entry* main_memory, int size);
int ws(ipt_entry* ipt, main_memory_entry* main_memory, int size, int pid, Queue* active, int window_size);
