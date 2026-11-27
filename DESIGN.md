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

trim()
buildCmd()
cmdType()
parseExec()
parseBuiltin()
parsePipe()
parseArgs() * to validate
split()
runcmd()
runExec()
runBuiltin()
runPipe()




