#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// initialize the tape with 30,000 zeroes
unsigned char tape[30000] = {0};

// set the pointer to point at the left-most cell of the tape
unsigned char* ptr = tape;


void interpret(char* input) {
	char current_char;
	size_t i;
	size_t loop;

	for (i = 0; input[i] != 0; i++) {
		current_char = input[i];
		if (current_char == '>') {
			++ptr;
		} else if (current_char == '<') {
			--ptr;
		} else if (current_char == '+') {
			++*ptr;
		} else if (current_char == '-') {
			--*ptr;
		} else if (current_char == '.' ) {
			putchar(*ptr);
		} else if (current_char == ',') {
			*ptr = getchar();
		} else if (current_char == '[' && *ptr == 0) { 	// jump behind loop
			loop = 1; 
			while (loop > 0) { 
				current_char = input[++i]; 
				if (current_char == '[') {
					loop++;
				} else if (current_char == ']') {
					loop--;
				}
			}
		} else if (current_char == '[') {			// do loop
			continue;
		} else if (current_char == ']' && *ptr) {	// repeat loop
			loop = 1;
			while (loop > 0) {
				current_char = input[--i];
				if (current_char == '[') {
					loop--;
				} else if (current_char == ']') {
					loop++;
				}
			}
		} else {			// random chars and comments
			continue
		}
	}
}


int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Usage: %s file.bf\n", argv[0]);
    exit(2);
  }

  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("Error: fopen failed\n");
    exit(1);
  }

  fseek(file, 0L, SEEK_END);
  long file_len = ftell(file);
  rewind(file);

  char *file_buf = malloc(file_len * sizeof(char) + 1);
  file_buf[file_len] = '\0';

  fread(file_buf, file_len, sizeof(char), file);
  fclose(file);

  interpret(file_buf);  // outputs input

  free(file_buf);
  return 0;
}

