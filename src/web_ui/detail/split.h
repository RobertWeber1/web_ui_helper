#pragma once
#include <string>
#include <vector>

namespace web_ui
{

namespace detail
{

std::vector<std::string> split(std::string input, char seperator = ',')
{
	std::vector<std::string> result;

	size_t start = 0;

	while(true)
	{
		const size_t end = input.find(seperator, start);

		result.push_back(
			std::string(&input[start], std::min(end-start, input.size()-start)));

		if(end == std::string::npos)
		{
			break;
		}

		start = end + 1;
	}

	return result;
}


} //namespace detail

} //namespace web_ui
