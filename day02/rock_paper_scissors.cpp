#include <iostream>
#include <fstream>
#include <vector>

enum Shape : int
{
	Rock = 1,
	Paper = 2,
	Scissors = 3
};

class Match
{
	public:
	Shape	left;
	Shape	right;

	int	get_score_shape(void)
	{
		return ((int)right);
	}

	int	get_score_match(void)
	{
		if (left == right)
			return (3);
		if ((left == Rock && right == Paper)
			|| (left == Paper && right == Scissors)
			|| (left == Scissors && right == Rock))
			return (6);
		return (0);
	}

	int	get_score(void)
	{
		return get_score_shape() + get_score_match();
	}
};

std::vector<Match>	parse_matches(std::ifstream& input)
{
	std::vector<Match>	matches;

	while (!input.eof())
	{
		std::string	line;
		std::getline(input, line);
		if (line.length() != 3)
			break ;
		Match m;
		m.left = static_cast<Shape>(Rock + line[0] - 'A');
		m.right = static_cast<Shape>(Rock + line[2] - 'X');
		matches.push_back(m);
	}
	return (matches);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (EXIT_FAILURE);
	
	std::ifstream	file;
	file.open(argv[1]);

	if (!file)
		return (EXIT_FAILURE);

	std::vector<Match> matches = parse_matches(file);
	int	total = 0;
	for (Match& m : matches)
	{
		total += m.get_score();
	}
	
	std::cout << "Total matches: " << matches.size() << std::endl;
	std::cout << "Total match score: " << total << std::endl;

	return (EXIT_SUCCESS);
}
