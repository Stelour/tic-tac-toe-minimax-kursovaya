# Tic‑Tac‑Toe N × N (Command‑Line Interface)

A fully configurable Tic‑Tac‑Toe that lets you play on **any square board size** – from the classic 3 × 3 up to 100 × 100 and beyond. The only real limit is the amount of RAM your machine can allocate at runtime, because the board is created dynamically.

Originally written as a university course project, the code is kept deliberately simple (pure ANSI C 99) and therefore easy to build on Linux, macOS and Windows.

---

## Features

* **Arbitrary board size** `N` (≥ 3) and winning length `K` (3 ≤ K ≤ N).
  Example: 15 × 15 board with 5‑in‑a‑row.
* **Two game modes**

  * **PvP** – two human players on one keyboard.
  * **PvC** – human vs. computer.
* **Three AI difficulty levels**

  1. **Easy** – random move.
  2. **Medium** – Minimax (depth ≤ 4).
  3. **Hard**  – Minimax + α‑β pruning with adaptive depth.
* Terminal‑only I/O – no external graphics or GUI libs.
* Clean, portable codebase (<1 kLOC) with descriptive comments.

---

## Quick Start

```bash
# clone the repo (HTTPS)
$ git clone https://github.com/Stelour/tic-tac-toe-minimax-kursovaya.git
$ cd tic-tac-toe-minimax-kursovaya

# build 
$ gcc -o tic_tac_toe main.c input.c game.c ai.c

# run
$ ./tic_tac_toe
```

---

## Controls & Gameplay

* **Main menu**
  `1` – Player vs Player
  `2` – Player vs Computer
  `3` – Settings
  `0` – Quit
* **During a move** – type row and column separated by space (`5 7`).
* **Leave the current game** – enter `0 0`.
* After a win/draw you’ll see the final board and a menu:
  `1` – New game  ·  `2` – Main menu  ·  `0` – Exit.

---

## File Overview

| File      | Purpose                                                |
| --------- | ------------------------------------------------------ |
| `main.c`  | entry point; main menu dispatcher                      |
| `game.c`  | game loops (PvP / PvC), board handling, win/draw check |
| `ai.c`    | three AI levels: random, Minimax, Minimax + αβ         |
| `input.c` | console helpers: clear screen, menus, safe input       |