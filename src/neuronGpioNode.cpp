// Copyright 2017 ADLINK Technology, Inc.
// Developer: Alan Chen (alan.chen@adlinktech.com)
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
#include "neuronGpioNode.hpp"


/* * * * * * * * * * 
 * Private Methods *
 * * * * * * * * * */
 void NeuronGpioNode::topic_callback(const std_msgs::msg::String::SharedPtr msg)
{
    // Print the received message
    printf("------------------------------------------------------------------\n");
    printf("=>>> receive from -- Topic <\"%s\">: \"%s\".\n", TOPIC_CMD, msg->data.c_str());
    printf("\n");

    // Check the availability of the SEMA library 
    if (NeuronGpio::IsAvailable() == false || gpio_ == NULL)
    {
        printf("[ERROR] NeuronGpioNode - SEMA Lib not found.\n");
        return;
    }

    // Toggle GPIO Level
    uint32_t level = 0;
    gpio_->SetDir(EAPI_GPIO_OUTPUT);
    gpio_->ReadLevel(level);
    //level = (level == EAPI_GPIO_LOW);
    gpio_->SetLevel(!!!level);

    // Send it out
    std::string stmp;
    stmp += "The GPIO Pin";
    stmp += std::to_string(GPIO_TOGGLE_PIN);
    stmp += " is set to ";
    stmp += level? "HIGH" : "LOW";

    msg->data = stmp;
    printf("<<<= send to ------- Topic <\"%s\">: \"%s\".\n", TOPIC_DATA, msg->data.c_str());
    publisher_->publish(msg);    
    return;
}


/* * * * * * * * * * 
 * Public Methods  *
 * * * * * * * * * */
NeuronGpioNode::NeuronGpioNode() : Node("neuron_gpio")
{
    publisher_ = this->create_publisher<std_msgs::msg::String>(
            TOPIC_DATA, rmw_qos_profile_sensor_data);

    subscription_ = this->create_subscription<std_msgs::msg::String>(
            TOPIC_CMD, std::bind(&NeuronGpioNode::topic_callback, this, _1),
            rmw_qos_profile_sensor_data);
        
    NeuronGpio::InitLib();
    
    gpio_ = NeuronGpio::IsAvailable()? std::make_shared<NeuronGpio>(GPIO_TOGGLE_PIN) : NULL;
}

NeuronGpioNode::~NeuronGpioNode()
{
    NeuronGpio::UnInitLib();
}
