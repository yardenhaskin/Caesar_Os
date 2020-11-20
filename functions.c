#include <stdio.h>


char decode_small_letters(char letter, int key) {
	return ('a' + (letter - 'a' - key) % 26);
}
char decode_capital_letters(char letter, int key) {
	return  ('A' + (letter - 'A' - key) % 26);
}
char decode_number(char letter, int key) {
	return ('0' + (letter - '0' - key) % 10);
}

char encode_small_letters(char letter, int key) {
	return ('a' + (letter - 'a' + key) % 26);
}
char encode_capital_letters(char letter, int key) {
	return  ('A' + (letter - 'A' + key) % 26);
}
char encode_number(char letter, int key) {
	return ('0' + (letter - '0' + key) % 10);
}

char decode(char letter, int key) {
	if (letter >= 'a' && letter <= 'z')
		return decode_small_letters(letter, key);
	else if (letter >= 'A' && letter <= 'Z')
		return decode_capital_letters(letter, key);
	else if (letter >= '0' && letter <= '9')
		return decode_number(letter, key);
	return letter;
}

char encode(char letter, int key) {
	if (letter >= 'a' && letter <= 'z')
		return encode_small_letters(letter, key);
	else if (letter >= 'A' && letter <= 'Z')
		return encode_capital_letters(letter, key);
	else if (letter >= '0' && letter <= '9')
		return encode_number(letter, key);
	return letter;
}






int find_num_of_rows(char* directory_with_input)
{
	FILE* p_stream_input = NULL;
	errno_t retval_of_input;
	int num_of_rows = 0;				//we assume we can store the num of char in LONG type
	retval_of_input = fopen_s(&p_stream_input, directory_with_input, "r");   //opens the txt folder written in the command line
	if (0 != retval_of_input)
	{
		printf("Failed to open file.\n");
		return -1;
	}
	char c;
	while (c=getc(p_stream_input) != EOF)
	{
		if(c=='\n')
			num_of_rows += 1;
	}
	fclose(p_stream_input);
	return num_of_rows; 
}

void start_end_thread_array(int num_of_rows, int num_of_threads, int* rows_per_thread_array, int** range_for_every_thread_array)
{
	int rows_per_thread_base = (int)(num_of_rows / num_of_threads);
	int extra_rows = num_of_rows % num_of_threads;
	for (int i = 1; i <= num_of_threads; i++)
	{
		if (extra_rows != 0)
		{
			rows_per_thread_array[i] = rows_per_thread_base + 1;
			extra_rows -= 1;
		}
		else
			rows_per_thread_array[i] = rows_per_thread_base;
	}
	for (int i = 0; i <= num_of_threads; i++)
		range_for_every_thread_array[i] = (int*)malloc(2 * sizeof(int));
	range_for_every_thread_array[1][0] = 0;
	range_for_every_thread_array[1][1] = rows_per_thread_array[1] - 1;
	for (int row = 2; row <= num_of_threads; row++)
	{
		range_for_every_thread_array[row][0] = range_for_every_thread_array[row - 1][1] + 1;
		range_for_every_thread_array[row][1] = range_for_every_thread_array[row][0] + rows_per_thread_array[row] - 1;
	}
}