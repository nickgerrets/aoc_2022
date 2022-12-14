#include <iostream>
#include <fstream>
#include <deque>

#include "../include/common.h"

using namespace aoc;

size_t find_marker(std::istream& stream, size_t marker_size)
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
	if (argc < 2)
		return (EXIT_FAILURE);
	
	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	std::cout << "Marker: " << find_marker(file, 14) << std::endl;

	return (EXIT_SUCCESS);
}
