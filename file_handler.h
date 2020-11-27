#include<windows.h> 

#ifndef file_handler
#define file_handler
HANDLE open_output_file(char* directory_with_input, LONG input_file_size, char* directory_with_output, int dir_and_out_len, const char* directory);
void find_input_file_sizes(char* directory_with_input, int* p_num_of_rows, LONG* p_input_file_size);
HANDLE open_input_file(char* directory_with_input);
const char* extract_directory(char* directory_with_input);
void set_up_Directory_with_output(char* directory_with_input, LONG input_file_size, char* directory_with_output, int dir_and_out_len, const char* directory, char op);
HANDLE open_output_file_in_threads(char* directory_with_output);
#endif // file_handler
