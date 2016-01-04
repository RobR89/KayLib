/*
 * Copyright 2016 Robert Reinhart.
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
#define	KFILE_H
#include <string>
#include <vector>
#include <memory>

namespace KayLib {

class KFile {
public:
  KFile(const std::string &nFileName);
  KFile(const KFile &nFile);
  virtual ~KFile();

  /**
   * Get the filename of this file.
   * @return
   */
  std::string getName() const {
    return fileName;
  };
  /**
   * Get the absolute path of this file.
   * @return The absolute path or an empty string on failure.
   */
  std::string getAbsolutePath() const;
  /**
   * Get the absolute path of this file.
   * @param fName The file resolve.
   * @return The absolute path or an empty string on failure.
   */
  static std::string getAbsolutePath(const std::string &fName);
  /**
   * Check if this file exists.
   * @return True if it exists.
   */
  bool exists() const;
  /**
   * Check if a specific file exists.
   * @param fName The file to check.
   * @return True if it exists.
   */
  static bool exists(const std::string &fName);
  /**
   * Check if this file is a file.
   * @return True if it is a file.
   */
  bool isFile() const;
  /**
   * Check if a specific file is a file.
   * @param fName The file to check.
   * @return True if it is a file.
   */
  static bool isFile(const std::string &fName);
  /**
   * Check if this file is a directory.
   * @return True if it is a directory.
   */
  bool isDirectory() const;
  /**
   * Check if a specific file is a directory.
   * @param fName The file to check.
   * @return True if it is a directory.
   */
  static bool isDirectory(const std::string &fName);
  /**
   * Check if this file can be read from.
   * @return True if readable.
   */
  bool canRead() const;
  /**
   * Check if a specific file can be read from.
   * @param fName The file to check.
   * @return True if readable.
   */
  static bool canRead(const std::string &fName);
  /**
   * Check if this file can be written to.
   * @return True if writable.
   */
  bool canWrite() const;
  /**
   * Check if a specific file can be written to.
   * @param fName The file to check.
   * @return True if writable.
   */
  static bool canWrite(const std::string &fName);
  /**
   * Check if this file can be executed.
   * @return True if executable.
   */
  bool canExecute() const;
  /**
   * Check if a specific file can be executed.
   * @param fName The file to check.
   * @return True if executable.
   */
  static bool canExecute(const std::string &fName);
  /**
   * Get a list of all files and directories in this directory.
   * @return The list of files.
   */
  std::vector<std::shared_ptr<KFile>> listFiles() const;
  bool equals(const KFile &file) const;

  /**
   * Attempt to make a new directory.
   * @param dirName The directory to make.
   * @return True if successful.
   */
  static bool mkdir(const std::string &dirName);
  /**
   * Get the current working directory.
   * @return The current working directory.
   */
  static std::string getCurrentDirectory();
  /**
   * Get the user home directory.
   * @return The user home directory.
   */
  static std::string getHomeDirectory();

  /**
   * Search backwards from the working directory to find the desired file or directory.
   * @param fileName The file or directory name to find.
   * @param recursion The limit of how far up the tree to search.
   * @return The found file or directory.  Or null if not found.
   */
  static KFile *searchWorkingParent(const std::string &fileName, const int recursion);

  /**
   * Search backwards from the specified directory to find the desired file or directory.
   * @param base The directory to search from.
   * @param fileName The file or directory name to find.
   * @param recursion The limit of how far up the tree to search.
   * @return The found file or directory.  Or null if not found.
   */
  static KFile *searchParent(const std::string &base, const std::string &fileName, const int recursion);

  /**
   * Search the directory for the file specified.
   * @param dir The directory to search.
   * @param fileName The file to search for.
   * @param recursion The limit of how many child directories to recurse.
   * @return The found file.
   */
  static KFile *searchDirectory(const KFile *dir, const std::string &fileName, const int recursion);

private:
  std::string fileName;
};

} // namespace KayLib

#endif	/* FILE_H */

