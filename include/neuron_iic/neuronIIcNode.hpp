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
#ifndef __NEURON_GPIO_NODE__
#define __NEURON_GPIO_NODE__

#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "sensor_msgs/msg/temperature.hpp"
#include "neuronIIc.hpp"

/*  Topic Name Settings */
#define TOPIC_CMD "neuron_gpio_cmd"

/* GPIO Settings */
#define I2C_IMU_ADDR_ (0x68)


using std::placeholders::_1;

class NeuronIIcNode : public rclcpp::Node
{
  public:
    explicit NeuronIIcNode();
    virtual ~NeuronIIcNode();

  private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg);
    
    rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr publisher_imu_;
    rclcpp::Publisher<sensor_msgs::msg::Temperature>::SharedPtr publisher_tmp_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    std::shared_ptr<NeuronIIc> imu_;
    std::shared_ptr<sensor_msgs::msg::Imu> imu_msg_;
    std::shared_ptr<sensor_msgs::msg::Temperature> tmp_msg_;

    
};


#endif /* __NEURON_GPIO_NODE__ */
