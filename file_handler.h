#include<windows.h> 

#ifndef file_handler
#define file_handler
HANDLE open_output_file(char* directory_with_input, LONG input_file_size);
LONG find_input_file_size(char* directory_with_input);
HANDLE open_input_file(char* directory_with_input);
#endif // file_handler
