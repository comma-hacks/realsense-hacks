#include "robot.h"
#include "wasd.h"

int main(int argc, char **argv)
{
    setupTerminal();
    while (true)
    {
        MOVE mov = checkTerminalInput();
        switch (mov)
        {
        case MOVE_FORWARD:
            robotMoveForward();
            break;
        case MOVE_BACKWARD:
            robotMoveBackward();
            break;
        case ROTATE_LEFT:
            robotRotateLeft();
            break;
        case ROTATE_RIGHT:
            robotRotateRight();
            break;
        default:
            break;
        }
    }
    resetTerminal();
    return 0;
}
