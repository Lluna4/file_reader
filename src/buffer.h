#pragma once
#include <cstdlib>
#include <stdexcept>
#include <cstring>
#define BUFFER_SIZE 1024

struct buffer
{
	buffer()
	{
		data = nullptr;
		size = 0;
		allocated = 0;
		allocations = 0;
	}
	char *data;
	size_t size;
	size_t allocated;
	int allocations;

	void write(char *data_in, size_t data_size);
	void remove(int offset, int remove_size);
	void allocate(size_t s);

	buffer(const buffer&) = delete;
	buffer & operator=(const buffer &) = delete;
	~buffer()
	{
		if (allocated > 0)
			free(data);
	}
};

struct buffer_unsigned
{
	buffer_unsigned()
	{
		data = nullptr;
		size = 0;
		allocated = 0;
		allocations = 0;
	}
	unsigned char *data;
	size_t size;
	size_t allocated;
	int allocations;

	void write(unsigned char *data_in, size_t data_size);
	void remove(int offset, int remove_size);
	void allocate(size_t s);

	buffer_unsigned(const buffer_unsigned&) = delete;
	buffer_unsigned & operator=(const buffer_unsigned &) = delete;
	~buffer_unsigned()
	{
		if (allocated > 0)
			free(data);
	}
};
