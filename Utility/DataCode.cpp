#include <string>
#include <fstream>

#include "DataCode.h"
#include "../String/KString.h"
#include <algorithm>

namespace KayLib {

DataCode::DataCode(const std::string headerFile, const std::string cppFile) {
  header.open(headerFile.c_str());
  cpp.open(cppFile.c_str());
}

DataCode::~DataCode() {
  header.close();
  cpp.close();
}

bool DataCode::writeDataToFile(const std::string variableName, const unsigned char *data, const int length) {
  if(header == nullptr || cpp == nullptr) {
    return false;
  }
  std::string declaration = "extern int " + variableName + "_SZ;\n";
  declaration += "extern unsigned char " + variableName + "[];\n\n";
  header << declaration;
  std::string instance = "int " + variableName + "_SZ = " + std::to_string(length) + ";\n";
  cpp << instance;
  instance = "unsigned char " + variableName + "[] = {\n";
  cpp << instance;
  int index = 0;
  while(index < length) {
    instance = " ";
    int codes = std::min(length - index, 16);
    instance += " 0x" + toHex(&data[index], codes, ", 0x");
    index += codes;
    if(index < length) {
      instance += ",";
    }
    instance += "\n";
    cpp << instance;
  }
  instance = "};\n\n";
  cpp << instance;
  return true;
}

} // namespace KayLib
