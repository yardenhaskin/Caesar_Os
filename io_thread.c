// io_thread.c

// Includes --------------------------------------------------------------------

#include <windows.h>
#include "io_Thread.h"

// Function Definitions --------------------------------------------------------
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	IO_THREAD_params_t* p_params;
	p_params = (IO_THREAD_params_t*)lpParam;
	int start_char = p_params->arr[0];
	int end_char = p_params->arr[1];
	char* path_of_input = p_params->full_path_of_input;
	char* path_of_output = p_params->full_path_of_output;
	int key = p_params->key;
	HANDLE output_file_handle = open_output_file_in_threads(path_of_output);
	HANDLE input_file_handle = open_input_file(path_of_input);
	int input_file_size = end_char - (start_char - 1);
	char* buffer = (char*)calloc(input_file_size, sizeof(char)); // initializing the Buffer
	OVERLAPPED ol = { 0 };
	if (start_char <= end_char)
	{

		SetFilePointer(input_file_handle, start_char, NULL, FILE_BEGIN);
		SetFilePointer(output_file_handle, start_char, NULL, FILE_BEGIN);


		//Reading the input file:
		//---------------------------------START--------------------------------------------------------------//


		if (FALSE == ReadFile(input_file_handle,					//A handle to the  file
			buffer,							//A pointer to the buffer that receives the data read from a file 
			input_file_size,						//The maximum number of bytes to be read.				
			&ol,									//A pointer to the variable that receives the number of bytes read when using a synchronous hFile parameter.
			NULL))								//A pointer to an OVERLAPPED structure is required if the hFile parameter was opened with FILE_FLAG_OVERLAPPED, otherwise it can be NULL.
		{
			printf("readFile error: %d\n", GetLastError()); 
			return;
		}
		//---------------------------------END------------------------------------------------------------------//

		//Decoding/Encoding:
		//---------------------------------START--------------------------------------------------------------//
		int out_len = strlen(path_of_output);
		char op = path_of_output[out_len - 13];
		if (op== 'd')
		{
			for (int i = 0; i < input_file_size; i++)
			{
				buffer[i] = decode(buffer[i], key);
			}
		}
		else
		{
			for (int i = 0; i < input_file_size; i++)
			{
				buffer[i] = encode(buffer[i], key);
			}
		}
		
		//---------------------------------END------------------------------------------------------------------//

		//writing to the output file:
		//---------------------------------START--------------------------------------------------------------//
		OVERLAPPED ol2 = { 0 };

		if (FALSE == (WriteFile(
			output_file_handle,
			buffer,
			input_file_size,
			&ol2,
			NULL)))
		{
			printf("WriteFile error: %d\n", GetLastError()); 
			return;
		}
		//---------------------------------END------------------------------------------------------------------//
	}
	free(buffer);
	CloseHandle(output_file_handle);
	CloseHandle(input_file_handle);
}