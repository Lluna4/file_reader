#pragma once
#include "file_deserialising.h"
#include <unistd.h>

class file_reader
{
	public:
		file_reader(int file_fd)
			:fd(file_fd)
		{
			buf.size = 0;
			buf.allocated = 0;
			buf.data = nullptr;
			file_size = lseek(fd, 0, SEEK_END);
			lseek(fd, 0, SEEK_SET);
			file_size_remaining = file_size;
		}

		template<typename ...T>
		std::tuple<T...> read_from_tuple(std::tuple<T...> in)
		{
			constexpr std::size_t size_tuple = std::tuple_size_v<decltype(in)>;
			int size = 0;
			const_for_<size_tuple>([&](auto i){size += sizeof(std::get<i.value>(in));});
			if (size > buf.size)
			{
				int extra_size = 0;
				if (size + BUFFER_SIZE <= file_size_remaining)
				{
					extra_size = BUFFER_SIZE;
				}
				else if (size > file_size_remaining)
				{
					extra_size = file_size_remaining - size;
				}
				char *new_read = (char *)calloc(size + extra_size, sizeof(char));
				size_t res = read(fd, new_read, size + extra_size);
				if (res != size + extra_size)
				{
					free(new_read);
					throw std::runtime_error("Read failed!");
				}
				buf.write(new_read, size + extra_size);
				file_size_remaining -= size + extra_size;
				free(new_read);
			}
			parsing_buffer par_buf(buf);
			par_buf.point = buf.data;
			par_buf.consumed_size = 0;
			read_comp(size_tuple, par_buf, in);
			buf.remove(0, size);
			return in;
		}
		buffer buf;
		int fd;
		off_t file_size;
		off_t file_size_remaining;
};
