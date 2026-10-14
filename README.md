# C Terminal Snake Game

A classic Snake game developed in C and played entirely in the terminal.

## Features

- Keyboard-controlled snake movement using W, A, S, and Z keys
- Score system
- Snake grows by collecting hearts
- Hearts disappear or respawn after 10 seconds
- Wrap-around movement across the edges of the game board
- Collision detection with the snake's own body
- Top 10 leaderboard
- Record saving and sorting based on score and completion time
- Colored terminal output

## How to Compile and Run

### Requirements
- A C compiler, such as GCC
- A Windows environment supporting `conio.h`

### Compile

```bash
gcc SnakeGame.c -o SnakeGame
```

### Run

```bash
SnakeGame.exe
```

## Language and Libraries

- **Language:** C
- **Libraries:** `stdio.h`, `conio.h`, `time.h`, `stdlib.h`, `stdbool.h`, and others

## Records

Player records are stored in `recordList.dat`.

## Author

MahdiBigli
