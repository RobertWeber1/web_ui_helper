cmake_minimum_required(VERSION 3.0.0)

project(WebUI LANGUAGES CXX)

#-------------------------------------------------------------------------------

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

#-------------------------------------------------------------------------------

add_library(rapidXML INTERFACE)

target_include_directories(rapidXML INTERFACE libs/rapidXML)

#-------------------------------------------------------------------------------

add_executable(test-WebUI
	test/catch.cc
	test/parse_html.cc
)

target_link_libraries(test-WebUI
	PRIVATE Catch2::Catch2
	PRIVATE rapidXML
)
