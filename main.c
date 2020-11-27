
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"functions.h"
#include"file_handler.h"
#include "io_Thread.h"

const int STATUS_CODE_FAILURE = -1;
static const int OUTPUT_FILE_NAME_LENGTH = 14;

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		printf("Error, Invalid number of arguments\n");
		return STATUS_CODE_FAILURE;
	}
	char* operation = argv[4];
	char op = operation[1];

	if (op !='e' && op !='d')
	{
		printf("Error! operation argument must be -d or -e only\n");
		return STATUS_CODE_FAILURE;
	}


	int key = atoi(argv[2]);														// getting the key as integer

	/*initializes the number of rows and number of chars in input file:*/
	//---START---------------------------------------------------------------------------------------------------------------------------------------//
	int num_of_rows = 0;
	int* p_num_of_rows = &num_of_rows;
	LONG input_file_size = 0;
	LONG* p_input_file_size = &input_file_size;
	find_input_file_sizes(argv[1], p_num_of_rows, p_input_file_size);	// initializes the number of rows and number of chars in input file

	//---END----------------------------------------------------------------------------------------------------------------------------------------------//

	/*making array[i]=(size of row i in chars)*/
	//---START---------------------------------------------------------------------------------------------------------------------------------------//
	int* sizes_of_rows_array = (int*)calloc(num_of_rows, sizeof(int));			
	make_sizes_of_rows_array(argv[1], sizes_of_rows_array); //function that makes an array that array[i]=(size of row i in chars)
	//---END----------------------------------------------------------------------------------------------------------------------------------------------//


	//---START---------------------------------------------------------------------------------------------------------------------------------------//
	char* directory_with_output = NULL;
	const char* directory = extract_directory(argv[1]);

	int dir_and_out_len = strlen(directory) + OUTPUT_FILE_NAME_LENGTH;
	directory_with_output = (char*)malloc((sizeof(char)) * dir_and_out_len);
	set_up_Directory_with_output(argv[1], input_file_size, directory_with_output, dir_and_out_len, directory,op);				//initializing the output file directory
	//HANDLE output_file_handle = open_output_file(argv[1], input_file_size,directory_with_output,dir_and_out_len,directory); //initializing the output file,
	//CloseHandle(output_file_handle);
	//---END----------------------------------------------------------------------------------------------------------------------------------------------//

	

	//Initializing an array of 2d arrays 
	//---------------------------------START--------------------------------------------------------------//
	// range_for_every_thread_array[0] array is always [0,0] , 
	//range_for_every_thread_array[i]=[start,end] shows the start and end char that thread i must read/write to
	// if start>END thread must not write/read any lines!
	// range_for_every_thread_array= [[0,0],
	//							[,],
	//							[,],
	//							[,],
	//							[,]]

	int num_of_threads= atoi(argv[3]);
	int* rows_per_thread_array = (int*)calloc(num_of_threads + 1, sizeof(int));
	int** range_for_every_thread_array = (int**)malloc(num_of_threads * sizeof(int*));
	start_end_thread_array(num_of_rows, num_of_threads, rows_per_thread_array, range_for_every_thread_array);
	start_end_thread_array_in_chars(sizes_of_rows_array, range_for_every_thread_array,num_of_threads); // Updates range_for_every_thread_array to chars counting instead of rows.
	//---------------------------------END------------------------------------------------------------------//

	/*Initializing Thread parameters:*/
	//---START---------------------------------------------------------------------------------------------------------------------------------------//
	IO_THREAD_params_t** p_thread_params= (IO_THREAD_params_t**)calloc(num_of_threads+1,sizeof(IO_THREAD_params_t*));
	if (NULL == p_thread_params)
	{
		printf("Error when allocating memory");
		return STATUS_CODE_FAILURE;
	}
	for (int i = 1; i <= num_of_threads; i++)
	{
		p_thread_params[i]= malloc(sizeof(IO_THREAD_params_t));
		if (NULL == p_thread_params[i])
		{
			printf("Error when allocating memory");
			return STATUS_CODE_FAILURE;
		}
	}

	HANDLE* aThread = (HANDLE*)malloc(sizeof(HANDLE) * (num_of_threads + 1));
	if (NULL == aThread)
	{
		printf("Error when allocating memory");
		return STATUS_CODE_FAILURE;
	}
	DWORD* ThreadID = (DWORD*)malloc(sizeof(DWORD) * (num_of_threads + 1));
	if (NULL == ThreadID)
	{
		printf("Error when allocating memory");
		return STATUS_CODE_FAILURE;
	}

	int i;
	//---------------------------------END------------------------------------------------------------------------------------------------------//
	

	/*Creating Threads:*/
	//---START---------------------------------------------------------------------------------------------------------------------------------------//
	for (i = 1; i <= num_of_threads; i++)
	{
		/* Prepare parameters for thread */
		(p_thread_params[i])->arr[0] = range_for_every_thread_array[i][0];
		(p_thread_params[i])->arr[1] = range_for_every_thread_array[i][1];
		(p_thread_params[i])->full_path_of_input = argv[1];
		(p_thread_params[i])->full_path_of_output = directory_with_output;
		(p_thread_params[i])->key = key;


		aThread[i] = CreateThread(
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE)ThreadProc,
			(LPVOID)  (p_thread_params[i]),        // here we need to give arguments
			0,          // default creation flags
			&ThreadID[i]); // receive thread identifier 

		if (aThread[i] == NULL)
		{
			//indicates if we have a problem with a thread
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}

	}
//---------------------------------END------------------------------------------------------------------------------------------------------//
	Sleep(2);

/*closing program:*/
//---START---------------------------------------------------------------------------------------------------------------------------------------//
	for (i = 1; i <= num_of_threads; i++)
	{
		CloseHandle(aThread[i]);
	}
	free(aThread);
	free(sizes_of_rows_array);
	free(rows_per_thread_array);
	for (int i = 1; i <= num_of_threads; i++)
		free(p_thread_params[i]);	
	free(p_thread_params);
	free(ThreadID);
	for (int i = 0; i <= num_of_threads; i++)
		free(range_for_every_thread_array[i]);

	return 0;
}




	