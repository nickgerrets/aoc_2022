#include <iostream>
#include <fstream>
#include <queue>
#include <array>
#include <vector>

struct position
{
	int64_t x, y;
};

template<typename T>
T least_common_multiple(T a, T b)
{
	// first find greatest common devisor
	T gcd = std::min<T>(a, b);
	while (gcd > 0)
	{
		if (a % gcd == 0 && b % gcd == 0) break ;
		--gcd;
	}
	return ((a / gcd) * b);
}

class Grid
{
	public:
	enum e_direction
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT
	};

	Grid(int64_t width, int64_t height, position entrance, position goal)
	: width(width), height(height), entrance(entrance), goal(goal)
	{
		for (size_t i = 0; i < 4; ++i)
			blizzards[i].resize(width * height, false);
		cycle = least_common_multiple(width, height);
	}

	position const& get_entrance(void) const { return (entrance); }
	position const& get_goal(void) const { return (goal); }

	void set_blizzard(position const& p, char blizzard)
	{
		switch (blizzard)
		{
			case '^': blizzards[UP][pti(p)] = true; break ;
			case 'v': blizzards[DOWN][pti(p)] = true; break ;
			case '<': blizzards[LEFT][pti(p)] = true; break ;
			case '>': blizzards[RIGHT][pti(p)] = true; break ;
			default: break ;
		}
	}

	bool has_blizzard_direction(position const& p, int64_t min, e_direction dir)
	{
		switch (dir)
		{
			case UP: return (blizzards[UP][pti({p.x, (p.y + min) % height})]);
			case DOWN: return (blizzards[DOWN][pti({p.x, (p.y + (height - 1) * min) % height})]);
			case LEFT: return (blizzards[LEFT][pti({(p.x + min) % width, p.y})]);
			case RIGHT: return (blizzards[RIGHT][pti({(p.x + (width - 1) * min) % width, p.y})]);
		}
		return (false);
	}

	bool has_blizzard(position const& p, int64_t min)
	{
		return (has_blizzard_direction(p, min, UP)
				|| has_blizzard_direction(p, min, DOWN)
				|| has_blizzard_direction(p, min, LEFT)
				|| has_blizzard_direction(p, min, RIGHT));
	}

	bool move(position const& p, int64_t min)
	{
		if (p.x < 0 || p.y < 0 || p.x >= width || p.y >= height)
			return (false);
		if (has_blizzard(p, min))
			return (false);
		int64_t index = pti(p) + (min % cycle) * width * height;
		bool r = !visited[index];
		visited[index] = true;
		return (r);
	}

	int64_t find_path(position const& startp, position const& endp, int64_t time)
	{
		struct State
		{
			State (position p, int64_t min) : p(p), min(min) {}
			position p;
			int64_t min;
		};

		//	comparison lambda for the queue
		auto compare = [&](State const& a, State const& b) -> bool {
			// Using "manhattan distance"
			int64_t score_a = (std::abs(endp.x - a.p.x) + std::abs(endp.y - a.p.y)) + a.min;
			int64_t score_b = (std::abs(endp.x - b.p.x) + std::abs(endp.y - b.p.y)) + b.min;
			return (score_a > score_b);
		};

		std::priority_queue<State, std::vector<State>, decltype(compare)> queue(compare);
		queue.emplace(startp, time);
		visited.clear();
		visited.resize(width * height * cycle, false);

		while (!queue.empty())
		{
			auto const current = queue.top();
			queue.pop();
			if (current.p.x == startp.x && current.p.y == startp.y && current.min + 1 - time < cycle)
				queue.emplace(current.p, current.min + 1);
			position const dirs[] = {{0, 0}, {0, -1}, {0, 1}, {-1, 0}, {1, 0}};
			for (auto const& d : dirs)
			{
				position next = {current.p.x + d.x, current.p.y + d.y};
				if (next.x == endp.x && next.y == endp.y)
					return (current.min + 1);
				if (move(next, current.min + 1))
					queue.emplace(next, current.min + 1);
			}
		}
		return (-1);
	}

	private:

	//	Position to index helper func
	int64_t pti(position const& p)
	{
		return ( p.x + p.y * width);
	}

	int64_t width, height;
	position entrance, goal;

	std::array<std::vector<bool>, 4> blizzards;

	std::vector<bool> visited;
	int64_t cycle;
};

Grid parse_grid(std::istream& stream)
{
	int64_t width, height;
	position entrance, goal;
	std::vector<std::string> blizzards;

	std::string line; std::getline(stream, line);
	width = line.size() - 2;
	entrance = {static_cast<int64_t>(line.find('.')) - 1, -1};
	while (std::getline(stream, line))
	{
		if (line[1] == '#' || line[2] == '#')
			break ;
		blizzards.push_back(line);
    }
	height = blizzards.size();
	goal = {static_cast<int64_t>(line.find('.')) - 1, height};

	Grid grid(width, height, entrance, goal);
	for (int64_t y = 0; y < height; ++y)
	{
		for (int64_t x = 0; x < width; ++x)
			grid.set_blizzard({x, y}, blizzards[y][x + 1]);
	}
	return (grid);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);

	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	Grid grid = parse_grid(file);

	//	Part 1
	int64_t min = grid.find_path(grid.get_entrance(), grid.get_goal(), 0);
	std::cout << "To goal: " << min << std::endl;

	//	Part 2
	min = grid.find_path(grid.get_goal(), grid.get_entrance(), min); // back up
	min = grid.find_path(grid.get_entrance(), grid.get_goal(), min); // and down
	std::cout << "Then back to the start and to the goal again: " << min << std::endl;

	return (EXIT_SUCCESS);
}
