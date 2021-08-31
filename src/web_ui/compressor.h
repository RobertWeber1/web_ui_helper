#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <web_ui/detail/base64.h>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <iostream>

namespace web_ui
{

struct Compressor
{
	static std::string get_file_content(std::string const& name)
	{
		std::ifstream file(name.c_str());
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	static std::string get_file_content_base64(std::string const& name)
	{
		return detail::to_base_64(get_file_content(name));
	}

	static std::string remove_ws(std::string content)
	{
		bool first_space = false;

		std::replace_if(
			content.begin(),
			content.end(),
			[](char val)
			{
				return val == '\n' or val == '\r' or val == '\t';
			},
			' ');

		content.erase(
			std::remove_if(
				content.begin(),
				content.end(),
				[&first_space](char val)
				{
					if(val == ' ' and first_space == false)
					{
						first_space = true;
						return false;
					}

					if(val == ' ')
					{
						return true;
					}

					first_space = false;
					return false;
				}),
			content.end());
		return content;
	}

	static rapidxml::xml_node<> * get_svg(
		char * content,
		rapidxml::xml_document<> & parent)
	{
		rapidxml::xml_document<> svg_doc;

		svg_doc.parse<0>(content);

		return parent.clone_node(svg_doc.first_node());
	}

	static std::string get_file_content_without_ws(std::string const& name)
	{
		return  remove_ws(get_file_content(name));
	}

	template<class Func>
	static rapidxml::xml_node<> * for_each(rapidxml::xml_node<> *root, Func func)
	{
		if(root->first_node() == nullptr)
		{
			return func(root);
		}
		else
		{
			for(rapidxml::xml_node<> *node=root->first_node();
				node;
				node=node->next_sibling())
			{
				rapidxml::xml_node<> * new_node = for_each(node, func);

				if(new_node != node)
				{
					root->insert_node(node, new_node);
					root->remove_node(node);
					node = new_node;
				}
			}
		}

		return root;
	}

	static std::string embed_externals(
		std::string content,
		std::string const& base_dir,
		bool no_indent = true,
		bool prepend_doctype = false)
	{
		rapidxml::xml_document<> doc;

		try
		{
			doc.parse<0>(&content[0]);
		}
		catch(rapidxml::parse_error const& e)
		{
			std::cout << std::string(&content[0], e.where<char>()) << std::endl;

			throw std::runtime_error("foooooooo");
		}

		for_each(
			doc.first_node(),
			[&doc, base_dir](rapidxml::xml_node<> *node) -> rapidxml::xml_node<> *
			{
				if(node->name() == std::string("link"))
				{
					rapidxml::xml_attribute<> *rel = node->first_attribute("rel");

					if(rel)
					{
						if(rel->value() == std::string("stylesheet"))
						{
							std::string path =
								base_dir + node->first_attribute("href")->value();
							char * content =
								doc.allocate_string(
									get_file_content_without_ws(path).c_str());

							return doc.allocate_node(
								rapidxml::node_element,
								"style",
								content);
						}
						else if(rel->value() == std::string("shortcut icon"))
						{
							std::string path =
								base_dir + node->first_attribute("href")->value();

							char * content =
								doc.allocate_string(
									(std::string("data:image/ico;base64,") +
									get_file_content_base64(path)).c_str());

							node->first_attribute("href")->value(content);
						}
					}
				}
				else if(node->name() == std::string("script"))
				{
					std::string path =
						base_dir + node->first_attribute("src")->value();

					node->remove_attribute(node->first_attribute("src"));

					char * content =
						doc.allocate_string(
							get_file_content_without_ws(path).c_str());

					node->value(content);
				}
				else if(node->name() == std::string("img"))
				{
					std::string src = node->first_attribute("src")->value();

					if(src.rfind(".svg") == src.size()-4)
					{
						char * content =
							doc.allocate_string(get_file_content(base_dir + src).c_str());

						return get_svg(content, doc);
					}
					else
					{
						std::string path = base_dir + src;
						std::string extension = src.substr(src.rfind(".")+1, src.size());

						char * content =
							doc.allocate_string(
								(std::string("data:image/"+ extension +";base64,") +
								get_file_content_base64(path)).c_str());

						node->first_attribute("src")->value(content);
					}
				}

				return node;
			});

		std::stringstream output ("<!DOCTYPE HTML>");
		rapidxml::print(
			std::ostream_iterator<char>(output),
			*doc.first_node(),
			(no_indent ? 0 : rapidxml::print_no_indenting)
			| rapidxml::print_no_quote_expansion);
		return output.str();
	}

	static void write_as_header(
		std::string const& src_file,
		std::string const& base_path,
		std::string const& dst_file,
		std::vector<std::string> const& namespaces,
		std::string const& array_name,
		bool output_html = false)
	{
		auto const data =
			embed_externals(get_file_content(src_file), base_path, false, true);

		std::fstream output(dst_file.c_str(), std::ios_base::out);

		if(output_html)
		{
			output << "<!DOCTYPE html>" << data;
			return;
		}

		output << "#pragma once\n\n";

		for(auto const& ns : namespaces)
		{
			output << "namespace " << ns << "\n{\n\n";
		}

		output
			<< "static constexpr char "
			<< array_name
			<< "[] = R\"XML("
			<< data
			<< ")XML\";\n";

		for(int index = namespaces.size()-1; index >= 0; --index)
		{
			printf("index: %d\n", index);
			output << "\n} //namespace " << namespaces[index] << "\n";
		}

		output << "\n";
	}
};

} //namespace web_ui
