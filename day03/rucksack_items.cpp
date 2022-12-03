#include <iostream>
#include <vector>
#include <fstream>

class Rucksack
{
	public:
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
		r.comp1 = line.substr(0, line.length() * 0.5);
		r.comp2 = line.substr(line.length() * 0.5, line.length());
		rucksacks.push_back(r);
	}
	return (rucksacks);
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
	int	total = 0;
	for (Rucksack& r : rucksacks)
		total += get_priority(r.get_first_common_item());

	std::cout << "Priority total: " << total << std::endl;

	return (EXIT_SUCCESS);
}
