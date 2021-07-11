#include <catch2/catch.hpp>
#include <web_ui/detail/base64.h>

TEST_CASE("base64")
{
	REQUIRE(web_ui::detail::to_base_64<>("M")   == "TQ==");
	REQUIRE(web_ui::detail::to_base_64<>("Ma")  == "TWE=");
	REQUIRE(web_ui::detail::to_base_64<>("Man") == "TWFu");

	REQUIRE(
		web_ui::detail::to_base_64<>("any carnal pleasure.")
		== "YW55IGNhcm5hbCBwbGVhc3VyZS4=");
	REQUIRE(
		web_ui::detail::to_base_64<>("any carnal pleasure")
		== "YW55IGNhcm5hbCBwbGVhc3VyZQ==");
	REQUIRE(
		web_ui::detail::to_base_64<>("any carnal pleasur")
		== "YW55IGNhcm5hbCBwbGVhc3Vy");
	REQUIRE(
		web_ui::detail::to_base_64<>("any carnal pleasu")
		== "YW55IGNhcm5hbCBwbGVhc3U=");
	REQUIRE(
		web_ui::detail::to_base_64<>("any carnal pleas")
		== "YW55IGNhcm5hbCBwbGVhcw==");

	REQUIRE(web_ui::detail::to_base_64<>("pleasure.") == "cGxlYXN1cmUu");
	REQUIRE(web_ui::detail::to_base_64<>( "leasure.") == "bGVhc3VyZS4=");
	REQUIRE(web_ui::detail::to_base_64<>(  "easure.") == "ZWFzdXJlLg==");
	REQUIRE(web_ui::detail::to_base_64<>(   "asure.") == "YXN1cmUu");
	REQUIRE(web_ui::detail::to_base_64<>(    "sure.") == "c3VyZS4=");

	REQUIRE(web_ui::detail::to_base_64<>("") == "");
	REQUIRE(web_ui::detail::to_base_64<>("f") == "Zg==");
	REQUIRE(web_ui::detail::to_base_64<>("fo") == "Zm8=");
	REQUIRE(web_ui::detail::to_base_64<>("foo") == "Zm9v");
	REQUIRE(web_ui::detail::to_base_64<>("foob") == "Zm9vYg==");
	REQUIRE(web_ui::detail::to_base_64<>("fooba") == "Zm9vYmE=");
	REQUIRE(web_ui::detail::to_base_64<>("foobar") == "Zm9vYmFy");


	REQUIRE(
		web_ui::detail::to_base_64<>(
			"123444444444444444444444444444444444444444444444444444444444444444"
			"444444444444444444444444444444444444444444444444444444444444444444"
			"44444444444444444444444444444444444444444444444444444444444ierioqw"
			"jerölkaslmfd-mskfdösadlfks.aflewofoisalcvlyxjvosaoidjflm.,e..ölsaf"
			"däsüpdfoü+qqüoweörskdfölsadkfölsakdfpoasf")
		==
		"MTIzNDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0ND"
		"Q0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0"
		"NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0ND"
		"Q0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDQ0NDRpZXJpb3F3amVyw7Zsa2FzbG1m"
		"ZC1tc2tmZMO2c2FkbGZrcy5hZmxld29mb2lzYWxjdmx5eGp2b3Nhb2lkamZsbS4sZS4uw7"
		"Zsc2FmZMOkc8O8cGRmb8O8K3Fxw7xvd2XDtnJza2Rmw7Zsc2Fka2bDtmxzYWtkZnBvYXNm");

}


