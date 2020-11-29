
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"functions.h"
#include"file_handler.h"
#include "io_Thread.h"

const int STATUS_CODE_FAILURE = -1;
static const int OUTPUT_FILE_NAME_LENGTH = 14;
HANDLE* ghSemaphore;
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


	int key = atoi(argv[2]);								// getting the key as integer

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
	char* directory_with_output = NULL;
	const char* directory = extract_directory(argv[1]);
	int dir_and_out_len = strlen(directory) + OUTPUT_FILE_NAME_LENGTH;
	directory_with_output = (char*)malloc((sizeof(char)) * dir_and_out_len);
	set_up_Directory_with_output(argv[1], input_file_size, directory_with_output, dir_and_out_len, directory,op);				//initializing the output file directory
	int num_of_threads =  atoi(argv[3]);
	int* rows_per_thread_array = (int*)calloc(num_of_threads + 1, sizeof(int));
	if (rows_per_thread_array == NULL)
	{
		printf("Memory allocation failed");
		return STATUS_CODE_FAILURE;
	}
	int** range_for_every_thread_array = (int**)malloc(num_of_threads * sizeof(int*));
	if (range_for_every_thread_array == NULL)
	{
		printf("Memory allocation failed");
		return STATUS_CODE_FAILURE;
	}
	start_end_thread_array(num_of_rows, num_of_threads, rows_per_thread_array, range_for_every_thread_array);
	start_end_thread_array_in_chars(sizes_of_rows_array, range_for_every_thread_array,num_of_threads); // Updates range_for_every_thread_array to chars counting instead of rows.
	//---------------------------------END------------------------------------------------------------------//

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

	HANDLE* a_Thread = (HANDLE*)malloc(sizeof(HANDLE) * (num_of_threads + 1));
	if (NULL == a_Thread)
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
	ghSemaphore = CreateSemaphore(
		NULL,           // default security attributes
		num_of_threads,  // initial count
		num_of_threads,  // maximum count
		NULL);          // unnamed semaphore

	if (ghSemaphore == NULL)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return 1;
	}
	for (i = 1; i <= num_of_threads; i++)
	{
		/* Prepare parameters for thread */
		(p_thread_params[i])->arr[0] = range_for_every_thread_array[i][0];
		(p_thread_params[i])->arr[1] = range_for_every_thread_array[i][1];
		(p_thread_params[i])->full_path_of_input = argv[1];
		(p_thread_params[i])->full_path_of_output = directory_with_output;
		(p_thread_params[i])->key = key;
		(p_thread_params[i])->ghSemaphore = ghSemaphore;

		a_Thread[i] = CreateThread(
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE)ThreadProc,
			(LPVOID)  (p_thread_params[i]),        // here we need to give arguments
			0,          // default creation flags
			&ThreadID[i]); // receive thread identifier 

		if (a_Thread[i] == NULL)
		{
			//indicates if we have a problem with a thread
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}

	}
//---------------------------------END------------------------------------------------------------------------------------------------------//
	Sleep(2);
	HANDLE* subset = (HANDLE*)malloc((num_of_threads) * sizeof(HANDLE));  
	if (NULL == subset)
	{
		printf("Error when allocating memory");
		return STATUS_CODE_FAILURE;
	}
	///due to problems with the initial implementation we created a new
	//array to have valid handles
	for (int j = 1; j < num_of_threads+1; j++)
		subset[j-1] = a_Thread[j];

	//waitForMultipleObject got 20000[ms] since that was defined as a 
	//reasonabe time to wait
	WaitForMultipleObjects(num_of_threads, subset, TRUE, 20000);
	//check "waitforMultipleObjects"://printf("WaitForMultipleObjects: %d\n", GetLastError());
/*closing program:*/
//---START---------------------------------------------------------------------------------------------------------------------------------------//
	for (i = 0; i < num_of_threads; i++)
	{
		CloseHandle(subset[i]);
	}
	CloseHandle(ghSemaphore);
	free(a_Thread);
	free(subset);
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




	