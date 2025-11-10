#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "level.h"
#include "logic.h"
#include "render.h"
#include <unistd.h>
#include <termios.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Error, please provide a filename!\n");
        exit(EXIT_FAILURE);
    }

    char *filename = argv[1];

    Level *loaded_level = load_level(filename);


    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // raw mode, no echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    
    char input;
    while(true) {

        printf("\033[H\033[J");

        render_map(loaded_level);


        if(check_win(loaded_level)) break;
        
        read(STDIN_FILENO, &input, 1);

        int dx = 0;
        int dy = 0;

        switch(input) {
            case 'w': dx = -1; break; // up
            case 's': dx = 1;  break; // down
            case 'a': dy = -1; break; // left
            case 'd': dy = 1;  break; // right
        }

        try_move(loaded_level, dx, dy);

         
    }

    free_level(loaded_level);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    printf("Congrats you have one the game !!!!!\n");

    return 0;
}
