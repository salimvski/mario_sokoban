# Mario Sokoban (C / SDL2)

[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
[![C](https://img.shields.io/badge/language-C-brightgreen)](https://www.iso.org/standard/74528.html)

A simple networked **Sokoban** game written in C using **SDL2**, **SDL2_mixer**, and **SDL2_image**.
Includes a **server** for game logic and a **client** for rendering and input.

---

## 🛠 Prerequisites

Install required packages on **Fedora**:

```bash
sudo dnf install gcc make SDL2-devel SDL2_mixer-devel SDL2_image-devel valgrind
```

Optional (for debugging memory leaks):

```bash
sudo dnf install glibc-devel libasan
```

---

## 📁 Project Structure

```
./src/
    client.c
    server.c
    level.c
    render.c
    logic.c
assets/
    tiles/
    player/
    mario_theme.mp3
levels/
    level1.txt
    level2.txt
Makefile
README.md
```

---

## 🏗 Compilation

### 1. Build everything

```bash
make
```

* **Server** → `./server`
* **Client** → `./sokoban` (compiled with AddressSanitizer for debugging)

### 2. Clean build

```bash
make clean
```

Removes binaries (`server` and `sokoban`).

---

## Running the Server

```bash
make run-server ARGS="<port> <level_file>"
```

Example:

```bash
make run-server ARGS="5666 levels/level2.txt"
```

* `<port>`: TCP port for the server.
* `<level_file>`: Path to the level file.

> Server runs under **Valgrind** by default to detect memory leaks.

---

## Running the Client

```bash
make run-client ARGS="<host> <port>"
```

Example:

```bash
make run-client ARGS="127.0.0.1 5666"
```

* `<host>`: Server IP address (`127.0.0.1` for local).
* `<port>`: TCP port server is listening on.

### Controls

* **W / A / S / D** → Move player
* **R** → Reset level (if implemented)
* **Q or ESC** → Exit the client

---

## Audio

* Background music: `assets/mario_theme.mp3`
* Default volume: 30%

```c
Mix_VolumeMusic(MIX_MAX_VOLUME * 0.3);
```

* Requires **SDL2_mixer**.

---

## ⚠ Known Memory Leaks

* Small leaks (~2–3 KB) may appear in **ASan** or **Valgrind** output.
* These come from **SDL2 internal allocations**, not your code.
* Your **textures, music, and surfaces are properly freed**.

---

## Example Workflow

1. Build everything:

```bash
make
```

2. Start server:

```bash
make run-server ARGS="5666 levels/level2.txt"
```

3. Start client in a new terminal:

```bash
make run-client ARGS="127.0.0.1 5666"
```

4. Control the player with **W/A/S/D**, and **Q** to quit.

---

## Notes

* Client is compiled with **AddressSanitizer** for debugging.
* Server is run under **Valgrind** to catch leaks and errors in network handling.
* Level files are in `levels/` and can be modified to create custom puzzles.
* Makefile supports `ARGS` to easily pass parameters for port, host, and level.

---

## License

This project is licensed under the **MIT License** – see [LICENSE](LICENSE) for details.