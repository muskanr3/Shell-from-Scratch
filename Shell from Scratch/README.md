All the programs have been compiled on a LINUX OS (ubuntu 20.04)


```
The program can be compiled using:
make

The executable will be run as:
./shell
```

- Executing the file will generate a prompt to accept input from the user.
- This shell is equipped to execute, cd, echo, pwd, ls, history, discover, pinfo,jobs, signal handling, piping, input-output redirection and foreground and background processes of a linux shell.
- The main driver code is written in shell.c and other header files have been included to make the code more modular.


***************************************

- The file headers.h is the header file that has all the common #include statements for all the files.
***************************************
- The file echo.c has the code required to execute the echo function.
***************************************

- The file pwd.c has the code required to execute the pwd function.
***************************************

- The file cd.c has the code required to execute the different cd functions.
***************************************

- The file cmd.h is the header file for the different functions written.
***************************************

- The file pinfo.c contains the code required to run the pinfo command.
***************************************

- The file history.c contains the code required to run the history command.
***************************************

- The file ls.c contains the code required to run the ls command.
***************************************

- The file discover.c contains the code required to run the discover command.
- The discover command returns "File not found." if the directory does not belong the given directory path.
- The discover command does not search for files if the directory is . or ..
- discover ~ returns the absolute path of the file from the root of the system.

***************************************

- The file fb.c contains the code required to run the foreground and background processes.

***************************************

- The file io.c has the code required to execute input and output redirection from and to files.
***************************************

- The file jobs.c has the code required to display all the background processes in alphabetical order.
***************************************

- The file jobs.h is the header file which has the function defintions for the jobs.c file.
***************************************

- The file pipe.c has the code required to perform piping of commands.
***************************************

- The file sig.c has the code required to take the job number of a running job and send the signal corresponding to the
signal number to that process.
***************************************

- The file pipe.c has the code required to perform piping of commands.
***************************************
- The file fgbg.c has the code required to perform the fg and bg commands anf fg.h is the header file with the fucntion definitions of fgbg.c.
***************************************

- The file signals.c has the code required to perform signal handling in the shell. 
***************************************

- The file sig.h is the header file that has the function definitions for the sig call and for all signal handling.
***************************************

```
The echo function can be run as:
-> echo <string>

The pwd function can be run as:
-> pwd

The cd function can be run as:
-> cd <., .., -, ~, <path>>

The ls function can be run as:
-> ls <., .., -, ~, <path>>

The history function can be run as:
-> history

The discover function can be run as:
-> discover <target_dir> <type_flags> <file_name>

The pinfo command can be run as:
-> pinfo <pid>

The jobs command can be run as:
-> jobs <-r, -s>

The fg or bg commands can be run as:
-> <fg, bg> <job number>

The sig command can be run as:
-> sig <job number> <signal number>

```

- The commands perform input or output redirection if the >, < or >> are given along with input or output files in the shell command.

- If > is given in the input, then the file redirects the output to the file name mentioned. If the file does not exist already it is created. The file is overwritten with the new output contents.

- If < is given in the input, then the file reads input from file name mentioned.

- If >> is given in the input, then the file redirects the output to the file name mentioned. If the file does not exist already it is created. The output is appended to the file contents that already exist.
