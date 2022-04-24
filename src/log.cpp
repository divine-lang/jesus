#pragma once
#include <iostream>
#include <string>

namespace jesus_log {
	class base {
		public:
		std::string name;
		std::string message;
		int bgcolor;
		int fgcolor;
		friend std::ostream& operator<<(std::ostream& os, const jesus_log::base& self){
			os << (self.bgcolor > -1 ? ("\x1b[4" + std::to_string(self.bgcolor) + "m") : "")
			   << (self.fgcolor > -1 ? ("\x1b[3" + std::to_string(self.fgcolor) + "m") : "")
			   << "\x1b[1m"
			   << self.name << "\x1b[m   "
			   << self.message;
			return os;
		}
	};
	class log : public jesus_log::base {
		public:
		log(std::string msg){
			this->fgcolor = 7;
			this->bgcolor = -1;
			this->message = msg;
			this->name = " LOG  ";
		}
	};
	class error : public jesus_log::base {
		public:
		error(std::string msg){
			this->fgcolor = 7;
			this->bgcolor = 1;
			this->message = msg;
			this->name = " ERR! ";
		}
	};
	class note : public jesus_log::base {
		public:
		note(std::string msg){
			this->fgcolor = 8;
			this->bgcolor = -1;
			this->message = msg;
			this->name = " NOTE ";
		}
	};
}
