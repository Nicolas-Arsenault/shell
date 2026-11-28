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
#include <sys/cdefs.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXARGS 10
#define EXEC 1
#define BUILTIN 2
#define PIPE 3
#define INVALID 0
#define EXIT_COMMAND 1
#define PWD_COMMAND 2
#define CD_COMMAND 3

struct cmd {
  int type;
};

struct cmdpipe {
  int type;
  struct cmd *left;
  struct cmd *right;
  int fd[2];
};

struct cmdexec {
  int type;
  std::vector<std::string> args{MAXARGS};
};

struct cmdbuiltin {
  int type;
  int builtintype;
  std::vector<std::string> args{MAXARGS};
};

const std::string STR_EXIT_COMMAND = "exit";
const std::string STR_PWD_COMMAND = "pwd";
const std::string STR_CD_COMMAND = "cd";
const char PIPE_OPERATOR = '|';
// volatile std::sig_atomic_t stop_requested = 0;

int main() {
  std::string line = "";
  // std::signal(SIGINT, handle_sigint);
  while (line != STR_EXIT_COMMAND) {
    readInput(line);
    struct cmd *command = buildCmd(trim(line));
    if (command->type != INVALID) {
      runcmd(command);
    }
  }
  return 0;
}

std::vector<std::string> split(std::string s, char c) {
  size_t start = 0;
  size_t end = s.find(c);
  std::vector<std::string> tokens;

  while (end != std::string::npos) {
    tokens.push_back(s.substr(start, end - start));
    start = end + 1;
    end = s.find(PIPE_OPERATOR, start);
  }
  tokens.push_back(s.substr(start));
  return tokens;
}

struct cmd *buildCmd(std::string s) {
  switch (findCmdType(s)) {
  case BUILTIN:
    return buildCmdBuiltin(s);
    break;
  case EXEC:
    return buildCmdExec(s);
    break;
  case PIPE:
    return buildCmdPipe(s);
    break;
  default:
    return buildCmdExec(s);
  }
}

int findCmdType(std::string s) {
  if (s.find(PIPE_OPERATOR) != std::string::npos) {
    return PIPE;
  }
  if (s.find(STR_EXIT_COMMAND) != std::string::npos ||
      s.find(STR_PWD_COMMAND) != std::string::npos ||
      s.find(STR_CD_COMMAND) != std::string::npos) {
    return BUILTIN;
  }
  return EXEC;
}

struct cmd *buildCmdExec(std::string s) {
  struct cmdexec *currCmd = new cmdexec();
  currCmd->type = EXEC;
  currCmd->args = parseArgs(s);
  return reinterpret_cast<cmd *>(currCmd);
}

struct cmd *buildCmdBuiltin(std::string s) {
  struct cmdbuiltin *currCmd = new cmdbuiltin();
  currCmd->type = BUILTIN;
  currCmd->builtintype = findBuiltinType(s);
  currCmd->args = parseArgs(s);
  return reinterpret_cast<cmd *>(currCmd);
}

int findBuiltinType(std::string s) {
  if (s.find(STR_CD_COMMAND) != std::string::npos) {
    return CD_COMMAND;
  }
  if (s.find(STR_EXIT_COMMAND) != std::string::npos) {
    return EXIT_COMMAND;
  }
  if (s.find(STR_PWD_COMMAND) != std::string::npos) {
    return PWD_COMMAND;
  }
  return -1;
}

std::vector<std::string> parseArgs(std::string s) {
  std::vector<std::string> words;
  std::string word;
  std::istringstream iss(s);

  while (iss >> word) {
    words.push_back(word);
  }

  if (words.size() > 10) {
    std::cerr << "Too many args";
    exit(1);
  }
  return words;
}

struct cmd *buildCmdPipe(std::string s) {
  struct cmdpipe *currCmd = new cmdpipe();
  currCmd->type = PIPE;

  std::vector<std::string> splitted = split(s, PIPE_OPERATOR);
  if (splitted.size() != 2) {
    std::cerr << "Invalid split pipe";
    currCmd->type = INVALID;
    exit(1);
  }
  currCmd->left = buildCmdExec(splitted[0]);
  currCmd->right = buildCmdExec(splitted[1]);
  return reinterpret_cast<cmd *>(currCmd);
}

void runcmd(struct cmd *currcmd) {
  switch (currcmd->type) {
  case EXEC:
    runExec(currcmd);
    break;
  case PIPE:
    runPipe(currcmd);
    break;
  case BUILTIN:
    runBuiltin(currcmd);
    break;
  }
}

void runExec(struct cmd *currcmd) {
  struct cmdexec *cmd_exec = reinterpret_cast<cmdexec *>(currcmd);
  if (fork() == 0) {
    execvp(cmd_exec->args[0].c_str(), vector_c_str(cmd_exec->args).data());
  }
}

std::vector<char *> vector_c_str(std::vector<std::string> v) {
  std::vector<char *> vc;
  vc.reserve(v.size() + 1);
  for (auto &s : v) {
    vc.push_back(s.data());
  }
  vc.push_back(nullptr);
  return vc;
}

void runBuiltin(struct cmd *currcmd) {
  struct cmdbuiltin *cmd_builtin = reinterpret_cast<cmdbuiltin *>(currcmd);

  switch (cmd_builtin->builtintype) {
  case PWD_COMMAND:
    runPwd();
    break;
  case CD_COMMAND:
    runCd(cmd_builtin);
    break;
  case EXIT_COMMAND:
    exit(1);
    break;
  }
}

void runCd(struct cmdbuiltin *cmd_builtin) {
  if (cmd_builtin->args.size() < 2) {
    std::cerr << "Not enough args to cd.";
    return;
  }
  std::error_code ec;
  std::filesystem::current_path(cmd_builtin->args[1], ec);

  if (ec) {
    std::cerr << "Error" << ec.message();
    return;
  }
}

void runPwd() { std::cout << std::filesystem::current_path(); }

void runPipe(struct cmd *currcmd) {
  struct cmdpipe *cmd_pipe = reinterpret_cast<cmdpipe *>(currcmd);

  struct cmdexec *cmd1 = reinterpret_cast<cmdexec *>(cmd_pipe->left);
  struct cmdexec *cmd2 = reinterpret_cast<cmdexec *>(cmd_pipe->right);
  pid_t pid1 = fork();
  if (pid1 == 0) {
    close(STDOUT_FILENO);
    dup(cmd_pipe->fd[1]);
    close(cmd_pipe->fd[1]);
    close(cmd_pipe->fd[0]);
    execvp(cmd1->args[0].c_str(), vector_c_str(cmd1->args).data());
    exit(1);
  }
  pid_t pid2 = fork();

  if (pid2 == 0) {
    close(STDIN_FILENO);
    dup(cmd_pipe->fd[0]);
    close(cmd_pipe->fd[1]);
    close(cmd_pipe->fd[0]);
    execvp(cmd2->args[0].c_str(), vector_c_str(cmd2->args).data());
    exit(1);
  }
  close(cmd_pipe->fd[0]);
  close(cmd_pipe->fd[1]);
  int status1;
  int status2;
  waitpid(pid1, &status1, 0);
  waitpid(pid2, &status2, 0);
}

std::string ltrim(std::string s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char chr) {
            return !std::isspace(chr);
          }));
  return s;
}

std::string rtrim(std::string s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
  return s;
}

std::string trim(std::string line) { return ltrim(rtrim(line)); }

std::string readInput(std::string &line) {
  // cout the working dir
  std::cout << "\n";
  std::cout << "$";
  std::getline(std::cin, line); // read a line
  std::cout << "\n";
  return line;
}
