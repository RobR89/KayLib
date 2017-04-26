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

#ifndef KFILE_H
#define KFILE_H
#include <string>
#include <vector>
#include <memory>
#include "../String/KString.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <vector>
#include <cstring>
#include <algorithm>

#include <iostream>

#include <cstdlib>
#ifdef _WIN32
#else
#include <pwd.h>
#include <bits/basic_string.h>
#endif


namespace KayLib
{

    class KFile
    {
    public:

        KFile()
        {
            fileName = "";
        }

        KFile(const std::string &nFileName)
        {
            if(&nFileName != nullptr)
            {
                fileName = nFileName;
            }
            else
            {
                fileName = "";
            }
        }

        KFile(const KFile &nFile)
        {
            if(&nFile != nullptr)
            {
                fileName = nFile.fileName;
            }
            else
            {
                fileName = "";
            }
        }

        virtual ~KFile() { }

        static std::shared_ptr<KFile> newFile(std::string nFileName)
        {
            return std::make_shared<KFile>(nFileName);
        }

        /**
         * Get the path and filename of this file.
         * @return
         */
        std::string getName() const
        {
            return fileName;
        };

        /**
         * Get the absolute path of this file.
         * @return The absolute path or an empty string on failure.
         */
        std::string getAbsolutePath() const
        {
            return getAbsolutePath(fileName);
        }

        /**
         * Get the absolute path of this file.
         * @param fName The file resolve.
         * @return The absolute path or an empty string on failure.
         */
        static std::string getAbsolutePath(const std::string &fName)
        {
            char absolute[PATH_MAX];
            if(realpath(fName.c_str(), absolute) == nullptr)
            {
                // failed.
                return "";
            }
            return absolute;
        }

        /**
         * Check if this file exists.
         * @return True if it exists.
         */
        bool exists() const
        {
            return exists(fileName);
        }

        /**
         * Check if a specific file exists.
         * @param fName The file to check.
         * @return True if it exists.
         */
        static bool exists(const std::string &fName)
        {
            struct stat fileStat;
            return stat(fName.c_str(), &fileStat) == 0;
        }

        /**
         * Check if this file is a file.
         * @return True if it is a file.
         */
        bool isFile() const
        {
            return isFile(fileName);
        }

        /**
         * Check if a specific file is a file.
         * @param fName The file to check.
         * @return True if it is a file.
         */
        static bool isFile(const std::string &fName)
        {
            struct stat fileStat;
            if(stat(fName.c_str(), &fileStat) != 0)
            {
                return false;
            }
            return S_ISREG(fileStat.st_mode);
        }

        /**
         * Check if this file is a directory.
         * @return True if it is a directory.
         */
        bool isDirectory() const
        {
            return isDirectory(fileName);
        }

        /**
         * Check if a specific file is a directory.
         * @param fName The file to check.
         * @return True if it is a directory.
         */
        static bool isDirectory(const std::string &fName)
        {
            struct stat fileStat;
            if(stat(fName.c_str(), &fileStat) != 0)
            {
                return false;
            }
            return S_ISDIR(fileStat.st_mode);
        }

        /**
         * Check if this file can be read from.
         * @return True if readable.
         */
        bool canRead() const
        {
            return canRead(fileName);
        }

        /**
         * Check if a specific file can be read from.
         * @param fName The file to check.
         * @return True if readable.
         */
        static bool canRead(const std::string &fName)
        {
            if(KFile::exists(fName))
            {
                if(access(fName.c_str(), R_OK) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        /**
         * Check if this file can be written to.
         * @return True if writable.
         */
        bool canWrite() const
        {
            return canWrite(fileName);
        }

        /**
         * Check if a specific file can be written to.
         * @param fName The file to check.
         * @return True if writable.
         */
        static bool canWrite(const std::string &fName)
        {
            if(KFile::exists(fName))
            {
                if(access(fName.c_str(), W_OK) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        /**
         * Check if this file can be executed.
         * @return True if executable.
         */
        bool canExecute() const
        {
            return canExecute(fileName);
        }

        /**
         * Check if a specific file can be executed.
         * @param fName The file to check.
         * @return True if executable.
         */
        static bool canExecute(const std::string &fName)
        {
            if(KFile::exists(fName))
            {
                if(access(fName.c_str(), X_OK) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        /**
         * Get a list of all files and directories in this directory.
         * @return The list of files.
         */
        std::vector<std::shared_ptr<KFile>> listFiles() const
        {
            return listFiles(fileName);
        }

        /**
         * Get a list of all files and directories in this directory.
         * @param fName The directory to search.
         * @return The list of files.
         */
        static std::vector<std::shared_ptr<KFile>> listFiles(const std::string &fName)
        {
            std::vector<std::shared_ptr < KFile>> files;
            std::vector<std::string> fileNames;
            DIR *dir = opendir(fName.c_str());
            if(dir == nullptr)
            {
                return files;
            }
            std::string dirName = fName;
            if(KString::beginsWith(dirName, "./"))
            {
                dirName = dirName.substr(2);
            }
            struct dirent *ent;
            while(ent = readdir(dir))
            {
                if(ent == nullptr)
                {
                    closedir(dir);
                    return files;
                }
                if(KString::endsWith(ent->d_name, "/.") || KString::endsWith(ent->d_name, "/.."))
                {
                    continue;
                }
                if(std::strcmp(ent->d_name, ".") == 0 || std::strcmp(ent->d_name, "..") == 0)
                {
                    continue;
                }
                fileNames.push_back(dirName + "/" + ent->d_name);
            }
            closedir(dir);
            // Sort names.
            std::sort(fileNames.begin(), fileNames.end());
            // Fill the return list.
            files.reserve(fileNames.size());
            for(auto file : fileNames)
            {
                if(!file.empty())
                {
                    files.push_back(std::shared_ptr<KFile>(new KFile(file)));
                }
            }
            return files;
        }

        /**
         * Attempt to make a new directory.
         * @param dirName The directory to make.
         * @return True if successful.
         */
        static bool mkdir(const std::string &dirName)
        {
            if(mkdir(dirName.c_str()) == 0)
            {
                return true;
            }
            return false;
        }

        /**
         * Get the current working directory.
         * @return The current working directory.
         */
        static std::string getCurrentDirectory()
        {
            return getcwd(NULL, 0);
        }

        /**
         * Get the user home directory.
         * @return The user home directory.
         */
        static std::string getHomeDirectory()
        {
#ifdef _WIN32
            std::string homeDrive = getenv("HOMEDRIVE");
            std::string homePath = getenv("HOMEPATH");
            std::string home = homeDrive;
            home += homePath;
            return home;
#else
            auto homeEnv = getenv("HOME");
            if(homeEnv == nullptr)
            {
                auto p = getpwuid(getuid());
                return p->pw_dir;
            }
            return homeEnv;
#endif
        }

        /**
         * Get the current working directory.
         * @return The current working directory.
         */
        static std::shared_ptr<KFile> getCurrentDirectoryFile()
        {
            return newFile(getCurrentDirectory());
        }

        /**
         * Get the user home directory.
         * @return The user home directory.
         */
        static std::shared_ptr<KFile> getHomeDirectoryFile()
        {
            return newFile(getHomeDirectory());
        }

        /**
         * Search backwards from the working directory to find the desired file or directory.
         * @param fileName The file or directory name to find.
         * @param recursion The limit of how far up the tree to search.
         * @return The found file or directory.  Or null if not found.
         */
        static std::shared_ptr<KFile> searchWorkingParent(const std::string &fileName, const int recursion)
        {
            return searchParent("", fileName, recursion);
        }

        /**
         * Search backwards from the specified directory to find the desired file or directory.
         * @param base The directory to search from.
         * @param fileName The file or directory name to find.
         * @param recursion The limit of how far up the tree to search.
         * @return The found file or directory.  Or null if not found.
         */
        static std::shared_ptr<KFile> searchParent(const std::string &base, const std::string &fileName, const int recursion)
        {
            std::shared_ptr<KFile> result;
            if(&fileName == nullptr)
            {
                return result;
            }
            std::string searchPath = base;
            // add trailing slash.
            if(!KString::endsWith(searchPath, "/") && !searchPath.empty())
            {
                searchPath += "/";
            }
            std::string fName = fileName;
            // remove leading slash.
            while(KString::beginsWith(fName, "/") && fName.length() > 1)
            {
                fName = fName.substr(1);
            }
            KFile sFile(searchPath + fName);
            int max = recursion;
            // Recurse the parent directorys till the file is found or the limit reached.
            while(!sFile.exists() && max > 0)
            {
                max--;
                searchPath += "../";
                sFile.fileName = searchPath + fName;
            }
            if(sFile.exists())
            {
                // We found the file.
                return std::make_shared<KFile>(sFile);
            }
            return result;
        }

        /**
         * Search the directory for the file specified.
         * @param dir The directory to search.
         * @param fileName The file to search for.
         * @param recursion The limit of how many child directories to recurse.
         * @return The found file.
         */
        static std::shared_ptr<KFile> searchDirectory(const KFile *dir, const std::string &fileName, const int recursion)
        {
            std::shared_ptr<KFile> result;
            if(&fileName == nullptr || &dir == nullptr)
            {
                return result;
            }
            if(dir->isDirectory() && dir->canRead())
            {
                std::string searchName = dir->fileName + "/" + fileName;
                if(exists(searchName))
                {
                    // We found the file.
                    return std::make_shared<KFile>(searchName);
                }
                // File not in this directory, look in subdirectories.
                if(recursion > 0)
                {
                    for(auto file : dir->listFiles())
                    {
                        if(file->isDirectory())
                        {
                            if(KString::endsWith(file->fileName, "/.") || KString::endsWith(file->fileName, "/.."))
                            {
                                // ignore parent directory and self directory entries.
                                continue;
                            }
                            result = searchDirectory(file.get(), fileName, recursion - 1);
                            if(result)
                            {
                                return result;
                            }
                        }
                    }
                }
            }
            return result;
        }

        /**
         * Get the file path of the file.
         * @return The path of the file not including the final '/'.
         */
        std::string getPath() const
        {
            return getPath(fileName);
        }

        /**
         * Get the file path of the file.
         * @return The path of the file not including the final '/'.
         */
        static std::string getPath(const std::string &fName)
        {
            size_t pos = fName.find_last_of("/\\");
            if(pos != std::string::npos)
            {
                return fName.substr(0, pos);
            }
            return "";
        }

        /**
         * Get the name of the file.
         * @return The name of the file not including the path.
         */
        std::string getFilename() const
        {
            return getFilename(fileName);
        }

        /**
         * Get the name of the file.
         * @return The name of the file not including the path.
         */
        static std::string getFilename(const std::string &fName)
        {
            size_t pos = fName.find_last_of("/\\");
            if(pos != std::string::npos)
            {
                return fName.substr(pos + 1);
            }
            return "";
        }

        /**
         * Get the file extension of the file.
         * @return The files extension string including the period.
         */
        std::string getExtension() const
        {
            return getExtension(fileName);
        }

        /**
         * Get the file extension of the file.
         * @return The files extension string including the period.
         */
        static std::string getExtension(const std::string &fName)
        {
            size_t pos = fName.find_last_of('.');
            if(pos != std::string::npos)
            {
                std::string ext = fName.substr(pos);
                if(ext.find_last_of("/\\") != std::string::npos)
                {
                    return "";
                }
                return ext;
            }
            return "";
        }

        bool operator==(const KFile &file)
        {
            if(&file != nullptr)
            {
                if(getAbsolutePath().compare(file.getAbsolutePath()) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        inline bool operator!=(const KFile& file)
        {
            return !(*this == file);
        }

    private:
        std::string fileName;
    };

} // namespace KayLib

#endif /* FILE_H */

