#include <iostream>
#include <vector>
#include <string>

#include <jarfetch.cpp>
#include <log.cpp>
#include <util.cpp>

void usage(){
	std::cout << "Jesus - The package manager for the Divine Programming Language" << std::endl
		  << "Usage: jesus <command> [options]" << std::endl
		  << std::endl
		  << "Commands:" << std::endl
		  << "  help:          Shows this help" << std::endl
		  << std::endl;
}
int main(int argc, char** argv){
	std::vector<std::string> args(argv, argc + argv);
	if(args.size() == 1){
		std::cerr << jesus_log::error("Pass the command") << std::endl;
		usage();
		std::exit(1);
	}else if(args[1] == "help"){
		usage();
		std::exit(0);
	}else{
		std::cerr << jesus_log::error("Unknown command: " + args[1]) << std::endl;
		std::exit(1);
	}
}
