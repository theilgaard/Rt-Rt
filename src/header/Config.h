#pragma once
#include <map>

class Config{
public:
	static Config& getInstance(){
		static Config instance;
		return instance;
	}
	static void parseConfigFile(const char* filepath){
		getInstance().parseConfigFileImpl(filepath);
	}
	static std::string lookupString(std::string s){
		return getInstance().lookupStringImpl(s);
	}
private: 
	std::map<std::string, std::string> options; 
	Config() {};                   // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    Config(Config const&);              // Don't Implement
    void operator=(Config const&); // Don't implement

	void parseConfigFileImpl(const char* filepath);
	std::string lookupStringImpl(std::string s);
};