# Sokoban (C Project)

A minimal Sokoban clone in C — text-based version for practicing dynamic memory, game logic, and modular design.

---

## Project Structure

include/   -> Header files (function signatures)
src/       -> Source files (implementation placeholders)
levels/    -> Level text files
Makefile   -> Build instructions

---

## Header Signatures

### level.h

Level *load_level(const char *filename);
void free_level(Level *lvl);

### logic.h

int try_move(Level *lvl, int dx, int dy);
int check_win(Level *lvl);

### render.h

void render_map(Level *lvl);
void render_status(Level *lvl);

---

## Level file format (levels/level1.txt)

#######
#     #
# .$@ #
#  *  #
#######

Legend:  
# = Wall  
@ = Player  
$ = Box  
* = Goal  
. = Floor  

---

## Build & Run

make
./sokoban

---

## Future Add-ons

- Multiple levels  
- Undo feature  
- SDL graphical version  
- Save/load progress


## Roadmap

### Core game
- Reset game state by press R - DONE
- Create full menu logic

### Multiplayer side
- Able to connect to a server to join a party
- Able to play 1 v 1 game two split screens


## Bugs

- Fix display of map does not look like the lvl.txt