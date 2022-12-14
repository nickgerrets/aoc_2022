#include <iostream>
#include <fstream>
#include <vector>
#include <array>

struct Instruction
{
	int value;
	int cycles;
};

std::vector<Instruction> parse_instructions(std::istream& stream)
{
	std::vector<Instruction> instructions;
	for (std::string line; std::getline(stream, line); )
	{
		Instruction i {0, 1};
		if (line[0] == 'a')
		{
			i.value = std::stoi(line.substr(line.find_first_of("-0123456789")));
			i.cycles = 2;
		}
		instructions.push_back(i);
	}
	return (instructions);
}

template<size_t _Width, size_t _Height>
void draw_display(std::array<bool, _Width * _Height> const& display)
{
	for (size_t y = 0; y < _Height; ++y)
	{
		for (size_t x = 0; x < _Width; ++x)
		{
			char c = ' ';
			if (display[x + y * _Width])
				c = '#';
			std::cout << c;
		}
		std::cout << std::endl;
	}
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);
	
	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	//	Parsing
	std::vector<Instruction> instructions = parse_instructions(file);
	std::array<bool, 40 * 6> display {false};

	size_t i = 1;
	int regx = 1;
	int signal_strength = 0;
	for (auto& inst : instructions)
	{
		while (inst.cycles--)
		{
			//	Update the display
			int x = i % 40 - 1;
			if (x >= regx - 1 && x <= regx + 1)
				display[i - 1] = true;
			//	Update signal
			if (i == 20 || i % 40 == 20)
				signal_strength += i * regx;
			++i;
		}
		regx += inst.value;
	}

	draw_display<40, 6>(display);

	std::cout << "Signal Strength: " << signal_strength << std::endl;

	return (EXIT_SUCCESS);
}
