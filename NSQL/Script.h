#pragma once
#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include <fstream>
#include <map>
#include "util.h"
#include <shellapi.h>

constexpr char ext[] = ".nsql";

class Script
{
public:
	Script(std::string path);
	void parse(std::map<std::string, std::function<bool(void*)>>* funcMapPtr, std::map<std::string, std::string>* configPtr);
	std::vector<std::string> getFiles();
	int getFilesArrSize();
private:
	std::string replaceSysVar(std::string& data);
	std::vector<std::string> files;
};

