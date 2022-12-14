#include <iostream>
#include <fstream>
#include <vector>

class Match
{
	public:

	enum Shape : int
	{
		Rock = 1,
		Paper = 2,
		Scissors = 3
	};

	enum Condition : int
	{
		Lose = 1,
		Draw = 2,
		Win = 3
	};

	Match(Shape left, Shape right, Condition condition)
	: left(left), right(right), con(condition) {}

	void	set_from_condition(void)
	{
		right = Rock;
		switch (con)
		{
		case Lose:
			if (left == Rock)
				right = Scissors;
			else if (left == Scissors)
				right = Paper;
			break;
		case Draw:
			right = left;
			break;
		case Win:
			if (left == Rock)
				right = Paper;
			else if (left == Paper)
				right = Scissors;
			break;
		default:
			break;
		}
	}

	int	get_score(void) const
	{
		int score = (int)right;
		if (left == right)
			score += 3;
		else if ((left == Rock && right == Paper)
			|| (left == Paper && right == Scissors)
			|| (left == Scissors && right == Rock))
			score += 6;
		return (score);
	}

	private:

	Shape		left;
	Shape		right;
	Condition	con;
};

std::vector<Match> parse_matches(std::istream& stream)
{
	std::vector<Match> matches;
	for (std::string line; std::getline(stream, line); )
	{
		matches.emplace_back(
			static_cast<Match::Shape>(Match::Rock + line[0] - 'A'),
			static_cast<Match::Shape>(Match::Rock + line[2] - 'X'),
			static_cast<Match::Condition>(Match::Lose + line[2] - 'X')
		);
	}
	return (matches);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);
	
	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	//	Parsing
	std::vector<Match> matches = parse_matches(file);

	//	part 1
	int	total = 0;
	for (Match const& m : matches)
		total += m.get_score();
	std::cout << "Total match score (Part 1): " << total << std::endl;

	total = 0;
	for (Match& m : matches)
	{
		m.set_from_condition();
		total += m.get_score();
	}
	std::cout << "Total match score (Part 2): " << total << std::endl;

	return (EXIT_SUCCESS);
}
