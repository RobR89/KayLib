#include "KFontProperties.h"
#include "../String/KString.h"
#include "../App/KApplication.h"

#include <sstream>
#include <mutex>
#include <bits/basic_string.h>

//#include <bits/stl_map.h>

namespace KayLib {

std::map<std::string, std::shared_ptr<KFontProperties>> fontList;

std::mutex lockPtr;

std::unique_lock<std::mutex> getLock() {
  static std::mutex lockPtr;
  return std::unique_lock<std::mutex>(lockPtr);
}

int getFirstGlyph(const TTF_Font *_Font) {
  int ch = 0;
  while(ch < 0x10000) {
    if(TTF_GlyphIsProvided(_Font, ch)) {
      return ch;
    }
    ch++;
  }
}

int getLastGlyph(const TTF_Font *_Font) {
  int last = 0;
  int ch = 0;
  while(ch < 0x10000) {
    if(TTF_GlyphIsProvided(_Font, ch)) {
      last = ch;
    }
    ch++;
  }
  return last;
}

int getTotalGlyphs(const TTF_Font *_Font) {
  int total = 0;
  int ch = 0;
  while(ch < 0x10000) {
    if(TTF_GlyphIsProvided(_Font, ch)) {
      total++;
    }
    ch++;
  }
  return total;
}

std::vector<KFontProperties::GlyphRange> getRanges(const TTF_Font *_Font) {
  std::vector<KFontProperties::GlyphRange> ranges;
  // find glyph ranges.
  int ch = 0;
  while(ch < 0x10000) {
    if(TTF_GlyphIsProvided(_Font, ch++)) {
      int rs = ch - 1;
      while(TTF_GlyphIsProvided(_Font, ch++)) {
      }
      ranges.push_back(KFontProperties::GlyphRange(rs, ch - 2));
    }
  }
  return ranges;
}

KFontProperties::KFontProperties(const TTF_Font *_Font, const KFile *file) :
fontPath(file->getAbsolutePath().c_str()),
familyName(TTF_FontFaceFamilyName(_Font)),
styleName(TTF_FontFaceStyleName(_Font)),
ttf_Style(TTF_GetFontStyle(_Font)),
height(TTF_FontHeight(_Font)),
ascent(TTF_FontAscent(_Font)),
descent(TTF_FontDescent(_Font)),
lineSkip(TTF_FontLineSkip(_Font)),
faces(TTF_FontFaces(_Font)),
monospace(TTF_FontFaceIsFixedWidth(_Font)),
firstGlyph(getFirstGlyph(_Font)),
lastGlyph(getLastGlyph(_Font)),
totalGlyphs(getTotalGlyphs(_Font)),
glyphRanges(getRanges(_Font)) {

}

KFontProperties::~KFontProperties() {
  auto lock = getLock();
  std::map<std::string, std::shared_ptr < KFontProperties>>::iterator fItr = fontList.find(fontPath);
  if(fItr != fontList.end()) {
    if(fontList[fontPath].get() == this) {
      fontList.erase(fontPath);
    }
  }
}

std::shared_ptr<KFontProperties> KFontProperties::getFontProperties(const std::string fontFile) {
  KFile file(fontFile);
  std::string absFont = file.getAbsolutePath();
  auto lock = getLock();
  auto found = fontList.find(absFont);
  if(found == fontList.end()) {
    lock.unlock();
    if(!KFontProperties::enumerateFont(&file)) {
      return nullptr;
    }
    lock.lock();
  }
  return fontList[absFont];
}

void KFontProperties::clearAllFontProperties() {
  auto lock = getLock();
  fontList.clear();
}

std::string KFontProperties::printProperties() const {
  std::ostringstream ss;
  ss << "  path: " << fontPath << std::endl;
  ss << "  family name: " << familyName << std::endl;
  ss << "  style name:  " << styleName << std::endl;
  ss << "  Fixed width: " << monospace << std::endl;
  return ss.str();
}

std::vector<std::shared_ptr<KFontProperties>> KFontProperties::findFonts(const std::string fnt) {
  std::vector<std::shared_ptr < KFontProperties>> found;
  auto lock = getLock();
  for(auto itr : fontList) {
    std::string p = itr.first;
    std::shared_ptr<KFontProperties> fp = itr.second;
    if(p.find(fnt) != std::string::npos) {
      found.push_back(fp);
    }
  }
  return found;
}

std::vector<std::shared_ptr<KFontProperties>> KFontProperties::findFonts(const std::string fnt, int style) {
  std::vector<std::shared_ptr < KFontProperties>> found;
  auto lock = getLock();
  for(auto itr : fontList) {
    std::string p = itr.first;
    std::shared_ptr<KFontProperties> fp = itr.second;
    if(p.find(fnt) != std::string::npos) {
      if(fp->ttf_Style == style) {
        found.push_back(fp);
      }
    }
  }
  return found;
}

bool KFontProperties::enumerateFont(const KFile *file) {
  if(file->isFile()) {
    if(strToLower(file->getExtension()) == ".ttf") {
      if(!TTF_WasInit()) {
        TTF_Init();
      }
      TTF_Font *_Font = TTF_OpenFont(file->getAbsolutePath().c_str(), 12);
      if(_Font != nullptr) {
        KFontProperties *fp = new KFontProperties(_Font, file);
        // free the font.
        TTF_CloseFont(_Font);
        auto lock = getLock();
        fontList[file->getAbsolutePath()].reset(fp);
        return true;
      }
    }
  }
  return false;
}

void KFontProperties::enumerateFonts(int depth) {
  std::string found;
  for(auto sPath : KApplication::FontPaths()) {
    enumerateDirectory(sPath, depth);
  }
}

void KFontProperties::enumerateDirectory(const std::string sPath, int depth) {
  if(KFile::isDirectory(sPath)) {
    auto dContents = KFile::listFiles(sPath);
    for(auto file : dContents) {
      if(file->isFile()) {
        KFontProperties::enumerateFont(file.get());
      }
      if(file->isDirectory() && depth > 0) {
        enumerateDirectory(file->getName(), depth - 1);
      }
    }
  }
}

int KFontProperties::glyphsInRange(const GlyphRange &range) const {
  int num = 0;

  for(auto rng : glyphRanges) {
    if(rng.start < range.start) {
      if(rng.end >= range.start) {
        int min = (rng.end < range.end) ? rng.end : range.end;
        num += (min - range.start) + 1;
      }
    } else {
      if(rng.start <= range.end) {
        int min = (rng.end < range.end) ? rng.end : range.end;
        num += (min - rng.start) + 1;
      }
    }
  }
  return num;
}

}
