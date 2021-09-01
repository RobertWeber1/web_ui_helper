#include <catch2/catch.hpp>
#include <web_ui/compressor.h>
#include <iostream>

TEST_CASE("get file content")
{
	REQUIRE(
		web_ui::Compressor::get_file_content(TEST_DATA_PATH "index.html")
		==
R"XML(<!DOCTYPE HTML>
<html>
	<head>
		<meta charset="UTF-8"/>
		<title>Foo</title>
		<link rel="stylesheet" type="text/css" href="style.css"/>
		<link rel="shortcut icon" href="fav.ico"/>
		<script src="javascript.js"></script>
	</head>
	<body onload="display_msg()">
		<p> <img src="drawing.svg" alt="test image 1"/> </p>
		<p> <img src="image.png" alt="test image 2"/> </p>
	</body>
</html>
)XML");

}

TEST_CASE("get file content base64 encoded")
{
	REQUIRE(
		web_ui::Compressor::get_file_content_base64(TEST_DATA_PATH "fav.ico")
		==
		"AAABAAEAEBAQAAEABAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAADqDg4ADg7qAA7qfAD///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAMzMzMzMzMzMxEREREREREzERERERERETMRERERERER"
		"MxAAAAABEREzEAAAAAERETMwAAAAAzMzMzAAAAADMzMzMAAAAAMzMzMiIiIiIiIzMyIiIi"
		"IiIjMzIiIiIiIiMzMiIiIiIiIzMyIiIiIiIjMzIiIiIiIiMzMzMzMzMzMzMAAAAAAAAAAA"
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
		"AAAA");

}

TEST_CASE("remove white space chars from CSS")
{
	REQUIRE(web_ui::Compressor::remove_ws("{}") == "{}");
	REQUIRE(web_ui::Compressor::remove_ws(" { } ") == " { } ");
	REQUIRE(web_ui::Compressor::remove_ws("{  }") == "{ }");
	REQUIRE(web_ui::Compressor::remove_ws("{  \n\r\t  }") == "{ }");
}


TEST_CASE("replace stylesheet-link-tag with style-tag")
{
	REQUIRE(
		web_ui::Compressor::embed_externals(
			R"XML(<html><link rel="stylesheet" type="text/css" href="style.css"/></html>)XML",
			TEST_DATA_PATH,
			false)
		== "<html><style> body { background-color: lightblue; } </style></html>");
}

TEST_CASE("replace faviocn ref with file base64 encoded content")
{
	REQUIRE(
		web_ui::Compressor::embed_externals(
			R"XML(<html><link rel="shortcut icon" href="fav.ico"/></html>)XML",
			TEST_DATA_PATH,
			false)
		== R"XML(<html><link rel="shortcut icon" href="data:image/ico;base64,)XML"
		   R"XML(AAABAAEAEBAQAAEABAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAAAAAAAAAAAAAAA)XML"
		   R"XML(AAAAAAAAAAAADqDg4ADg7qAA7qfAD///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA)XML"
		   R"XML(AAAAAAAAAAAAAAAAAAAAAAAAAAAAMzMzMzMzMzMxEREREREREzERERERERETMRERERERER)XML"
		   R"XML(MxAAAAABEREzEAAAAAERETMwAAAAAzMzMzAAAAADMzMzMAAAAAMzMzMiIiIiIiIzMyIiIi)XML"
		   R"XML(IiIjMzIiIiIiIiMzMiIiIiIiIzMyIiIiIiIjMzIiIiIiIiMzMzMzMzMzMzMAAAAAAAAAAA)XML"
		   R"XML(AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA)XML"
		   R"XML(AAAA"/></html>)XML");
}


TEST_CASE("embed javascript")
{
	REQUIRE(
		web_ui::Compressor::embed_externals(
			R"XML(<html><script src="javascript.js"></script></html>)XML",
			TEST_DATA_PATH,
			false)
		== R"XML(<html><script> function display_msg() { console.log("Hello World!"); } </script></html>)XML");
}

TEST_CASE("embed svg")
{
	REQUIRE(
		web_ui::Compressor::embed_externals(
			R"XML(<html><img src="drawing.svg" alt="test image 1"/></html>)XML",
			TEST_DATA_PATH,
			false)
		== R"XML(<html><svg xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:cc="http://creativecommons.org/ns#" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:svg="http://www.w3.org/2000/svg" xmlns="http://www.w3.org/2000/svg" xmlns:sodipodi="http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd" xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape" width="188.97638" height="196.50986" viewBox="0 0 49.999999 51.993235" version="1.1" id="svg8" inkscape:version="0.92.5 (2060ec1f9f, 2020-04-08)" sodipodi:docname="drawing.svg"><defs id="defs2"/><sodipodi:namedview id="base" pagecolor="#ffffff" bordercolor="#666666" borderopacity="1.0" inkscape:pageopacity="0.0" inkscape:pageshadow="2" inkscape:zoom="1.979899" inkscape:cx="261.85006" inkscape:cy="86.265814" inkscape:document-units="mm" inkscape:current-layer="layer1" showgrid="false" units="px" inkscape:window-width="1916" inkscape:window-height="1056" inkscape:window-x="0" inkscape:window-y="20" inkscape:window-maximized="1" fit-margin-top="0" fit-margin-left="0" fit-margin-right="0" fit-margin-bottom="0"/><metadata id="metadata5"><rdf:RDF><cc:Work rdf:about=""><dc:format>image/svg+xml</dc:format><dc:type rdf:resource="http://purl.org/dc/dcmitype/StillImage"/><dc:title/></cc:Work></rdf:RDF></metadata><g inkscape:label="Layer 1" inkscape:groupmode="layer" id="layer1" transform="translate(-0.30710585,-244.71155)"><g id="g59" transform="matrix(14.063359,0,0,14.063359,-4.0118313,-3875.9595)"><rect y="293.61002" x="1.0984933" height="2.0079985" width="1.7717634" id="rect12" style="fill:#00ff00;stroke-width:0.26458332"/><rect y="293.0076" x="2.0788691" height="1.8544457" width="1.7835751" id="rect14" style="fill:#800000;stroke-width:0.26458332"/><rect y="295.3345" x="0.30710566" height="1.3701665" width="2.775763" id="rect16" style="fill:#008080;stroke-width:0.26458332"/></g></g></svg></html>)XML");
}
