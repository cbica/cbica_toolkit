/**
\file  cbicaUtilities.h

\brief Some basic utility functions.

https://www.cbica.upenn.edu/sbia/software/ <br>
sbia-software@uphs.upenn.edu

\author Sarthak Pati

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <cmath>
//#include <type_traits>

/**
\struct CSVDict

\brief A Dictionary structure to for CSV parsing

The CSV File in question is basically a list file consisting of file names (or paths) and labels
*/
struct CSVDict
{
  //! Contains input image file names
  std::vector< std::string > inputImages;

  //! Contains Labels that correspond to each file
  std::vector< double > inputLabels;

  //! Constructor
  CSVDict(const CSVDict &origin) :
    inputImages(origin.inputImages), inputLabels(origin.inputLabels)
  { };

  //! Constructor
  CSVDict()
  {
    inputImages.resize(0);
    inputLabels.resize(0);
  }

  //! Constructor
  CSVDict(const std::vector< std::string > &inputImagesVector, const std::vector< double > &inputLabelVector) :
    inputImages(inputImagesVector), inputLabels(inputLabelVector) {};
};

namespace cbica
{
  //======================================== OS stuff ======================================//

  /**
  \brief Check if file exists using istream

  \param fName Filename to check
  \return True if file exists
  */
  bool fileExists(const std::string &fName);

  /**
  \brief Check if directory exists

  \param dName String to check
  \return True if directory exists
  */
  bool directoryExists(const std::string &dName);

  /**
  \brief Return True if path is an existing regular file

  Reimplementation for python's "os.path.isfile": This follows symbolic links,
  so both islink() and isfile() can be true for the same path.

  [Wrap of cbica::fileExists()]

  \param path Filename of file to check

  \return True if path is an existing regular file
  */
  bool isFile(const std::string &path);

  /**
  \brief Return True if path is an existing directory

  Reimplementation of python's "os.path.isdir": This follows symbolic links,
  so both islink() and isdir() can be true for the same path.

  [Wrap of cbica::directoryExists()]

  \param path Directory name to check

  \return True of path is an existing directory
  */
  bool isDir(const std::string &path);

  /**
  \brief Return True if path exists and false for broken symbolic links

  Reimplementation of python's "os.path.exists": On some platforms, this function may return False
  if permission is not granted to execute os.stat() on the requested file, even if the
  path physically exists.

  \param path Path to check

  \return True if path is valid and false for broken symbolic links
  */
  bool exists(const std::string &path);

  /**
  \brief Create a temporary directory

  \param returnDir Path of temporary directory

  \return True if success
  */
  bool createTmpDir(std::string &returnDir);

  /**
  \brief Create a temporary directory

  Wrap for createTmpDir()

  \return True if success
  */
  bool createTemporaryDirectory(std::string &returnDir);

  /**
  \brief Create a temporary directory

  Wrap for createTmpDir()

  \return True if success
  */
  bool makeTemporaryDirectory(std::string &returnDir);

  /**
  \brief Create a temporary directory

  Wrap for createTmpDir()

  \return True if success
  */
  bool makeTempDir(std::string &returnDir);

  /**
  \brief Create a directory

  \param dir_name Name of directory to be created with full path

  \return True if success
  */
  bool createDir(const std::string &dir_name);

  /**
  \brief Create a directory

  \param dir_name Name of directory to be created with full path
  */
  bool makeDir(const std::string &dir_name);

  /**
  \brief Recursively delete a folder and contents [internal function]

  \param dirname Folder to delete

  \return true for success
  */
  int removeDirectoryRecursively(const std::string &dirname, bool bDeleteSubdirectories);

  /**
  \brief Delete a folder and contents

  \param path Folder to delete

  \return true for success
  */
  bool removeDir(const std::string &path);

  /**
  \brief Delete a folder and contents

  \param path Folder to delete

  \return true for success
  */
  bool deleteDir(const std::string &path);

  /**
  \brief Gets the extension of the supplied file name using splitFileName()

  Prefer to use "/" as file path deliminators.

  \param filename The input filename
  \param checkFile Checks existence of file using fileExists
  \return std::string which has the file extension
  */
  std::string getFilenameExtension(const std::string &filename, bool checkFile = true);

  /**
  \brief Gets the base of the supplied file name using splitFileName()

  Prefer to use "/" as file path deliminators.

  \param filename The input filename
  \param checkFile Checks existence of file using fileExists
  \return std::string which has the file extension
  */
  std::string getFilenameBase(const std::string &filename, bool checkFile = true);

  /**
  \brief Gets the path of the supplied file name using splitFileName()

  Prefer to use "/" as file path deliminators.

  \param filename The input filename
  \param checkFile Checks existence of file using fileExists
  \return std::string which has the file extension
  */
  std::string getFilenamePath(const std::string &filename, bool checkFile = true);

  /**
  \brief Get the name of the Executable which is calling the function

  \return exe name
  */
  std::string getExecutableName();

  /**
  \brief Get the name of the Executable which is calling the function

  \return exe name
  */
  std::string getFullPath();

  /**
  \brief Get the name of the user who is calling the function

  \return user name
  */
  std::string getUserName();

  /**
  \brief Get the current working directory

  [Wrap for cbica::getcwd()]

  \return Current working directory
  */
  std::string getCWD();

  /**
  \brief Normalize a pathname by collapsing redundant separators and up-level references

  Reimplementation of python's "os.path.normpath": Normalize a pathname by collapsing redundant separators
  and up-level references so that A//B, A/B/, A/./B and A/foo/../B all become A/B. This string
  manipulation may change the meaning of a path that contains symbolic links. On Windows, it
  converts forward slashes to backward slashes.

  \param path Path to normalize

  \return std::string Normalized path
  */
  std::string normPath(const std::string &path);

  /**
  \brief Normalize a pathname by collapsing redundant separators and up-level references

  [Wrap for cbica::normPath()]

  \param path Path to normalize

  \return std::string Normalized path
  */
  std::string normalizePath(const std::string &path);

  /**
  \brief Return a relative filepath to path

  Reimplementation for python's "os.path.relpath": Return a relative filepath to path either from the
  current directory or from an optional start directory. This is a path computation: the
  filesystem is not accessed to confirm the existence or nature of path or start.

  \param path Path to check
  \param base Base file name

  \return Relative filepath to path either from current directory or from an optional start dir
  */
  std::string relPath(const std::string &path, const std::string &base);

  /**
  \brief Return a relative filepath to path

  Wrap of cbica::relPath()

  \param path Path to check
  \param base Base file name

  \return Relative filepath to path either from current directory or from an optional start dir
  */
  std::string relativePath(const std::string &path, const std::string &base);

  /**
  \brief Return the canonical path of the specified filename

  Reimplementation of python's "os.path.realpath": Return the canonical path of the specified
  filename, eliminating any symbolic links encountered in the path (if they are
  supported by the operating system).

  \param path Filename to check

  \return canonical path of path
  */
  std::string realPath(const std::string &path);

  /**
  \brief Return True if path refers to a directory entry that is a symbolic link

  Reimplementation of python's "os.path.islink": Always False if symbolic links are not
  supported by the python runtime.

  \param path Path to check

  \return True if path is symbolic link
  */
  bool isLink(const std::string &path);

  /**
  \brief Check if path refers to a symbolic entry

  [Wrap of cbica::isLink()]

  \param path Path to check

  \return True if path is symbolic link
  */
  bool isSymbolicLink(const std::string &path);

  /**
  \brief Make a symbolic link from file to another

  \param input_fileName Input file name for which symbolic link needs to be created
  \param output_fileName Output file name which is the symbolic link for input_fileName

  \return True if symbolic link successfully created
  */
  bool makeSymbolicLink(const std::string &input_fileName, const std::string &ouput_fileName);

  /**
  \brief Sets the environment variable

  \param variable_name Name of the Variable
  \param variable_value Value of variable_name

  \return True if successful
  */
  bool setEnvironmentVariable(const std::string &variable_name, const std::string &variable_value);

  /**
  \brief Delete the environment variable

  \param variable_name Name of the Variable

  \return True if successful
  */
  bool deleteEnvironmentVariable(const std::string &variable_name);

  /**
  \brief Find all files inside a directory

  \param dirName The directory to do the search in
  */
  std::vector< std::string > filesInDirectory(const std::string &dirName);

  /**
  \brief Find all sub-directories inside a directory

  \param dirName The directory to do the search in
  \param recursiveSearch Whether to do a recursive search or on a single level
  */
  std::vector<std::string> subdirectoriesInDirectory(const std::string &dirName, bool recursiveSearch = false);

  /**
  \brief Find number of rows in CSV file
  */
  size_t numberOfRowsInFile(const std::string &csvFileName, const std::string &delim = "\n");

  /**
  \brief Find number of cols in CSV file
  */
  size_t numberOfColsInFile(const std::string &csvFileName, const std::string &delim = ",");

  /**
  \brief Parse the supplied CSV File and obtain Row and Column information.

  Assumptions:
  1. Header information is in first row
  2. Full paths of images are given

  \param csvFileName The full path of the file to parse, all paths are absolute or relative to current working directory
  \param inputColumns The string of input columns which contain the data to be used for further processing
  \param inputLabels The string of input labels based on which further processing is to be done
  \param checkFile Check the validity of the file; defaults to true
  \param rowsDelimiter The delimiters used to distinguish rows in the file
  \param colsDelimiter The delimiters used to distinguish cols in the file
  \param optionsDelimiter The delimiters used in inputColumns and inputLabel
  \return Vector of CSV Dictionary items: Collection of input images are respective labels
  */
  std::vector< CSVDict > parseCSVFile(const std::string &csvFileName, const std::string &inputColumns, const std::string &inputLabels, bool checkFile = true, const std::string &rowsDelimiter = "\n", const std::string &colsDelimiter = ",", const std::string &optionsDelimiter = ",");

  /**
  \brief Parse the supplied CSV File and obtain Row and Column information.

  Assumptions:
  1. Header information is in first row
  2. Paths of images are given relative to the dataDir

  \param dataDir The full path of the directory where the data is present
  \param csvFileName The full path of the file to parse and all paths in CSV File are relative to dataDir
  \param inputColumns The string of input columns which contain the data to be used for further processing
  \param inputLabels The string of input labels based on which further processing is to be done
  \param checkFile Check the validity of the file; defaults to true
  \param rowsDelimiter The delimiters used to distinguish rows in the file, default = "\n"
  \param colsDelimiter The delimiters used to distinguish cols in the file, default = ","
  \param optionsDelimiter The delimiters used in inputColumns and inputLabels, default = ","
  \return Vector of CSV Dictionary items: Collection of input images are respective labels
  */
  std::vector< CSVDict > parseCSVFile(const std::string &dataDir, const std::string &csvFileName, const std::string &inputColumns, const std::string &inputLabels, bool checkFile = true, const std::string &rowsDelimiter = "\n", const std::string &colsDelimiter = ",", const std::string &optionsDelimiter = ",");


  //====================================== String stuff ====================================//

  /**
  \brief Splits the input file name into its constituents

  \param dataFile The full file name which is the input
  \param baseName Overwritten with file name without extension
  \param extension Overwritten with extension without '.'
  \param path Overwritten with path to file

  \return True if successful
  */
  bool splitFileName(const std::string &dataFile, std::string &path,
    std::string &baseName, std::string &extension);

  /**
  \brief Splits the string

  \param str String to split
  \param delim Delimiter on the basis of which splitting is to be done
  \return results Output in the form of vector of strings
  */
  std::vector<std::string> stringSplit(const std::string &str, const std::string &delim);

  /**
  \brief Searches for smaller string in larger string and then replaces it with user-defined input

  \param entireString String to search
  \param toReplace String to replace
  \param replaceWith String to replace toReplace with

  \return std::string of result
  */
  std::string replaceString(const std::string &entireString,
    const std::string &toReplace,
    const std::string &replaceWith);

  /**
  \brief Convert const char* to char*

  \param input constant std::string

  \return character pointer
  */
  char* constCharToChar(const std::string &input);

  /**
  \brief Convert const char* to char*

  \param input constant character pointer

  \return character pointer
  */
  char* constCharToChar(const char *input);

  //==================================== Template stuff ==================================//

  /**
  \brief Searches for an element in a vector and returns true/false and position

  Templated function to take in any kind of vector and element.

  \param vector_to_search_in Vector to do the search in
  \param element_to_search_for Element to search for
  \param position Last position of found element in vector (-1) if not found

  \return True if found
  \return Position if found (-1) if not
  */
  template<typename TContainerType>
  std::pair<bool, int> findInVector(std::vector<TContainerType> &vector_to_search_in,
    TContainerType element_to_search_for)
  {
    int position = -1;
    //std::vector<int>::const_iterator
    typename std::vector<TContainerType>::const_iterator iterator =
      std::find(vector_to_search_in.begin(), vector_to_search_in.end(), element_to_search_for);
    if (iterator != vector_to_search_in.end())
    {
      position = iterator - vector_to_search_in.begin();
      return std::make_pair(true, position);
    }
    else
      return std::make_pair(false, position);
  }

  /**
  \brief Convert first character to integer, double, unsigned int, etc.

  \param input_string Input character to be converted
  \return Templated to the type of return required
  */
  template<typename TConvertType>
  /*typename*/ TConvertType convertCharacter(const std::string &input_string)
  {
    return static_cast<TConvertType>(input_string.at(0));
  }

  /**
  \brief Convert entire string to integer, double, unsigned int, etc.

  \param input_string Input character to be converted
  \return Templated vector to the type of return required
  */
  template<typename TConvertType>
  std::vector</*typename*/ TConvertType> convertString(const std::string &input_string)
  {
    std::vector</*typename*/TConvertType>return_vector;
    for (int i = 0; i<input_string.length(); ++i)
      return_vector.push_back(static_cast<TConvertType>(input_string.at(i)));

    return return_vector;
  }
  
#if (_MSC_VER >= 1800) || __GXX_EXPERIMENTAL_CXX0X__
  /**
  \brief Base for compareEqual(...)
  */
  template <typename A, typename B>
  inline bool compareEqual(const A x, const B y)
  {
    return (x == y);
  }

  /**
  \brief Compare if equal for multiple inputs
  */
  template <typename A, typename B, typename... Others>
  bool compareEqual(const A x, const B y, Others const ... args)
  {
    return (x == y) && compareEqual(y, args...);
  }

  /**
  \brief Base for compareGreater(...)
  */
  template <typename A, typename B>
  inline bool compareGreater(const A x, const B y)
  {
    return (x > y);
  }

  /**
  \brief Compare if greater for multiple inputs
  */
  template <typename A, typename B, typename... Others>
  bool compareGreater(const A x, const B y, Others const ... args)
  {
    return (x > y) && compareGreater(y, args...);
  }

  /**
  \brief Base for compareLesser(...)
  */
  template <typename A, typename B>
  inline bool compareLesser(const A x, const B y)
  {
    return (x < y);
  }

  /**
  \brief Compare if greater for multiple inputs
  */
  template <typename A, typename B, typename... Others>
  bool compareLesser(const A x, const B y, Others const ... args)
  {
    return (x < y) && compareLesser(y, args...);
  }

#endif



};

#if defined(__GNUC__)  && (__GNUC__ < 5)
namespace std
{
  //! std::round wrap for GCC
  inline int round(const float &input)
  {
    int returnValue;
    const float inputWrap = abs(input);
    inputWrap >= floor(inputWrap) + 0.5 ? returnValue = ceil(inputWrap) : returnValue = floor(inputWrap);

    if (input < 0)
    {
      returnValue = -(returnValue);
    }

    return returnValue;
  }

  //! std::round wrap for GCC
  inline int round(const double &input)
  {
    int returnValue;
    const double inputWrap = abs(input);
    inputWrap >= floor(inputWrap) + 0.5 ? returnValue = ceil(inputWrap) : returnValue = floor(inputWrap);

    if (input < 0)
    {
      returnValue = -(returnValue);
    }

    return returnValue;
  }

  //! std::round wrap for GCC
  inline int round(const long int &input)
  {
    int returnValue;
    const long int inputWrap = abs(input);
    inputWrap >= floor(inputWrap) + 0.5 ? returnValue = ceil(inputWrap) : returnValue = floor(inputWrap);

    if (input < 0)
    {
      returnValue = -(returnValue);
    }

    return returnValue;
  }

  //! std::round wrap for GCC
  inline int round(const unsigned int &input)
  {
    int returnValue;
    const unsigned int inputWrap = abs(input);
    inputWrap >= floor(inputWrap) + 0.5 ? returnValue = ceil(inputWrap) : returnValue = floor(inputWrap);

    if (input < 0)
    {
      returnValue = -(returnValue);
    }

    return returnValue;
  }

  //! std::round wrap for GCC
  inline int round(const short &input)
  {
    int returnValue;
    const short inputWrap = abs(input);
    inputWrap >= floor(inputWrap) + 0.5 ? returnValue = ceil(inputWrap) : returnValue = floor(inputWrap);

    if (input < 0)
    {
      returnValue = -(returnValue);
    }

    return returnValue;
  }

  //! std::to_string wrap for GCC
  inline std::string to_string(const int &input)
  {
    std::ostringstream ss;
    ss << input;
    return ss.str();
  }

  //! std::to_string wrap for GCC
  inline std::string to_string(const unsigned int &input)
  {
    std::ostringstream ss;
    ss << input;
    return ss.str();
  }

  //! std::to_string wrap for GCC
  inline std::string to_string(const size_t &input)
  {
    std::ostringstream ss;
    ss << input;
    return ss.str();
  }

  //! std::to_string wrap for GCC
  inline std::string to_string(const long int &input)
  {
    std::ostringstream ss;
    ss << input;
    return ss.str();
  }

  //! std::to_string wrap for GCC
  inline std::string to_string(const double &input)
  {
    std::ostringstream ss;
    ss << input;
    return ss.str();
  }

  //! std::to_string wrap for GCC
  inline std::string to_string(const float &input)
  {
    std::ostringstream ss;
    ss << input;
    return ss.str();
  }
  
};
#endif
