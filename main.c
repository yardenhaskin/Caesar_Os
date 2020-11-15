#include <stdio.h>
#include <stdlib.h>

const int STATUS_CODE_FAILURE = -1;
//Step Number One
main(int argc, char* argv[])
{
	FILE* filePointer;
	char ch;
	if (argc != 2)
	{
		printf("Error, Invalid number of arguments\n");
		return STATUS_CODE_FAILURE;
	}
	open_output_file();//Edwar
	open_input_text();
	find_input_size();
	//////


	/////
	///Yarden
	while ((ch = fgetc(filePointer)) != EOF) {

	}
	{
		

		write_decoded_to_output();//input char
	}

	close_files();//Edwar
}