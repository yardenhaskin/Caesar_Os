
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"functions.h"
#include"file_handler.h"


const int STATUS_CODE_FAILURE = -1;
//Step Number One
main(int argc, char* argv[])
{
	int key = atoi(argv[2]);
	LONG input_file_size = find_input_file_size(argv[1]); //without EOF
	HANDLE output_file_handle = open_output_file(argv[1], input_file_size);
	HANDLE input_file_handle = open_input_file(argv[1]);
	char* buffer = (char*)calloc(input_file_size, sizeof(char));
	
	OVERLAPPED ol = { 0 };
	OVERLAPPED ol2 = { 0 };
	if (argc != 3)
	{
		printf("Error, Invalid number of arguments\n");
		return STATUS_CODE_FAILURE;
	}

	if (FALSE == ReadFile(input_file_handle,					//A handle to the  file
		buffer,							//A pointer to the buffer that receives the data read from a file 
		input_file_size,						//The maximum number of bytes to be read.				
		&ol,									//A pointer to the variable that receives the number of bytes read when using a synchronous hFile parameter.
		NULL))								//A pointer to an OVERLAPPED structure is required if the hFile parameter was opened with FILE_FLAG_OVERLAPPED, otherwise it can be NULL.
	{
		printf("Error, Unable to read from file\n"); // you can make this better by using GetLastError()
		return STATUS_CODE_FAILURE;
	}
	for (int i = 0; i <= input_file_size; i++)
	{
		buffer[i] = decode(buffer[i], key);
	}
	if (FALSE == WriteFile(
		output_file_handle,
		buffer,
		input_file_size,
		&ol2,
		NULL))
	{
		printf("Error, Unable to read from file\n"); // you can make this better by using GetLastError()
		return STATUS_CODE_FAILURE;
	}
	CloseHandle(output_file_handle);
	CloseHandle(input_file_handle);
	return 0;
}