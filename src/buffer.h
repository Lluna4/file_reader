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
	void allocate(size_t s);

	~buffer()
	{
		free(data);
	}
};

struct buffer_unsigned
{
	unsigned char *data;
	size_t size;
	size_t allocated;
	int allocations;

	void write(unsigned char *data_in, size_t data_size);
	void remove(int offset, int remove_size);
	void allocate(size_t s);

	~buffer_unsigned()
	{
		free(data);
	}
};
