#include <chrono>
#include <thread>

#include <librealsense2/rs.hpp>
#include <iostream>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry> 


using namespace Eigen;


int main(int argc, char **argv)
{
  // Declare RealSense pipeline, encapsulating the actual device and sensors
  rs2::pipeline pipe;
  // Create a configuration for configuring the pipeline with a non default profile
  rs2::config cfg;
  // Add pose stream
  cfg.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);
  // Start pipeline with chosen configuration
  pipe.start(cfg);

  // Main loop
  while (true)
  {
    // Wait for the next set of frames from the camera
    auto frames = pipe.wait_for_frames();
    // Get a frame from the pose stream
    auto f = frames.first_or_default(RS2_STREAM_POSE);
    // Cast the frame to pose_frame and get its data
    auto pose = f.as<rs2::pose_frame>().get_pose_data();

    int mult = 1000;

    auto x = pose.translation.x * mult;
    auto y = pose.translation.y * mult;
    auto z = pose.translation.z * mult;


    // Print the x, y, z values of the translation, relative to initial position
		std::cout << x << "," << y << "," << z << "," << pose.rotation.x << "," << pose.rotation.y << "," << pose.rotation.z << "," << pose.rotation.w << "\n";

    Quaternion q(pose.rotation.w, pose.rotation.x, pose.rotation.y, pose.rotation.z);
    auto euler = q.toRotationMatrix().eulerAngles(0, 1, 2);
    std::cout << "Euler from quaternion in roll, pitch, yaw"<< std::endl << euler << std::endl;


    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
