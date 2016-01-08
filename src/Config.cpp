#include "header/Config.h"
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>


void Config::parseConfigFileImpl(const char* filepath){
	std::ifstream file(filepath);

	if(file.is_open()){
		for(std::string line; std::getline(file,line);){
			std::string buf;
			std::vector<std::string> tokens;

			if(line[0] == '#' || line ==  "") continue; // Skip comments
			std::stringstream iss(line); // Split line.
			while(iss >> buf)
				tokens.push_back(buf);

			if(tokens.at(1) != "=") throw std::runtime_error("Config parse error!");
			options[tokens.at(0)] = tokens.at(2);
		}
	}else{
		throw std::runtime_error("Config file failed to open!");
	}
}

std::string Config::lookupStringImpl(std::string s){
	std::map<std::string, std::string>::iterator it = options.find(s);

	if(it == options.end()){
		std::cerr << "Config lookup failed!" << std::endl;
		return "";
	}
	return it->second;
}
