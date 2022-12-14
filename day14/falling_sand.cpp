#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "../include/common.h"

using namespace aoc;

class Map
{
	public:
	Map(size_t width, size_t height)
	: width(width), height(height), maxy(0), data(width * height, '.') {}

	Map(Map const& other)
	: width(other.width), height(other.height), maxy(other.maxy), data(other.data) {}

	bool simulate_particle(size_t xorigin, size_t yorigin)
	{
		size_t x, y;

		x = xorigin;
		y = yorigin;
		while (y < height)
		{
			// collision check
			if (data[x + (y + 1) * width] != '.')
			{
				//	Check left
				if (data[x - 1 + (y + 1) * width] == '.')
					--x;
				//	Check right
				else if (data[x + 1 + (y + 1) * width] == '.')
					++x;
				else
					break ;
			}
			++y;
		}
		if (y >= height - 1 || (x == xorigin && y == yorigin))
			return (false);
		data[x + y * width] = 'o';
		return (true);
	}

	//	No bounds check
	void construct_wall(size_t xfrom, size_t yfrom, size_t xto, size_t yto)
	{
		//	Build wall along x-axis
		data[xto + yto * width] = '#';
		while (xfrom != xto)
		{
			data[xfrom + yto * width] = '#';
			xfrom += SIGN(static_cast<int>(xto) - static_cast<int>(xfrom));
		}
		//	Build wall along y-axis
		while (yfrom != yto)
		{
			data[xto + yfrom * width] = '#';
			yfrom += SIGN(static_cast<int>(yto) - static_cast<int>(yfrom));
		}
	}

	void print(void) const
	{
		for (size_t y = 0; y < height; ++y)
		{
			for (size_t x = 0; x < width; ++x)
			{
				std::cout << data[x + y * width];
			}
			std::cout << std::endl;
		}
	}

	size_t width;
	size_t height;
	size_t maxy;
	std::vector<char> data;
};

Map parse_map(std::istream& stream)
{
	Map map(1000, 200);

	for (std::string line; std::getline(stream, line); )
	{
		std::stringstream ss(line);
		size_t x, y, xprev, yprev;
		ss >> getn >> xprev >> getn >> yprev;
		while (!ss.eof())
		{
			ss >> getn >> x >> getn >> y;
			map.construct_wall(xprev, yprev, x, y);
			if (y > map.maxy)
				map.maxy = y;
			xprev = x;
			yprev = y;
		}
	}
	return (map);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);

	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	//	PARSING
	Map map = parse_map(file);
	Map map1(map); // Deep copy

	//	Part 1
	size_t pcount = 0;
	while (map1.simulate_particle(500, 0))
		++pcount;

	std::cout << "Amount of particles (Part 1): " << pcount << std::endl;

	//	Part 2
	//	Just add another wall at max y + 2 height
	map.construct_wall(0, map.maxy + 2, map.width, map.maxy + 2);

	pcount = 0;
	while (map.simulate_particle(500, 0))
		++pcount;

	std::cout << "Amount of particles (Part 2): " << pcount + 1 << std::endl;

	return (EXIT_SUCCESS);
}