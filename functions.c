#include <stdio.h>

//input arguments : letter to decode, key for decoding
//output : letter - decoded char.
//funcionality : this function returns the decoded char by Caesar code 
char decode_small_letters(char letter, int key) {
	int code = letter - 'a' - key;
	while (code < 0)
		code += 26;
	return ('a' + (code) % 26);
}
//input arguments : letter to decode, key for decoding
//output : letter - decoded char.
//funcionality : this function returns the decoded char by Caesar code 
char decode_capital_letters(char letter, int key) {
	int code = letter - 'A' - key;
	while (code < 0)
		code += 26;
	return  ('A' + (code) % 26);
}
//input arguments : letter to decode, key for decoding
//output : letter - decoded char.
//funcionality : this function returns the decoded char by Caesar code 
char decode_number(char letter, int key) {
	int code = letter - '0' - key;
	while (code < 0)
		code += 10;
	return ('0' + (code) % 10);
}
//input arguments : letter to encode, key for encoding
//output : letter - encoded char.
//funcionality : this function returns the encoded char by Caesar code 
char encode_small_letters(char letter, int key) {
	return ('a' + (letter - 'a' + key) % 26);
}
//input arguments : letter to encode, key for encoding
//output : letter - encoded char.
//funcionality : this function returns the encoded char by Caesar code 
char encode_capital_letters(char letter, int key) {
	return  ('A' + (letter - 'A' + key) % 26);
}
//input arguments : letter to encode, key for encoding
//output : letter - encoded char.
//funcionality : this function returns the encoded char by Caesar code 
char encode_number(char letter, int key) {
	return ('0' + (letter - '0' + key) % 10);
}
//input arguments : letter to decode, key for decoding
//output : letter - decoded char.
//funcionality : this function returns the decoded char by Caesar code by calling 
// the right decoding function( we have a function for every relevant case)
char decode(char letter, int key) {
	if (letter >= 'a' && letter <= 'z')
		return decode_small_letters(letter, key);
	else if (letter >= 'A' && letter <= 'Z')
		return decode_capital_letters(letter, key);
	else if (letter >= '0' && letter <= '9')
		return decode_number(letter, key);
	return letter;
}
//input arguments : letter to encode, key for encoding
//output : letter - encoded char.
//funcionality : this function returns the encoded char by Caesar code by calling 
// the right encoding function( we have a function for every relevant case)
char encode(char letter, int key) {
	if (letter >= 'a' && letter <= 'z')
		return encode_small_letters(letter, key);
	else if (letter >= 'A' && letter <= 'Z')
		return encode_capital_letters(letter, key);
	else if (letter >= '0' && letter <= '9')
		return encode_number(letter, key);
	return letter;
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
//input arguments :directory with input - is the directory for the input file. 
//sizes of rows array- is a pointer to an array that will contain the start and end row for every thread
//output : void function
//funcionality : this function gives us the information how many rows each thread read, encode/decode and writes 

void make_sizes_of_rows_array(char* directory_with_input, int* sizes_of_rows_array)
{
	FILE* p_stream_input = NULL;
	errno_t retval_of_input;
	int num_of_rows = 0;
	int row_size = 0;
	retval_of_input = fopen_s(&p_stream_input, directory_with_input, "r");   //opens the txt folder written in the command line
	if (0 != retval_of_input)
	{
		printf("Failed to open file.\n");
		return;
	}
	char c;
	while ((c = getc(p_stream_input)) != EOF)
	{
		if (c == '\n')
		{
			row_size += 2;
			sizes_of_rows_array[num_of_rows] = row_size;
			num_of_rows += 1;
			row_size = 0;

		}
		else
			row_size += 1;
	}
	if (row_size != 0) // This one is to check if the last line is without \n but with EOF
	{
		sizes_of_rows_array[num_of_rows] = row_size;
		num_of_rows += 1;
		row_size = 0;
	}

	fclose(p_stream_input);
}
//input arguments :sizes of rows array-is a pointer to array that 
//will contain the sizes of each row.range for every thread array - is a pointer to an array that contains
//arrays , where every array has the range for it's thread
//output : void function
//funcionality : this function gives us the information how many chars( = bytes) each thread  writes 
void start_end_thread_array_in_chars(int* sizes_of_rows_array, int** range_for_every_thread_array, int num_of_threads)
{
	int sum = 0;
	int last_row = range_for_every_thread_array[1][1];
	for (int j = 0; j <= last_row ;j++)
	{
		sum = sum + sizes_of_rows_array[j];
	}
	range_for_every_thread_array[1][1] = sum;
	sum = 0;

	for (int i = 2; i <= num_of_threads; i++)
	{
		int start = range_for_every_thread_array[i][0];
		int end = range_for_every_thread_array[i][1];
		range_for_every_thread_array[i][0] = range_for_every_thread_array[i - 1][1] + 1;
		for (int k = start; k <= end; k++)
		{
			sum = sum + sizes_of_rows_array[k];
		}
		range_for_every_thread_array[i][1] = sum + range_for_every_thread_array[i - 1][1];
		sum = 0;
	}
}