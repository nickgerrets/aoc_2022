#include <iostream>
#include <fstream>
#include <vector>
#include <deque>


class Grid
{
	public:
	Grid() : width(0), height(0), start(0), end(0) {}

	std::vector<int> data;
	size_t start;
	size_t end;
	size_t width;
	size_t height;

	void print(void)
	{
		for (size_t y = 0; y < height; ++y)
		{
			for (size_t x = 0; x < width; ++x)
			{
				size_t index = x + y * width;
				char c = 'a' + data[index];
				if (index == start)
					c = 'S';
				else if (index == end)
					c = 'E';
				std::cout << c;
			}
			std::cout << std::endl;
		}
	}

	//	Using Dijkstra's
	size_t solve(bool find_shortest_to_a = false)
	{
		//	deque with pairs of STEPS & INDEX
		std::deque<std::pair<size_t, size_t>> Q;
		std::vector<bool> visited(data.size());
		Q.push_back( {0, end} );

		while (true)
		{
			// Pop front
			auto const current = Q.front();
			Q.pop_front();
			if (visited[current.second])
				continue ;
			
			//	Set to visited an check if at end ('start')
			visited[current.second] = true;
			if (data[current.second] == 0 && (find_shortest_to_a || current.second == start))
				return (current.first);
			
			// Push back all new options into Q
			int directions[] { 1, -1, -static_cast<int>(width), static_cast<int>(width) };
			for (int d : directions)
			{
				int index = current.second + d;
				//	Out of bounds check
				if (index < 0 || index > static_cast<int>(data.size()))
					continue ;
				//	Not visited and not more than 1 height difference
				if (!visited[index] && data[current.second] - data[index] <= 1)
					Q.push_back( { current.first + 1, index } );
			}
		}
	}
};

Grid parse_grid(std::ifstream& stream)
{
	Grid grid;

	for (std::string line; std::getline(stream, line); )
	{
		grid.width = line.length();
		for (size_t i = 0; i < grid.width; ++i)
		{
			char c = line[i];
			if (c == 'E')
			{
				c = 'z';
				grid.end = i + grid.height * grid.width;
			}
			else if (c == 'S')
			{
				c = 'a';
				grid.start = i + grid.height * grid.width;
			}
			grid.data.push_back(static_cast<int>(c - 'a'));
		}
		++grid.height;
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

	std::cout << "Steps required from S to E: " << grid.solve() << std::endl;
	std::cout << "Steps required from E to shortest a: " << grid.solve(true) << std::endl;

	return (EXIT_SUCCESS);
}
