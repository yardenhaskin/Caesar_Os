
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"functions.h"
#include"file_handler.h"
#include "io_Thread.h"

HANDLE ghSemaphore; // maybe it is better to initialize it on heap as said in recitation 4
DWORD WINAPI ThreadProc(LPVOID);

const int STATUS_CODE_FAILURE = -1;
static const int OUTPUT_FILE_NAME_LENGTH = 14;

main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("Error, Invalid number of arguments\n");
		return STATUS_CODE_FAILURE;
	}

	int key = atoi(argv[2]);														// getting the key as integer

	//---START---------------------------------------------------------------------------------------------------------------------------------------//
	int num_of_rows = 0;
	int* p_num_of_rows = &num_of_rows;
	LONG input_file_size = 0;
	LONG* p_input_file_size = &input_file_size;
	find_input_file_sizes(argv[1], p_num_of_rows, p_input_file_size);	// initializes the number of rows and number of chars in input file

	//---END----------------------------------------------------------------------------------------------------------------------------------------------//

	//---START---------------------------------------------------------------------------------------------------------------------------------------//
	int* sizes_of_rows_array = (int*)calloc(num_of_rows, sizeof(int));			
	make_sizes_of_rows_array(argv[1], sizes_of_rows_array); //function that makes an array that array[i]=(size of row i in chars)
	//---END----------------------------------------------------------------------------------------------------------------------------------------------//

	//---START---------------------------------------------------------------------------------------------------------------------------------------//
	char* directory_with_output = NULL;
	const char* directory = extract_directory(argv[1]);

	int dir_and_out_len = strlen(directory) + OUTPUT_FILE_NAME_LENGTH;
	directory_with_output = (char*)malloc((sizeof(char)) * dir_and_out_len);
	set_up_Directory_with_output(argv[1], input_file_size, directory_with_output, dir_and_out_len, directory);				//initializing the output file directory
	HANDLE output_file_handle = open_output_file(argv[1], input_file_size,directory_with_output,dir_and_out_len,directory); //initializing the output file,
	HANDLE input_file_handle = open_input_file(argv[1]);							// opening input file for threads to read
	//---END----------------------------------------------------------------------------------------------------------------------------------------------//

	

	//Initializing an array of 2d arrays 
	//---------------------------------START--------------------------------------------------------------//
	// range_for_every_thread_array[0] array is always [0,0] , 
	//range_for_every_thread_array[i]=[start,end] shows the start and end row that thread i must read/write to
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

	//---START---------------------------------------------------------------------------------------------------------------------------------------//
	IO_THREAD_params_t* p_thread_params;
	p_thread_params = (IO_THREAD_params_t*)malloc(sizeof(IO_THREAD_params_t));
	if (NULL == p_thread_params)
	{
		printf("Error when allocating memory");
		return STATUS_CODE_FAILURE;
	}
	//---END----------------------------------------------------------------------------------------------------------------------------------------------//




	//---START---------------------------------------------------------------------------------------------------------------------------------------//
	//(((((((((((((T h e s e    m a d e    b y      Y a r d e n))))))))))))
	//HANDLE aThread=(HANDLE*)malloc(sizeof(HANDLE)*num_of_threads);// at the moment I chose arbitrary
	//size of 4, but it actually needs to be allocated
	HANDLE* aThread[10];
	//= (HANDLE*)malloc(sizeof(HANDLE) * num_of_threads);
	DWORD ThreadID;
	int i;

	
	//---------------------------------END------------------------------------------------------------------//

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
	for (i = 1; i <= num_of_threads; i++) // (should fix this to i=1 --> num_of_threads, because the implementation of  range_for_every_thread_array)
	{
		//int* curPtr = range_for_every_thread_array[i];

		/* Prepare parameters for thread */
		////this is not working at the moment. I get arr[1] =0, arr[0] =1
		p_thread_params->arr[0] = range_for_every_thread_array[i][0];
		//printf("%d", p_thread_params->arr[0]);
		p_thread_params->arr[1] = range_for_every_thread_array[i][1];
		//printf("%d", p_thread_params->arr[1]);
		p_thread_params->full_path_of_input = argv[1];
		p_thread_params->full_path_of_output = directory_with_output;
		p_thread_params->key = key;
		p_thread_params->input_file_size = input_file_size;
		//////
		//strcpy_s(p_thread_params->directory_with_output, input_file_size, directory_with_output);
		//p_thread_params->dir_and_out_len = dir_and_out_len;
		//strcpy_s(p_thread_params->directory, input_file_size, directory);

		aThread[i] = CreateThread(
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE)ThreadProc,
			p_thread_params,        // here we need to give arguments
			0,          // default creation flags
			&ThreadID); // receive thread identifier (shouldn't this be different for every thread???)

		if (aThread[i] == NULL)
		{
			//indicates if we have a problem with a thread
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}
	}
	// Wait for all threads to terminate.arguments :
	//first argument : The number of object handles in the array pointed to by lpHandle
	//second argument: An array of object handles
	//third argument:If this parameter is TRUE, the function returns when the state of all objects in the lpHandles array is signaled
	//forth argument :The time-out interval, in milliseconds. was chosen arbitrary
	WaitForMultipleObjects(num_of_threads, aThread, TRUE, INFINITE);
	printf("WaitForMultipleObjects error: %d\n", GetLastError());
	for (i = 1; i <= num_of_threads; i++)
		CloseHandle(aThread[i]);

	//after all the handles were closed we need to close the semaphore handle
	CloseHandle(ghSemaphore);
	CloseHandle(output_file_handle);
	CloseHandle(input_file_handle);
	return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	DWORD dwWaitResult;
	BOOL bContinue = TRUE;

	while (bContinue)
	{
		// Try to enter the semaphore gate.

		dwWaitResult = WaitForSingleObject(
			ghSemaphore,   // handle to semaphore
			0L);           // zero-second time-out interval
	
		IO_THREAD_params_t* p_params;
		p_params = (IO_THREAD_params_t*)lpParam;
		//HANDLE input_file= (HANFLE*)
		int key = p_params->key;
		int input_file_size = p_params->input_file_size;
		int start_char = p_params->arr[0];
		int end_char = p_params->arr[1];
		if (start_char >= end_char) {
			break;
		}
		char* path_of_input = p_params->full_path_of_input;
		char* path_of_output = p_params->full_path_of_output;
		//int length = end_char - start_char + 1;
		//int length = (start_char-'0') - (end_char-'0') + 1;
		//printf("%d", length);
		char* directory_with_output =p_params-> directory_with_output;
		int dir_and_out_len =p_params-> dir_and_out_len;
		const char* directory = p_params->directory;
		HANDLE input_file_handle = open_input_file(path_of_input);

		switch (dwWaitResult)
		{
			// The semaphore object was signaled.
		case WAIT_OBJECT_0:
			SetFilePointer(input_file_handle, start_char, NULL, 1);

			//Reading the input file:
	//---------------------------------START--------------------------------------------------------------//
			char* buffer = (char*)calloc(input_file_size, sizeof(char)); // initializing the Buffer
			OVERLAPPED ol = { 0 };

			if (FALSE == ReadFile(input_file_handle,					//A handle to the  file
				buffer,							//A pointer to the buffer that receives the data read from a file 
				input_file_size,						//The maximum number of bytes to be read.				
				&ol,									//A pointer to the variable that receives the number of bytes read when using a synchronous hFile parameter.
				NULL))								//A pointer to an OVERLAPPED structure is required if the hFile parameter was opened with FILE_FLAG_OVERLAPPED, otherwise it can be NULL.
			{
				printf("readFile error: %d\n", GetLastError()); // you can make this better by using GetLastError()
				return STATUS_CODE_FAILURE;
			}
			//---------------------------------END------------------------------------------------------------------//

			//printf("%s", buffer);

			//Decoding:
			printf("%c", decode(buffer[0], key));
			//---------------------------------START--------------------------------------------------------------//
			for (int i = start_char; i <= end_char; i++)
			{
				char decodedChar = decode(buffer[i], key);
				printf("%c", decodedChar);
				buffer[i] = decodedChar;
			}
			printf("%s", buffer);
			//---------------------------------END------------------------------------------------------------------//

			//writing to the output file:
			//---------------------------------START--------------------------------------------------------------//
			printf("we are in a handle");
			OVERLAPPED ol2 = { 0 };
			//HANDLE output_file_handle = open_output_file(path_of_input, input_file_size,directory_with_output,dir_and_out_len,directory); // this function is used only by the main thread to initialize the output file as instructed in the recitation
			//if (FALSE == WriteFile(
			//	output_file_handle,
			//	buffer,
			//	input_file_size,
			//	&ol2,
			//	NULL))
			//{
			//	printf("WriteFile error: %d\n", GetLastError()); // you can make this better by using GetLastError()
			//	return STATUS_CODE_FAILURE;
			//}	

			printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
			bContinue = FALSE;

			// Simulate thread spending time on task
			//Sleep(5);

			// Release the semaphore when task is finished

			if (!ReleaseSemaphore(
				ghSemaphore,  // handle to semaphore
				1,            // increase count by one
				NULL))       // not interested in previous count
			{
				printf("ReleaseSemaphore error: %d\n", GetLastError());
			}
			break;

			// The semaphore was nonsignaled, so a time-out occurred.
		case WAIT_TIMEOUT:
			printf("Thread %d: wait timed out\n", GetCurrentThreadId());
			break;
		default:
			printf("WaitForSingleObject error: %d\n", GetLastError());
			printf("%d", dwWaitResult);
			break;
		}
	}
	return TRUE;
}
