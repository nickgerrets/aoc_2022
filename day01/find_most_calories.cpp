#include <iostream>
#include <fstream>
#include <vector>

std::vector<int>	get_calorie_sums(std::ifstream& input)
{
	std::vector<int>	v;

	int	calories = 0;
	while (!input.eof())
	{
		std::string	str;
		std::getline(input, str);
		if (str.length() == 0)
		{
			v.push_back(calories);
			calories = 0;
			continue ;
		}
		calories += std::stoi(str);
	}
	return (v);
}

int	main(int argc, char **argv)
{
	std::ifstream		input;
	
	if (argc != 2)
		return (EXIT_FAILURE);

	input.open(argv[1]);
	std::vector<int> calorie_vector = get_calorie_sums(input);

	int max = *max_element(calorie_vector.begin(), calorie_vector.end());
	std::cout << "Highest calorie amount: " << max << std::endl;

	return (EXIT_SUCCESS);
}
