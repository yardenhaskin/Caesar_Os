
#include <stdio.h>
#include<Windows.h>
#ifndef functions
#define functions
char decode(char letter, int key);
char decode_small_letters(char letter, int key);
char decode_capital_letters(char letter, int key);
char decode_number(char letter, int key);
char encode(char letter, int key);
char encode_small_letters(char letter, int key);
char encode_capital_letters(char letter, int key);
char encode_number(char letter, int key);
int getNumberOfLines(FILE *filePointer);
#endif // functions
