#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

#include "../include/common.h"

using namespace aoc;

#define DIGIT "0123456789"

class Operation
{
	public:
	
	enum OperationType
	{
		ADD,
		MULTIPLY,
		SQUARE
	};

	Operation(OperationType optype = ADD, long value = 0) : optype(optype), value(value) {}

	OperationType	optype;
	long			value;

	long execute(long item) const
	{
		switch (optype)
		{
			case ADD: return (item + value);
			case MULTIPLY: return (item * value);
			case SQUARE: return (item * item);
		}
		return (item);
	}
};

class Monkey
{
	public:

	Monkey() : inspected(0) {}

	std::vector<long>	items;
	Operation			operation;
	long				divisible_by;
	size_t				on_true;
	size_t				on_false;
	size_t				inspected;

	void print(void)
	{
		std::cout << "items: ";
		for (long& i : items)
			std::cout << i << ' ';
		std::cout << "\noperation: " << operation.optype << ' ' << operation.value;
		std::cout << "\ndivisible by: " << divisible_by;
		std::cout << "\non_true: " << on_true;
		std::cout << "\non_false: " << on_false << std::endl;
	}

};

Operation parse_operation(std::istream& stream)
{
	std::string line; std::getline(stream, line);
	std::string sub = line.substr(line.find("old") + 4);

	Operation::OperationType type = Operation::MULTIPLY;
	if (sub[0] == '+')
		type = Operation::ADD;
	else if (sub.find("old") != std::string::npos)
		type = Operation::SQUARE;
	
	long value = 0;
	if (type == Operation::ADD || type == Operation::MULTIPLY)
		value = std::stoi(sub.substr(sub.find_first_of(DIGIT)));
	return ( Operation(type, value) );
}

//	Need longs because otherwise the 10000 rounds version overflows
std::vector<long> parse_items(std::istream& stream)
{
	std::string line; std::getline(stream, line);
	std::stringstream ss(line);

	std::vector<long> items;
	while (!ss.eof())
	{
		long v;
		ss >> getn >> v;
		items.push_back(v);
	}
	return (items);
}

std::vector<Monkey> parse_monkeys(std::ifstream& stream)
{
	std::vector<Monkey> monkeys;
	while(!stream.eof())
	{
		Monkey m;
		stream >> skipl;
		m.items = parse_items(stream);
		m.operation = parse_operation(stream);
		stream	>> getn >> m.divisible_by
				>> getn >> m.on_true
				>> getn >> m.on_false
				>> skipl >> skipl;

		monkeys.push_back(m);
	}

	return (monkeys);
}

long least_common_multiple(long a, long b)
{
	// first find greatest common devisor
	long gcd = std::min(a, b);
	while (gcd > 0)
	{
		if (a % gcd == 0 && b % gcd == 0) break ;
		--gcd;
	}
	return ((a / gcd) * b);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);
	
	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	std::vector<Monkey> monkeys = parse_monkeys(file);

	//	creating a modulo that works for every divisible by getting the least common multiple
	long mod = monkeys[0].divisible_by;
	for (auto& m : monkeys)
		mod = least_common_multiple(mod, m.divisible_by);

	for (size_t round = 0; round < 10000; ++round)
	{
		for (auto& m : monkeys)
		{
			// Inspect items
			for (auto it = m.items.begin(); it != m.items.end(); ++it)
			{
				m.inspected++;
				*it = *it % mod;
				*it = m.operation.execute(*it);
				if (*it % m.divisible_by == 0)
					monkeys[m.on_true].items.push_back(*it);
				else
					monkeys[m.on_false].items.push_back(*it);
				m.items.erase(it--);
			}
		}
	}

	//	Sort based on the amount of items inspected
	std::sort(monkeys.begin(), monkeys.end(), [](Monkey& a, Monkey& b)
	{
		return (a.inspected > b.inspected);
	});

	std::cout << monkeys[0].inspected * monkeys[1].inspected << std::endl;

	return (EXIT_SUCCESS);
}
