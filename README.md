# 🐚 Minishell

**Minishell** is a small Unix shell written in C. It aims to replicate basic shell functionalities, including command execution, piping, redirection, and built-in commands. This project was developed as part of a 42 school assignment.

---

## 🚀 Features

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
  - Input/output redirection (`>`, `<`, `>>`)
- Handling of environment variables
- Signal handling (Ctrl+C, )

---

## 💻 Installation

1. **Clone the repository**
```bash
git clone https://github.com/icrackfofun/minishell.git
cd minishell
Compile the project

bash
Copy code
make
This will generate the executable minishell in the project directory.

⚡ Usage
Run the shell:

bash
Copy code
./minishell
Use it like a regular shell:

bash
Copy code
$ echo "Hello, World!"
Hello, World!

$ cd ..
$ pwd
/path/to/parent
Exit the shell:

bash
Copy code
$ exit
🛠️ Tools & Language Stack
Language: C

Tools: GCC, Make, Unix system calls

📝 Notes
The shell supports basic command execution and built-in functions but does not implement advanced features like job control.

Ensure your environment variables are set properly to run commands from the PATH.

📂 Project Structure
css
Copy code
minishell/
├── Makefile
├── src/
│   ├── main.c
│   ├── builtins.c
│   ├── parser.c
│   └── utils.c
├── include/
│   └── minishell.h
└── README.md
📧 Contact
For questions or feedback, reach me at miguelred2009@gmail.com.

