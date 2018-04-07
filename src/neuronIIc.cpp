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
#include <vector>
#include "neuronIIc.hpp"

/* * * * * * * * * * * * * *  
 * Static Private Members  *
 * * * * * * * * * * * * * */
bool NeuronIIc::isAvailable_ = false;

uint32_t NeuronIIc::libHandle_ = 0;

bool NeuronIIc::first_time_ = true;

/* * * * * * * * * * * * * 
 * Static Public Methods *
 * * * * * * * * * * * * */
bool NeuronIIc::IsAvailable() 
{
    return isAvailable_;
}


void NeuronIIc::InitLib()
{
    // Initialize the the SEMA library if it hasn't been initialized.
    if (isAvailable_ == false)
    {
        uint32_t ret = 0;
        char addr_[16] = "127.0.0.1";

        ret = SemaEApiLibInitialize(false, IP_V4, addr_, 0, (char *)"123", &libHandle_);
        isAvailable_ = (ret == EAPI_STATUS_SUCCESS)?  true : false;
        if (isAvailable_ == false)
        {
            printf("[ERROR] NeuronIIc - Can't initialize SEMA Lib. Error code: %X\n", ret);
        }
    }
}


void NeuronIIc::UnInitLib()
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



void NeuronIIc::ReadI2C(const uint8_t &cmd, std::vector<uint8_t> &data, const uint32_t &byte_cnt)
{
	uint32_t data_size = data.size() * sizeof(uint8_t);
	if(byte_cnt > data_size)
	{
		printf("Error reading %d bytes of data with only %d bytes of space", 
															byte_cnt, data_size);
		return;
	}
	uint32_t ret = 0;
    ret = SemaEApiI2CReadTransfer(libHandle_, 
                                  EAPI_ID_I2C_EXTERNAL, 
                                  addr_,
                                  cmd , 		// I2C Command/Offset, starting with register 0x3B (ACCEL_XOUT_H)
                                  &data[0] , 		// void *pBuffer, Transfer Data pBuffer
								  data_size, 		// uint32_t, Data pBuffer Length
								  byte_cnt);			// uint32_t	ByteCnt, Byte Count to read
	if (ret != EAPI_STATUS_SUCCESS) 
    {
        printf("Error Reading IIc: 0x%X\n\n", ret);
    }
}


void NeuronIIc::WriteI2C(const uint8_t &cmd, std::vector<uint8_t> &data, const uint32_t &byte_cnt)
{
	uint32_t data_size = data.size() * sizeof(uint8_t);
	if(byte_cnt > data_size)
	{
		printf("Error writing %d bytes of data, only %d bytes is given", 
														byte_cnt, data_size);
		return;
	}
	
	uint32_t ret = 0;
	ret = SemaEApiI2CWriteTransfer(libHandle_,
								   EAPI_ID_I2C_EXTERNAL,
								   addr_,
								   cmd,
								   &data[0],
								   byte_cnt);
	if (ret != EAPI_STATUS_SUCCESS) 
	{
        printf("Error Writing IIc: 0x%X\n\n", ret);
    }
	
}


void NeuronIIc::WakeUp6050()
{
    if(first_time_)
	{
   
    }
    std::vector<uint8_t> data(1, 0x00); // |reset|sleep|cycle|RSV|temp_dis|clk_sel*2|
	WriteI2C(0x6B, data, 1);
    first_time_ = false;
    return;
}