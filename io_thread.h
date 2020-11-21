//io_thread.h
#ifndef __IO_THREAD_H__
#define __IO_THREAD_H__

// Includes --------------------------------------------------------------------

#include <windows.h>

// Types -----------------------------------------------------------------------

/*
* A type for thread return codes
*/
//typedef enum
//{
//	MATH_THREAD__CODE_SUCCESS,
//	MATH_THREAD__CODE_NULL_PTR,
//} MATH_THREAD__return_code_t;

/*
* A type for thread parameters
*/
typedef struct
{
	int arr[2];
	HANDLE* output_file_handle;
} IO_THREAD_params_t;

// Function Declarations -------------------------------------------------------

/*
* Math thread. This thread adds up to numbers.
*/
//DWORD WINAPI MathThread(LPVOID lpParam);

#endif // __IO_THREAD_H__