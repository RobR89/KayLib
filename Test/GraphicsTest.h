#ifndef GRAPHICSTEST_H
#define GRAPHICSTEST_H

#include "Graphics/KFont.h"

void testFontProperties()
{
    KFont::addFontPath("c:/windows/fonts");
    KFont::addFontPath("/usr/share/fonts");
    KFont::addFontPath("/usr/share/fonts/truetype");
    KFont::addFontPath("/usr/local/share/fonts");
    KFont::addFontPath("/usr/local/share/fonts/truetype");
    KFont::addFontPath("./Data/freefont");

    std::cout << "Searching font directories..." << std::endl;
    KFontProperties::enumerateFonts(KFont::getFontPaths(), 2, 10);
    std::cout << KFontProperties::countFonts() << " fonts found." << std::endl;
    //std::string fnt = "arial";
    std::string fnt = "sans";
    //std::string fnt = ""; // Get ALL fonts.
    std::vector<std::shared_ptr < KFontProperties>> found = KFontProperties::findFonts(fnt);
    std::cout << "Search: " << found.size() << " fonts found." << std::endl;
    if(found.size() > 0)
    {
        for(uint i = 0; i < found.size(); i++)
        {
            std::shared_ptr < KFontProperties> fp = found[i];
            if(fp->ttf_Style == 0)
            {
                std::cout << "found " << fnt << "in font path name for font." << std::endl;
                std::cout << fp->printProperties("Found: ").c_str();
            }
        }
    }
    else
    {
        std::cout << "Font not found." << std::endl;
    }
    std::cout << "Font search complete." << std::endl << std::endl;

}

#endif /* GRAPHICSTEST_H */

