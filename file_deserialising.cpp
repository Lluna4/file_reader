#include "file_deserialising.h"


template <typename T>
T read_type(char *data)
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


template <typename Integer, Integer ...I, typename F> constexpr void const_for_each_(std::integer_sequence<Integer, I...>, F&& func)
{
	(func(std::integral_constant<Integer, I>{}), ...);
}

template <auto N, typename F> constexpr void const_for_(F&& func)
{
	if constexpr (N > 0)
		const_for_each_(std::make_integer_sequence<decltype(N), N>{}, std::forward<F>(func));
}