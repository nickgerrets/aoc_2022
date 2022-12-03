#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

class Rucksack
{
	public:
	std::string sack;
	std::string	comp1;
	std::string	comp2;

	char	get_first_common_item(void)
	{
		for (char c : comp1)
		{
			if (comp2.find(c) != std::string::npos)
				return (c);
		}
		return ('\0');
	}

};

class Group
{
	public:
	Rucksack	a;
	Rucksack	b;
	Rucksack	c;

	char	get_first_common_item(void)
	{
		for (char ch : a.sack)
		{
			if ((b.sack.find(ch) != std::string::npos)
				&& c.sack.find(ch) != std::string::npos)
				return (ch);
		}
		return ('\0');
	}
};

std::vector<Rucksack>	parse_rucksacks(std::ifstream& input)
{
	std::vector<Rucksack>	rucksacks;

	while (!input.eof())
	{
		std::string	line;
		std::getline(input, line);
		if (line.length() == 0)
			break ;
		Rucksack r;
		r.sack = line;
		r.comp1 = line.substr(0, line.length() * 0.5);
		r.comp2 = line.substr(line.length() * 0.5, line.length());
		rucksacks.push_back(r);
	}
	return (rucksacks);
}

std::vector<Group>	parse_groups(std::vector<Rucksack>& rucksacks)
{
	std::vector<Group>	groups;

	size_t	i = 0;
	while (i + 2 < rucksacks.size())
	{
		Group	g;
		g.a = rucksacks[i];
		g.b = rucksacks[i + 1];
		g.c = rucksacks[i + 2];
		groups.push_back(g);
		i += 3;
	}
	return (groups);
}

int	get_priority(char c)
{
	if (c == '\0')
		return (0);
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 1);
	else
		return (c - 'A' + 27);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (EXIT_FAILURE);

	std::ifstream	file;
	file.open(argv[1]);

	if (!file)
		return (EXIT_FAILURE);

	std::vector<Rucksack> rucksacks = parse_rucksacks(file);
	std::vector<Group> groups = parse_groups(rucksacks);

	int total = 0;
	for (Group& g : groups)
		total += get_priority(g.get_first_common_item());

	std::cout << "Priority total: " << total << std::endl;

	return (EXIT_SUCCESS);
}
