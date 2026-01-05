# Checkers Bot using Minimax algorithm.

![Performance](https://img.shields.io/badge/Benchmark-7M+%20nodes%2Fs-critical)
![Depth](https://img.shields.io/badge/Search%20Depth-12%20ply-blueviolet)

> **"A C++ Minimax engine engineered for raw speed, capable of evaluating over 7 million board states per second on a single thread."**

# Index
1. [Story](#story)
2. [Characteristics](#key-features)
3. [How does it work](#how-does-it-work)
4. [Installation and usage](#installation-and-usage)
5. [Real Execution Block](#execution-block)
6. [How to contribute](#how-to-contribute)
7. [Licence](#licence)


# Story

This project came as an evolution of two projects I made a few months ago: 
- a minimax, my [minimax implementation for a super tic tac toe game](https://github.com/Thrinkler/superTicTacToe-minimax-engine)
was a project I loved doing, but it had a major problem: it suffered from severe latency, only evaluating a depth-9 in 30 seconds

- my [rubiks solver](https://github.com/Thrinkler/rubiks-optimal-path-solver), my first C++ project where I learned and fell in love with the performance of C++, but had a major problem with the use of objects,
where I created a new rubiks cube for my BFS search.

So, I went looking around for new project ideas, and that's where I found about bitboards. It was something so beautiful, so great, so... high performance. I didn't have to use any more objects to represent 
boards, but just a 64-bit number. Even better, I just needed bit shifts to move all the pieces! It was a dream, the best way to work with a minimax... So I went head first to the project.

After about 3 days working on it, it was finished. It worked so well that when I tried to change from a sum inside the board evaluation to a division, the simulation slowdown was significant. I was at a point
where the project worked so well I wanted to make the AI better rather than getting those higher numbers, so I went from a Depth-15 with addition to a Depth-12 with a better board evaluation. 
I chose strategic intelligence over raw depth metrics


# Key Features

- **64-Bit Bitboard Architecture**:
    The entire board state is represented as a 64-bit integer. This allows the CPU to process moves using native register operations (AND, OR, XOR, Bit-shifts) instead of accessing RAM arrays, drastically reducing cache misses.

- **Hyper-Optimized Evaluation**:
    Avoids expensive CPU instructions (like division/modulo) in critical loops. The engine evaluates **7,000,000+ nodes per second** on a single thread.

- **tree Negamax with Alpha-Beta Pruning**:
    Implements aggressive pruning to ignore irrelevant branches, allowing search depths of 12+ ply in seconds.

- **Player vs. Computer (PVC)**:
    Includes a CLI interface to play directly against the engine.

# How does it work

### 1. The Representation
A checkerboard has 64 squares (8x8). A 64-bit integer (`uint64_t`) has exactly 64 bits. We can map every square to a specific bit.
* `1` means a piece exists at that location.
* `0` means the square is empty.

This means the entire game state (White pieces, Red pieces, Kings) fits into just three integers.


### 2. Move Generation via Bitwise Magic
Instead of looping through an array to check if a neighbor is empty, we perform **Whole-Board Operations**.
### 3. The Search (Negamax)
The engine uses **Negamax**, a variant of Minimax that relies on the property $\max(a, b) = -\min(-a, -b)$. This simplifies the code by treating both players with the same logic, 
just negating the score for the opponent. Combined with Alpha-Beta pruning, it cuts off branches of the tree that are mathematically impossible to be the best move, saving millions of calculations.

# Installation and usage

To get the maximum performance (7M+ nodes/s), **you must build in Release mode**.

```bash
# 1. Clone the repository
git clone https://github.com/Thrinkler/high-performance-minimax-checkers
cd high-performance-minimax-checkers

# 2. Create a build directory
mkdir build && cd build

# 3. Configure CMake in Release mode (Crucial for performance!)
cmake -DCMAKE_BUILD_TYPE=Release ..

(Linux and MacOS)

# 4. Compile
make

# 5. Run
./MinimaxCheckers

(Windows)

# 4. Compile
cmake --build . --config Release

# 5. Run
.\MinimaxCheckers.exe

```

# Execution block
**Benchmark Hardware:** Apple MacBook Air M4 (Apple Silicon)  
**Method:** Single-threaded execution via CLI.

The following log shows highlights from a real game execution. Note **Turn 36**, where the engine hits a peak of **8,287,606 nodes/sec**.
You can view the full, uninterrupted game log here: [gameLogs/gameExample.txt](https://github.com/Thrinkler/high-performance-minimax-checkers/blob/main/gameLogs/gameExample.txt)

<details>
<summary><strong>Click here to expand parts of the Game Log (Start, Peak & End)</strong></summary>
    
```text
0|#|#|#|#|#|#|#|#|
1|#|#|#|#|#|#|#|#|
2| | | | | | | | |
3| | | | | | | | |
4| | | | | | | | |
5| | | | | | | | |
6| | | | | | | | |
7| | | | | | | | |
  0 1 2 3 4 5 6 7
0| | | | | | | | |
1| | | | | | | | |
2| | | | | | | | |
3| | | | | | | | |
4| | | | | | | | |
5| | | | | | | | |
6|#|#|#|#|#|#|#|#|
7|#|#|#|#|#|#|#|#|
  0 1 2 3 4 5 6 7
Initial Board:
----------Turn 0-----------
W: 12 R: 12
WK: 0 RK: 0
White player
0| |x| |x| |x| |x|
1|x| |x| |x| |x| |
2| |x| |x| |x| |x|
3| | | | | | | | |
4| | | | | | | | |
5|o| |o| |o| |o| |
6| |o| |o| |o| |o|
7|o| |o| |o| |o| |
  0 1 2 3 4 5 6 7
Board evaluation for white is 0
Playing as:white, depth = 12
Tiempo de pensamiento: 7452.43 ms
Numero de iteraciones: 54862275
Numero de iteraciones por segundo: 7361667
Minimax valuation: 0
Move made: Move piece in coords: 5 6 RIGHT  UP 
---------------------
----------Turn 1-----------
W: 12 R: 12
WK: 0 RK: 0
Red Player
0| |x| |x| |x| |x|
1|x| |x| |x| |x| |
2| |x| |x| |x| |x|
3| | | | | | | | |
4| | | | | | | |o|
5|o| |o| |o| | | |
6| |o| |o| |o| |o|
7|o| |o| |o| |o| |
  0 1 2 3 4 5 6 7
Board evaluation for red is -2
Playing as:red, depth = 12
Tiempo de pensamiento: 9081.64 ms
Numero de iteraciones: 65215386
Numero de iteraciones por segundo: 7181011
Minimax valuation: 0
Move made: Move piece in coords: 2 7 LEFT  DOWN 
---------------------
----------Turn 2-----------
W: 12 R: 12
WK: 0 RK: 0
White player
0| |x| |x| |x| |x|
1|x| |x| |x| |x| |
2| |x| |x| |x| | |
3| | | | | | |x| |
4| | | | | | | |o|
5|o| |o| |o| | | |
6| |o| |o| |o| |o|
7|o| |o| |o| |o| |
  0 1 2 3 4 5 6 7
Board evaluation for white is 0
Playing as:white, depth = 12
Tiempo de pensamiento: 10142.9 ms
Numero de iteraciones: 71107427
Numero de iteraciones por segundo: 7010558
Minimax valuation: 0
Move made: Move piece in coords: 5 2 RIGHT  UP 
---------------------
----------Turn 3-----------
W: 12 R: 12
WK: 0 RK: 0
Red Player
0| |x| |x| |x| |x|
1|x| |x| |x| |x| |
2| |x| |x| |x| | |
3| | | | | | |x| |
4| | | |o| | | |o|
5|o| | | |o| | | |
6| |o| |o| |o| |o|
7|o| |o| |o| |o| |
  0 1 2 3 4 5 6 7
Board evaluation for red is -2
Playing as:red, depth = 12
Tiempo de pensamiento: 4644.2 ms
Numero de iteraciones: 34109365
Numero de iteraciones por segundo: 7344507
Minimax valuation: 0
Move made: Move piece in coords: 2 3 RIGHT  DOWN 
---------------------
----------Turn 4-----------
W: 12 R: 12
WK: 0 RK: 0
White player
0| |x| |x| |x| |x|
1|x| |x| |x| |x| |
2| |x| | | |x| | |
3| | | | |x| |x| |
4| | | |o| | | |o|
5|o| | | |o| | | |
6| |o| |o| |o| |o|
7|o| |o| |o| |o| |
  0 1 2 3 4 5 6 7
Board evaluation for white is 6
Playing as:white, depth = 12
Tiempo de pensamiento: 4747.8 ms
Numero de iteraciones: 34510449
Numero de iteraciones por segundo: 7268729
Minimax valuation: 0
Move made: Move piece in coords: 5 0 RIGHT  UP 
---------------------
----------Turn 5-----------
W: 12 R: 12
WK: 0 RK: 0
Red Player
0| |x| |x| |x| |x|
1|x| |x| |x| |x| |
2| |x| | | |x| | |
3| | | | |x| |x| |
4| |o| |o| | | |o|
5| | | | |o| | | |
6| |o| |o| |o| |o|
7|o| |o| |o| |o| |
  0 1 2 3 4 5 6 7
Board evaluation for red is 0
Playing as:red, depth = 12
Tiempo de pensamiento: 1876.35 ms
Numero de iteraciones: 13595669
Numero de iteraciones por segundo: 7245787
Minimax valuation: 0
Move made: Move piece in coords: 3 4 LEFT  DOWN 
---------------------
.
.
.
----------Turn 30-----------
W: 7 R: 7
WK: 0 RK: 0
White player
0| |x| |x| | | | |
1| | |x| |x| |x| |
2| | | | | |x| | |
3|o| | | | | | | |
4| | | |o| |x| |o|
5| | |o| | | | | |
6| |o| |o| | | |o|
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for white is 1
Playing as:white, depth = 12
Tiempo de pensamiento: 14551.4 ms
Numero de iteraciones: 109830809
Numero de iteraciones por segundo: 7547762
Minimax valuation: -201
Move made: Move piece in coords: 4 3 RIGHT  UP 
---------------------
----------Turn 31-----------
W: 7 R: 7
WK: 0 RK: 0
Red Player
0| |x| |x| | | | |
1| | |x| |x| |x| |
2| | | | | |x| | |
3|o| | | |o| | | |
4| | | | | |x| |o|
5| | |o| | | | | |
6| |o| |o| | | |o|
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for red is 0
Playing as:red, depth = 12
Tiempo de pensamiento: 963.226 ms
Numero de iteraciones: 7255544
Numero de iteraciones por segundo: 7532543
Minimax valuation: 252
Move made: Move piece in coords: 2 5 LEFT  DOWN 
---------------------
----------Turn 32-----------
W: 6 R: 7
WK: 0 RK: 0
White player
0| |x| |x| | | | |
1| | |x| |x| |x| |
2| | | | | | | | |
3|o| | | | | | | |
4| | | |x| |x| |o|
5| | |o| | | | | |
6| |o| |o| | | |o|
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for white is -150
Playing as:white, depth = 12
Tiempo de pensamiento: 1606.57 ms
Numero de iteraciones: 12646777
Numero de iteraciones por segundo: 7871903
Minimax valuation: -201
Move made: Move piece in coords: 5 2 RIGHT  UP 
---------------------
----------Turn 33-----------
W: 6 R: 6
WK: 0 RK: 0
Red Player
0| |x| |x| | | | |
1| | |x| |x| |x| |
2| | | | | | | | |
3|o| | | |o| | | |
4| | | | | |x| |o|
5| | | | | | | | |
6| |o| |o| | | |o|
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for red is -1
Playing as:red, depth = 12
Tiempo de pensamiento: 4431.44 ms
Numero de iteraciones: 33414235
Numero de iteraciones por segundo: 7540266
Minimax valuation: 252
Move made: Move piece in coords: 1 4 RIGHT  DOWN 
---------------------
----------Turn 34-----------
W: 6 R: 6
WK: 0 RK: 0
White player
0| |x| |x| | | | |
1| | |x| | | |x| |
2| | | | | |x| | |
3|o| | | |o| | | |
4| | | | | |x| |o|
5| | | | | | | | |
6| |o| |o| | | |o|
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for white is 5
Playing as:white, depth = 12
Tiempo de pensamiento: 3475.37 ms
Numero de iteraciones: 28687738
Numero de iteraciones por segundo: 8254584
Minimax valuation: -250
Move made: Move piece in coords: 6 3 LEFT  UP 
---------------------
----------Turn 35-----------
W: 6 R: 6
WK: 0 RK: 0
Red Player
0| |x| |x| | | | |
1| | |x| | | |x| |
2| | | | | |x| | |
3|o| | | |o| | | |
4| | | | | |x| |o|
5| | |o| | | | | |
6| |o| | | | | |o|
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for red is 0
Playing as:red, depth = 12
Tiempo de pensamiento: 1086.76 ms
Numero de iteraciones: 8185774
Numero de iteraciones por segundo: 7532243
Minimax valuation: 201
Move made: Move piece in coords: 2 5 LEFT  DOWN 
---------------------
----------Turn 36-----------
W: 5 R: 6
WK: 0 RK: 0
White player
0| |x| |x| | | | |
1| | |x| | | |x| |
2| | | | | | | | |
3|o| | | | | | | |
4| | | |x| |x| |o|
5| | |o| | | | | |
6| |o| | | | | |o|
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for white is -170
Playing as:white, depth = 12
Tiempo de pensamiento: 1907.44 ms
Numero de iteraciones: 15808144
Numero de iteraciones por segundo: 8287606
Minimax valuation: -201
Move made: Move piece in coords: 5 2 RIGHT  UP 
---------------------
----------Turn 37-----------
W: 5 R: 5
WK: 0 RK: 0
Red Player
0| |x| |x| | | | |
1| | |x| | | |x| |
2| | | | | | | | |
3|o| | | |o| | | |
4| | | | | |x| |o|
5| | | | | | | | |
6| |o| | | | | |o|
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for red is 0
Playing as:red, depth = 12
Tiempo de pensamiento: 4779.35 ms
Numero de iteraciones: 35089783
Numero de iteraciones por segundo: 7341955
Minimax valuation: 251
Move made: Move piece in coords: 4 5 LEFT  DOWN 
---------------------
.
.
.
----------Turn 163-----------
W: 2 R: 2
WK: 2 RK: 2
White player
0| | | | | | | | |
1| | | | | | | | |
2| |O| |X| | | | |
3| | | | | | | | |
4| | | | | | | | |
5| | |O| | | | | |
6| | | | | | | | |
7| | |X| | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for white is 0
Playing as:white, depth = 12
Tiempo de pensamiento: 9.93358 ms
Numero de iteraciones: 82329
Numero de iteraciones por segundo: 8287945
Minimax valuation: 0
Move made: Move piece in coords: 5 2 LEFT  UP 
---------------------
----------Turn 164-----------
W: 2 R: 2
WK: 2 RK: 2
Red Player
0| | | | | | | | |
1| | | | | | | | |
2| |O| |X| | | | |
3| | | | | | | | |
4| |O| | | | | | |
5| | | | | | | | |
6| | | | | | | | |
7| | |X| | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for red is 0
Playing as:red, depth = 12
Tiempo de pensamiento: 8.87063 ms
Numero de iteraciones: 67214
Numero de iteraciones por segundo: 7577143
Minimax valuation: 0
Move made: Move piece in coords: 7 2 RIGHT  UP 
---------------------
----------Turn 165-----------
W: 2 R: 2
WK: 2 RK: 2
White player
0| | | | | | | | |
1| | | | | | | | |
2| |O| |X| | | | |
3| | | | | | | | |
4| |O| | | | | | |
5| | | | | | | | |
6| | | |X| | | | |
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for white is 0
Playing as:white, depth = 12
Tiempo de pensamiento: 7.18883 ms
Numero de iteraciones: 48768
Numero de iteraciones por segundo: 6783854
Minimax valuation: 0
Move made: Move piece in coords: 4 1 RIGHT  DOWN 
---------------------
----------Turn 166-----------
W: 2 R: 2
WK: 2 RK: 2
Red Player
0| | | | | | | | |
1| | | | | | | | |
2| |O| |X| | | | |
3| | | | | | | | |
4| | | | | | | | |
5| | |O| | | | | |
6| | | |X| | | | |
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for red is 0
Playing as:red, depth = 12
Tiempo de pensamiento: 0.000125 ms
Numero de iteraciones: 0
Numero de iteraciones por segundo: 0
Minimax valuation: 0
Move made: Move piece in coords: 6 3 LEFT  UP 
---------------------
----------Turn 167-----------
W: 1 R: 2
WK: 1 RK: 2
White player
0| | | | | | | | |
1| | | | | | | | |
2| |O| |X| | | | |
3| | | | | | | | |
4| |X| | | | | | |
5| | | | | | | | |
6| | | | | | | | |
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for white is -501
Playing as:white, depth = 12
Tiempo de pensamiento: 0.057958 ms
Numero de iteraciones: 323
Numero de iteraciones por segundo: 0
Minimax valuation: 0
Move made: Move piece in coords: 2 1 RIGHT  DOWN 
---------------------
----------Turn 168-----------
W: 1 R: 2
WK: 1 RK: 2
Red Player
0| | | | | | | | |
1| | | | | | | | |
2| | | |X| | | | |
3| | |O| | | | | |
4| |X| | | | | | |
5| | | | | | | | |
6| | | | | | | | |
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for red is 1001
Playing as:red, depth = 12
Tiempo de pensamiento: 0.00125 ms
Numero de iteraciones: 7
Numero de iteraciones por segundo: 0
Minimax valuation: 0
Move made: Move piece in coords: 4 1 RIGHT  DOWN 
---------------------
----------Turn 169-----------
W: 1 R: 2
WK: 1 RK: 2
White player
0| | | | | | | | |
1| | | | | | | | |
2| | | |X| | | | |
3| | |O| | | | | |
4| | | | | | | | |
5| | |X| | | | | |
6| | | | | | | | |
7| | | | | | | | |
  0 1 2 3 4 5 6 7
Board evaluation for white is -500
Playing as:white, depth = 12
Tiempo de pensamiento: 0.000125 ms
Numero de iteraciones: 0
Numero de iteraciones por segundo: 0
Minimax valuation: -500
Move made: Move piece in coords: 3 2 RIGHT  UP 
---------------------
Red Player
0| | | | | | | | |
1| | | | |O| | | |
2| | | | | | | | |
3| | | | | | | | |
4| | | | | | | | |
5| | |X| | | | | |
6| | | | | | | | |
7| | | | | | | | |
  0 1 2 3 4 5 6 7
```
</details>


# How to contribute
Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are greatly appreciated.

If you have an idea for an improvement, please follow these steps:

1. Fork the Project.

2. Create your Feature Branch (git checkout -b feature/AmazingFeature).

3. Commit your Changes (git commit -m 'Add some AmazingFeature').

4. Push to the Branch (git push origin feature/AmazingFeature).

5. Open a Pull Request.

You can also simply open an issue with the tag "enhancement" to discuss a new feature or idea.

# Licence
This project is distributed under the MIT License. See the [LICENSE](LICENSE) file for more information.
