// Copyright 2018 ADLINK Technology, Inc.
// Developer: Ewing Kang (f039281310@yahoo.com.tw)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <stdio.h>
#include "neuronIIcNode.hpp"


/* * * * * * * * * * 
 * Private Methods *
 * * * * * * * * * */
 void NeuronIIcNode::topic_callback(const std_msgs::msg::String::SharedPtr msg)
{
    // Print the received message
    printf("------------------------------------------------------------------\n");
    printf("Topic \"%s\" called: \"%s\".\n", TOPIC_CMD, msg->data.c_str());

    // Check the availability of the SEMA library 
    if (NeuronIIc::IsAvailable() == false || imu_ == NULL)
    {
        printf("[ERROR] NeuronIIcNode - SEMA Lib not found.\n");
        return;
    }
	
	// For MPU 6050 registers, see:
	// https://www.i2cdevlib.com/devices/mpu6050#registers
	std::vector<uint8_t> data(14);
    imu_->WakeUp6050();
	imu_->ReadI2C(0x3B, data, 14);
    
	int16_t ac_x = data[0]<<8|data[1];
	int16_t ac_y = data[2]<<8|data[3];
    int16_t ac_z = data[4]<<8|data[5];
    int16_t temp_16 = data[6]<<8|data[7];
    int16_t gy_x = data[8]<<8|data[9];
    int16_t gy_y = data[10]<<8|data[11];
    int16_t gy_z = data[12]<<8|data[13];
    float temp_c = (temp_16/340.00+36.53);
    printf("MPU6050 acceleration x:%d, y:%d, z:%d\n", ac_x, ac_y, ac_z);
	printf("MPU6050 rotation x:%d, y:%d, z:%d\n", gy_x, gy_y, gy_z);
	printf("at temperature: %f\n", temp_c);
    
    imu_msg_->linear_acceleration.x = ac_x;
	imu_msg_->linear_acceleration.y = ac_y;
    imu_msg_->linear_acceleration.z = ac_z;
	imu_msg_->angular_velocity.x = gy_x;
	imu_msg_->angular_velocity.y = gy_y;
	imu_msg_->angular_velocity.z = gy_z;
    tmp_msg_-> temperature = temp_c;
    
    rclcpp::Clock ros_clock;
    rclcpp::Time now = ros_clock.now();
    imu_msg_->header.stamp = now;
    tmp_msg_->header.stamp = now;
    publisher_imu_->publish(imu_msg_);
    publisher_tmp_->publish(tmp_msg_);
    
    return;
}


/* * * * * * * * * * 
 * Public Methods  *
 * * * * * * * * * */
NeuronIIcNode::NeuronIIcNode() : Node("neuron_gpio")
{

    publisher_imu_ = this->create_publisher<sensor_msgs::msg::Imu>(
            "imu", rmw_qos_profile_sensor_data);
    publisher_tmp_ = this->create_publisher<sensor_msgs::msg::Temperature>(
            "imu_temp", rmw_qos_profile_sensor_data);
    subscription_ = this->create_subscription<std_msgs::msg::String>(
            TOPIC_CMD, std::bind(&NeuronIIcNode::topic_callback, this, _1),
            rmw_qos_profile_sensor_data);
        
    NeuronIIc::InitLib();
    imu_ = NeuronIIc::IsAvailable()? std::make_shared<NeuronIIc>(I2C_IMU_ADDR_) : NULL;
    
    imu_msg_ = std::make_shared<sensor_msgs::msg::Imu>();
    imu_msg_ -> header.frame_id = "imu";
    
    tmp_msg_ = std::make_shared<sensor_msgs::msg::Temperature>();
    tmp_msg_ -> header.frame_id = "imu";
}

NeuronIIcNode::~NeuronIIcNode()
{
    NeuronIIc::UnInitLib();
}
