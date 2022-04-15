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

void add_element(void **arr, int offset, int shift, int *len, data_structure *data, int index, int cnt);

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
			if (add_at(&arr, &len, input, index))
				printf("Cant add last.\n");
		}
		else if (!strncmp(token, "insert", 6))
		{
			data_structure *input = NULL;
			input = parse_insert_input(token);
			if (add_last(&arr, &len, input))
				printf("Cant add last.\n");
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
	*arr = realloc(*arr, *len + data->header->len + sizeof(unsigned char) + sizeof(unsigned int));
	if (!(*arr))
	{
		printf("Cant realloc memory.\n");
		return 1;
	}

	int offset = 0;
	int idx = 0;
	while (offset < *len)
	{
		int type = *((unsigned char *)(*arr + offset));
		offset += sizeof(unsigned char);
		offset += sizeof(unsigned int);
		if (type == 1)
		{
			int cnt = sizeof(unsigned int) + sizeof(unsigned char);
			cnt += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;
			cnt += 2 * sizeof(int8_t);
			offset += 2 * sizeof(int8_t);
			cnt += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;
			int shift = data->header->len + sizeof(unsigned int) + sizeof(unsigned char);
			if (idx == index)
			{
				add_element(arr, offset, shift, len, data, index, cnt);
				break;
			}
		}

		if (type == 2)
		{
			int cnt = sizeof(unsigned int) + sizeof(unsigned char);
			cnt += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;
			cnt += sizeof(int32_t) + sizeof(int16_t);
			offset += sizeof(int32_t) + sizeof(int16_t);
			cnt += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;
			int shift = data->header->len + sizeof(unsigned int) + sizeof(unsigned char);
			if (idx == index)
			{
				add_element(arr, offset, shift, len, data, index, cnt);
				break;
			}
		}

		if (type == 3)
		{
			int cnt = sizeof(unsigned int) + sizeof(unsigned char);
			cnt += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;
			cnt += 2 * sizeof(int32_t);
			offset += 2 * sizeof(int32_t);
			cnt += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;
			int shift = data->header->len + sizeof(unsigned int) + sizeof(unsigned char);
			if (idx == index)
			{
				add_element(arr, offset, shift, len, data, index, cnt);
				break;
			}
		}
		idx++;
	}
	free(data->data);
	free(data->header);
	free(data);
	return 0;
}

void add_element(void **arr, int offset, int shift, int *len, data_structure *data, int index, int cnt)
{
	*len += shift;

	if (index >= 1)
	{
		memmove(*arr + offset - cnt + shift, *arr + offset - cnt, *len - shift - offset + cnt);
		int offset1 = 0;
		memcpy(*arr + offset - cnt + offset1, &data->header->type, sizeof(unsigned char));
		offset1 += sizeof(unsigned char);

		memcpy(*arr + offset - cnt + offset1, &data->header->len, sizeof(unsigned int));
		offset1 += sizeof(unsigned int);

		memcpy(*arr + offset - cnt + offset1, data->data, data->header->len);
	}
	else
	{
		memmove(*arr + shift, *arr, *len - shift);

		int offset1 = 0;
		memcpy(*arr + offset1, &data->header->type, sizeof(unsigned char));
		offset1 += sizeof(unsigned char);

		memcpy(*arr + offset1, &data->header->len, sizeof(unsigned int));
		offset1 += sizeof(unsigned int);

		memcpy(*arr + offset1, data->data, data->header->len);
	}
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
	*arr = realloc(*arr, *len + data->header->len + sizeof(unsigned char) + sizeof(unsigned int));
	if (!(*arr))
	{
		printf("Cant realloc memory.\n");
		return 1;
	}

	int offset = 0;
	memcpy(*arr + *len + offset, &data->header->type, sizeof(unsigned char));
	offset += sizeof(unsigned char);

	memcpy(*arr + *len + offset, &data->header->len, sizeof(unsigned int));
	offset += sizeof(unsigned int);

	memcpy(*arr + *len + offset, data->data, data->header->len);

	*len += data->header->len + sizeof(unsigned int) + sizeof(unsigned char);
	free(data->data);
	free(data->header);
	free(data);
	return 0;
}

void print(void *arr, int len)
{
	int offset = 0;
	while (offset < len)
	{
		int type = *((unsigned char *)(arr + offset));
		offset += sizeof(unsigned char);
		printf("Tipul %d\n", type);
		offset += sizeof(unsigned int);
		if (type == 1)
		{
			char *nume1 = malloc(MAX_LINE);
			char *nume2 = malloc(MAX_LINE);
			int8_t b1, b2;
			strcpy(nume1, (char *)(arr + offset));
			offset += strlen((char *)(arr + offset)) + 1;
			b1 = *(int8_t *)(arr + offset);
			offset += sizeof(int8_t);
			b2 = *(int8_t *)(arr + offset);
			offset += sizeof(int8_t);
			strcpy(nume2, (char *)(arr + offset));
			offset += strlen((char *)(arr + offset)) + 1;

			printf("%s pentru %s\n", nume1, nume2);
			printf(P_TYPE_1, b1, b2);

			free(nume1);
			free(nume2);
		}

		if (type == 2)
		{
			char *nume1 = malloc(MAX_LINE), *nume2 = malloc(MAX_LINE);
			int16_t b1;
			int32_t b2;
			strcpy(nume1, (char *)(arr + offset));
			offset += strlen((char *)(arr + offset)) + 1;
			b1 = *(int16_t *)(arr + offset);
			offset += sizeof(int16_t);
			b2 = *(int32_t *)(arr + offset);
			offset += sizeof(int32_t);
			strcpy(nume2, (char *)(arr + offset));
			offset += strlen((char *)(arr + offset)) + 1;

			printf("%s pentru %s\n", nume1, nume2);
			printf(P_TYPE_2, b1, b2);

			free(nume1);
			free(nume2);
		}

		if (type == 3)
		{
			char *nume1 = malloc(MAX_LINE), *nume2 = malloc(MAX_LINE);
			int32_t b1;
			int32_t b2;
			strcpy(nume1, (char *)(arr + offset));
			offset += strlen((char *)(arr + offset)) + 1;
			b1 = *(int32_t *)(arr + offset);
			offset += sizeof(int32_t);
			b2 = *(int32_t *)(arr + offset);
			offset += sizeof(int32_t);
			strcpy(nume2, (char *)(arr + offset));
			offset += strlen((char *)(arr + offset)) + 1;

			printf("%s pentru %s\n", nume1, nume2);
			printf(P_TYPE_3, b1, b2);

			free(nume1);
			free(nume2);
		}
	}
}

void free_array(void *array, int len)
{
	free(array);
}

void find(void *data_block, int len, int index)
{
	int offset = 0;
	int idx = 0;
	while (offset < len)
	{
		int type = *((unsigned char *)(data_block + offset));
		offset += sizeof(unsigned char);
		if (idx == index)
			printf("Tipul %d\n", type);
		offset += sizeof(unsigned int);
		if (type == 1)
		{
			char *nume1 = malloc(MAX_LINE);
			char *nume2 = malloc(MAX_LINE);
			int8_t b1, b2;
			strcpy(nume1, (char *)(data_block + offset));
			offset += strlen((char *)(data_block + offset)) + 1;
			b1 = *(int8_t *)(data_block + offset);
			offset += sizeof(int8_t);
			b2 = *(int8_t *)(data_block + offset);
			offset += sizeof(int8_t);
			strcpy(nume2, (char *)(data_block + offset));
			offset += strlen((char *)(data_block + offset)) + 1;

			if (idx == index)
			{
				printf("%s pentru %s\n", nume1, nume2);
				printf(P_TYPE_1, b1, b2);
			}

			free(nume1);
			free(nume2);
		}

		if (type == 2)
		{
			char *nume1 = malloc(MAX_LINE), *nume2 = malloc(MAX_LINE);
			int16_t b1;
			int32_t b2;
			strcpy(nume1, (char *)(data_block + offset));
			offset += strlen((char *)(data_block + offset)) + 1;
			b1 = *(int16_t *)(data_block + offset);
			offset += sizeof(int16_t);
			b2 = *(int32_t *)(data_block + offset);
			offset += sizeof(int32_t);
			strcpy(nume2, (char *)(data_block + offset));
			offset += strlen((char *)(data_block + offset)) + 1;

			if (idx == index)
			{
				printf("%s pentru %s\n", nume1, nume2);
				printf(P_TYPE_1, b1, b2);
			}

			free(nume1);
			free(nume2);
		}

		if (type == 3)
		{
			char *nume1 = malloc(MAX_LINE), *nume2 = malloc(MAX_LINE);
			int32_t b1;
			int32_t b2;
			strcpy(nume1, (char *)(data_block + offset));
			offset += strlen((char *)(data_block + offset)) + 1;
			b1 = *(int32_t *)(data_block + offset);
			offset += sizeof(int32_t);
			b2 = *(int32_t *)(data_block + offset);
			offset += sizeof(int32_t);
			strcpy(nume2, (char *)(data_block + offset));
			offset += strlen((char *)(data_block + offset)) + 1;

			if (idx == index)
			{
				printf("%s pentru %s\n", nume1, nume2);
				printf(P_TYPE_1, b1, b2);
			}

			free(nume1);
			free(nume2);
		}
		idx++;
	}
}

int delete_at(void **arr, int *len, int index)
{
	int offset = 0;
	int idx = 0;
	while (offset < *len)
	{
		int type = *((unsigned char *)(*arr + offset));
		offset += sizeof(unsigned char);
		offset += sizeof(unsigned int);
		if (type == 1)
		{
			int shift = sizeof(unsigned int) + sizeof(unsigned char);
			shift += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;
			shift += 2 * sizeof(int8_t);
			offset += 2 * sizeof(int8_t);
			shift += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;

			if (idx == index)
			{
				memmove(*arr + offset - shift, *arr + offset, *len - offset);
				*len -= shift;
				*arr = realloc(*arr, *len);
				break;
			}
		}

		if (type == 2)
		{
			int shift = sizeof(unsigned int) + sizeof(unsigned char);
			shift += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;
			shift += sizeof(int32_t) + sizeof(int16_t);
			offset += sizeof(int32_t) + sizeof(int16_t);
			shift += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;

			if (idx == index)
			{
				memmove(*arr + offset - shift, *arr + offset, *len - offset);
				*len -= shift;
				*arr = realloc(*arr, *len);
				break;
			}
		}

		if (type == 3)
		{
			int shift = sizeof(unsigned int) + sizeof(unsigned char);
			shift += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;
			shift += 2 * sizeof(int32_t);
			offset += 2 * sizeof(int32_t);
			shift += strlen((char *)(*arr + offset)) + 1;
			offset += strlen((char *)(*arr + offset)) + 1;

			if (idx == index)
			{
				memmove(*arr + offset - shift, *arr + offset, *len - offset);
				*len -= shift;
				*arr = realloc(*arr, *len);
				break;
			}
		}
		idx++;
	}
	return 0;
}
