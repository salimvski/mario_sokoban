# Mario Sokoban (C / SDL2)

[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)  
[![C](https://img.shields.io/badge/language-C-brightgreen)](https://www.iso.org/standard/74528.html)

A network-enhanced **Sokoban** game in C using **SDL2**, **SDL2_ttf**, **SDL2_mixer**, **SDL2_image**.  
The client includes a **graphical menu**, **level selection**, **background music**, and can run either:

- in **Local Mode** (client automatically launches its own server), or  
- in **External Server Mode** (connect manually to a remote server).

---

## 🛠 Prerequisites

Install required packages (Fedora):

```bash
sudo dnf install gcc make SDL2-devel SDL2_mixer-devel SDL2_image-devel SDL2_ttf-devel valgrind
```

Optional (AddressSanitizer support):

```bash
sudo dnf install glibc-devel libasan
```

---

## 📁 Project Structure

```
src/
    client.c
    client.h
    server.c
    level.c
    logic.c
    render.c
assets/
    menu/
    tiles/
    player/
    audio/
    fonts/
levels/
    level1.txt
    level2.txt
Makefile
README.md
```

---

## 🏗 Compilation

### Build everything

```bash
make
```

Produces:
- **Client** → `./sokoban`
- **Server** → `./server`

### Clean build

```bash
make clean
```

---

## ▶ Running the Game

### 🟩 Local Mode (recommended)

You run **ONLY the client**.  
The client automatically launches its own server.

```bash
./sokoban
```

Then inside the game menu:

1. **Jouer**
2. Choose **Level 1** or **Level 2**
3. The server starts automatically  
4. Gameplay begins

---

## 🌐 Running With an External Server

To use a remote server:

### Start the server manually

```bash
./server <port> <level_file>
```

Example:

```bash
./server 5666 levels/level1.txt
```

### Start the client with host + port

```bash
./sokoban <server_ip> <port>
```

Example:

```bash
./sokoban 127.0.0.1 5666
```

> In this mode, the client **does NOT show the level selection menu**  
> (server decides the level).

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| **W / A / S / D** | Move player |
| **R** | Reset level |
| **ESC / Q** | Quit |
| **ENTER** | Validate menu selection |
| **↑ / ↓** | Navigate menu |

---

## 🔊 Audio

- Background music: `assets/audio/music/periwinkle.mp3`
- Volume: **30%**

```c
Mix_VolumeMusic(MIX_MAX_VOLUME * 0.3);
```

Requires `SDL2_mixer`.

---

## 🖥 Menu System

### Main Menu
- Jouer  
- Quitter  

### Level Selection Menu (Local Mode)
- Level 1  
- Level 2  
- Retour  

Full-screen background image adapts to window size.

---

## 🚧 Memory Notes

- Small leaks (< 3 KB) may appear from SDL internals (ASan / Valgrind).
- All textures, music, surfaces, and level data are properly freed.
- Server runs clean under Valgrind.

---

## 🧪 Example Workflow (Local Mode)

```bash
make
./sokoban
```

Navigate menus → choose level → play.

---

## 🧪 Example Workflow (External Server Mode)

Terminal 1:

```bash
./server 5666 levels/level2.txt
```

Terminal 2:

```bash
./sokoban 127.0.0.1 5666
```

---

## License

This project is licensed under the **MIT License**.  
See [LICENSE](LICENSE) for details.