#include <iostream>
#include <fstream>


struct Map
{
	size_t	width = 0;
	size_t	height = 0;
	std::string	data;

	char get(size_t x, size_t y)
	{
		return (data[x + y * width]);
	}

	bool	is_visible(size_t x, size_t y)
	{
		if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
			return (true);

		char c = get(x, y);
		size_t i = 0;
		size_t a = 0;
		while (i < x)
		{
			if (get(i, y) >= c)
			{
				a++;
				break ;
			}
			++i;
		}
		i = 0;
		while (i < y)
		{
			if (get(x, i) >= c)
			{
				a++;
				break ;
			}
			++i;
		}
		i = width - 1;
		while (i > x)
		{
			if (get(i, y) >= c)
			{
				a++;
				break ;
			}
			--i;
		}
		i = height - 1;
		while (i > y)
		{
			if (get(x, i) >= c)
			{
				a++;
				break ;
			}
			--i;
		}
		if (a < 4)
			return (true);
		return (false);
	}

	int	determine_score(size_t x, size_t y)
	{

		int	score = 
			count_row(x, 1, y)
			* count_row(x, -1, y)
			* count_column(y, 1, x)
			* count_column(y, -1, x);

		return (score);
	}

	int	count_row(size_t xfrom, int add, size_t y)
	{
		char c = get(xfrom, y);
		int x = (int)xfrom + add;
		int count = 0;
		while (x < (int)width && x >= 0)
		{
			if (get(x, y) < c)
				++count;
			else if (get(x, y) >= c)
				return (count + 1);
			x += add;
		}
		return (count);
	}

	int	count_column(size_t yfrom, int add, size_t x)
	{
		char c = get(x, yfrom);
		int y = (int)yfrom + add;
		int count = 0;
		while (y < (int)height && y >= 0)
		{
			if (get(x, y) < c)
				++count;
			else if (get(x, y) >= c)
				return (count + 1);
			y += add;
		}
		return (count);
	}

};

Map	parse_map(std::ifstream& stream)
{
	Map	map;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		if (line.length() == 0)
			break ;
		map.width = line.length();
		map.height++;
		map.data.append(line);
	}
	return (map);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (EXIT_FAILURE);
	
	std::ifstream	file;
	file.open(argv[1]);

	if (!file)
		return (EXIT_FAILURE);

	Map map = parse_map(file);
	size_t count = 0;
	int heighest_score = 0;
	for (size_t x = 0; x < map.width; ++x)
	{
		for (size_t y = 0; y < map.height; ++y)
		{
			if (map.is_visible(x, y))
				count++;
			int score = map.determine_score(x, y);
			if (score > heighest_score)
				heighest_score = score;
		}
	}

	std::cout << "Amount visible: " << count << std::endl;
	
	std::cout << "Heighest score: " << heighest_score << std::endl;

	return (EXIT_SUCCESS);
}