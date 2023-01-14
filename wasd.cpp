#include "robot.h"

#include <termios.h>
#include <unistd.h>
#include <cstdio>

void checkForKeypress() {
    struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~ICANON;
    new_tio.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    while(true) {
        if(getchar() != EOF) {
            char key = getchar();
            if(key == 'w' || key == 'W' || key == 'i' || key == 65) {
                robotMoveForward();
            } else if(key == 's' || key == 'S' || key == 'k' || key == 66) {
                robotMoveBackward();
            } else if(key == 'a' || key == 'A' || key == 'j' || key == 68) {
                robotRotateLeft();
            } else if(key == 'd' || key == 'D' || key == 'l' || key == 67) {
                robotRotateRight();
            }
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}


int main(int argc, char **argv)
{
  checkForKeypress();
  return 0;
}
