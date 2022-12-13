#include <iostream>
#include <fstream>
#include <vector>

class Value
{
	public:
	Value(std::vector<Value*>&& list) : type(List), data(std::move(list)) {}
	Value(int i) : type(Integer), data(i) {}

	~Value()
	{
		if (type == List)
			data.list.~vector();
	}

	int check_with(Value* other)
	{
		if (type == Integer && other->type == Integer)
		{
			
		}
	}

	enum Type
	{
		List,
		Integer
	}	type;
	
	union Data
	{
		Data(std::vector<Value*>&& list) : list(std::move(list)) {}
		Data(int i) : i(i) {}
		~Data() {}

		std::vector<Value*> list;
		int i;
	}	data;

	void print(void)
	{
		if (type == Integer)
			std::cout << data.i << ',';
		else
		{
			std::cout << '[';
			for (auto* v : data.list)
				v->print();
			std::cout << ']';
		}
	}

};

Value* parse_value(std::string const& line)
{
	Value* value = new Value(std::vector<Value*> {});

	size_t i = 0;
	size_t j = 0;
	while (i < line.length())
	{
		j = line.find_first_of(",[]", i);
		if (j == std::string::npos)
		{
			j = line.length() - 1;
			value->data.list.push_back(new Value(std::stoi(line.substr(i, j - i))));
		}
		else if ((line[j] == ',' || line[j] == ']') && j - i > 0)
			value->data.list.push_back(new Value(std::stoi(line.substr(i, j - i))));
		else if (line[j] == '[')
		{
			i = j;
			j = line.find_first_of(']', i);
			value->data.list.push_back(parse_value(line.substr(i + 1, j)));
		}
		j++;
		i = j;
	}
	return (value);
}

std::vector<std::pair<Value*, Value*>> parse_value_pairs(std::ifstream& stream)
{
	std::vector<std::pair<Value*, Value*>> value_pairs;

	for (std::string line; std::getline(stream, line); )
	{
		std::pair<Value*, Value*> pair;

		if (line.length() < 2)
			continue ;

		pair.first = parse_value(line.substr(1));
		std::getline(stream, line);
		pair.second = parse_value(line.substr(1));
		value_pairs.push_back(pair);
	}
	return (value_pairs);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);
	
	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	std::vector<std::pair<Value*, Value*>> pairs = parse_value_pairs(file);

	for (auto& p : pairs)
	{
		if (p.first->check_with(p.second));
	}

	return (EXIT_SUCCESS);
}
