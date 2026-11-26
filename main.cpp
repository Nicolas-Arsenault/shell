#include "utils.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

int main(int argc, char *argv[]) {
  std::string line = "";
  std::string EXIT_COMMAND = "exit";
  while (line != EXIT_COMMAND) {
    readInput(line); // TODO: the trimming of exit doesnt fully work...
    trim(line);
    std::cout << line;
    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
      words.push_back(word);
    }

    // TODO: free the memory here after using
  }
  // EXIT_COMMANDint rc = fork();
  //  if (rc < 0) {
  //   fprintf(stderr, "fork failed");
  //  exit(1);
  //} else if (rc == 0) { // we are in the child
  // printf("Child (pid:%d)\n", (int)getpid());

  // } else {
  //  printf("parent of %d (pid: %d)\n", rc, (int)getpid());
  // }
  return 0;
}

void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char chr) {
            return !std::isspace(chr);
          }));
}

void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

void trim(std::string &line) {
  rtrim(line);
  ltrim(line);
}

std::string readInput(std::string &line) {
  // cout the working dir
  std::cout << "$";
  std::getline(std::cin, line); // read a line
  std::cout << "\n";
  return line;
}
