
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
void start_end_thread_array(int num_of_rows, int num_of_threads, int* rows_per_thread_array, int** range_for_every_thread_array);
void start_end_thread_array_in_chars(int* sizes_of_rows_array, int** range_for_every_thread_array, int num_of_threads);
void make_sizes_of_rows_array(char* directory_with_input, int* sizes_of_rows_array);

#endif // functions

