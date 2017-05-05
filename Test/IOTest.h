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

//-------------------------------------------------------------------------
// Checksum tests

#include "IO/KChecksum.h"

/**
 * Perform standard tests.
 * @param type Checksum type
 * @param out Output stream for results. (can be null)
 * @return True if successful.
 */
bool testChecksum(const KChecksumType type, std::ostringstream *out)
{
    /*
     * these are the standard FIPS-180-2 test vectors
     */

    static constexpr char const *Test_msg[] = {"abc",
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", NULL};

    static constexpr char const *SHA256_Results[] = {
        "BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD",
        "248D6A61D20638B8E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1",
        "CDC76E5C9914FB9281A1C7E284D73E67F1809A48A497200E046D39CCC7112CD0"
    };

    static constexpr char const *SHA1_Results[] = {"A9993E364706816ABA3E25717850C26C9CD0D89D",
        "84983E441C3BD26EBAAE4AA1F95129E5E54670F1",
        "34AA973CD4C4DAA4F61EEB2BDBAD27316534016F"};

    static constexpr char const *MD5_Results[] = {"900150983CD24FB0D6963F7D28E17F72",
        "8215EF0796A20BCAAAE116D3876C664A", "7707D6AE4E027C70EEA2A935C2296F21"};

    int i, j;
    unsigned char buf[1000];

    if(out != nullptr)
    {
        switch(type)
        {
            case MD5:
                *out << "MD5 ";
                break;
            case SHA1:
                *out << "SHA-1 ";
                break;
            default:
                *out << "SHA-256 ";
                break;
        }
        *out << "Validation Tests:" << std::endl << std::endl;
    }

    for(i = 0; i < 3; i++)
    {
        if(out != nullptr)
        {
            *out << "  Test " << i + 1 << ": ";
        }
        KChecksum ck(type);

        if(i < 2)
        {
            const char *msg = Test_msg[i];
            ck.add(msg, ::strlen(Test_msg[i]));
        }
        else
        {
            ::memset(buf, 'a', 1000);

            for(j = 0; j < 1000; j++)
            {
                ck.add(buf, 1000);
            }
        }

        std::string hash = ck.getHashString();
        std::string desired;
        switch(type)
        {
            case MD5:
                desired = MD5_Results[i];
                break;
            case SHA1:
                desired = SHA1_Results[i];
                break;
            default:
                desired = SHA256_Results[i];
                break;
        }
        if(hash.compare(desired) != 0)
        {
            if(out != nullptr)
            {
                *out << "failed!" << std::endl;
                *out << "(" << hash << ")" << std::endl;
                *out << "  should be" << std::endl << "(" << desired << ")" << std::endl;
            }
            return false;
        }

        if(out != nullptr)
        {
            *out << "passed." << std::endl;
            *out << "(" << hash << ")" << std::endl << std::endl;
        }
    }

    return true;
}

/**
 * Perform standard tests.
 * @param out Output stream for results. (can be null)
 * @return True if successful.
 */
bool testChecksum(std::ostringstream *out)
{
    if(!testChecksum(MD5, out) || !testChecksum(SHA1, out)
       || !testChecksum(SHA256, out))
    {
        if(out != nullptr)
        {
            *out << "Some tests failed." << std::endl;
        }
        return false;
    }
    if(out != nullptr)
    {
        *out << "All tests passed." << std::endl;
    }
    return true;
}

void testChecksum()
{
    // run checksum test
    KChecksum ck(SHA256);
    std::ostringstream ss;
    testChecksum(&ss);
    std::cout << ss.str();
}

#endif /* IO_H */

