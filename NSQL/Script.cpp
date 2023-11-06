#include "Script.h"

Script::Script(std::string path)
{
	for (auto& p : std::filesystem::recursive_directory_iterator(path))
	{
		if (p.path().extension() == ext)
		{
			Script::files.push_back(p.path().string());
			printf("found: %s\n", p.path().string().c_str());
		}
	}
}

void Script::parse()
{
	ProgressBar* progressBar = new ProgressBar(files.size());
	for (std::string fileName : Script::files)
	{
		std::ifstream file(fileName);
		std::string data;
		while (std::getline(file, data))
		{
			if (data.starts_with("//") || data.size() == 0) continue;
			std::string sep = "=";
			data.erase(std::remove(data.begin(), data.end(), ' '));
			std::vector<std::string> commandslist = split(data, sep);
		}
		file.close();
		progressBar->update();
	}
	newline();
}

std::vector<std::string> Script::getFiles()
{
	return Script::files;
}

int Script::getFilesSize()
{
	return Script::files.size();
}