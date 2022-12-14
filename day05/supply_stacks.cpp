#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

#include "../include/common.h"

using namespace aoc;

class Instruction
{
	public:
	Instruction(size_t amount, std::stack<char>& from, std::stack<char>& to)
	: amount(amount), from(from), to(to) {};

	//	Iterative
	void	execute_iterative(void) const
	{
		size_t a = amount;
		while (a--)
		{
			to.push(from.top());
			from.pop();
		}
	}

	//	recursive
	void	execute_recursive(void) const
	{
		execute_recursive_impl(1);
	}

	private:

	void	execute_recursive_impl(size_t i) const
	{
		char c = from.top();

		from.pop();
		if (i < amount)
			execute_recursive_impl(i + 1);
		to.push(c);
	}

	size_t				amount;
	std::stack<char>&	from;
	std::stack<char>&	to;
};

std::vector<std::string> get_lines(std::istream& stream)
{
	std::vector<std::string> lines;
	for (std::string line; std::getline(stream, line); )
	{
		if (line.length() == 0)
			break ;
		if (line.length() < 2 || line[1] == '1')
			continue;

		std::string result((line.length() + 1) / 4, ' ');
		for (size_t i = 0; i < line.length(); ++i)
		{
			if (line[i] >= 'A' && line[i] <= 'Z')
				result[i / 4] = line[i];
		}
		lines.push_back(result);
	}
	return (lines);
}

std::vector<std::stack<char>> parse_stacks(std::istream& stream)
{
	std::vector<std::string> lines = get_lines(stream);
	std::vector<std::stack<char>> stacks(lines[0].length());

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

std::vector<Instruction> parse_instructions(std::istream& stream, std::vector<std::stack<char>>& stacks)
{
	std::vector<Instruction> instructions;
	while (!stream.eof())
	{
		size_t amount, from, to;
		stream >> getn >> amount >> getn >> from >> getn >> to;
		instructions.emplace_back(amount, stacks[from - 1], stacks[to - 1]);
	}
	return (instructions);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);
	
	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	std::vector<std::stack<char>> stacks = parse_stacks(file);
	std::vector<Instruction> instructions = parse_instructions(file, stacks);

	//	Iterative (part 1):
	// for (auto& i : instructions)
	// 	i.execute_iterative();

	//	Recursive (part 2):
	for (auto& i : instructions)
		i.execute_recursive();

	std::cout << "Stack tops: ";
	for (auto& s : stacks)
		std::cout << s.top();
	std::cout << std::endl;

	return (EXIT_SUCCESS);
}