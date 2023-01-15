#include "robot.h"
#include "wasd.h"

int main(int argc, char **argv)
{
    setupTerminal();
    while (true)
    {
        ROBOT_MOVE mov = checkTerminalInput();
        robotApplyMove(mov);
    }
    resetTerminal();
    return 0;
}
