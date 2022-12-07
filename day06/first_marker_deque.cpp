#include <iostream>
#include <fstream>
#include <deque>

/*
**	Checks if all elements in a given (iterable) container type are unique
**	I realised it's practically the same as std::unique.

template<typename T>
bool	check_unique(const T& container)
{
	for (size_t i = 0; i < container.size(); ++i)
	{
		typename T::value_type value = container[i];
		for (size_t j = i + 1; j < container.size(); ++j)
		{
			if (value == container[j])
				return (false);
		}
	}
	return (true);
}
*/

size_t	find_marker(std::ifstream& stream, size_t marker_size)
{
	std::deque<char> deque;

	size_t i = 0;
	int c;
	while ((c = (char)stream.get()) != EOF)
	{
		i++;
		if (deque.size() >= marker_size)
			deque.pop_back();
		deque.push_front((char)c);
		if (deque.size() == marker_size && std::unique(deque.begin(), deque.end()) != deque.end() )
			return (i);
	}
	return (std::string::npos);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (EXIT_FAILURE);
	
	std::ifstream	file;
	file.open(argv[1]);

	if (!file)
		return (EXIT_FAILURE);

	std::cout << "Marker: " << find_marker(file, 14) << std::endl;

	return (EXIT_SUCCESS);
}
