#include "robot.h"
#include "wasd.h"

int main(int argc, char **argv)
{
  setupTerminal();
  while (true) {
    ROBOT_MOVE mov = checkTerminalInput();
    if (mov != 0) {
      robotApplyMove(mov);
      continue;
    }
  }
  resetTerminal();
}
