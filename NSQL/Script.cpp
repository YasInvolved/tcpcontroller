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

void Script::parse(std::map<std::string, std::function<bool(void*)>>* funcMapPtr, std::map<std::string, std::string>* configPtr)
{
	ProgressBar* progressBar = new ProgressBar(files.size(), "Parsing...");

	for (std::string fileName : Script::files)
	{
		std::ifstream file(fileName);
		std::string data;
		while (std::getline(file, data))
		{
			if (data.starts_with("//") || data.size() == 0) continue;
			if (data.starts_with("config.")) {
				std::string configStr = split(data, "config.")[1];
				std::vector<std::string> config = split(removeWhitespacesWithinWrap(configStr), "=");
				std::string value = config[1];
				value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
				configPtr->insert(std::pair<std::string, std::string> {config[0], value});
				continue;
			}
			data = removeWhitespacesWithinWrap(data);
			std::vector<std::string> commandslist = split(data, "=");
			commandslist[1] = replaceSysVar(commandslist[1]);
			commandslist[1] += (commandslist.size() >= 3 ? "=" + commandslist[2] : "");
			std::pair<std::string, std::function<bool(void*)>> function = {
				commandslist[0],
				[=](void* data) {
					HINSTANCE app = ShellExecute(NULL, NULL, commandslist[1].c_str(), NULL, NULL, SW_SHOWNORMAL);
					CloseHandle(app);
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

std::string Script::replaceSysVar(std::string& data)
{
	std::string result = data;
	size_t startPos = 0;
	while ((startPos = data.find('%', startPos)) != std::string::npos)
	{
		size_t endPos = result.find('%', startPos + 1);
		if (endPos != std::string::npos) {
			std::string variable = data.substr(startPos + 1, endPos - startPos - 1);
			const char* value = std::getenv(variable.c_str());
			if (value != nullptr)
			{
				result.replace(startPos, endPos - startPos + 1, value);
			}
			startPos = endPos + 1;
		}
		else {
			break;
		}
	}
	return result;
}

std::vector<std::string> Script::getFiles()
{
	return Script::files;
}

int Script::getFilesArrSize()
{
	return Script::files.size();
}