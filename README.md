# minishell-42
Minishell project for 42 common core

This project is about recreating a shell (kinda bash-like)

I've done this project in group with [Armandase](https://github.com/Armandase)

Here is the final version of the project, without leaks nor crashes

It's possible to suppress readline leaks by using `valgrind --suppressions=vsupp`

With vsupp file as following:
```
{
   readline
   Memcheck:Leak
   ...
   fun:readline
   ...
}

{
   readline
   Memcheck:Leak
   ...
   fun:add_history
   ...
}
```
