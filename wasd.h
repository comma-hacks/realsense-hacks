#ifndef WASD_H
#define WASD_H
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include "moves.h"

struct termios old_tio, new_tio;

void setupTerminal()
{
  tcgetattr(STDIN_FILENO, &old_tio);
  new_tio = old_tio;
  new_tio.c_lflag &= ~ICANON;
  new_tio.c_lflag &= ~ECHO;
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

void resetTerminal()
{
  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

ROBOT_MOVE checkTerminalInput()
{
  fd_set set;
  struct timeval timeout;
  int rv;

  FD_ZERO(&set);
  FD_SET(0, &set);
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  rv = select(1, &set, NULL, NULL, &timeout);
  if (rv == -1) {
    perror("select");
  } else if (rv == 1) {
    char key = getchar();
    if (key == 'w' || key == 'W' || key == 'i' || key == 65)
    {
      return ROBOT_MOVE::MOVE_FORWARD;
    }
    else if (key == 's' || key == 'S' || key == 'k' || key == 66)
    {
      return ROBOT_MOVE::MOVE_BACKWARD;
    }
    else if (key == 'a' || key == 'A' || key == 'j' || key == 68)
    {
      return ROBOT_MOVE::ROTATE_LEFT;
    }
    else if (key == 'd' || key == 'D' || key == 'l' || key == 67)
    {
      return ROBOT_MOVE::ROTATE_RIGHT;
    }
  }
  return ROBOT_MOVE::NONE;
}
#endif