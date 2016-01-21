#include "KFont.h"
#include "../String/KString.h"

#include <mutex>
#include <algorithm>

namespace KayLib {

/**
 * A list of paths to search when trying to load fonts.
 */
std::vector<KFile> fontPaths;

std::mutex lockPtr;

std::unique_lock<std::mutex> getLock() {
  static std::mutex lockPtr;
  return std::unique_lock<std::mutex>(lockPtr);
}

KFile findFont(const std::string font);
std::shared_ptr<KFont> KFont::defaultFont;

KFont::KFont(const std::shared_ptr<KFontProperties> prop) : fontProperties(prop) {
  font = nullptr;
  fontColor = {0, 0, 0};
  backgroundColor = {255, 255, 255};
  quality = BLENDED;
}

KFont::~KFont() {
  if(font != nullptr) {
    TTF_CloseFont(font);
  }
}

std::shared_ptr<KFont> KFont::create(const std::string &fontFile, int ptSize) {
  KFile fntFile = findFont(fontFile);
  if(!fntFile.isFile()) {
    return defaultFont;
  }
  TTF_Font *font = TTF_OpenFont(fntFile.getAbsolutePath().c_str(), ptSize);
  if(font == nullptr) {
    return defaultFont;
  }
  std::shared_ptr<KFont> nFont(new KFont(KFontProperties::create(fntFile, ptSize)));
  if(defaultFont.get() == nullptr) {
    defaultFont = nFont;
  }
  return nFont;
}

bool KFont::addFontPath(const std::string &sPath) {
  KFile path(sPath);
  return addFontPath(path);
}

bool KFont::addFontPath(const KFile &path) {
  auto lock = getLock();
  // Is it a directory?
  if(path.isDirectory()) {
    // yes, is the path already registered?
    if(std::find(fontPaths.begin(), fontPaths.end(), path) != fontPaths.end()) {
      return true;
    }
    // no, register it.
    fontPaths.push_back(path);
    return true;
  }
  return false;
}

std::vector<KFile> KFont::getFontPaths() {
  auto lock = getLock();
  return std::vector<KFile>(fontPaths);
}

SDL_Surface *KFont::drawText(const std::string &text, const TextQuality &quality) const {
  return drawText(text, fontColor, backgroundColor, quality);
}

SDL_Surface *KFont::drawText(const std::string &text, const SDL_Color &tColor, const TextQuality &quality) const {
  return drawText(text, tColor, backgroundColor, quality);
}

SDL_Surface *KFont::drawText(const std::string &text, const SDL_Color &tColor, const SDL_Color &bColor, const TextQuality &quality) const {
  SDL_Surface *resulting_text;
  TextQuality useQuality = quality;
  if(useQuality == DEFAULT) {
    useQuality = quality;
  }

  if(useQuality == SOLID) {
    resulting_text = TTF_RenderUTF8_Solid(font, text.c_str(), tColor);
  } else if(useQuality == SHADED) {
    resulting_text = TTF_RenderUTF8_Shaded(font, text.c_str(), tColor, bColor);
  } else if(useQuality == BLENDED) {
    resulting_text = TTF_RenderUTF8_Blended(font, text.c_str(), tColor);
  }

  return resulting_text;
}

int KFont::lineLenght(const std::string &text) const {
  if(font == nullptr) {
    return 0;
  }
  int w, h;
  int r = TTF_SizeText(font, text.c_str(), &w, &h);
  if(r != 0) {
    return -1;
  }
  return w;
}

int KFont::LineHeight() const {
  if(fontProperties.get() == nullptr) {
    return 1;
  }
  return fontProperties->lineSkip;
}

int KFont::getStyle() const {
  if(font == nullptr) {
    return 0;
  }
  return TTF_GetFontStyle(font);
}

void KFont::setStyle(int style) {
  if(font == nullptr) {
    return;
  }
  TTF_SetFontStyle(font, style);
}

bool KFont::isNormal() const {
  return (getStyle() & TTF_STYLE_NORMAL) == 0 ? false : true;
}

bool KFont::isBold() const {
  return (getStyle() & TTF_STYLE_BOLD) == 0 ? false : true;
}

bool KFont::isItalic() const {
  return (getStyle() & TTF_STYLE_ITALIC) == 0 ? false : true;
}

bool KFont::isUnderline() const {
  return (getStyle() & TTF_STYLE_UNDERLINE) == 0 ? false : true;
}

bool KFont::isStrikeThrough() const {
  return (getStyle() & TTF_STYLE_STRIKETHROUGH) == 0 ? false : true;
}

void KFont::setNormal() {
  setStyle(0);
}

void KFont::setBold() {
  setStyle(getStyle() | TTF_STYLE_BOLD);
}

void KFont::setItalic() {
  setStyle(getStyle() | TTF_STYLE_ITALIC);
}

void KFont::setUnderline() {
  setStyle(getStyle() | TTF_STYLE_UNDERLINE);
}

void KFont::setStrikeThrough() {
  setStyle(getStyle() | TTF_STYLE_STRIKETHROUGH);
}

bool KFont::isMonospaced() const {
  if(font == nullptr) {
    return false;
  }
  return TTF_FontFaceIsFixedWidth(font);
}

bool KFont::isGlyphProvided(int ch) const {
  if(font == nullptr) {
    return false;
  }
  return TTF_GlyphIsProvided(font, ch);
}

//----------------------------------------------------------
// font support functions

KFile findFont(const std::string font) {
  KFile fFile(font);
  // check for TTF file extension.
  std::string ext = fFile.getExtension();
  if(ext.length() > 0) {
    if(strToLower(fFile.getExtension()) != ".ttf") {
      return KFile();
    }
  } else {
    fFile = KFile(fFile.getName() + ".ttf");
  }
  // if the font name was specified with a full path then accept it.
  if(fFile.exists()) {
    return fFile;
  }
  auto lock = getLock();
  // Search through the font paths.
  for(auto path : fontPaths) {
    // Is it a directory?
    if(path.isDirectory()) {
      // Yes, check for file.
      KFile fPath(path.getPath() + "/" + fFile.getFilename());
      if(fPath.exists()) {
        // file exists!  Return the result.
        return fPath;
      }
    }
  }
  // font not found return empty path.
  return KFile();
}

}