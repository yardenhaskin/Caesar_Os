const int STATUS_CODE_FAILURE = -1;
//Step Number One
main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Error, Invalid number of arguments\n");
		return STATUS_CODE_FAILURE;
	}
	open_output_file();
	open_input_text()
	{}
	find_input_size();

	for ()// on the input file
	{
		// if a-z
		decode_small_letters();
		//if A-Z
		decode_capital_letters();
		//if 0-9 
		decode_number();
		// other than that
		/*do nothing*/


		write_decoded_to_output();
	}
	close_files();
}