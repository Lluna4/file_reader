#pragma once
#include <tuple>
#include <cstring>
#include <print>
#include "buffer.h"
#ifdef __APPLE__
#include <libkern/OSByteOrder.h>
#define read_comp(size, ptr, t) const_for_<size>([&](auto i){std::get<i.value>(t) = read_var<std::tuple_element_t<i.value, decltype(t)>>::call(&ptr);});


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
static T read_type(char *data)
{
	T a;

	std::memcpy(&a, data, sizeof(T));

	switch (sizeof(T))
	{
		case 1:
			break;
		case 2:
			a = be16toh(a);
			break;
		case 4:
			a = be32toh(a);
			break;
		case 8:
			a = be64toh(a);
			break;
		default:
			std::println("This integer is not supported!");
	}
	return a;
}

template <>
float read_type<float>(char *data)
{
	uint32_t num_as_uint32;
	float num;

	memcpy(&num_as_uint32, data, sizeof(uint32_t));
	num_as_uint32 = be32toh(num_as_uint32);
	memcpy(&num, &num_as_uint32, sizeof(float));

	return num;
}

template <>
double read_type<double>(char *data)
{
	uint64_t num_as_uint64;
	double num;

	memcpy(&num_as_uint64, data, sizeof(uint64_t));
	num_as_uint64 = be64toh(num_as_uint64);
	memcpy(&num, &num_as_uint64, sizeof(double));

	return num;
}


template<typename T>
struct read_var
{
	static T call(parsing_buffer* v)
	{
		if (sizeof(T) + v->consumed_size > v->buf.size)
			return T{};
		T ret = read_type<T>(v->point);
		v->point += sizeof(T);
		v->consumed_size += sizeof(T);
		return ret;
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
