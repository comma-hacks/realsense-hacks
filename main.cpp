#include <librealsense2/rs.hpp>
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
    
    Eigen::Quaternionf setpoint_quat(pose.rotation.w, pose.rotation.x, pose.rotation.y, pose.rotation.z);

    ROBOT_MOVE mov = checkTerminalInput();
    if (robotApplyMove(mov)) {
      continue;
    }
    
  }
  resetTerminal();
}

/* Update the above program such that in the default case, the yaw of the two-wheeled robot is kept near a given setpoint.
  This setpoint should be saved a few seconds after we cease to call the robot APIs.
  Think about it step by step. Then write the code. */