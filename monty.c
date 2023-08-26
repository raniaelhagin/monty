#include "monty.h"

stack_t *head = NULL;

/**
 * main - entry point
 * @argc: argments count
 * @argv: argumnts values
 * Return: 0 in success
 */
int main(int argc, char *argv[])
{
	char *file_name;
	FILE *file_descriptor;
	int cmd_num, format = 0;
	char *buf = NULL;
	size_t len = 0;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}

	file_name = argv[1];
	file_descriptor = fopen(file_name, "r");
	if (file_name == NULL || file_descriptor == NULL)
	{
		err(2, file_name);
	}

	cmd_num = 1;
	while (getline(&buf, &len, file_descriptor) != -1)
	{
		format = parse_cmd(buf, cmd_num, format);
		cmd_num++;
	}
	free(buf);

	return (0);
}

/**
 * parse_cmd - parse command line
 * @buffer: command line
 * @command_num: command number
 * @format: 0 for stack and 1 for queue
 * Return: 0 if the opcode is stack and 1 if it's queue
 */
int parse_cmd(char *buffer, int command_num, int format)
{
	char *opcode, *value;
	const char *delim = "\n";

	if (buffer == NULL)
	{
		err(4);
	}

	opcode = strtok(buffer, delim);
	if (opcode == NULL)
	{
		return (format);
	}
	value = strtok(NULL, delim);

	if (strcmp(opcode, "stack") == 0)
	{
		return (0);
	}
	if (strcmp(opcode, "queue") == 0)
	{
		return (1);
	}

	get_opcode(opcode, value, command_num, format);
	return (format);
}


/**
 * get_opcode - get the opcode function
 * @opcode: the opcode
 * @value: values of the opcode
 * @cmd_number: command line number
 * @format: 0 if stack and 1 if queue
 * Return: void
 */
void get_opcode(char *opcode, char *value, int cmd_number, int format)
{
	int i;
	int flag;
	instruction_t opcode_funs[] = {
		{"push", push_to_stack},
		{"pall", print_stack}
	}

	if (opcode[0] == '#')
		return;

	i = 0;
	flag = 1;
	while (opcode_funs[i].opcode != NULL)
	{
		if (strcmp(opcode, opcode_funs[i].opcode) == 0)
		{
			get_func(opcode_funs[i], opcode, value, cmd_number, format);
			flag = 0;
		}

		if (flag == 1)
		{
			err(3, cmd_number, opcode);
		}

		i++;
	}
}


void get_func(opcode_f f, char *op, char *val, int cmd, int format)
{
	stack_t *node;
	int flag, i;

	flag = 1;
	if (strcmp(op, "push") == 0)
	{
		if (val == NULL)
		{
			err(5, cmd);
		}

		if (val != NULL && val[0] == '-')
		{
			val = val + 1;
			flag = -1;
		}

		i = 0;
		while (val[i] != '\0')
		{
			if (isdigit(val[i]) == 0)
			{
				err(5, cmd);
			}
			i++;
		}

		node = create_node(atoi(val) * flag);
		if (format == 0)
		{
			f(&node, cmd);
		}
		if (format == 1)
		{
			push_to_queue(&node, cmd);
		}
		else
		{
			f(&head, cmd);
		}

	}
}