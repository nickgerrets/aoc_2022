#include <iostream>
#include <fstream>
#include <unordered_map>

class Directory
{
	public:
	Directory(Directory* prev_dir)
	: prev_dir(prev_dir) {}

	Directory*									prev_dir;
	std::unordered_map<std::string, size_t>		files;
	std::unordered_map<std::string, Directory>	sub_dirs;

	size_t get_total_size(void) const
	{
		size_t total = 0;
		for (auto& pair : files)
			total += pair.second;
		for (auto& pair : sub_dirs)
			total += pair.second.get_total_size();
		return (total);
	}
};

void	parse_directory(Directory& dir, std::istream& stream)
{
	for (std::string line; stream.peek() != '$' && std::getline(stream, line); )
	{
		if (line.find("dir ") == 0)
		{
			dir.sub_dirs.insert(
			{
				line.substr(line.find(" ") + 1), 
				Directory(&dir)
			});
		}
		else
		{
			dir.files.insert(
			{
				line.substr(line.find_first_not_of("0123456789")), 
				std::stoul(line.substr(0, line.find_first_not_of("0123456789")))
			});
		}
	}
}

Directory* change_directory(Directory* current_dir, std::string const& line)
{
	std::string str = line.substr(5);

	if (str == "..")
	{
		if (current_dir->prev_dir)
			return (current_dir->prev_dir);
		return (current_dir);
	}
	if (str.length())
	{
		auto it = current_dir->sub_dirs.find(str);
		if (it != current_dir->sub_dirs.end())
			return &(it->second);
	}	
	return (current_dir);
}

Directory parse(std::ifstream& stream)
{
	Directory	root(nullptr);
	Directory*	current_dir = &root;

	for (std::string line; stream.peek() == '$' && std::getline(stream, line); )
	{
		if (line.find(" cd ") != std::string::npos)
			current_dir = change_directory(current_dir, line);
		else if (line.find(" ls") != std::string::npos)
			parse_directory(*current_dir, stream);
	}
	return (root);
}

size_t get_big_dirs_total_size(Directory& root, size_t max_size)
{
	size_t total = 0;
	for (auto& d : root.sub_dirs)
	{
		size_t size = d.second.get_total_size();
		if (size <= max_size)
			total += size;
		total += get_big_dirs_total_size(d.second, max_size);
	}
	return (total);
}

size_t get_deleted_dir_size(Directory& root, size_t space_needed)
{
	size_t del_dir_size = UINT64_MAX;
	for (auto& d : root.sub_dirs)
	{
		size_t size = d.second.get_total_size();
		if (size >= space_needed &&  size < del_dir_size)
			del_dir_size = size;
		size = get_deleted_dir_size(d.second, space_needed);
		if (size >= space_needed &&  size < del_dir_size)
			del_dir_size = size;
	}
	return (del_dir_size);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (EXIT_FAILURE);
	
	std::ifstream file(argv[1]);
	if (!file)
		return (EXIT_FAILURE);

	Directory root = parse(file);

	constexpr const size_t max_size = 100000;
	constexpr const size_t space_required = 30000000;
	constexpr const size_t space_available = 70000000;

	std::cout << "Total root size: " << root.get_total_size() << std::endl;
	std::cout << "Big directories total size: " << get_big_dirs_total_size(root, max_size) << std::endl;
	std::cout << "Size of deleted dir: " << get_deleted_dir_size(root, space_required - (space_available - root.get_total_size())) << std::endl;

	return (EXIT_SUCCESS);
}
