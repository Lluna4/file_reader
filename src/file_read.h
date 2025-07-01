#pragma once
#include "file_deserialising.h"
#include <unistd.h>
#include <errno.h>
#include <print>
#include <fcntl.h>

enum RESULT
{
	READ_CORRECT, //read was as expected
	READ_INCOMPLETE, //read is incomplete (probably because of the file ending)
	READ_FILE_ENDED //cannot read because there's not more file to read
};

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

		file_reader(std::string path)
		{
			fd = open(path.c_str(), O_RDONLY);
			buf.size = 0;
			buf.allocated = 0;
			buf.data = nullptr;
			file_size = lseek(fd, 0, SEEK_END);
			lseek(fd, 0, SEEK_SET);
			file_size_remaining = file_size;
		}

		template<typename ...T>
		std::pair<size_t, RESULT> read_from_tuple(std::tuple<T...> &in)
		{
			RESULT ret = READ_CORRECT;
			if (file_size_remaining <= 0)
				return std::make_pair(0, READ_FILE_ENDED);
			constexpr std::size_t size_tuple = sizeof...(T);
			int size = 0;
			int extra_size = 0;
			const_for_<size_tuple>([&](auto i)
			{
				if constexpr (typeid(std::get<i.value>(in)) == typeid(buffer))
					size += std::get<i.value>(in).size;
				else
					size += sizeof(std::get<i.value>(in));
			});
			if (size > buf.size)
			{
				if (size + BUFFER_SIZE <= file_size_remaining)
				{
					extra_size = BUFFER_SIZE;
				}
				else if (size > file_size_remaining)
				{
					extra_size = file_size_remaining - size;
					ret = READ_INCOMPLETE;
				}
				char *new_read = (char *)calloc(size + extra_size, sizeof(char));
				size_t res = read(fd, new_read, size + extra_size);
				if (res != size + extra_size)
				{
					free(new_read);
					std::println("Read error {}", strerror(errno));
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
			return std::make_pair(size + extra_size, ret);
		}
		buffer buf;
		int fd;
		off_t file_size;
		off_t file_size_remaining;
};
