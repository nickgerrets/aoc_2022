
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "../include/common.h"

using namespace aoc;

struct point
{
	int64_t x, y;

	bool operator==(point const& rhs)
	{
		return (x == rhs.x && y == rhs.y);
	}

};

struct Sensor
{
	Sensor(point S, point B)
	: S(S), B(B)
	{
		point p = { S.x - B.x, S.y - B.y };
		this->radius = std::abs(p.x) + std::abs(p.y);
	}

	point S;
	point B;
	int radius; // manhattan distance

};

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

	bool operator<(Range const& rhs)
	{
		return (bound_lower < rhs.bound_lower
			|| (!(rhs.bound_lower < bound_lower) && bound_higher < rhs.bound_higher));

	}

	T	bound_lower;
	T	bound_higher;
};

std::vector<Sensor> parse_sensors(std::istream& stream)
{
	std::vector<Sensor> sensors;
	while (!stream.eof())
	{
		int64_t a, b, c, d;
		stream >> getn >> a >> getn >> b >> getn >> c >> getn >> d >> skipl;

		sensors.emplace_back(Sensor {{a, b}, {c, d}});
	}
	return (sensors);
}

std::vector<Range<int64_t>> get_ranges(std::vector<Sensor> const& sensors, int64_t targety)
{
	std::vector<Range<int64_t>> ranges;
	for (auto const& s : sensors)
	{
		int64_t dy = std::abs(s.S.y - targety);
		int64_t xdist = s.radius - dy;
		if (xdist < 0)
			continue ; // this sensor has no effect on targety
		ranges.emplace_back(s.S.x - xdist, s.S.x + xdist);
	}
	std::sort(ranges.begin(), ranges.end());
	return (std::move(ranges));
}

int64_t seek_frequency(std::vector<Sensor> const& sensors, int64_t max_y)
{
	for (int64_t y = 0; y <= max_y; ++y)
	{
		std::vector<Range<int64_t>> ranges(std::move(get_ranges(sensors, y)));
		Range<int64_t> prev = ranges[0];
		int64_t count = 0;
		for (auto& r : ranges)
		{
			if (prev.bound_lower <= r.bound_lower && r.bound_higher <= prev.bound_higher)
				continue ; // skip this one
			else if (prev.bound_lower <= r.bound_lower && prev.bound_higher <= r.bound_higher && r.bound_lower <= prev.bound_higher)
				prev.bound_higher = r.bound_higher; // Just extend prev range
			else
				return (y + (max_y * (1 + prev.bound_higher)));
		}
	}
	return (-1);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);

	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	//	PARSING
	std::vector<Sensor> sensors = parse_sensors(file);

	//	Part 1
	int64_t targety = 2'000'000;
	if (argc > 2)
		targety = std::stoul(argv[2]);
	
	std::vector<Range<int64_t>> ranges = get_ranges(sensors, targety);

	Range<int64_t> prev = ranges[0];
	int64_t count = 0;
	for (auto& r : ranges)
	{
		if (prev.bound_lower <= r.bound_lower && r.bound_higher <= prev.bound_higher)
			continue ; // skip this one
		else if (prev.bound_lower <= r.bound_lower && prev.bound_higher <= r.bound_higher && r.bound_lower <= prev.bound_higher)
			prev.bound_higher = r.bound_higher; // Just extend prev range
		else
		{
			count += 1 + (prev.bound_higher - prev.bound_lower);
			prev = r;
		}
	}
	count += 1 + (prev.bound_higher - prev.bound_lower);

	std::vector<point> bees;
	for (auto& s : sensors)
	{
		if (s.B.y == targety && std::count(bees.begin(), bees.end(), s.B) == 0)
			bees.push_back(s.B);
	}
	count -= bees.size();

	std::cout << "Positions that cannot contain beacon: " << count << std::endl;

	//	Part 2
	constexpr const int64_t MAX_Y = 4'000'000;
	std::cout << "Seeking frequency... " << std::flush;
	std::cout << "Frequency: " << seek_frequency(sensors, MAX_Y) << std::endl;

	return (EXIT_SUCCESS);
}
