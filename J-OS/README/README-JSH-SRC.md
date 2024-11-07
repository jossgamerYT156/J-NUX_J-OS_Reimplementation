
## JSH Code Structure
JSH is formatted in:
```
functions
mainFunction
```
with comments in between where they are required.
as well as commented lines and TODO lines(a lot of em').

JSH Code is badly structured, and this is something that will be fixed soon.

# Function Explanations

- Basic functions like recognicing some system commands and executing them in a specific way, unnecessary, but better to have them and don't use them than have to use them and don't have them.

- Specific J-CMP's
> These basic commands follow a specific structure, shipping things like HELP messages and OSINFO meassures to help YOU use your OS in a more smooth way.
we are working on making these commands as simple and short as possible, so that way users don't need to memorize 3,000 lines of commands every time(since JSH has no `]^B` function for now.)


These specific commands are as follow:
dev : show developer info and JSH version.      (hardcoded on jsh.c)
OSINFO : get the OS INFO for you.               (process on osinfo.c)
HLP    : Shows some useful commands             (hardcoded on jsh.c)