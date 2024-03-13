# daemonize

How to daemonize process: here is a simple example with double fork in order to avoid defunct (zombie) process.

```c
...

    if(fork() == 0) {
        setsid();
        if(fork() == 0) {
            execvp((char*)Command, (char**)Options);
        }
        exit(0);
    }
    exit(0);

...
```

## How to compile

```bash
gcc -static -o daemonize daemonize.c
```

## How to run

```bash
./daemonize
```

```log
Usage: daemonize Command [Options...]
```

> Reference: https://thelinuxjedi.blogspot.com/2014/02/why-use-double-fork-to-daemonize.html
