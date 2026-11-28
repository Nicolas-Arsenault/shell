#include "utils.h"
#include <algorithm>
#include <cctype>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

const std::string EXIT_COMMAND = "exit";
const std::string PWD_COMMAND = "pwd";
const std::string CD_COMMAND = "cd";
const char PIPE_OPERATOR = '|';
// volatile std::sig_atomic_t stop_requested = 0;

int main(int argc, char *argv[]) {
  std::string line = "";
  // std::signal(SIGINT, handle_sigint);
  while (line != EXIT_COMMAND) {
    readInput(line);
    trim(line);

    if (line.find(PIPE_OPERATOR) != std::string::npos) {
      auto twoCommands = split(line.data());
      if (twoCommands.size() > 2) {
        std::cout << "Only one pipe is allowed";
        continue;
      }
      std::string cmd1{twoCommands[0]};
      std::string cmd2{twoCommands[1]};
      std::vector<std::string> words1;
      std::vector<std::string> words2;
      auto args1 = formatInput(cmd1, words1);
      auto args2 = formatInput(cmd2, words2);

      handlePipe(args1, args2);
      continue;
    }
    std::vector<std::string> words;
    auto args = formatInput(line, words);

    if (args.size() <= 1) {
      std::cout << "Empty line";
      continue; // safegiard
    }

    if (args[0] == CD_COMMAND) {
      execCd(args);
      continue;
    } else if (args[0] == PWD_COMMAND) {
      std::cout << std::filesystem::current_path();
      continue;
    }

    pid_t pid = fork();
    if (pid < 0) {
      fprintf(stderr, "fork failed");
      exit(1);
    } else if (pid == 0) {
      // in the child
      execvp(args[0], args.data());
      perror("execvp failed");
      exit(1);
    } else {
      // in the parent
      int status;
      waitpid(pid, &status, 0);
    }
  }
  return 0;
}

std::vector<std::string_view> split(char *s) {
  std::string_view sv(s);
  size_t start = 0;
  size_t end = sv.find(PIPE_OPERATOR);
  std::vector<std::string_view> tokens;

  while (end != std::string_view::npos) {
    tokens.push_back(sv.substr(start, end - start));
    start = end + 1;
    end = sv.find(PIPE_OPERATOR, start);
  }
  tokens.push_back(sv.substr(start));
  return tokens;
}

void handlePipe(std::vector<char *> &args1, std::vector<char *> &args2) {
  int pfd[2];
  pipe(pfd);
  // 1. exec first command and make sure the stdout goes to the pipe [1]
  int pid = fork();
  if (pid < 0) {
    perror("fork failed");
    exit(1);
  }
  if (pid == 0) {
    // int child
    close(1);
    dup(pfd[1]);
    close(pfd[1]);
    close(pfd[0]);
    execvp(args1[0], args1.data());
    perror("execvp failed");
    exit(1);
  } else {
    // in parent
    int status1;
    // done, launch other one
    int pid2 = fork();
    if (pid2 < 0) {
      perror("fork failed");
    } else if (pid2 == 0) {
      close(0);
      dup(pfd[0]);
      close(pfd[0]);
      close(pfd[1]);
      execvp(args2[0], args2.data());
      perror("execvp failed");
      exit(1);
    } else {
      int status;
      close(pfd[0]);
      close(pfd[1]);
      waitpid(pid2, &status, 0);
      waitpid(pid, &status1, 0);
    }
  }
}
// to validate if this is ok
bool containsPipe(char *args) {
  for (size_t i = 0; i < strlen(args); i++) {
    char c = args[i];
    if (c == PIPE_OPERATOR) {
      return true;
    }
  }
  return false;
}

void execCd(std::vector<char *> args) {
  if (args.size() <= 2) { // acount for the nullptr
    std::cout << "Provide a destination path.";
    return;
  }
  std::error_code ec;
  std::filesystem::current_path(args[1], ec);
  if (ec) {
    std::cerr << "Error changing directory.";
  }
}
std::vector<char *> formatInput(std::string &line,
                                std::vector<std::string> &words) {
  std::istringstream iss(line);
  std::string word;

  while (iss >> word) {
    words.push_back(std::move(word));
  }
  // int execvp(const char* file, char* const argv[]);
  std::vector<char *> args;
  args.reserve(words.size() + 1);
  for (auto &w : words) {
    args.push_back(w.data());
  }
  args.push_back(nullptr);
  return args;
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
  std::cout << "\n";
  std::cout << "$";
  std::getline(std::cin, line); // read a line
  std::cout << "\n";
  return line;
}
