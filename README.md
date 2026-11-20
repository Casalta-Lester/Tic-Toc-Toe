# Tic Tac Toe Game

A C++ GUI Tic Tac Toe game built with **raylib**, featuring user login/registration saved in JSON (`src/UserInfo.JSON`), a 5-round match system, and multiple game modes (Player vs. Computer Easy, Player vs. Computer Hard using Minimax, and Player vs. Player).

## Prerequisites

Before running the project, make sure you have:
* **raylib** library installed and configured.
* A **C++ compiler** such as **MinGW-w64** (or another compatible C++ compiler set up with VS Code).
* **Visual Studio Code** (recommended for opening the workspace and running/debugging).

## How to Run in VS Code

1. Open the project folder and click **`main.code-workspace`** to open it in VS Code.
2. Once the workspace opens in VS Code, open the **`src`** folder in the Explorer sidebar and click **`main.cpp`**.
3. Press **`F5`** to build and run the application.

## Features

* **Account System:** Register a new account or log in with an existing one. User credentials and win counts (`Score(easy)` and `Score(hard)`) are stored in `src/UserInfo.JSON`.
* **Game Modes:**
  * **Player vs. Computer (Easy):** Computer makes random moves.
  * **Player vs. Computer (Hard):** Computer uses the **Minimax** algorithm to choose optimal moves.
  * **Player vs. Player:** Two players take turns on the same screen.
* **5-Round Matches:** Each match plays for 5 rounds before declaring the overall winner or a tie.
