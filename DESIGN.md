## Data structures

EXEC,
BUILTIN,
PIPE

```
```
cmd:
{
  type // exec, builtin, pipe
}
```
```

```
```
cmdpipe{ //inherits...
  int type
  *cmd left
  *cmd right
  int fd\[2\];
}
```

```
```
```
cmdexec{
  int type
  args
}
```
```

```
```
cmdbuiltin{
  int type
  builtintype
  args
}
```


```
## Steps/states;

1. user input string

3. build cmd:

a) determine type
b) parse accordingly and return the cmd

4. run the cmd

a) based on the type 


## Contracts
int execvp(const char *file, char *const argv[]);
pid_t fork(void);

std::vector<char*> vector_c_str(std::vector<std::string> v){

}

```
```
string trim(string)
*cmd buildCmd(string)
int findCmdType(string)
*cmd buildCmdExec(string)
*cmd buildCmdBuiltin(string)
*cmd buildCmdPipe(string) // will call buildcmd 2x on each side
vector<string> parseArgs(string) // split & trim
vector<string> split(char c)
void runcmd(*cmd)
void runExec(*cmd)
void runBuiltin(*cmd)
void runPipe(*cmd)
```
```
