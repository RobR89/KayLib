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
#endif

namespace KayLib {

KFile::KFile(const std::string &nFileName) {
  if(&nFileName != nullptr) {
    fileName = nFileName;
  } else {
    fileName = "";
  }
}

KFile::KFile(const KFile &nFile) {
  if(&nFile != nullptr) {
    fileName = nFile.fileName;
  } else {
    fileName = "";
  }
}

KFile::~KFile() {
}

std::string KFile::getAbsolutePath() const {
  char absolute[PATH_MAX];
  if(realpath(fileName.c_str(), absolute) == nullptr) {
    // failed.
    return "";
  }
  return absolute;
}

std::string KFile::getAbsolutePath(const std::string &fName) {
  char absolute[PATH_MAX];
  if(realpath(fName.c_str(), absolute) == nullptr) {
    // failed.
    return "";
  }
  return absolute;
}

bool KFile::exists() const {
  struct stat fileStat;
  return stat(fileName.c_str(), &fileStat) == 0;
}

bool KFile::exists(const std::string &fName) {
  struct stat fileStat;
  return stat(fName.c_str(), &fileStat) == 0;
}

bool KFile::isFile() const {
  struct stat fileStat;
  if(stat(fileName.c_str(), &fileStat) != 0) {
    return false;
  }
  return S_ISREG(fileStat.st_mode);
}

bool KFile::isFile(const std::string &fName) {
  struct stat fileStat;
  if(stat(fName.c_str(), &fileStat) != 0) {
    return false;
  }
  return S_ISREG(fileStat.st_mode);
}

bool KFile::isDirectory() const {
  struct stat fileStat;
  if(stat(fileName.c_str(), &fileStat) != 0) {
    return false;
  }
  return S_ISDIR(fileStat.st_mode);
}

bool KFile::isDirectory(const std::string &fName) {
  struct stat fileStat;
  if(stat(fName.c_str(), &fileStat) != 0) {
    return false;
  }
  return S_ISDIR(fileStat.st_mode);
}

bool KFile::canRead() const {
  if(exists()) {
    if(access(fileName.c_str(), R_OK) == 0) {
      return true;
    }
  }
  return false;
}

bool KFile::canRead(const std::string &fName) {
  if(KFile::exists(fName)) {
    if(access(fName.c_str(), R_OK) == 0) {
      return true;
    }
  }
  return false;
}

bool KFile::canWrite() const {
  if(exists()) {
    if(access(fileName.c_str(), W_OK) == 0) {
      return true;
    }
  }
  return false;
}

bool KFile::canWrite(const std::string &fName) {
  if(KFile::exists(fName)) {
    if(access(fName.c_str(), W_OK) == 0) {
      return true;
    }
  }
  return false;
}

bool KFile::canExecute() const {
  if(exists()) {
    if(access(fileName.c_str(), X_OK) == 0) {
      return true;
    }
  }
  return false;
}

bool KFile::canExecute(const std::string &fName) {
  if(KFile::exists(fName)) {
    if(access(fName.c_str(), X_OK) == 0) {
      return true;
    }
  }
  return false;
}

std::vector<std::shared_ptr<KFile>> KFile::listFiles() const {
  std::vector<std::shared_ptr < KFile>> files;
  std::vector<std::string> fileNames;
  DIR *dir = opendir(fileName.c_str());
  if(dir == nullptr) {
    return files;
  }
  std::string dirName = fileName;
  if(beginsWith(dirName, "./")) {
    dirName = dirName.substr(2);
  }
  struct dirent *ent;
  while(ent = readdir(dir)) {
    if(ent == nullptr) {
      closedir(dir);
      return files;
    }
    if(endsWith(ent->d_name, "/.") || endsWith(ent->d_name, "/..")) {
      continue;
    }
    if(std::strcmp(ent->d_name, ".") == 0 || std::strcmp(ent->d_name, "..") == 0) {
      continue;
    }
    fileNames.push_back(dirName + "/" + ent->d_name);
  }
  closedir(dir);
  // Sort names.
  std::sort(fileNames.begin(), fileNames.end());
  // Fill the return list.
  files.reserve(fileNames.size());
  for(auto file : fileNames) {
    if(!file.empty()) {
      files.push_back(std::shared_ptr<KFile>(new KFile(file)));
    }
  }
  return files;
}

std::vector<std::shared_ptr<KFile>> KFile::listFiles(const std::string &fName) {
  KFile file(fName);
  if(file.isDirectory()) {
    return file.listFiles();
  }
  return std::vector<std::shared_ptr < KFile >> ();
}

bool KFile::mkdir(const std::string &dirName) {
  if(mkdir(dirName.c_str()) == 0) {
    return true;
  }
  return false;
}

std::string KFile::getCurrentDirectory() {
  return getcwd(NULL, 0);
}

std::string KFile::getHomeDirectory() {
#ifdef _WIN32
  auto homeDrive = getenv("HOMEDRIVE");
  auto homePath = getenv("HOMEPATH");
  std::string home = homeDrive;
  home += homePath;
  return home;
#else
  auto homeEnv = getenv("HOME");
  if(homeEnv == nullptr) {
    auto p = getpwuid(getuid());
    return p->pw_dir;
  }
  return homeEnv;
#endif
}

KFile *KFile::searchWorkingParent(const std::string &fileName, const int recursion) {
  return searchParent("", fileName, recursion);
}

KFile *KFile::searchParent(const std::string &base, const std::string &fileName, const int recursion) {
  if(&fileName == nullptr) {
    return nullptr;
  }
  std::string searchPath = base;
  // remove leading slash.
  if(!endsWith(searchPath, "/") && !searchPath.empty()) {
    searchPath += "/";
  }
  std::string fName = fileName;
  // remove leading slash.
  while(beginsWith(fName, "/") && fName.length() > 1) {
    fName = fName.substr(1);
  }
  KFile dir(searchPath + fName);
  int max = recursion;
  while(!dir.exists() && max > 0) {
    max--;
    searchPath += "../";
    dir.fileName = searchPath + fName;
  }
  if(dir.exists()) {
    return new KFile(dir);
  }
  return nullptr;
}

KFile *KFile::searchDirectory(const KFile *dir, const std::string &fileName, const int recursion) {
  if(&fileName == nullptr || &dir == nullptr) {
    return nullptr;
  }
  if(dir->isDirectory() && dir->canRead()) {
    std::string searchName = dir->fileName + "/" + fileName;
    for(auto file : dir->listFiles()) {
      if(file->isDirectory()) {
        if(recursion > 0) {
          if(endsWith(file->fileName, "/.") || endsWith(file->fileName, "/..")) {
            // ignore parent directory and self directory entries.
            continue;
          }
          KFile *result = searchDirectory(file.get(), fileName, recursion - 1);
          if(result != nullptr) {
            return result;
          }
        }
      } else {
        if(file->getName().compare(searchName) == 0) {
          return new KFile(*file.get());
        }
      }
    }
  }
  return nullptr;
}

std::string KFile::getPath() const {
  size_t pos = fileName.find_last_of("/\\");
  if(pos != std::string::npos) {
    return fileName.substr(0, pos);
  }
  return "";
}

std::string KFile::getFilename() const {
  size_t pos = fileName.find_last_of("/\\");
  if(pos != std::string::npos) {
    return fileName.substr(pos + 1);
  }
  return "";
}

std::string KFile::getExtension() const {
  size_t pos = fileName.find_last_of('.');
  if(pos != std::string::npos) {
    return fileName.substr(pos);
  }
  return "";
}

bool KFile::equals(const KFile &file) const {
  if(&file != nullptr) {
    if(fileName.compare(file.fileName) == 0) {
      return true;
    }
  }
  return false;
}

} // namespace KayLib
