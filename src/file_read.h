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
			if (file_size == -1)
			{
				std::println("lseek failed {}", strerror(errno));
				throw std::runtime_error("lseek failed");
			}
			if (lseek(fd, 0, SEEK_SET) == -1)
			{
				std::println("lseek failed {}", strerror(errno));
				throw std::runtime_error("lseek failed");
			}
			file_size_remaining = file_size;
		}

		file_reader(std::string path)
		{
			fd = open(path.c_str(), O_RDONLY);
			if (fd == -1)
			{
				std::println("Error opening the file {}", strerror(errno));
				throw std::runtime_error("Error opening file");
			}
			buf.size = 0;
			buf.allocated = 0;
			buf.data = nullptr;
			file_size = lseek(fd, 0, SEEK_END);
			if (file_size == -1)
			{
				std::println("lseek failed {}", strerror(errno));
				throw std::runtime_error("lseek failed");
			}
			if (lseek(fd, 0, SEEK_SET) == -1)
			{
				std::println("lseek failed {}", strerror(errno));
				throw std::runtime_error("lseek failed");
			}
			file_size_remaining = file_size;
		}

		file_reader(const file_reader&) = delete;
		file_reader & operator=(const file_reader &) = delete;
		~file_reader()
		{
			close(fd);
		}

		template<typename ...T>
		std::pair<size_t, RESULT> read_from_tuple(std::tuple<T...> &in)
		{
			RESULT ret = READ_CORRECT;
			if (file_size_remaining <= 0 && buf.size <= 0)
				return std::make_pair(0, READ_FILE_ENDED);
			constexpr std::size_t size_tuple = sizeof...(T);
			int size = 0;
			int extra_size = 0;
			size_t total_read_size = 0;
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
				total_read_size = size + extra_size;
				size_t read_done = 0;
				size_t res = 0;
				while (read_done != size + extra_size)
				{
					res = read(fd, &new_read[read_done], (size + extra_size) - read_done);
					read_done += res;
					if (res == -1)
					{
						free(new_read);
						std::println("Read error {}", strerror(errno));
						throw std::runtime_error("Read failed!");
					}
					if (res == 0)
					{
						if (read_done != size + extra_size)
							ret = READ_INCOMPLETE;
						total_read_size = read_done;
						break;
					}
				}
				buf.write(new_read, total_read_size);
				file_size_remaining -= total_read_size;
				free(new_read);
			}
			size_t prev_size = buf.size;
			parsing_buffer par_buf(buf);
			par_buf.point = buf.data;
			if (prev_size > size)
				par_buf.buf.size = size;
			par_buf.consumed_size = 0;
			read_comp(size_tuple, par_buf, in);
			buf.size = prev_size;
			buf.remove(0, size);
			return std::make_pair(size, ret);
		}
	private:
		buffer buf;
		int fd;
		off_t file_size;
		off_t file_size_remaining;
};
