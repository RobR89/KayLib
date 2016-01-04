#include "KString.h"

#include <sstream>

namespace KayLib {

int digit(const int c) {
  int d = c - '0';
  if(d >= 0 && d <= 9) {
    return d;
  }
  return -1;
}

int digitHex(const int c) {
  int d = c - '0';
  if(d >= 0 && d <= 9) {
    return d;
  }
  d = c - 'a';
  if(d >= 0 && d <= 5) {
    return d + 10;
  }
  d = c - 'A';
  if(d >= 0 && d <= 5) {
    return d + 10;
  }
  return -1;
}

std::string toHex(const unsigned char *data, const int length, const std::string separator) {
  char hexChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  std::string hex;
  for(int i = 0; i < length; i++) {
    unsigned char v = data[i];
    unsigned char low = v & 0x0f;
    unsigned char high = (v >> 4) & 0x0f;
    hex += hexChar[high];
    hex += hexChar[low];
    if(i < length - 1) {
      hex += separator;
    }
  }
  return hex;
}

std::string toHex(const unsigned char *data, const int length) {
  char hexChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  std::string hex;
  for(int i = 0; i < length; i++) {
    unsigned char v = data[i];
    unsigned char low = v & 0x0f;
    unsigned char high = (v >> 4) & 0x0f;
    hex += hexChar[high];
    hex += hexChar[low];
  }
  return hex;
}

std::string toHex(const unsigned char value) {
  char hexChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  unsigned char low = value & 0x0f;
  unsigned char high = (value >> 4) & 0x0f;
  std::string hex;
  hex += hexChar[high];
  hex += hexChar[low];
  return hex;
}

bool beginsWith(const std::string &str, const std::string &begin) {
  return str.find(begin) == 0;
}

bool endsWith(const std::string &str, const std::string &end) {
  if(&str != nullptr && &end != nullptr) {
    int sLen = str.length();
    int eLen = end.length();
    if(sLen >= eLen) {
      if(str.substr(sLen - eLen).compare(end) == 0) {
        return true;
      }
    }
  }
  return false;
}

std::string escape(const std::string &str, bool assumeEscapes) {
  std::stringstream out;
  int len = str.length();
  for(int i = 0; i < len; i++) {
    char c = str[i];
    switch(c) {
      case '\a': // 0x07
        out << "\\a";
        continue;
      case '\b': // 0x08
        out << "\\b";
        continue;
      case '\t': // 0x09
        out << "\\t";
        continue;
      case '\n': // 0x0a
        out << "\\n";
        continue;
      case '\v': // 0x0b
        out << "\\v";
        continue;
      case '\f': // 0x0c
        out << "\\f";
        continue;
      case '\r': // 0x0d
        out << "\\r";
        continue;
      case '\"':
        out << "\\\"";
        continue;
      case '\\':
        if(i < len - 1 && assumeEscapes) {
          // backslash or quote already escaped.
          if(str[i + 1] == '\\' || str[i + 1] == '\"') {
            i++;
            out << c << str[i];
            continue;
          }
        }
        out << "\\\\";
        continue;
      default:
        if(c <= 0x0F) {
          out << "\\u00" + toHex(c);
        } else {
          out << c;
        }
        continue;
    }
  }
  return out.str();
}

std::string xmlEscape(const std::string &str) {
  std::stringstream out;
  int len = str.length();
  for(int i = 0; i < len; i++) {
    char c = str[i];
    switch(c) {
      case '<':
        out << "&lt";
        continue;
      case '>':
        out << "&gt";
        continue;
      case '&':
        out << "&amp";
        continue;
      case '\"':
        out << "&quot";
        continue;
      case '\'':
        out << "&apos";
        continue;
      default:
        out << c;
    }
  }
  return out.str();
}

}

