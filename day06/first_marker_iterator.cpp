#include <iostream>
#include <fstream>

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
	std::string line;
	std::getline(stream, line);
	for (size_t i = marker_size; i <= line.size(); ++i)
	{
		if (first_not_unique(line.begin() + i - marker_size, line.begin() + i) == line.begin() + i)
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
