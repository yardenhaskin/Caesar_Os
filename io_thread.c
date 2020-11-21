// io_thread.c

// Includes --------------------------------------------------------------------

#include <windows.h>
#include "io_Thread.h"

// Function Definitions --------------------------------------------------------
/*
DWORD WINAPI MathThread(LPVOID lpParam)
{
	IO_THREAD_params_t* p_params;

	// Check if lpParam is NULL
	if (NULL == lpParam)
	{
		return MATH_THREAD__CODE_NULL_PTR;
	}

	
	//* Convert (void *) to parameters type.
	//* In this example, MATH_THREAD_params_t is a simple struct.
	//* In general, it could be any type:
	//* integer, array, array of struct, string etc.
	
	p_params = (MATH_THREAD_params_t*)lpParam;

	p_params->res = p_params->num1 + p_params->num2;

	return MATH_THREAD__CODE_SUCCESS;
}
*/