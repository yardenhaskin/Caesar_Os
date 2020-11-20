
#include <stdio.h>
#include<Windows.h>
#ifndef functions
#define functions
char decode(char letter, int key);
char decode_small_letters(char letter, int key);
char decode_capital_letters(char letter, int key);
char decode_number(char letter, int key);
char encode(char letter, int key);
char encode_small_letters(char letter, int key);
char encode_capital_letters(char letter, int key);
char encode_number(char letter, int key);
int find_num_of_rows(char* directory_with_input);
void start_end_thread_array(int num_of_rows, int num_of_threads, int* rows_per_thread_array, int** range_for_every_thread_array);
#endif // functions

