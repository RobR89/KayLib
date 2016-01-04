#include "KUTF.h"
#include "KString.h"

#include <sstream>

namespace KayLib {

UTFCodeParser::UTFCodeParser() {
}

UTFCodeParser::UTFCodeParser(const UTFCodeParser& orig) {
}

UTFCodeParser::~UTFCodeParser() {
}

int UTFCodeParser::addChar(char c) {
  if(byte == 0) {
    // First byte of sequence.
    if((c & 0x80) == 0) {
      // Single byte code.
      code = c;
      return byte;
    }
    // Find number of bytes in code.
    while(c & 0x80) {
      c <<= 1;
      byte++;
    }
    code = c >> byte;
    byte--;
    if(byte == 0) {
      // Error: in middle of character.
      code = 0;
      return -1;
    }
    return byte;
  }
  // Continuing character.
  if((c & 0xC0) != 0x80) {
    // Error: continuation code expected and not found.
    code = 0;
    return -1;
  }
  byte--;
  code <<= 6;
  code += c & 0x3f;
  return byte;
}

int UTFCodeParser::addChar(char16_t c) {
  if(byte == 0) {
    if(c < 0xD800 || c >= 0xE000) {
      code = c;
      return byte;
    }
    if(c >= 0xDC00) {
      // Error! This is a continuation character.
      code = 0;
      return -1;
    }
    byte = 1;
    code = c & 0x3FF;
    return byte;
  }
  if(c < 0xDC00 || c >= 0xE000) {
    // Error: Expected continuation character not found.
    if(c < 0xD800) {
      // Assume, good code following bad character.
      code = c;
      byte = 0;
      return 0;
    }
    // Assume, good start code following bad character.
    byte = 1;
    code = c & 0x3FF;
    return byte;
  }
  byte--;
  code <<= 10;
  code += c & 0x3FF;
  code += 0x010000;
  return byte;
}

int UTFCodeParser::addChar(char32_t c) {
  code = c;
  byte = 0;
  return byte;
}

std::string UTFCodeParser::getUTF8() {
  if(byte == 0) {
    return codeToUTF8(code);
  }
  return "";
}

std::u16string UTFCodeParser::getUTF16() {
  if(byte == 0) {
    return codeToUTF16(code);
  }
  return u"";
}

std::u32string UTFCodeParser::getUTF32() {
  if(byte == 0) {
    std::u32string out;
    out += code;
    return out;
  }
  return U"";
}

void UTFCodeParser::reset() {
  code = 0;
  byte = 0;
}

//---------------------------------------------------------------
// Global functions.

std::string codeToUTF8(char32_t code) {
  std::string out;
  if(code < 0x80) {
    out += (char)code;
    return out;
  }
  if(code < 0x800) {
    out += (char)(0xC0 + ((code >> 6) & 0x1f));
    out += (char)(0x80 + (code & 0x3f));
    return out;
  }
  if(code < 0x10000) {
    out += (char)(0xE0 + ((code >> 12) & 0x1f));
    out += (char)(0x80 + ((code >> 6) & 0x3f));
    out += (char)(0x80 + (code & 0x3f));
    return out;
  }
  if(code < 0x200000) {
    out += (char)(0xF0 + ((code >> 18) & 0x1f));
    out += (char)(0x80 + ((code >> 12) & 0x3f));
    out += (char)(0x80 + ((code >> 6) & 0x3f));
    out += (char)(0x80 + (code & 0x3f));
    return out;
  }
  if(code < 0x4000000) {
    out += (char)(0xF8 + ((code >> 24) & 0x1f));
    out += (char)(0x80 + ((code >> 18) & 0x3f));
    out += (char)(0x80 + ((code >> 12) & 0x3f));
    out += (char)(0x80 + ((code >> 6) & 0x3f));
    out += (char)(0x80 + (code & 0x3f));
    return out;
  }
  out += (char)(0xFC + ((code >> 30) & 0x1f));
  out += (char)(0x80 + ((code >> 24) & 0x3f));
  out += (char)(0x80 + ((code >> 18) & 0x3f));
  out += (char)(0x80 + ((code >> 12) & 0x3f));
  out += (char)(0x80 + ((code >> 6) & 0x3f));
  out += (char)(0x80 + (code & 0x3f));
  return out;
}

std::u16string codeToUTF16(char32_t code) {
  std::u16string out;
  if(code < 0x010000) {
    out += (char16_t)code;
    return out;
  }
  char32_t ch = code - 0x010000;
  char32_t cl = ch & 0x03FF;
  ch >>= 10;
  out += (0xD800 + ch);
  out += (0xDC00 + cl);
  return out;
}

std::u16string utf8to16(std::string &str) {
  std::u16string out;
  UTFCodeParser code;
  for(auto c : str) {
    int res = code.addChar(c);
    if(res == -1) {
      // There was an error.
      return u"";
    }
    if(res == 0) {
      out += code.getUTF16();
    }
  }
  return out;
}

std::u32string utf8to32(std::string &str) {
  std::u32string out;
  UTFCodeParser code;
  for(auto c : str) {
    int res = code.addChar(c);
    if(res == -1) {
      // There was an error.
      return U"";
    }
    if(res == 0) {
      out += code.getUTF32();
    }
  }
  return out;
}

std::string utf16to8(std::u16string &str) {
  std::string out;
  UTFCodeParser code;
  for(auto c : str) {
    int res = code.addChar(c);
    if(res == -1) {
      // There was an error.
      return "";
    }
    if(res == 0) {
      out += code.getUTF8();
    }
  }
  return out;
}

std::u32string utf16to32(std::u16string &str) {
  std::u32string out;
  UTFCodeParser code;
  for(auto c : str) {
    int res = code.addChar(c);
    if(res == -1) {
      // There was an error.
      return U"";
    }
    if(res == 0) {
      out += code.getUTF32();
    }
  }
  return out;
}

std::string utf32to8(std::u32string &str) {
  std::string out;
  UTFCodeParser code;
  for(auto c : str) {
    int res = code.addChar(c);
    if(res == -1) {
      // There was an error.
      return "";
    }
    if(res == 0) {
      out += code.getUTF8();
    }
  }
  return out;
}

std::u16string utf32to16(std::u32string &str) {
  std::u16string out;
  UTFCodeParser code;
  for(auto c : str) {
    int res = code.addChar(c);
    if(res == -1) {
      // There was an error.
      return u"";
    }
    if(res == 0) {
      out += code.getUTF16();
    }
  }
  return out;
}

std::string utfEscapeChar(char c) {
  return "\\x" + toHex(c);
}

std::string utfEscapeChar(char16_t c) {
  char low = c & 0x0ff;
  char high = (c >> 8) & 0x0ff;
  return "\\u" + toHex(high) + toHex(low);
}

std::string utfEscapeChar(char32_t c) {
  char low1 = c & 0x0ff;
  char high1 = (c >> 8) & 0x0ff;
  char low2 = (c >> 16) & 0x0ff;
  char high2 = (c >> 24) & 0x0ff;
  if(high2 == 0 && low2 == 0) {
    if(high1 == 0) {
      return "\\x" + toHex(low1);
    }
    return "\\u" + toHex(high1) + toHex(low1);
  }
  return "\\U" + toHex(high2) + toHex(low2) + toHex(high1) + toHex(low1);
}

std::string utfEscape(std::string &str) {
  std::string out;
  UTFCodeParser code;
  for(auto c : str) {
    int res = code.addChar(c);
    if(res == -1) {
      // There was an error.
      return "";
    }
    if(res == 0) {
      if(code.getCode() < 0x80) {
        out += code.getUTF8();
      } else {
        out += utfEscapeChar(code.getCode());
      }
    }
  }
  return out;
}

std::string utfUnEscape(std::string &str) {
  std::string out;
  bool esc = false;
  int sz = -1;
  char32_t code;
  for(auto c : str) {
    if(esc) {
      // We are in an escape.
      if(sz == -1) {
        // This is the escape character.
        if(c != 'u' && c != 'U' && c != 'x') {
          // Was not a unicode escape.
          esc = false;
          out += "\\" + c;
          continue;
        }
        // This is a unicode escape start reading it.
        sz = 4; // UTF16
        if(c == 'U') {
          sz = 8; // UTF32
        }
        if(c == 'x') {
          sz = 2; // UTF8
        }
        code = 0;
        continue;
      }
      // We are in a unicode escape sequence.
      int val = digitHex(c);
      if(val == -1) {
        // Error parsing unicode sequence.
        return "";
      }
      code <<= 4;
      code += val;
      sz--;
      if(sz > 0) {
        continue;
      }
      // End of sequence.
      out += KayLib::codeToUTF8(code);
      esc = false;
      continue;
    }
    if(c == '\\') {
      // This is an escape sequence.
      esc = true;
      sz = -1;
      continue;
    }
    // Just part of the string, pass it through.
    out += c;
  }
  if(esc && sz > 0) {
    // Technically this is an error...
    // Add the code if we ended the string while parsing a unicode escape.
    out += KayLib::codeToUTF8(code);
  }
  return out;
}

}
