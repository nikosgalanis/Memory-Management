#pragma once

#include "../headers/structs.h"
#include "../headers/simulator.h"

int find_page_to_sacrifise(int algorithm, main_memory_entry* main_memory, int size);
int lru(main_memory_entry* main_memory, int size);
