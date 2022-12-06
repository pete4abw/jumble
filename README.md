# Jumble Problem Solver

## What it is
This is the Jumble Word Puzzle solver.

It takes input as a list of letters and will match them
against a dictionary file.

Use this to solve your favorite puzzle.

A file or symlink "wordlist" must exist to check words.

## Usage

`$ jps scramble`

Output:

> Found word!: line 7918, clambers\
> Found word!: line 17549, scramble\
> \
> Read:         138599 words\
> Examined:     21388 - 8 letter words\
> Found word #: 2\

## Build

`$ gcc -o jps jumble.c`

jps can be any other name you choose.

### License
This program is free to use, modify, distribute.

December 2022\
Peter Hyman\
pete@peterhyman.com
