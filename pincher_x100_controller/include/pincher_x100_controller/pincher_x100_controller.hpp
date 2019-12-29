﻿/*******************************************************************************
* Copyright 2019 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Authors: Darby Lim, Hye-Jong KIM, Ryan Shim, Yong-Ho Na */
/* Modified by Takara Kasai for Trossen's PincherX100 arm */

#ifndef PINCHER_X100_CONTROLLER_HPP
#define PINCHER_X100_CONTROLLER_HPP

#include <chrono>
#include <cstdio>
#include <memory>
#include <unistd.h>

#include "geometry_msgs/msg/pose_stamped.hpp"
#include <rclcpp/rclcpp.hpp>
#include "sensor_msgs/msg/joint_state.hpp"
#include "std_msgs/msg/empty.hpp"
#include "std_msgs/msg/float64.hpp"
#include "std_msgs/msg/string.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "trajectory_msgs/msg/joint_trajectory_point.hpp"

#include "open_manipulator_msgs/srv/set_joint_position.hpp"
#include "open_manipulator_msgs/srv/set_kinematics_pose.hpp"
#include "open_manipulator_msgs/srv/set_drawing_trajectory.hpp"
#include "open_manipulator_msgs/srv/set_actuator_state.hpp"
#include "open_manipulator_msgs/srv/get_joint_position.hpp"
#include "open_manipulator_msgs/srv/get_kinematics_pose.hpp"
#include "open_manipulator_msgs/msg/open_manipulator_state.hpp"
#include "pincher_x100_libs/pincher_x100.hpp"
// Only if You Have MoveIt! Dependencies
// #include "pincher_x100_controller/pincher_x100_controller_moveit.hpp"


namespace pincher_x100_controller
{
class PincherX100Controller : public rclcpp::Node
{
 public:
  PincherX100Controller(std::string usb_port, std::string baud_rate);
  virtual ~PincherX100Controller();

  void process_callback(); 
  void publish_callback();  
  double get_control_period() { return control_period_; }

  void process(double time);

  bool calc_planned_path(const std::string planning_group, open_manipulator_msgs::msg::KinematicsPose msg);
  bool calc_planned_path(const std::string planning_group, open_manipulator_msgs::msg::JointPosition msg);

  rclcpp::TimerBase::SharedPtr process_timer_;
  rclcpp::TimerBase::SharedPtr publish_timer_;

 private:
  /*****************************************************************************
  ** Parameters
  *****************************************************************************/
  bool use_platform_;
  double control_period_;
  bool use_moveit_;

  /*****************************************************************************
  ** Variables
  *****************************************************************************/
  // Robotis_manipulator related 
  PincherX100 pincher_x100_;

  // Only if You Have MoveIt! Dependencies
  // PincherX100ControllerMoveit pincher_x100_controller_moveit_;

  /*****************************************************************************
  ** Init Functions
  *****************************************************************************/
  void init_parameters();
  void init_publisher();
  void init_subscriber();
  void init_server();

  /*****************************************************************************
  ** ROS Publishers, Callback Functions and Relevant Functions
  *****************************************************************************/
  rclcpp::Publisher<open_manipulator_msgs::msg::OpenManipulatorState>::SharedPtr pincher_x100_states_pub_;
  std::vector<rclcpp::Publisher<open_manipulator_msgs::msg::KinematicsPose>::SharedPtr> pincher_x100_kinematics_pose_pub_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pincher_x100_joint_states_pub_;
  std::vector<rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr> gazebo_goal_joint_position_pub_;

  void publish_pincher_x100_states();
  void publish_kinematics_pose();
  void publish_joint_states();
  void publish_gazebo_command();

  /*****************************************************************************
  ** ROS Subscribers and Callback Functions
  *****************************************************************************/
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr pincher_x100_option_sub_;

  void pincher_x100_option_callback(const std_msgs::msg::String::SharedPtr msg);

  /*****************************************************************************
  ** ROS Servers and Callback Functions
  *****************************************************************************/
  rclcpp::Service<open_manipulator_msgs::srv::SetJointPosition>::SharedPtr goal_joint_space_path_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetKinematicsPose>::SharedPtr goal_joint_space_path_to_kinematics_pose_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetKinematicsPose>::SharedPtr goal_joint_space_path_to_kinematics_position_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetKinematicsPose>::SharedPtr goal_joint_space_path_to_kinematics_orientation_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetKinematicsPose>::SharedPtr goal_task_space_path_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetKinematicsPose>::SharedPtr goal_task_space_path_position_only_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetKinematicsPose>::SharedPtr goal_task_space_path_orientation_only_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetJointPosition>::SharedPtr goal_joint_space_path_from_present_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetKinematicsPose>::SharedPtr goal_task_space_path_from_present_position_only_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetKinematicsPose>::SharedPtr goal_task_space_path_from_present_orientation_only_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetKinematicsPose>::SharedPtr goal_task_space_path_from_present_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetJointPosition>::SharedPtr goal_tool_control_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetActuatorState>::SharedPtr set_actuator_state_server_;
  rclcpp::Service<open_manipulator_msgs::srv::SetDrawingTrajectory>::SharedPtr goal_drawing_trajectory_server_;

  void goal_joint_space_path_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetJointPosition::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetJointPosition::Response> res);
  void goal_joint_space_path_to_kinematics_pose_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Response> res);
  void goal_joint_space_path_to_kinematics_position_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Response> res);
  void goal_joint_space_path_to_kinematics_orientation_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Response> res);
  void goal_task_space_path_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Response> res);
  void goal_task_space_path_position_only_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Response> res);
  void goal_task_space_path_orientation_only_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Response> res);
  void goal_joint_space_path_from_present_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetJointPosition::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetJointPosition::Response> res);
  void goal_task_space_path_from_present_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Response> res);
  void goal_task_space_path_from_present_position_only_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Response> res);
  void goal_task_space_path_from_present_orientation_only_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetKinematicsPose::Response> res);
  void goal_tool_control_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetJointPosition::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetJointPosition::Response> res);
  void set_actuator_state_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetActuatorState::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetActuatorState::Response> res);
  void goal_drawing_trajectory_callback(
    const std::shared_ptr<open_manipulator_msgs::srv::SetDrawingTrajectory::Request> req,
    const std::shared_ptr<open_manipulator_msgs::srv::SetDrawingTrajectory::Response> res);
};
}  // namespace pincher_x100_controller
#endif //PINCHER_X100_CONTROLLER_HPP
