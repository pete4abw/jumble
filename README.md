# Jumble Problem Solver

## What it is
This is the Jumble Word Puzzle solver.

It takes input as a list of letters and will match them
against a dictionary file.

Use this to solve your favorite puzzle.

A file or symlink "wordlist" must exist to check words.

## HOWTO Create a wordlist
For English US variant for example: `aspell -d en_US dump master > wordlist`

see [Superuser.com thread on aspell list expansion](https://superuser.com/questions/137957/how-to-convert-aspell-dictionary-to-simple-list-of-words) for details on handling affixes, if any.

## Usage

`$ jumble scramble`

Output:

> Found word!: line 7918, clambers\
> Found word!: line 17549, scramble\
> \
> Read:         138599 words\
> Examined:     21388 - 8 letter words\
> Found word #: 2

## Build

`$ gcc [-o name] jumble.c`

Default executable will be `jumble`.\
*name* can be any other name you choose.

### License
This program is free to use, modify, distribute.

April 2023\
Peter Hyman\
pete@peterhyman.com
