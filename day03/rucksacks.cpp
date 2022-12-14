#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

class Rucksack
{
	public:
	Rucksack(std::string const& sack)
	:	sack(sack),
		comp1(sack.substr(0, sack.length() * 0.5)),
		comp2(sack.substr(sack.length() * 0.5)) {}

	char	get_first_common_item(void)
	{
		for (char c : comp1)
		{
			if (comp2.find(c) != std::string::npos)
				return (c);
		}
		return ('\0');
	}

	std::string sack;

	private:
	std::string comp1;
	std::string comp2;
};

class Group
{
	public:
	Group(Rucksack a, Rucksack b, Rucksack c)
	: a(a), b(b), c(c) {}

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

	private:
	Rucksack a;
	Rucksack b;
	Rucksack c;
};

std::vector<Rucksack>	parse_rucksacks(std::istream& stream)
{
	std::vector<Rucksack> rucksacks;

	for (std::string line; std::getline(stream, line); )
		rucksacks.emplace_back(line);
	return (rucksacks);
}

std::vector<Group>	parse_groups(std::vector<Rucksack>& rucksacks)
{
	std::vector<Group> groups;
	size_t	i = 0;
	while (i + 2 < rucksacks.size())
	{
		groups.emplace_back(
			rucksacks[i],
			rucksacks[i + 1],
			rucksacks[i + 2]
		);
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
	return (c - 'A' + 27);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (EXIT_FAILURE);

	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	//	parsing
	std::vector<Rucksack> rucksacks = parse_rucksacks(file);
	std::vector<Group> groups = parse_groups(rucksacks);

	//	Part 1
	int total = 0;
	for (Rucksack& r : rucksacks)
		total += get_priority(r.get_first_common_item());
	std::cout << "Priority total (Part 1): " << total << std::endl;

	//	Part 2
	total = 0;
	for (Group& g : groups)
		total += get_priority(g.get_first_common_item());
	std::cout << "Priority total (Part 2): " << total << std::endl;

	return (EXIT_SUCCESS);
}
