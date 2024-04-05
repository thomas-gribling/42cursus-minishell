<h1 align="center">
        minishell
</h1>

<p align="center">
        <i>Group project with ccadoret</i><br>
        <i>42 cursus - Rank 3</i><br>
        Final grade: <b>?/100</b>
</p>

# 📝 Subject
This project consists in coding a simplified version of a linux shell.<br>
The program gives a prompt, the string you give to the prompt must be entirely parsed from scratch.<br>

Minishell has to implement the following features:
- The prompt must have a working history (up/down arrows)
- Find and launch executables, either with absolute, relative, or from environment PATHs (`./minishell` is relative, `/usr/ls` is absolute)
- Implement the following wildcards: `~` (home folder) and `$` environment variables (`$PATH` give the paths)
- Implement `$?` which gives the last program exit status
- Implement simple `'` and double `"` quotes. They both ignore every meta-characters between them, considering their content as a pure string, except for `$` in double strings.
- Not implement meta-characters such as `\`, `!` and not manage unclosed quotes.
- Implement pipes `|`, which redirects standard output of a process to the standard input of the next one.
- Implement redirections:
    - `< file` - Redirects the input file
    - `<< keyword` - Reads standard input until `keyword` and redirects it to input
    - `> file` - Redirects the output file in truncate mode (replace content)
    - `>> file` - Redirects the output file in append mode (add content)
- Redirect the following signals:
    - `CTRL-C`: must display a new line in the prompt
    - `CTRL-D`: must properly leave the minishell
    - `CTRL-\`: must do nothing
- Finally, implement the following builtins (even if they exist in PATH, we must recode them):
    - `exit`: properly leaves the shell
    - `pwd`: with no options or arguments
    - `cd`: works with absolute and relative paths
    - `echo`: works with `-n` option
    - `env`: works without options or arguments
    - `export`: with or without arguments
    - `unset`: with arguments

There might be leaks in the project, they are caused by the `readline()` function and can't be fixed.

# 🖥️ Usage
To compile the project, you simply need to type `make` in the repo.<br>
You will get a `minishell` executable.

The program takes no arguments:
```bash
./minishell
```
