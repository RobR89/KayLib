#include "KFile.h"
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

    KFile::KFile()
    {
        fileName = "";
    }

    KFile::KFile(const std::string &nFileName)
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

    KFile::KFile(const KFile &nFile)
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

    KFile::~KFile() { }

    std::string KFile::getAbsolutePath() const
    {
        char absolute[PATH_MAX];
        if(realpath(fileName.c_str(), absolute) == nullptr)
        {
            // failed.
            return "";
        }
        return absolute;
    }

    std::string KFile::getAbsolutePath(const std::string &fName)
    {
        char absolute[PATH_MAX];
        if(realpath(fName.c_str(), absolute) == nullptr)
        {
            // failed.
            return "";
        }
        return absolute;
    }

    bool KFile::exists() const
    {
        struct stat fileStat;
        return stat(fileName.c_str(), &fileStat) == 0;
    }

    bool KFile::exists(const std::string &fName)
    {
        struct stat fileStat;
        return stat(fName.c_str(), &fileStat) == 0;
    }

    bool KFile::isFile() const
    {
        struct stat fileStat;
        if(stat(fileName.c_str(), &fileStat) != 0)
        {
            return false;
        }
        return S_ISREG(fileStat.st_mode);
    }

    bool KFile::isFile(const std::string &fName)
    {
        struct stat fileStat;
        if(stat(fName.c_str(), &fileStat) != 0)
        {
            return false;
        }
        return S_ISREG(fileStat.st_mode);
    }

    bool KFile::isDirectory() const
    {
        struct stat fileStat;
        if(stat(fileName.c_str(), &fileStat) != 0)
        {
            return false;
        }
        return S_ISDIR(fileStat.st_mode);
    }

    bool KFile::isDirectory(const std::string &fName)
    {
        struct stat fileStat;
        if(stat(fName.c_str(), &fileStat) != 0)
        {
            return false;
        }
        return S_ISDIR(fileStat.st_mode);
    }

    bool KFile::canRead() const
    {
        if(exists())
        {
            if(access(fileName.c_str(), R_OK) == 0)
            {
                return true;
            }
        }
        return false;
    }

    bool KFile::canRead(const std::string &fName)
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

    bool KFile::canWrite() const
    {
        if(exists())
        {
            if(access(fileName.c_str(), W_OK) == 0)
            {
                return true;
            }
        }
        return false;
    }

    bool KFile::canWrite(const std::string &fName)
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

    bool KFile::canExecute() const
    {
        if(exists())
        {
            if(access(fileName.c_str(), X_OK) == 0)
            {
                return true;
            }
        }
        return false;
    }

    bool KFile::canExecute(const std::string &fName)
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

    std::vector<std::shared_ptr<KFile>> KFile::listFiles() const
    {
        std::vector<std::shared_ptr < KFile>> files;
        std::vector<std::string> fileNames;
        DIR *dir = opendir(fileName.c_str());
        if(dir == nullptr)
        {
            return files;
        }
        std::string dirName = fileName;
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

    std::vector<std::shared_ptr<KFile>> KFile::listFiles(const std::string &fName)
    {
        KFile file(fName);
        if(file.isDirectory())
        {
            return file.listFiles();
        }
        return std::vector<std::shared_ptr < KFile >> ();
    }

    bool KFile::mkdir(const std::string &dirName)
    {
        if(mkdir(dirName.c_str()) == 0)
        {
            return true;
        }
        return false;
    }

    std::string KFile::getCurrentDirectory()
    {
        return getcwd(NULL, 0);
    }

    std::string KFile::getHomeDirectory()
    {
#ifdef _WIN32
        auto homeDrive = getenv("HOMEDRIVE");
        auto homePath = getenv("HOMEPATH");
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

    std::shared_ptr<KFile> KFile::searchWorkingParent(const std::string &fileName, const int recursion)
    {
        return searchParent("", fileName, recursion);
    }

    std::shared_ptr<KFile> KFile::searchParent(const std::string &base, const std::string &fileName, const int recursion)
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

    std::shared_ptr<KFile> KFile::searchDirectory(const KFile *dir, const std::string &fileName, const int recursion)
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

    std::string KFile::getPath() const
    {
        size_t pos = fileName.find_last_of("/\\");
        if(pos != std::string::npos)
        {
            return fileName.substr(0, pos);
        }
        return "";
    }

    std::string KFile::getFilename() const
    {
        size_t pos = fileName.find_last_of("/\\");
        if(pos != std::string::npos)
        {
            return fileName.substr(pos + 1);
        }
        return "";
    }

    std::string KFile::getExtension() const
    {
        size_t pos = fileName.find_last_of('.');
        if(pos != std::string::npos)
        {
            std::string ext = fileName.substr(pos);
            if(ext.find_last_of("/\\") != std::string::npos)
            {
                return "";
            }
            return ext;
        }
        return "";
    }

    bool KFile::operator==(const KFile &file)
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

}
