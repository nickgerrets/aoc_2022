#include <iostream>
#include <fstream>
#include <queue>

//	Checks if all elements in a given (iterable) container type are unique
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

int	find_marker(std::ifstream& stream, size_t marker_size)
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
		if (deque.size() == marker_size && check_unique(deque))
			return (i);
	}
	return (i);
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
