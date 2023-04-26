# Jumble Problem Solver

## What it is
This is the Jumble Word Puzzle solver.

It takes input as a list of letters and will match them
against a dictionary file using the aspell library.

If more than one solution possible, all will be shown.

Use this to solve your favorite puzzle. 8 characters max.

## Usage

`$ jumble ebmarcsl`

Output:

> Words to be tested: 40320
> Solved: ebmarcsl 26399 --> clambers
> Solved: ebmarcsl 35279 --> scramble

## Build

Required aspell library.

`$ gcc -o jumble -laspell jumble.c`

or

`$ gcc -o [your name here] -laspell jumble.c`

### License
This program is free to use, modify, distribute.

April 2023\
Peter Hyman\
pete@peterhyman.com
