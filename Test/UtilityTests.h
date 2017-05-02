/*
 * Copyright 2017 Robert Reinhart.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UTILITYTESTS_H
#define UTILITYTESTS_H

#include "../Utility/Endian.h"

void testEndian()
{
    std::cout << "Starting endian swap test." << std::endl;
    const int sz = 4;
    EndianSwap<int> swap;
    swap.value = 0x11223344;
    std::cout << "Union size for int: " << sizeof(swap) << std::endl;
    std::cout << "Value: " << std::hex << swap.value << std::endl;
    for(int i = 0; i < sz;i++)
    {
        std::cout << "byte " << i << ": " << std::hex << (int)swap.bVal[i] << std::endl;
    }
    swap.swap();
    std::cout << "Swapped: " << std::hex << swap.value << std::endl;
    for(int i = 0; i < sz;i++)
    {
        std::cout << "byte " << i << ": " << std::hex << (int)swap.bVal[i] << std::endl;
    }
    
    const int szf = sizeof(float);
    EndianSwap<float> swapf;
    swapf.value = 5.5;
    std::cout << "Union size for float: " << sizeof(swapf) << std::endl;
    std::cout << "Value: " << std::hex << swapf.value << std::endl;
    for(int i = 0; i < szf;i++)
    {
        std::cout << "byte " << i << ": " << std::hex << (int)swapf.bVal[i] << std::endl;
    }
    swapf.swap();
    std::cout << "Swapped: " << std::hex << swapf.value << std::endl;
    for(int i = 0; i < szf;i++)
    {
        std::cout << "byte " << i << ": " << std::hex << (int)swapf.bVal[i] << std::endl;
    }
    
    const int szd = sizeof(double);
    EndianSwap<double> swapd;
    swapd.value = 5.5;
    std::cout << "Union size for double: " << sizeof(swapd) << std::endl;
    std::cout << "Value: " << std::hex << swapd.value << std::endl;
    for(int i = 0; i < szd;i++)
    {
        std::cout << "byte " << i << ": " << std::hex << (int)swapd.bVal[i] << std::endl;
    }
    swapd.swap();
    std::cout << "Swapped: " << std::hex << swapd.value << std::endl;
    for(int i = 0; i < szd;i++)
    {
        std::cout << "byte " << i << ": " << std::hex << (int)swapd.bVal[i] << std::endl;
    }
    
    std::cout << "Endian swap test complere." << std::endl;
    // Reset to decimal output and add blank line.
    std::cout << std::dec << std::endl;
}

#include "Utility/DataCode.h"

static constexpr int test_SZ = 24;
static constexpr unsigned char test[24] = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01,
    0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23
};

void testDataCode()
{
    // test writing data to files.
    std::cout << "Data code test." << std::endl;
    std::cout << "Header output:" << std::endl;
    std::cout << KayLib::DataCode::generateHeader("key", test_SZ);
    std::cout << "Code output:" << std::endl;
    std::cout << KayLib::DataCode::generateCode("key", test_SZ, test, 8);
    std::cout << "Static header-only Code output:" << std::endl;
    std::cout << KayLib::DataCode::generateStaticHeaderOnly("key", test_SZ, test, 8);
    std::cout << "Data code test compelte." << std::endl;
    std::cout << std::endl;
}

#endif /* UTILITYTESTS_H */

