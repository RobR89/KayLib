#include "KFontProperties.h"
#include "../String/KString.h"
#include "../App/KApplication.h"

#include <sstream>
#include <mutex>

// include SDL headers
#include <SDL2/SDL_ttf.h>
#include <bits/stl_map.h>

namespace KayLib {

std::map<std::string, std::shared_ptr<KFontProperties>> fontList;

std::mutex lockPtr;

std::unique_lock<std::mutex> getLock() {
  static std::mutex lockPtr;
  return std::unique_lock<std::mutex>(lockPtr);
}

KFontProperties::KFontProperties(std::string font) {
  if(!TTF_WasInit()) {
    TTF_Init();
  }
  TTF_Font *_Font = nullptr;
  _Font = TTF_OpenFont(font.c_str(), 12);
  if(_Font == nullptr) {
    return;
  }
  KFile file(font);
  fontPath = file.getAbsolutePath();

  // get font properties.
  familyName = TTF_FontFaceFamilyName(_Font);
  styleName = TTF_FontFaceStyleName(_Font);
  ttf_Style = TTF_GetFontStyle(_Font);
  height = TTF_FontHeight(_Font);
  ascent = TTF_FontAscent(_Font);
  descent = TTF_FontDescent(_Font);
  lineSkip = TTF_FontLineSkip(_Font);
  monospace = false;
  if(TTF_FontFaceIsFixedWidth(_Font))
    monospace = true;
  faces = TTF_FontFaces(_Font);

  // find glyph ranges.
  int ch = 0;
  totalGlyphs = 0;
  while(ch < 0x10000) {
    if(TTF_GlyphIsProvided(_Font, ch)) {
      if(glyphRanges.size() == 0) {
        firstGlyph = ch;
      }
      GlyphRange range;
      range.start = ch;
      while(TTF_GlyphIsProvided(_Font, ch)) {
        ch++;
      }
      range.end = ch - 1;
      lastGlyph = range.end;
      totalGlyphs += (range.end - range.start) + 1;
      glyphRanges.push_back(range);
    }
    ch++;
  }

  // free the font.
  TTF_CloseFont(_Font);
}

KFontProperties::~KFontProperties() {
  std::map<std::string, std::shared_ptr < KFontProperties>>::iterator fItr = fontList.find(fontPath);
  if(fItr != fontList.end()) {
    if(fontList[fontPath].get() == this) {
      fontList.erase(fontPath);
    }
  }
}

std::shared_ptr<KFontProperties> KFontProperties::getFontProperties(std::string font) {
  KFile file(font);
  std::string absFont = file.getAbsolutePath();
  auto found = fontList.find(absFont);
  if(found == fontList.end()) {
    if(!KFontProperties::enumerateFont(&file)) {
      return nullptr;
    }
  }
  return fontList[absFont];
}

void KFontProperties::clearAllFontProperties() {
  fontList.clear();
}

std::string KFontProperties::printProperties() {
  std::ostringstream ss;
  ss << "  path: " << fontPath << std::endl;
  ss << "  family name: " << familyName << std::endl;
  ss << "  style name:  " << styleName << std::endl;
  ss << "  Fixed width: " << monospace << std::endl;
  return ss.str();
}

std::vector<std::shared_ptr<KFontProperties>> KFontProperties::findFonts(std::string fnt) {
  std::vector<std::shared_ptr < KFontProperties>> found;
  for(auto itr : fontList) {
    std::string p = itr.first;
    std::shared_ptr<KFontProperties> fp = itr.second;
    if(p.find(fnt) != std::string::npos) {
      found.push_back(fp);
    }
  }
  return found;
}

std::vector<std::shared_ptr<KFontProperties>> KFontProperties::findFonts(std::string fnt, int style) {
  std::vector<std::shared_ptr < KFontProperties>> found;
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
      KFontProperties *fp = new KFontProperties(file->getAbsolutePath());
      fontList[file->getAbsolutePath()].reset(fp);
      return true;
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

int KFontProperties::glyphsInRange(const GlyphRange range) {
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
