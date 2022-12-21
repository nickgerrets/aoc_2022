#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "../include/common.h"

using namespace aoc;

class Monkey
{
	public:

	enum
	{
		NONE,
		ADD,
		SUBTRACT,
		MULTIPLY,
		DIVIDE
	} operation;

	std::string m1, m2;
	int64_t n;

	void print(void) const
	{
		if (operation == NONE)
		{
			std::cout << n << std::endl;
			return ;
		}
		std::cout << m1 << ' ';
		switch (operation)
		{
			case ADD : std::cout << '+'; break ;
			case SUBTRACT : std::cout << '-'; break ;
			case MULTIPLY : std::cout << '*'; break ;
			case DIVIDE : std::cout << '/'; break ;
			default : break ;
		}
		std::cout << ' ' << m2 << std::endl;
	}
};

std::unordered_map<std::string, Monkey> parse_monkeys(std::istream& stream)
{
	std::unordered_map<std::string, Monkey> monkeys;

	for (std::string line; std::getline(stream, line); )
	{
		Monkey m;
		std::string key, a, b, c;
		std::stringstream ss(line);

		ss >> key >> a >> b >> c;
		key.erase(key.end() - 1);
		if (b.empty())
		{
			m.operation = Monkey::NONE;
			m.n = std::stol(a);
		}
		else
		{
			switch (b[0])
			{
				case '+': m.operation = Monkey::ADD; break ;
				case '-': m.operation = Monkey::SUBTRACT; break ;
				case '*': m.operation = Monkey::MULTIPLY; break ;
				case '/': m.operation = Monkey::DIVIDE; break ;
			}
			m.m1 = a;
			m.m2 = c;
		}
		monkeys.emplace(key, m);
	}
	return (monkeys);
}

int64_t execute_monkey(std::string const& key, std::unordered_map<std::string, Monkey>& monkeys)
{
	Monkey& m = monkeys[key];

	if (m.operation == Monkey::NONE)
		return (m.n);
	
	int64_t l = execute_monkey(m.m1, monkeys);
	int64_t r = execute_monkey(m.m2, monkeys);
	switch (m.operation)
	{
		case Monkey::ADD: return (l + r);
		case Monkey::SUBTRACT: return (l - r);
		case Monkey::MULTIPLY: return (l * r);
		case Monkey::DIVIDE: return (l / r);
		default : break ;
	}
	return (0);
}

bool contains(std::string const& key, std::string const& search, std::unordered_map<std::string, Monkey>& monkeys)
{
	Monkey& m = monkeys[key];

	if (key == search)
		return (true);

	if (m.operation == Monkey::NONE)
		return (false);
	
	if (m.m1 == search || m.m2 == search)
		return (true);
	return (contains(m.m1, search, monkeys) || contains(m.m2, search, monkeys));
}

void set_humn(std::string const& key, std::unordered_map<std::string, Monkey>& monkeys, int64_t target_val)
{
	Monkey& m = monkeys[key];
	
	if (key == "humn")
	{
		m.n = target_val;
		return ;
	}
	if (m.operation == Monkey::NONE)
		return ;
	
	bool left = contains(m.m1, "humn", monkeys);
	std::string key_set = m.m2;
	std::string key_val = m.m1;
	if (left)
	{
		key_set = m.m1;
		key_val = m.m2;
	}
	if (key == "root")
	{
		set_humn(key_set, monkeys, target_val);
		return ;
	}

	switch (m.operation)
	{
		case Monkey::ADD: set_humn(key_set, monkeys, target_val - execute_monkey(key_val, monkeys)); break ;
		case Monkey::SUBTRACT:
			if (left)
				set_humn(key_set, monkeys, target_val + execute_monkey(key_val, monkeys));
			else
				set_humn(key_set, monkeys, execute_monkey(key_val, monkeys) - target_val);
			break ;
		case Monkey::MULTIPLY: set_humn(key_set, monkeys, target_val / execute_monkey(key_val, monkeys)); break ;
		case Monkey::DIVIDE:
			if (left)
				set_humn(key_set, monkeys, target_val * execute_monkey(key_val, monkeys));
			else
				set_humn(key_set, monkeys, execute_monkey(key_val, monkeys) / target_val);
			break ;
		default : break ;
	}
}

int main(int argc, char **argv) {
	if (argc < 2)
		return (EXIT_FAILURE);

	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	std::unordered_map<std::string, Monkey> monkeys = parse_monkeys(file);

	//	Part 1
	std::cout << "Part1: " << execute_monkey("root", monkeys) << std::endl;

	//	part 2
	int64_t target_val;
	if (contains(monkeys["root"].m1, "humn", monkeys))
		target_val = execute_monkey(monkeys["root"].m2, monkeys);
	else
		target_val = execute_monkey(monkeys["root"].m1, monkeys);

	set_humn("root", monkeys, target_val);

	std::cout << "Part2: " << monkeys["humn"].n << std::endl;

	return (EXIT_SUCCESS);
}