#include "KFont.h"
#include "../String/KString.h"
#include "../App/KApplication.h"

namespace KayLib {

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
  if(!fntFile.exists()) {
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

SDL_Surface *KFont::drawText(const std::string &text, const TextQuality &quality) {
  return drawText(text, fontColor, backgroundColor, quality);
}

SDL_Surface *KFont::drawText(const std::string &text, const SDL_Color &tColor, const TextQuality &quality) {
  return drawText(text, tColor, backgroundColor, quality);
}

SDL_Surface *KFont::drawText(const std::string &text, const SDL_Color &tColor, const SDL_Color &bColor, const TextQuality &quality) {
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

int KFont::lineLenght(const std::string &text) {
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

int KFont::LineHeight() {
  if(fontProperties.get() == nullptr) {
    return 1;
  }
  return fontProperties->lineSkip;
}

int KFont::getStyle() {
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

bool KFont::isNormal() {
  return (getStyle() & TTF_STYLE_NORMAL) == 0 ? false : true;
}

bool KFont::isBold() {
  return (getStyle() & TTF_STYLE_BOLD) == 0 ? false : true;
}

bool KFont::isItalic() {
  return (getStyle() & TTF_STYLE_ITALIC) == 0 ? false : true;
}

bool KFont::isUnderline() {
  return (getStyle() & TTF_STYLE_UNDERLINE) == 0 ? false : true;
}

bool KFont::isStrikeThrough() {
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

bool KFont::isMonospaced() {
  if(font == nullptr) {
    return false;
  }
  return TTF_FontFaceIsFixedWidth(font);
}

bool KFont::isGlyphProvided(int ch) {
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
  std::vector<std::string> fPaths(KApplication::FontPaths());
  // Search through the font paths.
  for(auto path : fPaths) {
    KFile sPath(path);
    // Yes, is it a directory?
    if(sPath.isDirectory()) {
      // Yes, check for file.
      KFile fPath(sPath.getPath() + "/" + fFile.getFilename());
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