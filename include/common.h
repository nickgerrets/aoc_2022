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

} // namespace aoc

#endif	//	COMMON_H
