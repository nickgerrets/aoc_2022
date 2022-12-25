#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <math.h>

std::unordered_map<char, int> char_to_int = {{'=', -2}, {'-', -1}, {'0', 0}, {'1', 1}, {'2', 2}};
std::unordered_map<int, char> int_to_char;

void construct_map(void)
{
	for (auto const& p : char_to_int)
		int_to_char[p.second] = p.first;
}

int64_t convert_to_decimal(std::string const& snafu)
{
	size_t i = 0;
	int64_t sum = 0;
	for (auto it = snafu.rbegin(); it != snafu.rend(); ++it)
	{
		sum += pow(5, i) * char_to_int[*it];
		++i;
	}
	return (sum);
}

std::string convert_to_snafu(int64_t n)
{
	std::string snafu;

	while (n > 0)
	{
		int64_t r = n % 5;
		if (r > 2)
		{
			n += r;
			//	Insert at the front
			snafu.insert(0, 1, int_to_char[r - 5]);
		}
		else
			snafu.insert(0, std::to_string(r));
		n /= 5;
	}
	return (snafu);
}

std::vector<std::string> parse_lines(std::istream& stream)
{
	std::vector<std::string> lines;
	for (std::string line; std::getline(stream, line); )
		lines.push_back(line);
	return (lines);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);

	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	construct_map();

	std::vector<std::string> lines = parse_lines(file);

	int64_t sum = 0;
	for (auto const& s : lines)
		sum += convert_to_decimal(s);
	
	std::cout << "Decimal Fuel number: " << sum << std::endl;
	std::cout << "SNAFU Fuel number: " << convert_to_snafu(sum) << std::endl;

	return (EXIT_SUCCESS);
}
