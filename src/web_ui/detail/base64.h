#pragma once
#include <string>
#include <algorithm>


namespace web_ui
{

namespace detail
{

constexpr char default_alphabet[64] = {
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

template<char const(&Alphabet)[64] = default_alphabet>
std::string to_base_64(std::string const& input)
{
	std::string result;
	unsigned int pos = 0;

	while (pos < input.size())
	{
		result.push_back(Alphabet[(input[pos + 0] & 0xfc) >> 2]);

		if (pos + 1 < input.size())
		{
			result.push_back(Alphabet[((input[pos + 0] & 0x03) << 4) + ((input[pos + 1] & 0xf0) >> 4)]);


			if (pos + 2 < input.size())
			{
				result.push_back(Alphabet[((input[pos + 1] & 0x0f) << 2) + ((input[pos + 2] & 0xc0) >> 6)]);
				result.push_back(Alphabet[  input[pos + 2] & 0x3f]);
			}
			else
			{
				result.push_back(Alphabet[(input[pos + 1] & 0x0f) << 2]);
				result.push_back('=');
			}
		}
		else
		{
			result.push_back(Alphabet[(input[pos + 0] & 0x03) << 4]);
			result.push_back('=');
			result.push_back('=');
		}

		pos += 3;
	}

	return result;
}

} //namespace detail

} //namespace web_ui
