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

void Script::parse(std::map<std::string, std::function<bool(void*)>>* funcMapPtr)
{
	ProgressBar* progressBar = new ProgressBar(files.size(), "Parsing...");

	for (std::string fileName : Script::files)
	{
		std::ifstream file(fileName);
		std::string data;
		while (std::getline(file, data))
		{
			if (data.starts_with("//") || data.size() == 0) continue;
			data.erase(std::remove(data.begin(), data.end(), ' '));
			std::vector<std::string> commandslist = split(data, "=");
			std::pair<std::string, std::function<bool(void*)>> function = {
				commandslist[0],
				[=](void* data) {
					ShellExecute(NULL, NULL, commandslist[0].c_str(), NULL, NULL, SW_SHOWNORMAL);
					return true; 
				}
			};
			funcMapPtr->insert(function);

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