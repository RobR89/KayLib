/*
 * Copyright 2016 Robert Reinhart.
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
#define	DATACODE_H

#include <fstream>

namespace KayLib {
/**
 * A class for writing data to variables in cpp files as unsigned char arrays.
 */
class DataCode {
public:
  /**
   * A class for writing data to variables in cpp files as unsigned char arrays.
   * @param headerFile The header file where the variable names should be defined.
   * @param cppFile The cpp file where the data should written.
   */
  DataCode(const std::string headerFile, const std::string cppFile);
  virtual ~DataCode();

  /**
   * Write the data to the code files as an unsigned char array.
   * @param variableName The name of the variable to create.
   * @param data The data to write.
   * @param length The length of the data.  Will be saved in the variable 'variableName_SZ'
   * @return True if successful.
   */
  bool writeDataToFile(const std::string variableName, const unsigned char *data, const int length);

private:
  std::ofstream header;
  std::ofstream cpp;

};

} // namespace KayLib
#endif	/* DATACODE_H */

