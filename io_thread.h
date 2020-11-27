//io_thread.h
#ifndef __IO_THREAD_H__
#define __IO_THREAD_H__

// Includes --------------------------------------------------------------------

#include <windows.h>

// Types -----------------------------------------------------------------------
/*
* A type for thread parameters
*/
typedef struct
{
	//Thread should get only these arguments:
	int arr[2];
	char* full_path_of_input;
	char* full_path_of_output;
	int key;
} IO_THREAD_params_t;

// Function Declarations -------------------------------------------------------

DWORD WINAPI ThreadProc(LPVOID);

#endif // __IO_THREAD_H__