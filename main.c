
#include <windows.h>
#include<stdio.h>
#include<stdlib.h>
#include"functions.h"
#include"file_handler.h"

HANDLE ghSemaphore;
DWORD WINAPI ThreadProc(LPVOID);

const int STATUS_CODE_FAILURE = -1;
//Step Number One
main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("Error, Invalid number of arguments\n");
		return STATUS_CODE_FAILURE;
	}

	int key = atoi(argv[2]);
	int num_of_threads= atoi(argv[3]);
	LONG input_file_size = find_input_file_size(argv[1]); //without EOF
	int num_of_rows= find_num_of_rows(argv[1]);
	HANDLE output_file_handle = open_output_file(argv[1], input_file_size);
	HANDLE input_file_handle = open_input_file(argv[1]);
	char* buffer = (char*)calloc(input_file_size, sizeof(char));
	OVERLAPPED ol = { 0 };
	OVERLAPPED ol2 = { 0 };
	
	int THREADCOUNT = 4;
	int indexes[2][2] = { {1,2}, {3,4} };
	//HANDLE aThread=(HANDLE*)malloc(sizeof(HANDLE)*THREADCOUNT);// at the moment I chose arbitrary
	//size of 4, but it actually needs to be allocated
	HANDLE aThread[4];
	//printf("%d", 6666);
	//printf("%d", sizeof(aThread));
	DWORD ThreadID;
	int i;

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
	ghSemaphore = CreateSemaphore(
		NULL,           // default security attributes
		THREADCOUNT,  // initial count
		THREADCOUNT,  // maximum count
		NULL);          // unnamed semaphore

	if (ghSemaphore == NULL)
	{
		printf("CreateSemaphore error: %d\n", GetLastError());
		return 1;
	}
	for (i = 0; i < THREADCOUNT; i++)
	{
		int* curPtr = indexes[i];
		aThread[i] = CreateThread(
			NULL,       // default security attributes
			0,          // default stack size
			(LPTHREAD_START_ROUTINE)ThreadProc,
			curPtr,       // here we need to give arguments
			0,          // default creation flags
			&ThreadID); // receive thread identifier

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
	WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, 20000);
	for (i = 0; i < THREADCOUNT; i++)
		CloseHandle(aThread[i]);

	//after all the handles were closed we need to close the semaphore handle
	CloseHandle(ghSemaphore);
	CloseHandle(output_file_handle);
	CloseHandle(input_file_handle);
	return 0;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{

	int arr[2] = &lpParam;

	DWORD dwWaitResult;
	BOOL bContinue = TRUE;

	while (bContinue)
	{
		// Try to enter the semaphore gate.

		dwWaitResult = WaitForSingleObject(
			ghSemaphore,   // handle to semaphore
			0L);           // zero-second time-out interval

		switch (dwWaitResult)
		{
			// The semaphore object was signaled.
		case WAIT_OBJECT_0:
			// TODO: Perform task
			printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
			bContinue = FALSE;

			// Simulate thread spending time on task
			Sleep(5);

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
		}
	}
	return TRUE;
}
