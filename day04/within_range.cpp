#include "../include/common.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace aoc;

template<typename T>
class Range
{
	public:
	Range(T const& lower, T const& higher)
	: bound_lower(lower), bound_higher(higher) {}

	bool contains(const Range& r) const
	{
		return (bound_lower <= r.bound_lower && bound_higher >= r.bound_higher);
	}

	bool overlaps(const Range& r) const
	{
		return ((r.bound_lower >= bound_lower && r.bound_lower <= bound_higher)
			|| (r.bound_higher >= bound_lower && r.bound_higher <= bound_higher));
	}

	private:
	T	bound_lower;
	T	bound_higher;
};

using range_pair_t = std::pair<Range<int>, Range<int>>;
std::vector<range_pair_t> parse_pairs(std::istream& stream)
{
	std::vector<range_pair_t> pairs;
	for (std::string line; std::getline(stream, line); )
	{
		int v[4] {0};
		std::stringstream ss(line);
		ss	>> getn >> v[0]
			>> getn >> v[1]
			>> getn >> v[2]
			>> getn >> v[3];
		pairs.emplace_back( range_pair_t {
			Range<int>(v[0], v[1]),
			Range<int>(v[2], v[3])
		});
	}
	return (pairs);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (EXIT_FAILURE);
	
	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	//	Parsing
	std::vector<range_pair_t> pairs = parse_pairs(file);

	size_t total_contained = 0;
	size_t total_overlap = 0;
	for (auto& r : pairs)
	{
		if (r.first.contains(r.second) || r.second.contains(r.first))
			total_contained++;
		if (r.first.overlaps(r.second) || r.second.overlaps(r.first))
			total_overlap++;
	}

	std::cout << "Total contained pairs: " << total_contained << std::endl;
	std::cout << "Total overlapping pairs: " << total_overlap << std::endl;

	return (EXIT_SUCCESS);
}
