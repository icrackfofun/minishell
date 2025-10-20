# Minishell

This project is a small Unix shell written in C. It aims to replicate some shell functionality, including command execution, piping, redirection, and built-in commands. This project was developed as part of a 42 school assignment.

---

## Features

- Execute commands from the system PATH
- Built-in commands:
  - `cd`
  - `echo`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Support for:
  - Command arguments
  - Pipes (`|`)
  - Input/output redirection (`>`, `<`, `>>`, `<<`)
- Handling of environment variables
- Signal handling (Ctrl+C, Ctrl+D, Ctrl+\)

---

## Installation

1. **Clone the repository**
```bash
git clone https://github.com/icrackfofun/minishell.git
```

2. **Compile the project**
```bash
make
```

This will generate the executable minishell in the project directory.

**Usage**
Run the shell:
```bash
./minishell
```

Use it like a regular shell:
```bash
$ echo "Hello, World!"
Hello, World!

$ cd ..
$ pwd
/path/to/parent
```

Exit the shell:
```bash
$ exit
```

**Tools & Language Stack**
Language: C

Tools: GCC, Make, Unix system calls

Ensure your environment variables are set properly to run commands from the PATH.

