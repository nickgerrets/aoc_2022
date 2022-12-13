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

	int is_in_order(Value* other)
	{
		//	Left side is smaller than right side
		if (type == Integer && other->type == Integer)
		{
			if (data.i < other->data.i)
				return (1);
			if (data.i == other->data.i)
				return (0);
			if (data.i > other->data.i)
				return (-1);
		}
		if (type == List && other->type == List)
			return (compare_lists(data.list, other->data.list));
		if (type != other->type)
		{
			std::vector<Value*> v;
			if (type == Integer)
			{
				v.push_back(this);
				return (compare_lists(v, other->data.list));
			}
			else if (other->type == Integer)
			{
				v.push_back(other);
				return (compare_lists(data.list, v));
			}
		}
		return (-1);
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

	int compare_lists(std::vector<Value*>& a, std::vector<Value*>& b)
	{
		size_t i = 0;
		for (i = 0; i < a.size(); ++i)
		{
			//	Right side ran out of items
			if (i >= b.size())
				return (-1);
			//	is child in order
			int r = a[i]->is_in_order(b[i]);
			if (r != 0)
				return (r);
		}
		//	Left side ran out of items
		if (i < b.size())
			return (1);
		return (0);
	}

};

Value* parse_value(std::string const& line)
{
	Value* value = new Value(std::vector<Value*> {});

	size_t i = 0;
	size_t j = 0;
	size_t listc = 0;
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
			++listc;
			++j;
			while (listc)
			{
				j = line.find_first_of("[]", j);
				if (line[j] == '[')
					++listc;
				else
					--listc;
				++j;
			}
			std::string sub = line.substr(i + 1, j - i - 1);
			value->data.list.push_back(parse_value(sub));
		}
		j++;
		i = j;
	}
	return (value);
}

std::vector<Value*> parse_values(std::ifstream& stream)
{
	std::vector<Value*> values;

	for (std::string line; std::getline(stream, line); )
	{
		if (line.length() < 2)
			continue ;
		values.push_back(parse_value(line.substr(1)));
	}
	return (values);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);

	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	//	PARSING
	std::vector<Value*> values = parse_values(file);

	//	PART 1
	size_t sum = 0;
	for (size_t i = 1; i < values.size(); ++++i)
	{
		if (values[i - 1]->is_in_order(values[i]) == 1)
			sum += i * 0.5 + 1;
	}
	std::cout << "Sum: " << sum << std::endl;

	//	PART 2
	Value* p1 = new Value(std::vector<Value*>({new Value(2)}));
	Value* p2 = new Value(std::vector<Value*>({new Value(6)}));
	values.push_back(p1);
	values.push_back(p2);

	std::sort(values.begin(), values.end(), [](Value* a, Value* b)
	{
		if (a->is_in_order(b) > 0)
			return(true);
		return (false);
	});

	size_t dec_key = 0;
	for (size_t i = 0; i < values.size(); ++i)
	{
		if (values[i] == p1)
			dec_key = i + 1;
		if (values[i] == p2)
		{
			dec_key *= i + 1;
			break ;
		}
	}

	std::cout << "Decoder key: " << dec_key << std::endl;

	return (EXIT_SUCCESS);
}
