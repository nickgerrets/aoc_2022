#include <iostream>
#include <fstream>
#include <vector>

std::vector<int>	get_calorie_sums(std::ifstream& input)
{
	std::vector<int>	v;

	int	calories = 0;
	std::string	str;
	while (std::getline(input, str))
	{
		if (str.length() == 0)
		{
			v.push_back(calories);
			calories = 0;
			continue ;
		}
		calories += std::stoi(str);
	}
	v.push_back(calories);
	return (v);
}

int	main(int argc, char **argv)
{
	std::ifstream		input;
	
	if (argc != 2)
		return (EXIT_FAILURE);

	input.open(argv[1]);
	if (!input)
		return (EXIT_FAILURE);

	std::vector<int> calorie_vector = get_calorie_sums(input);

	int	sum = 0;
	auto it = max_element(calorie_vector.begin(), calorie_vector.end());
	std::cout << "Top Highest calorie amount: " << *it << std::endl;
	sum += *it;
	calorie_vector.erase(it);

	it = max_element(calorie_vector.begin(), calorie_vector.end());
	std::cout << "Second Highest calorie amount: " << *it << std::endl;
	sum += *it;
	calorie_vector.erase(it);

	it = max_element(calorie_vector.begin(), calorie_vector.end());
	std::cout << "Third Highest calorie amount: " << *it << std::endl;
	sum += *it;

	std::cout << "Sum: " << sum << std::endl;

	return (EXIT_SUCCESS);
}
