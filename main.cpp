#include <librealsense2/rs.hpp>
#include <eigen3/Eigen/Geometry>
#include "robot.h"
#include "wasd.h"
#include "pid.h"

void robotRotateTo(Eigen::Quaternionf target_quat, rs2_pose pose) {
    Eigen::Quaternionf current_quat(pose.rotation.w, pose.rotation.x, pose.rotation.y, pose.rotation.z);
    float dotProduct = current_quat.dot(target_quat);
    if(dotProduct < 0) {
        robotRotateRight();
    } else {
        robotRotateLeft();
    }
}


int main(int argc, char **argv)
{
  PID pid_controller(1, 0, 0);

  setupTerminal();
  rs2::pipeline pipe;
  rs2::config cfg;
  cfg.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);
  pipe.start(cfg);
  
  Eigen::Quaternionf setpoint_quat;
  
  while (true) {
    auto frames = pipe.wait_for_frames();
    auto f = frames.first_or_default(RS2_STREAM_POSE);
    auto pose = f.as<rs2::pose_frame>().get_pose_data();
    Eigen::Quaternionf current_quat(pose.rotation.w, pose.rotation.x, pose.rotation.y, pose.rotation.z);
    
    auto current_time = std::chrono::high_resolution_clock::now();
    auto time_since_last_api_call = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_api_call_time()).count();


    ROBOT_MOVE mov = checkTerminalInput();
    if (mov != 0) {
      printf("Manual Control: %d\n", mov);
      robotApplyMove(mov);
      continue;
    }

    if(time_since_last_api_call > 1){
      float error = (setpoint_quat.conjugate() * current_quat).vec().norm();
      float control_signal = pid_controller.update(error);
      Eigen::Quaternionf target_quat = current_quat.slerp(control_signal, setpoint_quat);
  std::cout << "Error: " << error << " Control Signal: " << control_signal << " Target Quat: " << target_quat.w() << " " << target_quat.vec().transpose() << std::endl;


      robotRotateTo(target_quat, pose);  
    } else {
      setpoint_quat = current_quat;
    }
        
    
  }
  resetTerminal();
}
