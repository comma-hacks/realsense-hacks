#include <chrono>
#include <thread>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <iomanip>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include "robot.h"
#include "wasd.h"

using namespace Eigen;

#define SLIP_REACTION_LIMIT 0.1
#define MOVEMENT_SPEED 0.1
#define ROTATION_SPEED 0.1

int main(int argc, char **argv)
{
  // Setup our input method (keyboard stdin)
  setupTerminal();

  // Declare RealSense pipeline, encapsulating the actual device and sensors
  rs2::pipeline pipe;
  // Create a configuration for configuring the pipeline with a non default profile
  rs2::config cfg;
  // Add pose stream
  cfg.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);
  // Start pipeline with chosen configuration
  pipe.start(cfg);

  Vector3d desired_position(0.0, 0.0, 0.0);
  Quaternion<double> desired_rotation(1.0, 0.0, 0.0, 0.0);
  Vector3d movement_increment(0.0, 0.0, 0.0);
  double rotation_increment = 0.0;

  while (true)
  {
    // Wait for the next set of frames from the camera
    auto frames = pipe.wait_for_frames();
    // Get a frame from the pose stream
    auto f = frames.first_or_default(RS2_STREAM_POSE);
    // Cast the frame to pose_frame and get its data
    auto pose = f.as<rs2::pose_frame>().get_pose_data();
    Vector3d current_position(pose.translation.x, pose.translation.y, pose.translation.z);
    Quaternion<double> current_rotation(pose.rotation.w, pose.rotation.x, pose.rotation.y, pose.rotation.z);

    MOVE mov = checkTerminalInput();
    switch (mov)
    {
    case MOVE_FORWARD:
      movement_increment += Vector3d(MOVEMENT_SPEED, 0.0, 0.0);
      break;
    case MOVE_BACKWARD:
      movement_increment += Vector3d(-MOVEMENT_SPEED, 0.0, 0.0);
      break;
    case ROTATE_LEFT:
      rotation_increment += ROTATION_SPEED;
      break;
    case ROTATE_RIGHT:
      rotation_increment -= ROTATION_SPEED;
      break;
    case NONE:
    {
      // Apply movement increment
      desired_position += movement_increment;
      movement_increment = Vector3d(0.0, 0.0, 0.0);

    // Apply rotation increment
      Quaternion<double> yaw_rotation(AngleAxisd(rotation_increment, Vector3d::UnitZ()));
      desired_rotation = yaw_rotation * desired_rotation;
      rotation_increment = 0.0;

      // Calculate position and rotation errors
      Vector3d position_error = desired_position - current_position;
      Quaternion<double> rotation_error = desired_rotation * current_rotation.conjugate();

      // Print position error and yaw error
      double yaw_error = 2.0 * std::atan2(rotation_error.vec().z(), rotation_error.w());
      std::cout << std::fixed << std::setprecision(3) << "Position Error: (" << std::left << std::setw(6) << position_error.x() << ", " << std::setw(6) << position_error.y() << ") Yaw Error: " << yaw_error << std::endl;

      // Apply inputs to the robot's web API based on the position error
      if (position_error.x() > SLIP_REACTION_LIMIT)
        robotMoveBackward();
      else if (position_error.x() < -SLIP_REACTION_LIMIT)
        robotMoveForward();

      // Apply inputs to the robot's web API based on the rotation error
      if (yaw_error > SLIP_REACTION_LIMIT)
        robotRotateLeft();
      else if (yaw_error < -SLIP_REACTION_LIMIT)
        robotRotateRight();
    }
    default:
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}