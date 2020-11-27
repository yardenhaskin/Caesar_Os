#include<stdio.h>
#include<stdlib.h>
#include<windows.h> 

//const int STATUS_CODE_FAILURE = -1;
static const int OUTPUT_FILE_NAME_LENGTH = 14;

const char* extract_directory(char* directory_with_input) // got to pay attention to that C may get \t as tab button and not t and \ */
{														  // you have to free(directory_only);
	int back_slash_counter = 0;

	for (int i = 0; i < strlen(directory_with_input); i++)
	{
		if (directory_with_input[i] == '\\')
			back_slash_counter++;
	}

	for (int i = 0; i < strlen(directory_with_input); i++)
	{

		if (directory_with_input[i] == '\\')
		{
			back_slash_counter -= 1;


		}
		if (back_slash_counter == 0)
		{
			int directory_only_len = i + 2;
			char* directory_only = (char*)calloc(directory_only_len + OUTPUT_FILE_NAME_LENGTH, sizeof(char));


			for (int j = 0; j < directory_only_len - 1; j++)
			{


			directory_only[j] = directory_with_input[j];

				if (j == (directory_only_len - 2))
				{
					directory_only[directory_only_len - 1] = '\0';
					return directory_only;
				}
			}

		}

	}


}
void find_input_file_sizes(char* directory_with_input, int* p_num_of_rows, LONG* p_input_file_size) // Be careful the last row may have EOF instead of   \n
{
	FILE* p_stream_input = NULL;
	errno_t retval_of_input;
	LONG input_file_size = *p_input_file_size;				//we assume we can store the num of char in LONG type
	int num_of_rows = *p_num_of_rows;		//we assume we can store the num of rows in int type
	retval_of_input = fopen_s(&p_stream_input, directory_with_input, "r");   //opens the txt folder written in the command line
	if (0 != retval_of_input)
	{
		printf("Failed to open file.\n");
		return -1;
	}
	char c;
	int flag;
	while ((c=fgetc(p_stream_input)) != EOF)
	{
		if (c == '\n' || c =='\t' )
		{
			num_of_rows += 1;
			input_file_size += 1;
			flag = 1;
		}
		else
		{
			input_file_size += 1;
			flag = 0;
		}
		
	}
	if (flag == 0) // This one is to check if the last line is without \n but with EOF
	{
		num_of_rows += 1;	
	}
	*p_input_file_size = input_file_size;
	*p_num_of_rows = num_of_rows;
	fclose(p_stream_input);
	//return input_file_size; //without EOF symbol
}
void set_up_Directory_with_output(char* directory_with_input, LONG input_file_size, char* directory_with_output, int dir_and_out_len, const char* directory, char op)
{
	LONG l_distance_to_move = input_file_size;
	errno_t retval_of_strcat1 = NULL, retval_of_strcat2 = NULL;
	const char* output_file_name_for_d = "decrypted.txt";
	const char* output_file_name_for_e = "encrypted.txt";
	if (op == 'd')
	{
		directory_with_output[0] = '\0';
		retval_of_strcat1 = strcat_s(directory_with_output, dir_and_out_len, directory);
		retval_of_strcat2 = strcat_s(directory_with_output, dir_and_out_len, output_file_name_for_d);

		if (0 != retval_of_strcat1 || 0 != retval_of_strcat2)
		{
			printf("Failed to strcat directory and output file name.\n");
			return;
		}
	}
	else
	{
		directory_with_output[0] = '\0';
		retval_of_strcat1 = strcat_s(directory_with_output, dir_and_out_len, directory);
		retval_of_strcat2 = strcat_s(directory_with_output, dir_and_out_len, output_file_name_for_e);

		if (0 != retval_of_strcat1 || 0 != retval_of_strcat2)
		{
			printf("Failed to strcat directory and output file name.\n");
			return;
		}
	}
	
}
HANDLE open_output_file(char* directory_with_input, LONG input_file_size, char* directory_with_output, int dir_and_out_len, const char* directory)
{
	LONG l_distance_to_move = input_file_size;

	//int int_key = atoi(key);
	//errno_t retval_of_strcat1 =NULL, retval_of_strcat2 = NULL;
	//const char* output_file_name = "decrypted.txt";
	//directory_with_output[0] = '\0';
	//retval_of_strcat1 = strcat_s(directory_with_output, dir_and_out_len, directory);
	//retval_of_strcat2 = strcat_s(directory_with_output, dir_and_out_len, output_file_name);

	//if (0 != retval_of_strcat1 || 0 != retval_of_strcat2)
	//{
	//	printf("Failed to strcat directory and output file name.\n");
	//	/*Need to free files,heap ...*/
	//	return NULL;
	//}
	LPCSTR lp_output_file_name = (LPCSTR)directory_with_output;
	HANDLE output_file_handle =CreateFileA(lp_output_file_name, GENERIC_ALL,
		(FILE_SHARE_READ | FILE_SHARE_WRITE),
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	SetFilePointer(output_file_handle, l_distance_to_move + 1, NULL, FILE_BEGIN); // +1 to not lose the last letter
	SetEndOfFile(output_file_handle);
	SetFilePointer(output_file_handle, 0, NULL, FILE_BEGIN);
	return output_file_handle;
	
}

HANDLE open_input_file(char* directory_with_input)
{

	LPCSTR lp_input_file_name = (LPCSTR)directory_with_input;

	HANDLE input_file_handle = CreateFileA(lp_input_file_name,		//The name of the file to be created or opened
		GENERIC_READ,												//The requested access to the file 
		FILE_SHARE_READ,											//The requested sharing mode of the file 
			NULL,													//no security
			OPEN_EXISTING,											//Opens a file or device, only if it exists
			FILE_ATTRIBUTE_NORMAL,									//FILE_ATTRIBUTE_NORMAL being the most common default value for files.
			NULL);													//This parameter can be NULL.When opening an existing file CreateFile ignores this parameter.

	return input_file_handle;
	//CloseHandle(input_file_handle);
}

HANDLE open_output_file_in_threads(char* directory_with_output)
{
	LPCSTR lp_output_file_name = (LPCSTR)directory_with_output;
	HANDLE output_file_handle = CreateFileA(lp_output_file_name, GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	return output_file_handle;
	//	CloseHandle(output_file_handle);
}