#include <iostream>
#include <fstream>
#include <vector>

std::vector<int> get_calorie_sums(std::istream& stream)
{
	std::vector<int>	v;

	int	calories = 0;
	for (std::string line; std::getline(stream, line); )
	{
		if (line.length() == 0)
		{
			v.push_back(calories);
			calories = 0;
			continue ;
		}
		calories += std::stoi(line);
	}
	return (v);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);

	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	std::vector<int> cal_sums = get_calorie_sums(file);

	int	sum = 0;
	auto it = max_element(cal_sums.begin(), cal_sums.end());
	std::cout << "Top Highest calorie amount: " << *it << std::endl;
	sum += *it;
	cal_sums.erase(it);

	it = max_element(cal_sums.begin(), cal_sums.end());
	std::cout << "Second Highest calorie amount: " << *it << std::endl;
	sum += *it;
	cal_sums.erase(it);

	it = max_element(cal_sums.begin(), cal_sums.end());
	std::cout << "Third Highest calorie amount: " << *it << std::endl;
	sum += *it;

	std::cout << "Sum: " << sum << std::endl;

	return (EXIT_SUCCESS);
}
