#include <web_ui/compressor.h>
#include <iostream>
#include <web_ui/detail/split.h>

std::ostream& operator<<(std::ostream & out, std::vector<std::string> const& data)
{
	out << "[";
	for(auto const& val : data)
	{
		out << " " << val;
	}
	return out << " ]";
}


int main(int arg_count, char * values[])
{
	const char program_name[] = "html2header";
	std::string src_file;
	std::string base_path;
	std::string dst_file;
	std::vector<std::string> namespaces;
	std::string array_name;
	bool parmaeter_error = false;
	bool output_html = false;

	for(int index = 1; index < arg_count; ++index)
	{
		const std::string parameter(values[index]);

		if(parameter.find("i=") == 0)
		{
			src_file = parameter.substr(2, parameter.size()-2);

			size_t pos = src_file.rfind('/');
			if(pos == std::string::npos)
			{
				base_path = "./";
			}
			else
			{
				base_path = src_file.substr(0, src_file.rfind('/')+1);
			}
		}
		else if(parameter.find("o=") == 0)
		{
			dst_file = parameter.substr(2, parameter.size()-2);
		}
		else if(parameter.find("n=") == 0)
		{
			namespaces =
				web_ui::detail::split(parameter.substr(2, parameter.size()-2));
		}
		else if(parameter.find("a=") == 0)
		{
			array_name = parameter.substr(2, parameter.size()-2);
		}
		else if(parameter.find("output_html") == 0)
		{
			output_html = true;
		}
		else
		{
			std::cout
				<< program_name
				<< ": Unknown parameter "
				<< parameter
				<< std::endl;

			parmaeter_error = true;
		}
	}

	if(src_file == "")
	{
		std::cout
			<< program_name
			<< ": parmaeter for input file (i=...) is required"
			<< std::endl;
		parmaeter_error = true;
	}

	if(dst_file == "")
	{
		std::cout
			<< program_name
			<< ": parmaeter for output file (o=...) is required"
			<< std::endl;
		parmaeter_error = true;
	}

	if(array_name == "" and not output_html)
	{
		std::cout
			<< program_name
			<< ": parmaeter for data array name (a=...) is required"
			<< std::endl;
		parmaeter_error = true;
	}

	if(not parmaeter_error)
	{
		web_ui::Compressor::write_as_header(
			src_file,
			base_path,
			dst_file,
			namespaces,
			array_name,
			output_html);
	}

	return 0;
}
