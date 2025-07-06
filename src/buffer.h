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
	int allocations;

	void write(char *data_in, size_t data_size);
	void remove(int offset, int remove_size);

	~buffer()
	{
		free(data);
	}
};
