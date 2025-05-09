cmake_minimum_required(VERSION 3.16)
project(mkpsylink LANGUAGES CXX)

add_executable(mkpsylink
	"src/main.cpp")

install(TARGETS mkpsylink)