# shell
Educational purpose, to learn about threads, memory management and low level code as well as getting familiar with nvim.

<img width="650" height="400" alt="image" src="https://github.com/user-attachments/assets/83e9596f-3d54-4723-8022-e7a646e28593" />


### Current features
The current shell supports the following builtin commands:

- cd
- pwd
- exit

It also supports launching executables such as ls, wc, etc... with their respective arguments.

It supports exactly one pipe operator.

### Limitations

Their are some edge cases I am aware of which could lead to a crash. For example entering " | wc" would provide a nullptr in the first part of the args and cause an error.

Such edge cases are present throughout the code, the goal of the project is not to make exhaustive error handling but learning purposes.

### Learning

Talking about learning, you can follow my personal and *unordered* thoughts through the journey @ learning.md.
