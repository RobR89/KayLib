#include "StringParser.h"
#include "../String/KString.h"
#include "../String/KUTF.h"

#include <sstream>

namespace KayLib {

char32_t StringParserUTF8::peekCharUTF() {
  if(index >= length) {
    return -1;
  }
  int i = index;
  int res;
  while((res = code.addChar(string[i])) != 0 && i < length) {
    i++;
  }
  if(res != 0) {
    // There was an error.
    code.reset();
    return -1;
  }
  return code.getCode();
}

char32_t StringParserUTF16::peekCharUTF() {
  if(index >= length) {
    return -1;
  }
  int i = index;
  int res;
  while((res = code.addChar(string[i])) != 0 && i < length) {
    i++;
  }
  if(res != 0) {
    // There was an error.
    code.reset();
    return -1;
  }
  return code.getCode();
}

char32_t StringParserUTF8::getCharUTF() {
  if(index >= length) {
    return -1;
  }
  int res;
  do {
    res = code.addChar(string[index++]);
  } while(res != 0 && index < length);
  if(res != 0) {
    // There was an error.
    code.reset();
    return -1;
  }
  return code.getCode();
}

char32_t StringParserUTF16::getCharUTF() {
  if(index >= length) {
    return -1;
  }
  int res;
  do {
    res = code.addChar(string[index++]);
  } while(res != 0 && index < length);
  if(res != 0) {
    // There was an error.
    code.reset();
    return -1;
  }
  return code.getCode();
}

std::string StringParserUTF8::getWordUTF() {
  std::string out;
  int start = index;
  int sz = 0;
  char32_t c;
  while((c = getCharUTF()) != ' ') {
    if(c == -1 || c == '\t') {
      return out;
    }
    // 'code' still has our character so convert it to UTF8.
    out += code.getUTF8();
  }
  return out;
}

std::u16string StringParserUTF16::getWordUTF() {
  std::u16string out;
  int start = index;
  int sz = 0;
  char32_t c;
  while((c = getCharUTF()) != ' ') {
    if(c == -1 || c == '\t') {
      return out;
    }
    // 'code' still has our character so convert it to UTF16.
    out += code.getUTF16();
  }
  return out;
}

}
