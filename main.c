#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include <inttypes.h>
#define MAX_LINE 256

int add_last(void **arr, int *len, data_structure *data);

int add_at(void **arr, int *len, data_structure *data, int index);

void find(void *data_block, int len, int index);

int delete_at(void **arr, int *len, int index);

data_structure *parse_insert_input(char *token);

void create_type(data_structure *result, unsigned int first_size, unsigned int second_size, char *token);

void print(void *arr, int len);

void free_array(void *array, int len);

int main()
{
	// the vector of bytes u have to work with
	// good luck :)
	void *arr = NULL;
	int len = 0;
	char *command = malloc(MAX_LINE);
	if (!command)
	{
		printf("Cant alloc memory.\n");
		exit(-1);
	}
	fgets(command, MAX_LINE, stdin);
	command[strlen(command) - 1] = '\0';
	arr = malloc(sizeof(data_structure));
	if (!arr)
	{
		printf("Cant alloc memory.\n");
		exit(-1);
	}
	while (strncmp(command, "exit", 4))
	{
		char *token = strtok(command, " ");
		if (!strncmp(token, "insert_at", 9))
		{
			token = strtok(NULL, " ");
			int index = atoi(token);
			data_structure *input = NULL;
			input = parse_insert_input(token);
			if (!add_at(&arr, &len, input, index))
				len++;
		}
		else if (!strncmp(token, "insert", 6))
		{
			data_structure *input = NULL;
			input = parse_insert_input(token);
			if (!add_last(&arr, &len, input))
				len++;
		}

		else if (!strncmp(token, "find", 4))
		{
			token = strtok(NULL, " ");
			int index = atoi(token);
			find(arr, len, index);
		}
		else if (!strncmp(token, "delete_at", 9))
		{
			token = strtok(NULL, " ");
			int index = atoi(token);
			if (!delete_at(&arr, &len, index))
				len--;
		}
		else if (!strncmp(token, "print", 5))
		{
			print(arr, len);
		}

		fgets(command, MAX_LINE, stdin);
		command[strlen(command) - 1] = '\0';
	}

	free_array(arr, len);
	free(command);
	return 0;
}

int add_at(void **arr, int *len, data_structure *data, int index)
{
	if (index > *len)
		index = *len;
	*arr = realloc(*arr, ((*len) + 1) * sizeof(data_structure));
	if (!(*arr))
	{
		printf("Cant realloc memory.\n");
		return 1;
	}
	data_structure *start, *end;
	if (index >= 0)
	{
		start = (data_structure *)(*arr + index * sizeof(data_structure));
		end = (data_structure *)(*arr + (index + 1) * sizeof(data_structure));
	}
	else
	{
		printf("Index out of bounds.\n");
		return 1;
	}
	memcpy(end, start, (*len - index) * sizeof(data_structure));

	start->header = malloc(sizeof(head));
	start->data = malloc(data->header->len);

	start->header->type = data->header->type;
	start->header->len = data->header->len;

	memcpy(start->data, data->data, data->header->len);
	free(data->data);
	free(data->header);
	free(data);
	return 0;
}

data_structure *parse_insert_input(char *token)
{
	data_structure *result = malloc(sizeof(data_structure));
	result->header = malloc(sizeof(head));

	token = strtok(NULL, " ");
	result->header->type = atoi(token);

	if (!result || !result->header)
	{
		printf("Cant alloc memory.\n");
		return NULL;
	}

	if (result->header->type == 1)
	{
		create_type(result, sizeof(int8_t), sizeof(int8_t), token);
	}
	else if (result->header->type == 2)
	{
		create_type(result, sizeof(int16_t), sizeof(int32_t), token);
	}
	else if (result->header->type == 3)
	{
		create_type(result, sizeof(int32_t), sizeof(int32_t), token);
	}

	return result;
}

void create_type(data_structure *result, unsigned int first_size, unsigned int second_size, char *token)
{

	token = strtok(NULL, " ");
	char *name1 = malloc(strlen(token) + 1);
	strcpy(name1, token);

	token = strtok(NULL, " ");
	int32_t bill1 = atoi(token);

	token = strtok(NULL, " ");
	int32_t bill2 = atoi(token);

	token = strtok(NULL, " ");
	char *name2 = malloc(strlen(token) + 1);
	strcpy(name2, token);

	result->data = malloc(first_size + second_size + strlen(name1) + strlen(name2) + 2);
	int offset = 0;

	memcpy(result->data + offset, name1, strlen(name1) + 1);
	offset += strlen(name1) + 1;

	memcpy(result->data + offset, &bill1, first_size);
	offset += first_size;

	memcpy(result->data + offset, &bill2, second_size);
	offset += second_size;

	memcpy(result->data + offset, name2, strlen(name2) + 1);
	offset += strlen(name2) + 1;

	result->header->len = offset;

	free(name1);
	free(name2);
}

int add_last(void **arr, int *len, data_structure *data)
{
	*arr = realloc(*arr, ((*len) + 1) * sizeof(data_structure));
	if (!(*arr))
	{
		printf("Cant realloc memory.\n");
		return 1;
	}

	((data_structure *)(*arr + (*len) * sizeof(data_structure)))->header = malloc(sizeof(head));
	((data_structure *)(*arr + (*len) * sizeof(data_structure)))->data = malloc(data->header->len);

	((data_structure *)(*arr + (*len) * sizeof(data_structure)))->header->type = data->header->type;
	((data_structure *)(*arr + (*len) * sizeof(data_structure)))->header->len = data->header->len;

	memcpy(((data_structure *)(*arr + (*len) * sizeof(data_structure)))->data, data->data, data->header->len);
	free(data->data);
	free(data->header);
	free(data);
	return 0;
}

void print(void *arr, int len)
{
	for (int i = 0; i < len; i++)
	{
		printf("Tipul %d\n", ((data_structure *)arr)[i].header->type);
		if (((data_structure *)arr)[i].header->type == 1)
		{
			int offset = strlen(((data_structure *)arr)[i].data) + 2 * sizeof(int8_t) + 1;
			printf("%s pentru %s\n", (char *)((data_structure *)arr)[i].data, (char *)(((data_structure *)arr)[i].data + offset));
			offset = strlen(((data_structure *)arr)[i].data) + 1;
			printf(P_TYPE_1, *((int8_t *)(((data_structure *)arr)[i].data + offset)), *((int8_t *)(((data_structure *)arr)[i].data + offset + sizeof(int8_t))));
		}
		else if (((data_structure *)arr)[i].header->type == 2)
		{
			int offset = strlen(((data_structure *)arr)[i].data) + sizeof(int16_t) + sizeof(int32_t) + 1;
			printf("%s pentru %s\n", (char *)((data_structure *)arr)[i].data, (char *)(((data_structure *)arr)[i].data + offset));
			offset = strlen(((data_structure *)arr)[i].data) + 1;
			printf(P_TYPE_2, *((int16_t *)(((data_structure *)arr)[i].data + offset)), *((int32_t *)(((data_structure *)arr)[i].data + offset + sizeof(int16_t))));
		}
		else if (((data_structure *)arr)[i].header->type == 3)
		{
			int offset = strlen(((data_structure *)arr)[i].data) + 2 * sizeof(int32_t) + 1;
			printf("%s pentru %s\n", (char *)((data_structure *)arr)[i].data, (char *)(((data_structure *)arr)[i].data + offset));
			offset = strlen(((data_structure *)arr)[i].data) + 1;
			printf(P_TYPE_3, *((int32_t *)(((data_structure *)arr)[i].data + offset)), *((int32_t *)(((data_structure *)arr)[i].data + offset + sizeof(int32_t))));
		}
	}
}

void free_array(void *array, int len)
{
	for (int i = 0; i < len; i++)
	{
		free(((data_structure *)array)[i].data);
		free(((data_structure *)array)[i].header);
	}
	free(array);
}

void find(void *data_block, int len, int index)
{
	data_structure *start;
	start = (data_structure *)(data_block);
	if (index >= 0 && index <= len)
		start = (data_structure *)(data_block + index * sizeof(data_structure));
	else
	{
		printf("Index out of bounds.\n");
		return;
	}

	printf("Tipul %d\n", start->header->type);
	if (start->header->type == 1)
	{
		int offset = strlen(start->data) + 2 * sizeof(int8_t) + 1;
		printf("%s pentru %s\n", (char *)(start->data), (char *)(start->data + offset));
		offset = strlen(start->data) + 1;
		printf(P_TYPE_1, *((int8_t *)(start->data + offset)), *((int8_t *)(start->data + offset + sizeof(int8_t))));
	}
	else if (start->header->type == 2)
	{
		int offset = strlen(start->data) + sizeof(int16_t) + sizeof(int32_t) + 1;
		printf("%s pentru %s\n", (char *)(start->data), (char *)(start->data + offset));
		offset = strlen(start->data) + 1;
		printf(P_TYPE_2, *((int16_t *)(start->data + offset)), *((int32_t *)(start->data + offset + sizeof(int16_t))));
	}
	else if (start->header->type == 3)
	{
		int offset = strlen(start->data) + 2 * sizeof(int32_t) + 1;
		printf("%s pentru %s\n", (char *)(start->data), (char *)(start->data + offset));
		offset = strlen(start->data) + 1;
		printf(P_TYPE_3, *((int32_t *)(start->data + offset)), *((int32_t *)(start->data + offset + sizeof(int32_t))));
	}
}

int delete_at(void **arr, int *len, int index)
{
	data_structure *start = NULL, *end = NULL;
	if (index >= 0 && index <= *len)
	{
		start = (data_structure *)(*arr + index * sizeof(data_structure));
		end = (data_structure *)(*arr + (index + 1) * sizeof(data_structure));
	}
	else
	{
		printf("Index out of bounds.\n");
		return 1;
	}

	free(start->header);
	free(start->data);

	memcpy(start, end, (*len - index - 1) * sizeof(data_structure));

	*arr = realloc(*arr, ((*len) - 1) * sizeof(data_structure));
	return 0;
}
