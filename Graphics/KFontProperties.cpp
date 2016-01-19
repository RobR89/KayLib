#include "KFontProperties.h"
#include "../String/KString.h"
#include "../App/KApplication.h"

#include <sstream>
#include <mutex>
#include <map>

namespace KayLib {

std::map<std::string, std::map<int, std::shared_ptr<KFontProperties>>> fontList;

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

KFontProperties::KFontProperties(const TTF_Font *_Font, const KFile *file, int ptSize) :
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
glyphRanges(getRanges(_Font)),
pointSize(ptSize) {

}

KFontProperties::KFontProperties(const KFontProperties& orig) :
fontPath(orig.fontPath),
familyName(orig.familyName),
styleName(orig.styleName),
ttf_Style(orig.ttf_Style),
height(orig.height),
ascent(orig.ascent),
descent(orig.descent),
lineSkip(orig.lineSkip),
faces(orig.faces),
monospace(orig.monospace),
firstGlyph(orig.firstGlyph),
lastGlyph(orig.lastGlyph),
totalGlyphs(orig.totalGlyphs),
glyphRanges(orig.glyphRanges),
pointSize(orig.pointSize) {

}

KFontProperties::~KFontProperties() {
  auto lock = getLock();
  if(fontList.find(fontPath) != fontList.end()) {
    if(fontList[fontPath].find(pointSize) != fontList[fontPath].end()) {
      if(fontList[fontPath][pointSize].get() == this) {
        fontList[fontPath].erase(pointSize);
      }
    }
  }
}

std::shared_ptr<KFontProperties> KFontProperties::create(const std::string &fontFile, int ptSize) {
  KFile file(fontFile);
  return create(file, ptSize);
}

std::shared_ptr<KFontProperties> KFontProperties::create(const KFile &file, int ptSize) {
  std::string absFont = file.getAbsolutePath();
  auto lock = getLock();
  if(fontList.find(absFont) != fontList.end()) {
    if(fontList[absFont].find(ptSize) != fontList[absFont].end()) {
      return fontList[absFont][ptSize];
    }
  }
  lock.unlock();
  if(!KFontProperties::enumerateFont(&file, ptSize)) {
    return std::shared_ptr<KFontProperties>();
  }
  lock.lock();
  return fontList[absFont][ptSize];
}

void KFontProperties::clearGlobalFontProperties() {
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

std::vector<std::shared_ptr<KFontProperties>> KFontProperties::findFonts(const std::string &fnt) {
  std::vector<std::shared_ptr < KFontProperties>> found;
  auto lock = getLock();
  for(auto itr : fontList) {
    if(itr.first.find(fnt) != std::string::npos) {
      for(auto points : itr.second) {
        std::shared_ptr<KFontProperties> fp = points.second;
        found.push_back(fp);
      }
    }
  }
  return found;
}

std::vector<std::shared_ptr<KFontProperties>> KFontProperties::findFonts(const std::string &fnt, int style) {
  std::vector<std::shared_ptr < KFontProperties>> found;
  auto lock = getLock();
  for(auto itr : fontList) {
    if(itr.first.find(fnt) != std::string::npos) {
      for(auto points : itr.second) {
        std::shared_ptr<KFontProperties> fp = points.second;
        if(fp->ttf_Style == style) {
          found.push_back(fp);
        }
      }
    }
  }
  return found;
}

bool KFontProperties::enumerateFont(const KFile *file, int ptSize) {
  if(file->isFile()) {
    if(strToLower(file->getExtension()) == ".ttf") {
      if(!TTF_WasInit()) {
        TTF_Init();
      }
      TTF_Font *_Font = TTF_OpenFont(file->getAbsolutePath().c_str(), ptSize);
      if(_Font != nullptr) {
        KFontProperties *fp = new KFontProperties(_Font, file, ptSize);
        // free the font.
        TTF_CloseFont(_Font);
        auto lock = getLock();
        fontList[file->getAbsolutePath()][ptSize].reset(fp);
        return true;
      }
    }
  }
  return false;
}

void KFontProperties::enumerateFonts(int depth, int ptSize) {
  std::string found;
  for(auto sPath : KApplication::FontPaths()) {
    enumerateDirectory(sPath, depth, ptSize);
  }
}

void KFontProperties::enumerateDirectory(const std::string sPath, int depth, int ptSize) {
  if(KFile::isDirectory(sPath)) {
    auto dContents = KFile::listFiles(sPath);
    for(auto file : dContents) {
      if(file->isFile()) {
        KFontProperties::enumerateFont(file.get(), ptSize);
      }
      if(file->isDirectory() && depth > 0) {
        enumerateDirectory(file->getName(), depth - 1, ptSize);
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
