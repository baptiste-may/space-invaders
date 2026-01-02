# Space Invaders

This project is a recreation of the famous game [Space Invaders](https://en.wikipedia.org/wiki/Space_Invaders) in C. This work with an [MVC](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) with two views : [Ncurses](https://en.wikipedia.org/wiki/Ncurses) and [SDL3](https://en.wikipedia.org/wiki/Simple_DirectMedia_Layer).

## Compatibility

The project available on Ubuntu, Debian and Fedora. If you want to use it on other OS, you will need to install SDL3 and ncurses depedencies.

## How to play

First, clone the project with sdl using

```shell
git clone --recursive https://github.com/baptiste-may/space-invaders.git
```

And voilà ! Everything compile using

```shell
make
```

You can start the game with Ncurses using

```shell
make run-ncurses
```

or with SDL

```shell
make run-sdl
```

The documentation can be generated using

```shell
make doc
```

A valgrind test can be made using

```shell
make valgrind
```

If you want to clean the project, just use

```shell
make clean
```
