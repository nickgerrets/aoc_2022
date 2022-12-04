#include <iostream>
#include <fstream>
#include <vector>

template<typename T>
struct Pair
{
	T	left;
	T	right;
};

template<typename T>
class Range
{
	public:
	T	bound_lower;
	T	bound_higher;

	bool	fully_contains(const Range& r) const
	{
		return (bound_lower <= r.bound_lower && bound_higher >= r.bound_higher);
	}

	bool	overlaps(const Range& r) const
	{
		return ((r.bound_lower >= bound_lower && r.bound_lower <= bound_higher)
			|| (r.bound_higher >= bound_lower && r.bound_higher <= bound_higher));
	}
};

std::vector<Pair<Range<int>>>	parse_pairs(std::ifstream& input)
{
	std::vector<Pair<Range<int>>>	pairs;

	while (!input.eof())
	{
		std::string line;
		std::getline(input, line);
		if (line.length() == 0)
			break ;
		std::string left = line.substr(0, line.find(','));
		std::string right = line.substr(line.find(',') + 1, line.length());

		Pair<Range<int>>	p;
		p.left.bound_lower = std::stoi(left.substr(0, left.find('-')));
		p.left.bound_higher = std::stoi(left.substr(left.find('-') + 1, left.length()));
		p.right.bound_lower = std::stoi(right.substr(0, right.find('-')));
		p.right.bound_higher = std::stoi(right.substr(right.find('-') + 1, right.length()));
		pairs.push_back(p);
	}
	return (pairs);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (EXIT_FAILURE);
	
	std::ifstream	file;
	file.open(argv[1]);

	if (!file)
		return (EXIT_FAILURE);

	//	A vector of pairs of ranges of ints :)
	std::vector<Pair<Range<int>>> pairs = parse_pairs(file);
	int	total_contained = 0;
	int total_overlap = 0;
	for (auto& r : pairs)
	{
		if (r.left.fully_contains(r.right) || r.right.fully_contains(r.left))
			total_contained++;
		if (r.left.overlaps(r.right) || r.right.overlaps(r.left))
			total_overlap++;
	}

	std::cout << "Total contained pairs: " << total_contained << std::endl;
	std::cout << "Total overlapping pairs: " << total_overlap << std::endl;

	return (EXIT_SUCCESS);
}
