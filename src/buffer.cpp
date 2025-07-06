#include "buffer.h"

void buffer::write(char *data_in, size_t data_size)
{
	if (data_size > allocated - size)
	{
		allocated += (size + data_size) + BUFFER_SIZE;
		data = (char *)realloc(data, allocated);
		allocations++;
		if (!data)
		{
			throw std::runtime_error("Allocation failed");
		}
	}
	std::memcpy(&data[size], data_in, data_size);
	size += data_size;
}

void buffer::remove(int offset, int remove_size)
{
	if (offset + remove_size > size)
		return ;
	char *new_data = (char *)calloc(allocated, sizeof(char));
	if (!new_data)
	{
		throw std::runtime_error("Allocation failed");
	}
	if (offset > 0)
	{
		std::memcpy(new_data, data, offset);
	}
	int new_size = size - remove_size;
	std::memcpy(&new_data[offset], &data[offset + remove_size], new_size - offset);
	free(data);
	data = new_data;
	size = new_size;
	allocations++;
}

void buffer::allocate(size_t s)
{
	data = (char *)realloc(data, allocated + s);
	if (!data)
	{
		throw std::runtime_error("Allocation failed");
	}
	allocated += s;
}

void buffer_unsigned::write(unsigned char *data_in, size_t data_size)
{
	if (data_size > allocated - size)
	{
		allocated += (size + data_size) + BUFFER_SIZE;
		data = (unsigned char *)realloc(data, allocated);
		allocations++;
		if (!data)
		{
			throw std::runtime_error("Allocation failed");
		}
	}
	std::memcpy(&data[size], data_in, data_size);
	size += data_size;
}

void buffer_unsigned::remove(int offset, int remove_size)
{
	if (offset + remove_size > size)
		return ;
	unsigned char *new_data = (unsigned char *)calloc(allocated, sizeof(char));
	if (!new_data)
	{
		throw std::runtime_error("Allocation failed");
	}
	if (offset > 0)
	{
		std::memcpy(new_data, data, offset);
	}
	int new_size = size - remove_size;
	std::memcpy(&new_data[offset], &data[offset + remove_size], new_size - offset);
	free(data);
	data = new_data;
	size = new_size;
	allocations++;
}

void buffer_unsigned::allocate(size_t s)
{
	data = (unsigned char *)realloc(data, allocated + s);
	if (!data)
	{
		throw std::runtime_error("Allocation failed");
	}
	allocations++;
	allocated += s;
}
