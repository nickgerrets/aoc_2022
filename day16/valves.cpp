#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>

#include "../include/common.h"

using namespace aoc;

struct Valve
{
	std::vector<std::string> tunnels;
	int pressure;
};

std::unordered_map<std::string, Valve> parse_valves(std::istream& stream)
{
	std::unordered_map<std::string, Valve> valves;

	while (!stream.eof())
	{
		Valve v; std::string name; std::string g;
		stream >> name >> name;
		stream >> getn >> v.pressure;
		stream >> g >> g >> g >> g >> g >> std::ws;
		while (!stream.eof())
		{
			std::string tunnel;
			tunnel += stream.get();
			tunnel += stream.get();
			v.tunnels.push_back(tunnel);
			if (stream.get() == '\n')
				break ;
			stream.get();
		}
		valves.emplace(name, v);
	}
	return (valves);
}

std::map<std::pair<std::string, std::string>, int> floyd_warshall(std::unordered_map<std::string, Valve>& valves)
{
	std::map<std::pair<std::string, std::string>, int> shortest_path;

	//	Fill shortest path
	for (auto const& pair : valves)
	{
		for (auto const& s : pair.second.tunnels)
			shortest_path[{pair.first, s}] = 1;
		shortest_path[{pair.first, pair.first}] = 0;
	}
	//	Calculate the actual weights
	for (auto const& pair1 : valves)
	{
		for (auto const& pair2 : valves)
		{
			if (shortest_path.find({pair1.first, pair2.first}) == shortest_path.end())
				continue ;
			for (auto const& pair3 : valves)
			{
				if (shortest_path.find({pair1.first, pair3.first}) == shortest_path.end())
					continue ;
				if (shortest_path.find({pair2.first, pair3.first}) != shortest_path.end())
				{
					shortest_path[{pair2.first, pair3.first}] = std::min(
						shortest_path[{pair2.first, pair3.first}],
						shortest_path[{pair2.first, pair1.first}] + shortest_path[{pair1.first, pair3.first}]);
				}
				else
				{
					shortest_path[{pair2.first, pair3.first}] = 
						shortest_path[{pair2.first, pair1.first}] + shortest_path[{pair1.first, pair3.first}];
				}
			}
		}
	}
	return (shortest_path);
}

std::map<std::pair<std::string, std::string>, int> shortest_path;
std::map<std::string, Valve> with_pressure;

int find_path(int minutes,
	std::string const& from = "AA",
	int time = 0,
	std::set<std::string> open_valves = {})
{
	if (with_pressure.size() == open_valves.size())
		return (0);
	int total_pressure = 0;
	for (auto const& pair : with_pressure)
	{
		if (open_valves.find(pair.first) != open_valves.end())
			continue ;
		int time_to_open = time + shortest_path[{from, pair.first}] + 1;
		if  (time_to_open <= minutes)
		{
			int pressure = pair.second.pressure * (minutes - time_to_open);
			open_valves.insert(pair.first);
			int recursive_pressure = find_path(minutes, pair.first, time_to_open, open_valves);
			open_valves.erase(pair.first);
			total_pressure = std::max(total_pressure, pressure + recursive_pressure);
		}
	}
	return (total_pressure);
}

std::set<std::string> open_valves;
int find_path_with_elephant(int minutes,
	std::string const& from = "AA",
	std::string const& from_elephant = "AA",
	int time = 0,
	int time_elephant = 0)
{
	if (with_pressure.size() == open_valves.size())
		return (0);

	int total_pressure = 0;
	for (auto const& pair : with_pressure)
	{
		auto const& destination = pair.first;
		if (open_valves.find(destination) != open_valves.end())
			continue ;
		int time_to_open = time + shortest_path.at({from, destination}) + 1;
		int time_to_open_elephant = time_elephant + shortest_path.at({from_elephant, destination}) + 1;
		if (time_to_open <= time_to_open_elephant && time_to_open <= minutes)
		{
			int pressure = pair.second.pressure * (minutes - time_to_open);
			open_valves.insert(destination);
			int recursive_pressure = find_path_with_elephant(minutes, destination, from_elephant, time_to_open, time_elephant);
			open_valves.erase(destination);
			total_pressure = std::max(total_pressure, pressure + recursive_pressure);
		}
		else if (time_to_open_elephant <= time_to_open && time_to_open_elephant <= minutes)
		{
			int pressure = pair.second.pressure * (minutes - time_to_open_elephant);
			open_valves.insert(destination);
			int recursive_pressure = find_path_with_elephant(minutes, from, destination, time, time_to_open_elephant);
			open_valves.erase(destination);
			total_pressure = std::max(total_pressure, pressure + recursive_pressure);
		}
	}

	return (total_pressure);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);

	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	//	PARSING
	std::unordered_map<std::string, Valve> valves = parse_valves(file);

	for (auto const& v : valves)
	{
		if (v.second.pressure > 0)
			with_pressure.emplace(v);
	}

	shortest_path = floyd_warshall(valves); 

	//	Pretty much a recursive brute-force
	std::cout << "Without Elephant: " << find_path(30) << std::endl;
	std::cout << "With Elephant: " << find_path_with_elephant(26) << std::endl;

	return (EXIT_SUCCESS);
}
