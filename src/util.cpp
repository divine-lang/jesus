#pragma once
#include <vector>
#include <string>
#include <algorithm>

namespace jesus_util {
	template <typename T>
	size_t find(std::vector<T> vec, T v){
		auto itr = std::find(vec.begin(), vec.end(), v);
		return std::distance(vec.begin(), itr);
	}
	size_t find_str(std::vector<std::string> vec, std::string str){
		return jesus_util::find<std::string>(vec, str);
	}
	template <typename T>
	bool has(std::vector<T> vec, T v){
		return jesus_util::find<T>(vec, v) != vec.size();
	}
	size_t has_str(std::vector<std::string> vec, std::string str){
		return jesus_util::has<std::string>(vec, str);
	}
}
