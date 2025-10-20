# Minishell

## Overview
This project is a small Unix shell written in C. It aims to replicate base shell functionality, including command execution, piping, redirection, and built-in commands. This was a group project developed during my time at 42 Lisbon.

---

## Features

- Allows Execution of commands using the system PATH:
  ```bash
  $ echo
  ```

  Or a through including path to executable:
  ```bash
  $ /bin/echo
  ```
  
- Implemented the Following Built-in commands:
  - `cd` (Change Directories)
  - `echo` (Print Message to a file descriptor)
  - `pwd` (Print Current Working Directory)
  - `export` (Add or Update Environment Variable)
  - `unset` (Remove Environment Variable)
  - `env` (No arguments, List Environment Variables)
  - `exit` (Exit Shell)
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

## Usage

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

