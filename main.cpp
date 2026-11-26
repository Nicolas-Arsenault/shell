#include "utils.h"
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
    readInput(line);
    trim(line);
    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
      words.push_back(word);
    }
    //
  }
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "fork failed");
    exit(1);
  } else if (rc == 0) { // we are in the child
    printf("Child (pid:%d)\n", (int)getpid());

  } else {
    printf("parent of %d (pid: %d)\n", rc, (int)getpid());
  }
  return 0;
}

void trim(std::string &line) {
  const std::string whitespace = "\t\n\r\f\v";
  size_t start = line.find_first_not_of(whitespace);
  size_t end = line.find_last_not_of(whitespace);
  if (start == std::string::npos) {
    return;
  }
  line = line.substr(start, end - start + 1);
}
std::string readInput(std::string &line) {
  // cout the working dir

  std::cout << "$";
  std::getline(std::cin, line); // read a line
  std::cout << "\n";
  return line;
}
