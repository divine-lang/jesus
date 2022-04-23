#pragma once
#include <curl/curl.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

namespace jesus_jarfetch {
	typedef struct {
	    char *m;
	    size_t size;
	} Memory;
	size_t callback(char *ptr, size_t size, size_t nmemb, void *userdata){
		char *p;
		Memory *mem = (Memory *)userdata;
		size_t append_size = size * nmemb;
		p = (char *)realloc(mem->m, mem->size + append_size + 1);
		if (p == NULL) {
			return 0;
		}
		memcpy(p + mem->size, ptr, append_size);
		mem->m = p;
		mem->size += append_size;
		mem->m[mem->size] = '\0';
		return size * nmemb;
	}
	void fetch(){
		std::cout << "Fetching the jar info... " << std::flush;
		CURL* curl;
		CURLcode res;
		Memory body;
		curl_global_init(CURL_GLOBAL_DEFAULT);
		curl = curl_easy_init();
		if(curl){
			body.m = (char *)malloc(1);
			body.m[0] = '\0';
		        body.size = 0;
			curl_easy_setopt(curl, CURLOPT_URL, "https://raw.githubusercontent.com/divine-lang/meta/master/info.ini");
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
			res = curl_easy_perform(curl);
			if(res != CURLE_OK){
				std::cerr << "\x1b[1m\x1b[31mERROR\x1b[m: " << curl_easy_strerror(res) << std::endl;
			}
			std::string data(body.m);
			free(body.m);
			std::cout << "Done." << std::endl;
			std::string section = "";
			std::map<std::string, std::map<std::string, std::string>> values;
			std::string buf;
			std::stringstream ss{data};
			while(getline(ss, buf)){
				if(buf[0] == '[' && buf[buf.size() - 1] == ']'){
					section = buf.substr(1, buf.size() - 2);
				}else if(buf.find_first_of("=") != std::string::npos){
					std::string name = buf.substr(0, buf.find_first_of("="));
					name = name.substr(0, name.find_first_not_of(" "));
					if(values.count(section) == 0) values[section] = std::map<std::string, std::string>();
					values[section][name] = buf.substr(buf.find_first_of("=") + 1);
				}
			}
			goto begin;
			error:
			std::cerr << "Please type the number between 1 and 3." << std::endl;
			begin:
			std::cout << "1) Nightly: " << values["version"]["nightly"] << std::endl
				  << "2)  Stable: " << values["version"]["stable"] << std::endl
				  << "3)  Latest: " << values["version"]["latest"] << std::endl;
			std::cout << "Choose the version [1-3]: " << std::flush;
			std::string choice;
			std::cin >> choice;
			int n;
			try{
				n = stoi(choice);
			}catch(...){
				goto error;
			}
			if(!(1 <= n && n <= 3)){
				goto error;
			}
		}
		curl_global_cleanup();
	}
}
