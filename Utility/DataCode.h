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

#ifndef DATACODE_H
#define DATACODE_H

#include <fstream>

namespace KayLib
{

    /**
     * A class for writing data to variables in cpp files as unsigned char arrays.
     */
    class DataCode
    {
    public:
        /**
         * generate the header code for the data as an unsigned char array.
         * @param variableName The name of the variable to create.
         * @param length The length of the data.  Will be saved in the variable 'variableName_SZ'
         * @return True if successful.
         */
        static std::string generateHeader(const std::string variableName, const int length)
        {
            std::string declaration = "extern const int " + variableName + "_SZ;\n";
            declaration += "extern const unsigned char " + variableName + "[" + std::to_string(length) + "];\n";
            return declaration;
        }
        
        /**
         * generate the cpp code for the data as an unsigned char array.
         * @param variableName The name of the variable to create.
         * @param data The data to write.
         * @param length The length of the data.  Will be saved in the variable 'variableName_SZ'
         * @param bytesPerLine Number of bytes to put on each line of code.
         * @return True if successful.
         */
        static std::string generateCode(const std::string variableName, const unsigned char *data, const int length, const int bytesPerLine = 16)
        {
            // Make sure their is at least one byte per line.
            int bpl = std::max(bytesPerLine, 1);
            // Generate variable names.
            std::string instance = "const int " + variableName + "_SZ = " + std::to_string(length) + ";\n";
            instance += "const unsigned char " + variableName + "[" + std::to_string(length) + "] = {\n";
            int index = 0;
            // Generate byte codes.
            while(index < length)
            {
                int codes = std::min(length - index, bpl);
                instance += " 0x" + toHex(&data[index], codes, ", 0x");
                index += codes;
                if(index < length)
                {
                    instance += ",";
                }
                instance += "\n";
            }
            instance += "};\n";
            return instance;
        }

    };

} // namespace KayLib
#endif /* DATACODE_H */

