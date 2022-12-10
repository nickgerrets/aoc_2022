#include <iostream>
#include <fstream>
#include <vector>
#include <array>

struct Instruction
{
	int value;
	int cycles;
};

std::vector<Instruction> parse_instructions(std::ifstream& stream)
{
	std::vector<Instruction> instructions;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		if (line.length() == 0)
			break ;
		Instruction i;
		switch (line[0])
		{
			case 'a': i.cycles = 2; i.value = std::stoi(line.substr(line.find_first_of("-0123456789"))); break;
			case 'n': i.cycles = 1; i.value = 0; break;
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
			char c = '.';
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
	
	std::ifstream	file;
	file.open(argv[1]);

	if (!file)
		return (EXIT_FAILURE);

	std::vector<Instruction> instructions = parse_instructions(file);
	std::array<bool, 40 * 6> display {0};

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
			if (i == 20 || i == 60 || i == 100 || i == 140 || i == 180 || i == 220)
				signal_strength += i * regx;
			++i;
		}
		regx += inst.value;
	}

	draw_display<40, 6>(display);

	std::cout << "Signal Strength: " << signal_strength << std::endl;

	return (EXIT_SUCCESS);
}
