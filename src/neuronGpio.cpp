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
#include "neuronGpio.hpp"

/* * * * * * * * * * * * * *  
 * Static Private Members  *
 * * * * * * * * * * * * * */
bool NeuronGpio::isAvailable_ = false;

uint32_t NeuronGpio::libHandle_ = 0;

/* * * * * * * * * * * * * 
 * Static Public Methods *
 * * * * * * * * * * * * */
bool NeuronGpio::IsAvailable() 
{
    return isAvailable_;
}


void NeuronGpio::InitLib()
{
    // Initialize the the SEMA library if it hasn't been initialized.
    if (isAvailable_ == false)
    {
        uint32_t ret = 0;
        char addr[16] = "127.0.0.1";

        ret = SemaEApiLibInitialize(false, IP_V4, addr, 0, (char *)"123", &libHandle_);
        isAvailable_ = (ret == EAPI_STATUS_SUCCESS)?  true : false;
        if (isAvailable_ == false)
        {
            printf("[ERROR] NeuronGpio - Can't initialize SEMA Lib. Error code: %X\n", ret);
        }
    }
}


void NeuronGpio::UnInitLib()
{
    // Uninitialize the SEMA library if it has been initialized
    if (isAvailable_ == true)
    {
        SemaEApiLibUnInitialize(libHandle_);
        isAvailable_ = false;
    }
}



/* * * * * * * * * * * * * 
 *    Public Methods     *
 * * * * * * * * * * * * */
void NeuronGpio::SetDir(uint32_t dir)
{
    uint32_t dirCheck = 0, ret = 0;
	
    ret = SemaEApiGPIOGetDirection(libHandle_, EAPI_GPIO_GPIO_ID(pin_), 0x01, &dirCheck);
    if (ret != EAPI_STATUS_SUCCESS) 
    {
        printf("Error checking GPIO direction: 0x%X\n\n", ret);
    }

    if (dirCheck != dir)
    {
        ret = SemaEApiGPIOSetDirection(libHandle_, EAPI_GPIO_GPIO_ID(pin_), 0x01, dir);
        if (ret != EAPI_STATUS_SUCCESS) 
        {
            printf("Error setting GPIO direction: 0x%X\n", ret);
        }
    }
}


void NeuronGpio::SetLevel(uint32_t level)
{
    uint32_t ret = 0;
    ret = SemaEApiGPIOSetLevel(libHandle_, EAPI_GPIO_GPIO_ID(pin_), 0x01, level);
    if (ret != EAPI_STATUS_SUCCESS)
    {
        printf("(Error setting GPIO level: 0x%X)\n", ret);
    }
}


void NeuronGpio::ReadLevel(uint32_t& level)
{																				
    uint32_t ret = 0;
    ret = SemaEApiGPIOGetLevel(libHandle_, EAPI_GPIO_GPIO_ID(pin_), 0x01, &level);
    if (ret != EAPI_STATUS_SUCCESS) 
    {
        printf("Error getting GPIO level: 0x%X\n\n", ret);
    }
}
