#include <iostream>
#include <fstream>
#include <deque>

template<typename ItInput>
ItInput	first_not_unique(ItInput begin, ItInput end)
{
	for (ItInput it1 = begin; it1 != end; ++it1)
	{
		for (ItInput it2 = it1 + 1; it2 != end; ++it2)
		{
			if (*it1 == *it2)
				return (it2);
		}
	}
	return (end);
}

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
		if (deque.size() == marker_size && first_not_unique(deque.begin(), deque.end()) == deque.end())
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