// By Pytel
/*
Based on code by:
@maxcountryman
@himehowareu
@D0tty
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OK 0
#define NOK -1

// help
void help(char *path_to_interpreter)
{
	printf("Usage: %s file.bf\n", path_to_interpreter);
	exit(2);
}

/* 
 * Returns the size of the file in bytes
 */
long int file_size(FILE *file)
{
	fseek(file, 0L, SEEK_END);
	long int file_len = ftell(file);
	rewind(file);
	return file_len;
}

/*
 * Reads the file and returns the size of the file in bytes
 * Returns -1 on error
 */
int read_file(char *path, char **buffer)
{
	FILE *file = fopen(path, "r");
	if (file == NULL)
	{
		return NOK;
	}

	long file_len = file_size(file);

	char *file_buf = malloc(file_len * sizeof(char) + 1);
	file_buf[file_len] = '\0';

	fread(*buffer, file_len, sizeof(char), file);
	fclose(file);

	return file_len;
}

char *add_cell(char *tape, size_t *tape_size)
{
	char *tmp = (char *)realloc(tape, (++*tape_size) * sizeof(char));

	if (tmp == NULL)
	{
		printf("FATAL: Could not reallocate tape memory!");
		exit(1);
	}

	if (!tape[*tape_size - 1])
	{
		tape[*tape_size - 1] = 0;
	}

	return tmp;
}

char *remove_cell(char *tape, size_t *tape_size, char *ptr)
{
	char tail = tape[*tape_size - 1];

	// if the cell is zeroed and not pointed to currently
	if (tail == 0 && ptr != &tape[*tape_size - 1] && *tape_size > 0)
	{
		char *tmp = (char *)realloc(tape, (--*tape_size) * sizeof(char));
		if (tmp == NULL)
		{
			printf("FATAL: Could not reallocate tape memory!");
			exit(1);
		}
		return tmp;
	}
	return tape;
}

void interpret(char *input)
{
	size_t tape_size = 1;
	char *tape;
	char *ptr;
	long int index = 0;

	char current_char;
	size_t i;
	size_t loop;

	tape = (char *)calloc(tape_size, sizeof(char));
	ptr = tape;

	for (i = 0; input[i] != 0; i++)
	{
		current_char = input[i];
		if (current_char == '>')
		{
			++index;
			if (index >= tape_size)
			{
				tape = add_cell(tape, &tape_size);
			}
			++ptr;
		}
		else if (current_char == '<')
		{
			--index;
			--ptr;
		}
		else if (current_char == '+')
		{
			++*ptr;
		}
		else if (current_char == '-')
		{
			--*ptr;
		}
		else if (current_char == '.')
		{
			putchar(*ptr);
		}
		else if (current_char == ',')
		{
			*ptr = getchar();
		}
		else if (current_char == '[' && *ptr == 0)
		{ // jump behind loop
			loop = 1;
			while (loop > 0)
			{
				current_char = input[++i];
				if (current_char == '[')
				{
					loop++;
				}
				else if (current_char == ']')
				{
					loop--;
				}
			}
		}
		else if (current_char == '[')
		{ // do loop
			continue;
		}
		else if (current_char == ']' && *ptr)
		{ // repeat loop
			loop = 1;
			while (loop > 0)
			{
				current_char = input[--i];
				if (current_char == '[')
				{
					loop--;
				}
				else if (current_char == ']')
				{
					loop++;
				}
			}
		}
		else
		{ // random chars and comments
			continue;
		}
		// garbage collect
		tape = remove_cell(tape, &tape_size, ptr);
	}
	free(tape);
}

int main(int argc, char *argv[])
{
	// check args
	if (argc != 2)
	{
		help(argv[0]);
	}

	// load file
	char *file_buf = NULL;
	long file_len = read_file(argv[1], &file_buf);
	if (file_len <= 0 )
	{
		printf("Error: fopen failed\n");
		exit(1);
	}

	// interpret
	interpret(file_buf); // outputs input

	// free memory
	free(file_buf);
	return 0;
}

/*
END
*/
