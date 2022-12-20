#include <iostream>
#include <fstream>
#include <vector>

#include "../include/common.h"

using namespace aoc;

struct Data
{
	std::vector<int64_t> values;
	std::vector<int64_t> positions;
	int64_t zero_position;
};

int64_t get_value_at(Data const& data, int64_t index)
{
	for (int64_t i = 0; i < data.positions.size(); ++i)
	{
		if (data.positions[i] == index % static_cast<int64_t>(data.positions.size()))
			return (data.values[i]);
	}
	//	Shouldn't happen
	std::cerr << "BAD POSITION" << std::endl;
	return (0);
}

void mix(Data& data)
{
	int64_t modulo = static_cast<int64_t>(data.values.size());
	for (int64_t i = 0; i < modulo; ++i)
	{
		if (data.values[i] % (modulo - 1) == 0)
			continue ;
		int64_t oldp = data.positions[i];
		int64_t newp = oldp + data.values[i] % (modulo - 1);
		if (!(0 < newp && newp < modulo))
		{
			if (newp <= 0)
				newp = newp + modulo - 1;
			else
				newp = newp - modulo + 1;
		}
		for (auto &p : data.positions)
		{
			if (oldp < newp && oldp <= p && p <= newp)
				p = (p - 1 + modulo) % modulo;
			else if (newp < oldp && newp <= p && p <= oldp)
				p = (p + 1) % modulo;
		}
		data.positions[i] = newp;
	}
}

Data parse_data(std::istream& stream, int64_t key)
{
	Data data;
	size_t i = 0;
	while (!stream.eof())
	{
		int v;
		stream >> v;
		stream >> skipl;
		if (v == 0)
			data.zero_position = i;
		data.values.push_back(v * key);
		data.positions.push_back(i);
		++i;
	}
	return (data);
}

int main(int argc, char **argv) {
	if (argc < 2)
		return (EXIT_FAILURE);

	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	int64_t decr_key = 1;
	if (argc > 2)
		decr_key = std::stol(argv[2]);

	Data data = parse_data(file, decr_key);

	mix(data);

	if (decr_key != 1)
	{
		for (size_t count = 0; count < 9; ++count)
			mix(data);
	}

	int64_t v1 = get_value_at(data, data.positions[data.zero_position] + 1000);
	int64_t v2 = get_value_at(data, data.positions[data.zero_position] + 2000);
	int64_t v3 = get_value_at(data, data.positions[data.zero_position] + 3000);
	
	std::cout << v1 + v2 + v3 << std::endl;
}