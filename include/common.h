#ifndef COMMON_H
# define COMMON_H

# include <istream>

# define SIGN(x) (x > 0) - (x < 0)

namespace aoc
{

//	ignore stream until next digit
std::istream& getn(std::istream& in)
{
	char c;
	while (in && (c = (in >> std::ws).peek()) != EOF
		&& !(c >= '0' && c <= '9'))
		in.ignore();
	return (in);
}

//	returns iterator to the first element thats not unique
template<typename ItInput>
ItInput	first_not_unique(ItInput begin, ItInput end)
{
	for (ItInput it1 = begin; it1 != end; ++it1)
	{
		for (ItInput it2 = it1 + 1; it2 != end; ++it2)
		{
			if (*it1 == *it2)
				return (it2);
		}
	}
	return (end);
}

} // namespace aoc

#endif	//	COMMON_H
