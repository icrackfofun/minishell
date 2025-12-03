Minishell
=========

Overview
--------

**Minishell** is a small Unix shell written in C. It aims to replicate core shell functionality, including command execution, piping, redirection, and built-in commands.\
This project was developed as a **group project** during my time at **42 Lisbon**.

The shell interprets user input, parses commands, handles environment variables, and executes commands in a manner similar to a basic Bash shell.

* * * * *

Features
--------

### Command Execution

-   Execute programs using the **system PATH**:

```
$ echo "Hello"
```

-   Execute programs by providing the **absolute path**:

```
$ /bin/echo "Hello"
```

### Built-in Commands

-   `cd` → Change directories

-   `echo` → Print message to a file descriptor

-   `pwd` → Print the current working directory

-   `export` → Add or update environment variables

-   `unset` → Remove environment variables

-   `env` → List all environment variables (no arguments)

-   `exit` → Exit the shell

### Redirection and Piping

-   **Input redirection**: `<`

-   **Output redirection**: `>`

-   **Append output**: `>>`

-   **Here-document**: `<<`

-   **Pipes**: `|` to chain multiple commands

### Environment Variables

-   Supports reading, updating, and removing environment variables

-   Handles `$VAR` expansion in commands

### Signal Handling

-   `Ctrl+C` → Interrupt running command without exiting shell

-   `Ctrl+D` → Exit shell when at empty input

-   `Ctrl+\` → Quit running foreground process

### Additional Features

-   Handles multiple arguments per command

-   Graceful error handling for invalid commands or file operations

-   Supports relative and absolute paths for executables

* * * * *

Installation
------------

1.  **Clone the repository**

```
git clone https://github.com/icrackfofun/minishell.git
cd minishell
```

1.  **Compile the project**

```
make
```

This generates the executable `minishell` in the project directory.

* * * * *

Usage
-----

Run the shell:

```
./minishell
```

Use it like a standard shell:

```
$ echo "Hello, World!"
$> Hello, World!
$ cd ..
$ pwd
$> /path/to/parent
```

Exit the shell:

```
$ exit
```

* * * * *

Project Structure (Optional)
----------------------------

-   `src/` → Source code files (parsing, execution, builtins)

-   `includes/` → Header files

-   `Makefile` → Build instructions
