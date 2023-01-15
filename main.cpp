/* Alter the following program so as to define each case of the switch.
The MOVE and ROTATE conditions should invoke one of the robotMoveForward, robotMoveBackward, robotRotateLeft, robotRotateRight functions.
In the default case, (when there is no terminal input) check if at least 5 seconds of idle time has passed (maintain a timer for this). This allows for the robot to stabilize in its new position before doing the main body of the default case. In this main body, begin to keep the robot in the same spot by automatically checking if position and rotation has changed (via the RealSense pose).
Keep in mind this is a two-wheeled robot, so be careful about how you interpret the quaternion. Do not fall victim to gimbal lock. 
As for translation, remember that RealSense is providing your position in a room, so you must convert this into a heading in order to know if you should move forward or backward. 
Let us call these dimensions.
If either any dimension has changed, begin to apply the corrections.
The purpose to achieve is to keep the robot in the same place in the room and in the same orientation (facing the same direction). An adjustable amount of drift should be allowed.
Handle only one dimension at a time.
In other words, if both rotation and position are wrong,
correct the rotation first before moving on to position. */ 

#include <chrono>
#include <thread>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <iomanip>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include "robot.h"
#include "wasd.h"

int main(int argc, char **argv)
{
  setupTerminal();
  rs2::pipeline pipe;
  rs2::config cfg;
  cfg.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);
  pipe.start(cfg);
  while (true) {
    auto frames = pipe.wait_for_frames();
    auto f = frames.first_or_default(RS2_STREAM_POSE);
    auto pose = f.as<rs2::pose_frame>().get_pose_data();

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
    }
  }
  resetTerminal();
}