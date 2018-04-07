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
#ifndef __NEURON_GPIO__
#define __NEURON_GPIO__

#include <string>
#include <vector>
#include "linux/EApiOs.h"
#include "EApi.h"
#include "semaeapi.h"


class NeuronIIc
{
    public:
        static bool IsAvailable();
        static void InitLib();
        static void UnInitLib();

		//void SetDir(uint32_t dir);
		//void SetLevel(uint32_t level);
		//void ReadLevel(uint32_t& level);

		void ReadI2C(const uint8_t &cmd, std::vector<uint8_t> &data, const uint32_t &byte_cnt);
		void WriteI2C(const uint8_t &cmd, std::vector<uint8_t> &data, const uint32_t &byte_cnt);
        void WakeUp6050();

        explicit NeuronIIc(uint8_t addr) : addr_(EAPI_I2C_ENC_7BIT_ADDR(addr)) {};
        virtual ~NeuronIIc() {};

    private:
        static bool isAvailable_;
        static uint32_t libHandle_;
        static bool first_time_;
		const uint8_t addr_;
};


#endif /* __NEURON_GPIO__ */
