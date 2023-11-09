#pragma once
#undef UNICODE
#include <string>
#include <vector>
#include <functional>
#include <filesystem>
#include <fstream>
#include <Windows.h>
#include <shellapi.h>
#include <ShlObj.h>
#include <map>
#include "util.h"

constexpr char ext[] = ".nsql";

class Script
{
public:
	Script(std::string path);
	void parse(std::map<std::string, std::function<bool(void*)>>* funcMapPtr);
	std::vector<std::string> getFiles();
	int getFilesSize();
private:
	std::vector<std::string> files;
};

