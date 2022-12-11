#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

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

Operation parse_operation(std::string const& line)
{
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
std::vector<long> parse_items(std::string const& line)
{
	std::vector<long> items;
	std::string sub = line.substr(line.find_first_of(DIGIT));

	size_t i = 0;
	size_t j = 0;
	while (i < sub.length())
	{
		j = sub.find_first_of(',', i);
		if (j == std::string::npos)
			j = sub.length();
		items.push_back(std::stol(sub.substr(i, j - i)));
		j++;
		i = j;
	}
	return (items);
}

std::vector<Monkey> parse_monkeys(std::ifstream& stream)
{
	std::vector<Monkey> monkeys;

	Monkey monkey;
	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		if (line.length() == 0)
		{
			monkeys.push_back(monkey);
			monkey = Monkey();
		}
		if (line.find("items") != line.npos)
			monkey.items = parse_items(line);
		else if (line.find("Operation") != line.npos)
			monkey.operation = parse_operation(line);
		else if (line.find("Test") != line.npos)
			monkey.divisible_by = std::stoi(line.substr(line.find_first_of(DIGIT)));
		else if (line.find("true") != line.npos)
			monkey.on_true = std::stoi(line.substr(line.find_first_of(DIGIT)));
		else if (line.find("false") != line.npos)
			monkey.on_false = std::stoi(line.substr(line.find_first_of(DIGIT)));
	}
	monkeys.push_back(monkey);
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
	
	std::ifstream	file;
	file.open(argv[1]);

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
