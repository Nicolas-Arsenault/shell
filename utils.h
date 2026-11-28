#include <string>
#include <vector>

int findBuiltinType(std::string s);
std::string readInput(std::string &line);
std::string ltrim(std::string s);
std::string rtrim(std::string s);
std::string trim(std::string line);
struct cmd *buildCmd(std::string s);
struct cmd *buildCmdExec(std::string s);
int findCmdType(std::string s);
struct cmd *buildCmdBuiltin(std::string s);
struct cmd *buildCmdPipe(std::string s);
std::vector<char *> vector_c_str(std::vector<std::string> v);
void runcmd(struct cmd *);
void runCd(struct cmdbuiltin *);
void runPwd();
void runExec(struct cmd *);
void runBuiltin(struct cmd *);
void runPipe(struct cmd *);
std::vector<std::string> parseArgs(std::string s);
std::vector<std::string> split(std::string, char s);
