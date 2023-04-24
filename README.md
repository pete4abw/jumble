# Jumble Problem Solver

## What it is
This is the Jumble Word Puzzle solver.

It takes input as a list of letters and will match them
against a dictionary file using the aspell library.

Use this to solve your favorite puzzle. 8 characters max.

## Usage

`$ jumble ebmarcsl`

Output:

> Solved: sermablc 4483 --> scramble

## Build

`$ gcc -o jumble -laspell jumble.c`

Default executable will be `jumble`.\
*name* can be any other name you choose.

### License
This program is free to use, modify, distribute.

April 2023\
Peter Hyman\
pete@peterhyman.com
