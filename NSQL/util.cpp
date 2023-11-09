#include "util.h"

std::vector<std::string> split(std::string str, std::string separator)
{
    std::vector<std::string> tokens;
    size_t pos;
    while ((pos = str.find(separator)) != std::string::npos)
    {
        tokens.push_back(str.substr(0, pos));
        str.erase(0, pos + separator.length());
    }
    tokens.push_back(str);
    if (tokens.size() == 1) tokens.push_back("\0");
    return tokens;
}

int getConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
}

void showCursor(bool flag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = flag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

ProgressBar::ProgressBar(int max)
{
    ProgressBar::max = max;
    ProgressBar::text = "\0";
    current = 0;
    progress = 0;
    draw();
}

ProgressBar::ProgressBar(int max, std::string text)
{
    ProgressBar::max = max;
    ProgressBar::text = text;
    current = 0;
    progress = 0;
    draw();
}

void ProgressBar::draw()
{
    if (progress < 1.0)
    {
        progress = current / max;
        if (text.empty() != true) std::cout << text;
        std::cout << " [";
        pos = width * progress;
        for (int i = 0; i < width; i++) {
            if (i < pos) std::cout << "#";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(progress * 100.0) << " %\r";
        std::cout.flush();
    }
}

void ProgressBar::update()
{
    current += 1;
    draw();
}