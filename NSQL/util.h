#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <math.h>
#include <future>

#define newline() std::cout << std::endl;

std::vector<std::string> split(std::string string, std::string separator);
std::string removeWhitespacesWithinWrap(std::string& data);

int getConsoleWidth();
void showCursor(bool flag);
    
class ProgressBar{
public:
    ProgressBar(int max);
    ProgressBar(int max, std::string text);
    void update();
private:
    void draw();
    std::string text;
    float progress;
    int current, max, pos, iter, width = getConsoleWidth() / 1.61;
};