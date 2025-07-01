#pragma once
#include <cstdlib>
#include <stdexcept>
#include <cstring>
#define BUFFER_SIZE 1024

struct buffer
{
	char *data;
	size_t size;
	size_t allocated;

	void write(char *data_in, int data_size);
	void remove(int offset, int remove_size);

	~buffer()
	{
		free(data);
	}
};
