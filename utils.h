#include <string>
#include <vector>

std::string readInput(std::string &line);
void execCd(std::vector<char *> args);
std::vector<char *> formatInput(std::string &s, std::vector<std::string> &w);
void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &line);
