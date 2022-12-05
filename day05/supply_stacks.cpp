#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

class Instruction
{
	public:
	Instruction(size_t amount, std::stack<char>& from, std::stack<char>& to)
		: amount(amount), from(from), to(to) {};

	size_t				amount;
	std::stack<char>&	from;
	std::stack<char>&	to;

	//	Iterative
	void	execute_iterative(void)
	{
		while (amount--)
		{
			to.push(from.top());
			from.pop();
		}
	}

	void	execute_recursive(void)
	{
		execute_recursive_impl(1);
	}

	private:

	//	recursive
	void	execute_recursive_impl(size_t i)
	{
		char c = from.top();

		from.pop();
		if (i < amount)
			execute_recursive_impl(i + 1);
		to.push(c);
	}
};

std::vector<std::string>	get_lines(std::ifstream& input)
{
	std::vector<std::string>	lines;

	while (!input.eof())
	{
		std::string line;
		std::getline(input, line);
		if (line.length() < 2 || line[1] == '1')
			break ;
		
		std::string result((line.length() + 1) / 4, ' ');
		for (size_t i = 0; i < line.length(); ++i)
		{
			if (line[i] >= 'A' && line[i] <= 'Z')
				result[i / 4] = line[i];
		}
		lines.push_back(result);
	}
	//	skip line (I feel like theres a better way)
	std::string s;
	std::getline(input, s);
	return (lines);
}

std::vector<std::stack<char>>	parse_stacks(std::ifstream& input)
{
	std::vector<std::string>	lines;
	
	lines = get_lines(input);
	std::vector<std::stack<char>>	stacks(lines[0].length());
	for (size_t i = lines.size(); i > 0; --i)
	{
		for (size_t j = 0; j < lines[i - 1].length(); ++j)
		{
			if (lines[i - 1][j] != ' ')
				stacks[j].push(lines[i - 1][j]);
		}
	}
	return (stacks);
}

std::vector<Instruction>	parse_instructions(std::ifstream& input, std::vector<std::stack<char>>& stacks)
{
	constexpr const char* nb = {"0123456789"};
	std::vector<Instruction>	instructions;

	while (!input.eof())
	{
		std::string line;
		std::getline(input, line);
		if (line.length() == 0)
			continue ;
		
		size_t i = 0;
		int	amount	= std::stoi(line.substr(i = line.find_first_of(nb)));
		int from	= std::stoi(line.substr(i = line.find_first_of(nb, line.find_first_not_of(nb, i))));
		int to		= std::stoi(line.substr(i = line.find_first_of(nb, line.find_first_not_of(nb, i))));
		instructions.push_back( Instruction(amount, stacks[from - 1], stacks[to - 1]) );
	}
	return (instructions);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (EXIT_FAILURE);
	
	std::ifstream	file;
	file.open(argv[1]);

	if (!file)
		return (EXIT_FAILURE);

	std::vector<std::stack<char>> stacks = parse_stacks(file);
	std::vector<Instruction> instructions = parse_instructions(file, stacks);

	//	Iterative:
	// for (auto& i : instructions)
	// 	i.execute_iterative();

	//	Recursive:
	for (auto& i : instructions)
		i.execute_recursive();

	for (auto& s : stacks)
		std::cout << s.top();
	std::cout << std::endl;

	return (EXIT_SUCCESS);
}