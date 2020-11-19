#include <stdio.h>


char decode_small_letters(char letter, int key) {
	return ('a' + (letter - 'a' - key) % 26);
}
char decode_capital_letters(char letter, int key) {
	return  ('A' + (letter - 'A' - key) % 26);
}
char decode_number(char letter, int key) {
	return ('0' + (letter - '0' - key) % 10);
}

char encode_small_letters(char letter, int key) {
	return ('a' + (letter - 'a' + key) % 26);
}
char encode_capital_letters(char letter, int key) {
	return  ('A' + (letter - 'A' + key) % 26);
}
char encode_number(char letter, int key) {
	return ('0' + (letter - '0' + key) % 10);
}
char decode(char letter, int key) {
	if (letter >= 'a' && letter <= 'z')
		return decode_small_letters(letter, key);
	else if (letter >= 'A' && letter <= 'Z')
		return decode_capital_letters(letter, key);
	else if (letter >= '0' && letter <= '9')
		return decode_number(letter, key);
	return letter;
}

char encode(char letter, int key) {
	if (letter >= 'a' && letter <= 'z')
		return encode_small_letters(letter, key);
	else if (letter >= 'A' && letter <= 'Z')
		return encode_capital_letters(letter, key);
	else if (letter >= '0' && letter <= '9')
		return encode_number(letter, key);
	return letter;
}

int getNumberOfLines(FILE *filePointer) {
	char c;
	int count = 0;
	for (c = getc(filePointer); c != EOF; c = getc(filePointer)) {
		if (c == '\n') // Increment count if this character is newline 
			count += 1;
	}
	return count;
}