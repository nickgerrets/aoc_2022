#include <iostream>
#include <fstream>
#include <queue>

//	queue derived class to make queue iterable
template<typename T, typename Container = std::deque<T>>
class iterable_queue : public std::queue<T, Container>
{
	public:
	typedef typename Container::iterator iterator;
	typedef typename Container::const_iterator const_iterator;

	iterator		begin() { return this->c.begin(); }
	iterator		end() { return this->c.end(); }
	const_iterator	begin() const { return this->c.begin(); }
	const_iterator	end() const { return this->c.end(); }

	const T& operator[](unsigned int index) const
	{
		// if (index >= this->size()) return (exception);
		return *(begin() + index);
	}
};

//	Checks if all elements in a given (iterable) container type are unique
template<typename T>
bool	check_unique(const T& container)
{
	for (size_t i = 0; i < container.size(); ++i)
	{
		typename T::value_type value = container[i];
		for (size_t j = i + 1; j < container.size(); ++j)
		{
			if (value == container[j])
				return (false);
		}
	}
	return (true);
}

int	find_marker(std::ifstream& stream, size_t marker_size)
{
	iterable_queue<char> queue;

	size_t i = 0;
	int c;
	while ((c = (char)stream.get()) != EOF)
	{
		i++;
		if (queue.size() >= marker_size)
			queue.pop();
		queue.push((char)c);
		if (queue.size() == marker_size && check_unique(queue))
			return (i);
	}
	return (i);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (EXIT_FAILURE);
	
	std::ifstream	file;
	file.open(argv[1]);

	if (!file)
		return (EXIT_FAILURE);

	std::cout << "Marker: " << find_marker(file, 14) << std::endl;

	return (EXIT_SUCCESS);
}
