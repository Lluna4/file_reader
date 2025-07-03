#pragma once
#include <tuple>
#include <cstring>
#include <print>
#include <type_traits>
#include "buffer.h"
#define read_comp(size, ptr, t) const_for_<size>([&](auto i){read_var<std::tuple_element_t<i.value, std::remove_cvref_t<decltype(t)>>>::call(std::get<i.value>(t), &ptr);});
#ifdef __APPLE__
#include <libkern/OSByteOrder.h>



#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)
#endif

struct parsing_buffer
{
	parsing_buffer(buffer &b)
	:buf(b)
	{}
	buffer &buf;
	char *point;
	int consumed_size;
};

template <typename T>
static void read_type(T& val, char *data)
{
	std::memcpy(&val, data, sizeof(T));
	switch (sizeof(T))
	{
		case 1:
			break;
		case 2:
			val = be16toh(val);
			break;
		case 4:
			val = be32toh(val);
			break;
		case 8:
			val = be64toh(val);
			break;
		default:
			std::println("This integer is not supported!");
	}
}

template <>
inline void read_type<float>(float &val, char *data)
{
	uint32_t num_as_uint32;

	memcpy(&num_as_uint32, data, sizeof(float));
	num_as_uint32 = be32toh(num_as_uint32);
	memcpy(&val, &num_as_uint32, sizeof(float));
}

template <>
inline void read_type<double>(double &val, char *data)
{
	uint64_t num_as_uint64;

	memcpy(&num_as_uint64, data, sizeof(double));
	num_as_uint64 = be64toh(num_as_uint64);
	memcpy(&val, &num_as_uint64, sizeof(double));
}

template<>
inline void read_type<buffer>(buffer &val, char *data)
{
	val.allocated = val.size + 1;
	val.data = (char *)malloc(val.size * sizeof(char) + 1);
	std::memcpy(val.data, data, val.size);
}

template<typename T>
struct read_var
{
	static void call(T& val, parsing_buffer* v)
	{
		int size = 0;
		if constexpr (typeid(T) == typeid(buffer))
			size = val.size;
		else
			size = sizeof(T);
		if (size + v->consumed_size > v->buf.size)
		{
			if constexpr (typeid(T) == typeid(buffer))
			{
				val.size = v->buf.size;
			}
			else
				return ;
		}
		read_type<T>(val, v->point);
		v->point += size;
		v->consumed_size += size;
	}
};


template <typename Integer, Integer ...I, typename F> constexpr void const_for_each_(std::integer_sequence<Integer, I...>, F&& func)
{
	(func(std::integral_constant<Integer, I>{}), ...);
}

template <auto N, typename F> constexpr void const_for_(F&& func)
{
	if constexpr (N > 0)
		const_for_each_(std::make_integer_sequence<decltype(N), N>{}, std::forward<F>(func));
}
