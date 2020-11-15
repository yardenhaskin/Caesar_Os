
#include<stdio.h>
#include<stdlib.h>
#include"functions.h"
#include"file_handler.h"


const int STATUS_CODE_FAILURE = -1;
//Step Number One
main(int argc, char* argv[])
{

	FILE* filePointer;
	char ch;

	if (argc != 3)

	{
		printf("Error, Invalid number of arguments\n");
		return STATUS_CODE_FAILURE;
	}

	open_output_file(argv[1],argv[2]);//Edwar
	find_input_file_size(argv[1]);

	while ((ch = fgetc(filePointer)) != EOF) {
	//for ()// on the input file
	//{
		// if a-z
		//decode_small_letters(); //outputs char
		//if A-Z
		//decode_capital_letters();
		//if 0-9 
		//decode_number();
		// other than that
		/*do nothing*/
	//	write_decoded_to_output();//input char
		//{
	return 0;
}