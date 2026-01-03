# Space Invaders in C

A classic Space Invaders implementation in C, featuring a modular MVC architecture with support for both a modern graphical interface (SDL3) and a retro terminal interface (ncurses).

## 🚀 Features

*   **🎮 Dual Interface Support**:
    *   **🖼️ SDL View**: High-quality graphics, smooth animations, and sound support using the latest SDL3 library.
    *   **📟 Ncurses View**: A fully functional, text-based version playable directly in your terminal.
*   **🏗️ MVC Architecture**: Clean separation of concerns between game logic (Model), rendering (View), and input handling (Controller).
*   **👾 Classic Gameplay**: Destroy waves of aliens, dodge enemy fire, and take cover behind destructible shields.
*   **🏆 Score Tracking**: Local high-score tracking.
*   **📂 Menus**: Fully navigable main menu, credits, and game over screens.

## 🛠️ Prerequisites

This project is designed for **🐧 Linux** systems.

You will need standard build tools installed on your system:
*   `gcc` (GNU Compiler Collection)
*   `make`
*   `git`
*   `cmake` (required for building SDL3 dependencies)

## 📦 Installation & Compilation

1.  **📥 Clone the repository**:
    ```bash
    # Clone with submodules
    git clone --recursive https://github.com/baptiste-may/space-invaders.git
    cd space-invaders

    # OR if already cloned without submodules
    git submodule update --init --recursive
    ```

2.  **🏗️ Build the project**:
    The project includes a `Makefile` that handles the compilation of the game and its dependencies (SDL3).
    ```bash
    make
    ```
    *Note: ⏳ The first compilation might take some time as it builds the SDL3 libraries from the `3rdParty` directory.*

## 🕹️ How to Play

You can launch the game in either SDL (graphical) or Ncurses (terminal) mode.

### 🏃 Running the Game

*   **🎨 Graphical Mode (SDL)**:
    ```bash
    make run-sdl
    ```

*   **📟 Terminal Mode (Ncurses)**:
    ```bash
    make run-ncurses
    ```

### ⌨️ Controls

The game supports both arrow keys and WASD/ZQSD layouts.

| Action | Key 1 | Key 2 |
| :--- | :---: | :---: |
| **⬅️ Move Left** | `←` (Left Arrow) | `Q` / `A` |
| **➡️ Move Right** | `→` (Right Arrow) | `D` |
| **💥 Shoot** | `Space` | |
| **⬆️ Menu Up** | `↑` (Up Arrow) | `Z` / `W` |
| **⬇️ Menu Down** | `↓` (Down Arrow) | `S` |
| **✅ Select / Confirm** | `Enter` |
| **🚪 Pause / Back / Quit** | `Escape` | |

## 📐 Architecture

This project strictly follows the **Model-View-Controller (MVC)** design pattern to ensure code modularity and maintainability.

### 1. 🧠 Model (`src/model/`)
The **Model** contains all the data and logic of the game, completely independent of how it is displayed.
*   **Entities**: Manages the state of the `Player`, `Aliens`, `Shields`, and `Projectiles`.
*   **Logic**: Handles collision detection, movement updates, and score calculation.
*   **State**: Holds the current state of menus (Main, Credits, Game Over).

### 2. 📺 View (`src/views/`)
The **View** is responsible for rendering the game state to the user. It uses a polymorphic interface (`ViewInterface`) to support multiple display backends:
*   **`sdl-view.c`**: Renders sprites, textures, and text using SDL3.
*   **`ncurses-view.c`**: Renders ASCII art and characters using the ncurses library.
*   The rest of the application interacts with the view through generic function pointers, allowing seamless switching between graphical and text modes.

### 3. 🕹️ Controller (`src/controller/`)
The **Controller** bridges the gap between the user, the model, and the view.
*   **⌨️ Input Handling**: Scans for user inputs via the active view's event system.
*   **🔄 Game Loop**: Orchestrates the main application loop, updating the model based on inputs and time, and then triggering a render of the view.
*   **State Management**: Transitions between the menu, game, and game-over states.

## 👥 Credits

*   **Baptiste MAY**
*   **Kamil CHARBENAGA**

## 📜 License

This project is open-source. Please check the `LICENCE` file for details.
