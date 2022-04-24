#pragma once
#include <curl/curl.h>
#include <log.cpp>
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
		std::cout << jesus_log::log("Fetching the jar info") << std::endl;
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
				std::cerr << std::endl
				          << jesus_log::error("Failed to establish the connectiion!")
				          << std::endl
				          << jesus_log::error("Reason: " + std::string(curl_easy_strerror(res))) << std::endl;
				goto exiting;
			}
			curl_off_t cl;
			long status_code;
			goto skipsize;
			filesize:
			res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &cl);
			if(res == CURLE_OK){
				std::cout << jesus_log::log("Size: " + std::to_string(cl) + " bytes") << std::endl;
			}else{
				std::cout << jesus_log::note("Failed to fetch the size") << std::endl;
			}
			goto comeback_size;
			skipsize:
			res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);
			if(res == CURLE_OK){
				if(status_code == 200){
					std::cout << jesus_log::log("Status code: 200") << std::endl;
				}else{
					std::cout << jesus_log::error("Status code: " + std::to_string(status_code)) << std::endl;
					goto exiting;
				}
			}else{
				std::cout << jesus_log::note("Failed to fetch the status code") << std::endl;

			}
			goto filesize;
			comeback_size:
			std::string data(body.m);
			free(body.m);
			curl_off_t total;
			res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &total);
			std::cout << jesus_log::log("Fetched the jar info successfully") << std::endl
		                  << jesus_log::log("Took " + std::to_string(total / 1000000) + "." + std::to_string(total % 1000000) + " s to fetch the jar info") << std::endl;
			std::cout << jesus_log::log("Parsing the jar info") << std::endl;
			std::string section = "";
			std::map<std::string, std::map<std::string, std::string>> values;
			std::string buf;
			std::stringstream ss{data};
			while(getline(ss, buf)){
				if(buf[0] == '[' && buf[buf.size() - 1] == ']'){
					section = buf.substr(1, buf.size() - 2);
					std::cout << jesus_log::log("Section: " + section) << std::endl;
				}else if(buf.find_first_of("=") != std::string::npos){
					std::string name = buf.substr(0, buf.find_first_of("="));
					if(name.find_first_of(" ") != std::string::npos){
						name = name.substr(0, name.find_first_of(" "));
					}
					if(values.count(section) == 0) values[section] = std::map<std::string, std::string>();
					values[section][name] = buf.substr(buf.find_first_of("=") + 1);
					std::cout << jesus_log::log("\t" + name + " = " + values[section][name]) << std::endl;
				}
			}
			std::cout << jesus_log::log("Parsed the jar info successfully") << std::endl;
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
		goto skip;
		exiting:
		std::cerr << jesus_log::error("Exiting due to the error") << std::endl;
		curl_global_cleanup();
		std::exit(1);
		skip:
		curl_global_cleanup();
	}
}
