#include <iostream>
#include <fstream>
#include <vector>

# define SIGN(x) (x > 0) - (x < 0)

struct Point
{
	int x = 0;
	int y = 0;

	Point() {}
	Point(int x, int y) : x(x), y(y) {}

	Point& operator+(Point& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return (*this);
	}
};

static const Point UP = {0, 1};
static const Point DOWN = {0, -1};
static const Point LEFT = {-1, 0};
static const Point RIGHT = {1, 0};

struct Instruction
{
	Point to;
	size_t amount;
};

class Grid
{
	public:
	Grid(size_t width, size_t height, size_t snake_length)
	: width(width), height(height)
	{
		visited.insert(visited.begin(), width * height, false);
		Point head(width * 0.5, height * 0.5);
		tail.insert(tail.begin(), snake_length, head);
		visited[head.x + head.y * width] = true;
	}

	size_t width;
	size_t height;
	std::vector<bool> visited;
	std::vector<Point> tail;

	void move(Point to)
	{
		std::vector<Point> prev = tail;

		tail[0] = tail[0] + to;
		for (size_t i = 1; i < tail.size(); ++i)
		{
			if (is_too_far(tail[i], tail[i - 1]))
			{
				tail[i].x += SIGN(tail[i - 1].x - tail[i].x);
				tail[i].y += SIGN(tail[i - 1].y - tail[i].y);
			}
		}

		Point& last = tail.back();
		visited[last.x + width * last.y] = true;
	}

	private:

	bool is_too_far(Point& a, Point& b)
	{
		return (a.x < b.x - 1 || a.x > b.x + 1 || a.y < b.y - 1 || a.y > b.y + 1);
	}
};

std::vector<Instruction>	parse_instructions(std::ifstream& stream)
{
	std::vector<Instruction> instructions;

	while (!stream.eof())
	{
		std::string line;
		std::getline(stream, line);
		if (line.length() == 0)
			break ;
		Point to;
		Instruction i;
		switch (line[0])
		{
			case 'U': i.to = UP; break;
			case 'D': i.to = DOWN; break;
			case 'L': i.to = LEFT; break;
			case 'R': i.to = RIGHT; break;
		}
		i.amount = std::stoul(line.substr(1));
		instructions.push_back(i);
	}
	return (instructions);
}

// ./program_name <input_file> [snake_length]
int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);
	
	std::ifstream	file;
	file.open(argv[1]);

	size_t snake_length = 10;
	if (argc > 2)
	{
		try { snake_length = std::stoul(argv[2]); } catch(...) {}
	}

	if (!file)
		return (EXIT_FAILURE);

	std::vector<Instruction> instructions = parse_instructions(file);

	//	Too lazy to have it expand automatically, therefore gridsize 1000
	Grid grid(1000, 1000, snake_length);
	for (auto& i : instructions)
	{
		// std::cout << "executing instruction: {" << i.amount << ", (" << i.to.x << ',' << i.to.y << ")}" << std::endl;
		while (i.amount--)
		{
			grid.move(i.to);
		}
	}

	size_t amount_visited = 0;
	for (bool v : grid.visited)
	{
		if (v)
			amount_visited++;
	}

	std::cout << "Spaces visited: " << amount_visited << std::endl;

	return (EXIT_SUCCESS);
}