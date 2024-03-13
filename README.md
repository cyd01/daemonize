# daemonize

How to daemonize process: here is a simple example with double fork in order to avoid defunct (zombie) process.

## How to compile

```bash
gcc -static -o daemonize daemonize.c
```

## How to run

```bash
./daemonize
Usage: daemonize Command [Options...]
```
