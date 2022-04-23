#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv){
	std::vector<std::string> args(argv, argc + argv);
	std::cout << args[0] << std::endl;
}
