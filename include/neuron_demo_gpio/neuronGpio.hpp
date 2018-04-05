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
#ifndef __NEURON_GPIO__
#define __NEURON_GPIO__

#include <string>
#include "linux/EApiOs.h"
#include "EApi.h"
#include "semaeapi.h"


class NeuronGpio
{
    public:
        static bool IsAvailable();
        static void InitLib();
        static void UnInitLib();

	void SetDir(uint32_t dir);
	void SetLevel(uint32_t level);
	void ReadLevel(uint32_t& level);

        explicit NeuronGpio(uint32_t pin) : pin_(pin) {};
        virtual ~NeuronGpio() {};

    private:
        static bool isAvailable_;
        static uint32_t libHandle_;        
		uint32_t pin_;
};


#endif /* __NEURON_GPIO__ */
