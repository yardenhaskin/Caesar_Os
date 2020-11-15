#include<windows.h> 

#ifndef file_handler
#define file_handler
int open_output_file(char* directory_with_input, char* key);
LONG find_input_file_size(char* directory_with_input);
#endif // file_handler
