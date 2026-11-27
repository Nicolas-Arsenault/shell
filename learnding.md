Day1 :

Essentially, execvp() will take up the stack and the heap with the new program that is executed.
the fork() allows to create a new process of the same process such that we can run execvp safely after. fork() will start from the fork() call and its return value will be 0 so we can differentiate like that ex: rc = fork() rc == 0.

also we have the wait() api such that the parent can wait after the child and the code remains deterministic.

using pipe() we can connect the output of a process to be the input of another process

learned that file descriptors are IDs that are used to map to which location stdout is. for example STDOUT_FILENO is slot 1, STDIN_FILENO is 0 STDERR_FILENO is 2. by default all 3 are open, so if we close one like STDOUT_FILENO and then we call open() creat() or dup() it will use the next avail id so it will call 1. 

This is usefull because filedescriptors stay across execevp so when the program we call writes its stdout to 1 then it writes to the file.

also learned that we can call wait(NULL) in the parent so that it will wait till its childs finished

learned that using signal() system call allows to handle SIGINTS and other events

pipe() returns a pair of file descriptors, one for the writer and one for reading. there is about 10k limit without reading it

its used to communicate through forks()....

so I just learned about pipes. Pretty intesresting. Essentially its an object we create such as an int array that will be used as reference / pointer for the filedescriptors. for example, we create a new int [] and we dup it in the slot for the read which is 0 using dup() and close(). Then we can close our initial reference through int[] and keep only the reference in the slot table. That is needed for the kernel to know when to close a pipe or not. To avoid deadlocks. 

also learned that the max memory in a pipe is roiughly 64kb on my system, hence I can launch both childs at once and one will read at the same time as the other writes, being more optimal. The reader only returns once it catches EOF, so when the writer stops writing to it.
