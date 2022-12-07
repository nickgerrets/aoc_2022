#include <iostream>
#include <fstream>
#include <set>

size_t	find_marker(std::ifstream& stream, size_t marker_size)
{
	std::string line;
	std::getline(stream, line);
	for (size_t i = marker_size; i <= line.size(); ++i)
	{
		std::set<char> set(line.begin() + i - marker_size, line.begin() + i);
		if (set.size() == marker_size)
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
