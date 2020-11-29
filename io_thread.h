//io_thread.h
#ifndef __IO_THREAD_H__
#define __IO_THREAD_H__

// Includes --------------------------------------------------------------------

#include <windows.h>

// Types -----------------------------------------------------------------------
/*
* A type for thread parameters
//this type is an input for the threads, and contains arr-an array that
//contains start row and end row
//contains pathes for input and output file , contains key for encryption/decryption
//contains the semaphore object that was declared in main
*/ 
typedef struct
{
	//Thread should get only these arguments:
	int arr[2];
	char* full_path_of_input;
	char* full_path_of_output;
	int key;
	HANDLE ghSemaphore;
} IO_THREAD_params_t;

// Function Declarations -------------------------------------------------------

DWORD WINAPI ThreadProc(LPVOID);

#endif // __IO_THREAD_H__