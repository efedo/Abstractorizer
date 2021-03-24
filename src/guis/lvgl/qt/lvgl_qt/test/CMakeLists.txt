cmake_minimum_required(VERSION 3.10)

project(DesignatedInitializer VERSION 1.0 LANGUAGES CXX C)

add_library(test_lib
	cinclude.c
	test.c
	test.cpp
)