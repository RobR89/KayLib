/*
 * Copyright 2017 Robert Reinhart.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IO_H
#define IO_H

#include <iostream>

//-------------------------------------------------------------------------
// String parser tests

#include "IO/KFile.h"

using namespace KayLib;

void testKFile()
{
    std::cout << "KFile tests started." << std::endl;
    std::cout << "Directory search test." << std::endl;
    std::cout << "Home directory: " << KFile::getHomeDirectory() << std::endl;
    std::cout << "Current directory: " << KFile::getCurrentDirectory() << std::endl;
    std::cout << "Absolute path for empty string: " << KFile::getAbsolutePath("") << std::endl;
    std::cout << "Absolute path for \"./\": " << KFile::getAbsolutePath("./") << std::endl;
    std::cout << "Attempting to find KayLib directory in parent tree of working directory." << std::endl;
    // Search back through parent directories for the "KayLib" directory.
    std::shared_ptr<KFile> klDir = KFile::searchWorkingParent("KayLib", 6);
    if(!klDir)
    {
        std::cout << "Unable to find KayLib in parent of working directory, trying home directory." << std::endl;
        
        klDir = KFile::searchDirectory(KFile::getHomeDirectoryFile().get(), "KayLib", 6);
    }
    if(klDir)
    {
        std::cout << "Found KayLib directory at: " << klDir->getName() << std::endl;
        std::cout << "Absolute path for KayLib: " << klDir->getAbsolutePath() << std::endl;
        std::cout << "Attempting to find ReadMe.md" << std::endl;
        std::shared_ptr<KFile> rmFile = KFile::searchDirectory(klDir.get(), "ReadMe.md", 5);
        if(rmFile)
        {
            std::cout << "Found ReadMe.md at " << rmFile->getName() << std::endl;
            std::cout << "Absolute path for ReadMe.md: " << rmFile->getAbsolutePath() << std::endl;
        }
        else
        {
            std::cout << "unable to find ReadMe.md" << std::endl;
        }
        std::cout << "Attempting to find KFile.h" << std::endl;
        rmFile = KFile::searchDirectory(klDir.get(), "KFile.h", 5);
        if(rmFile)
        {
            std::cout << "Found KFile.h at " << rmFile->getName() << std::endl;
            std::cout << "Absolute path for KFile.h: " << rmFile->getAbsolutePath() << std::endl;
        }
        else
        {
            std::cout << "unable to find KFile.h" << std::endl;
        }
        std::cout << "Listing files in KayLib directory." << std::endl;
        auto files = klDir->listFiles();
        for(auto file : files)
        {
            std::cout << "Found: " << file->getName().c_str() << std::endl;
        }
    }
    else
    {
        std::cout << "Unable to find KayLib directory." << std::endl;
    }
    std::cout << "KFile tests complete." << std::endl;
    std::cout << std::endl;
}


#endif /* IO_H */

