#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "util.h"

constexpr char ext[] = ".nsql";

class Script
{
public:
	Script(std::string path);
	void parse();
	std::vector<std::string> getFiles();
	int getFilesSize();
private:
	std::vector<std::string> files;
};

