<div align="center">
<h3 align="center">Conway's Game of Life</h3>
  <p align="center">
    A simple terminal-based implementation of Conway's Game of Life running on QNX in real-time.
  </p>
</div>

## About
A terminal-based simulation of Conway's Game of Life. The program is written in ANSI C to run on QNX (real-time operating system) for one of university courses. It supports custom board sizes, rule modifications, and file-based board saving/loading.

![Screenshot of application](https://placehold.co/600x400)

### Built With

[![ANSI C][c-badge]][c-url]

[![NCurses][curses-badge]][curses-url]

[![PThread][pthread-badge]][pthread-url]

## Features
* **Real-Time Simulation:** Program can be run on real-time operating system like QNC.
* **Customizable Board:** Adjust board dimensions and enable board edge wrapping.
* **Rule Customization:** Easily modify cell survival and birth rules to explore alternative versions of the Game of Life.
* **State Persistence:** Save and load board states from files, allowing you to resume simulations.
* **Speed Control:** Fine-tune simulation speed to slow down or accelerate the evolution of cellular patterns.
* **Toggle Wrapping Mode:** Enable or disable board wrapping to alter edge behavior during simulation.
* **Use built-in maps:** In `maps/` directory there already a few example map files that can be loaded into program. Path to board file should start from program's directory (eg. maps/glider).

## Getting Started

### Prerequisites
You need the following:
* QCC or GCC compier
- Ncurses library (on Ubuntu/Debian, install with: `sudo apt install libncurses-dev`).
- Pthread library (usually included by default, but if needed, install via your package manager).

### Installation
1. Clone the repository:
```
git clone https://github.com/dawidbieniek/GameOfLife.git
```
2. Build the project:
```
make
```
3. Run the program:
```
./gol
```

### Configuration
The Makefile supports optional flags:
* **WIDE=1** - each cell will be wider making each cell square
* **REDRAW=1** - board will be redrawn on each update. This can prevent screen tearing-like artifacts from showing

> [!note]
> To remove build files (object files), run:
> ```
> make clean
> ```

## Screenshots
Add screenshots of the application here for demonstration.

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[app-screenshot]: https://placehold.co/600x400

[c-badge]: https://img.shields.io/badge/ANSI_C-A8B9CC?style=for-the-badge&logo=C&logoColor=white
[c-url]: https://en.wikipedia.org/wiki/ANSI_C
[curses-badge]: https://img.shields.io/badge/NCurses-CCCCCC?style=for-the-badge
[curses-url]: https://invisible-island.net/ncurses/
[pthread-badge]: https://img.shields.io/badge/PThead-CCCCCC?style=for-the-badge
[pthread-url]: https://en.wikipedia.org/wiki/POSIX_Threads
