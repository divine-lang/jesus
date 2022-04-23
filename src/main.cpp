#include <iostream>
#include <vector>
#include <string>

#include <jarfetch.cpp>

int main(int argc, char** argv){
	std::vector<std::string> args(argv, argc + argv);
	jesus_jarfetch::fetch();
}
