/**
\file  BasicFunctionTests.cxx

\brief File for testing the basic classes.

https://www.cbica.upenn.edu/sbia/software/
software@cbica.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html.

*/
#include <iostream>
#include <iterator>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <exception>
#include <typeinfo>
#include <stdexcept>
#include <stdio.h>
#if(WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#define TEST_DATA "/nifit1/"

#include "cbicaUtilities.h"
#include "cbicaStatistics.h"
#include "cbicaLogging.h"
#include "cbicaCmdParser.h"

int main(int argc, char** argv)
{
  cbica::CmdParser parser(argc, argv);
  parser.addOptionalParameter("b1", "buffer", cbica::Parameter::NONE, "", "Buffer Test");
  parser.addOptionalParameter("p1", "cmdParser", cbica::Parameter::NONE, "", "CmdParser Test");
  parser.addOptionalParameter("c1", "configFileReader", cbica::Parameter::NONE, "", "ConfigFileReader Test");
  parser.addOptionalParameter("c2", "copyFile", cbica::Parameter::NONE, "", "copyFile Test");
  parser.addOptionalParameter("c3", "copyFolder", cbica::Parameter::NONE, "", "copyFolder Test");
  parser.addOptionalParameter("f2", "createFile", cbica::Parameter::NONE, "", "createFile Test");
  parser.addOptionalParameter("f1", "createFolder", cbica::Parameter::NONE, "", "createFolder Test");
  parser.addOptionalParameter("p2", "csvParser", cbica::Parameter::NONE, "", "envName Test");
  parser.addOptionalParameter("p3", "csvParserRaw", cbica::Parameter::NONE, "", "envName Test");
  parser.addOptionalParameter("d2", "deleteFile", cbica::Parameter::NONE, "", "deleteFile Test");
  parser.addOptionalParameter("d1", "deleteFolder", cbica::Parameter::NONE, "", "deleteFolder Test");
  parser.addOptionalParameter("e", "envName", cbica::Parameter::NONE, "", "envName Test");
  parser.addOptionalParameter("l", "logging", cbica::Parameter::NONE, "", "logging Test");
  parser.addOptionalParameter("m", "makeTempDir", cbica::Parameter::NONE, "", "makeTempDir Test");
  parser.addOptionalParameter("n2", "noFile", cbica::Parameter::NONE, "", "noFile Test");
  parser.addOptionalParameter("n1", "noFolder", cbica::Parameter::NONE, "", "noFolder Test");
  parser.addOptionalParameter("s1", "symbolic", cbica::Parameter::NONE, "", "symbolic Test");
  parser.addOptionalParameter("s2", "subDir", cbica::Parameter::NONE, "", "subDir Test");
  parser.addOptionalParameter("v", "variadic", cbica::Parameter::NONE, "", "variadic Test");
  parser.addOptionalParameter("r", "roc", cbica::Parameter::NONE, "", "ROC test");
  parser.addOptionalParameter("z", "zscore", cbica::Parameter::NONE, "", "ZScore test");

  int tempPostion;
  if (parser.compareParameter("buffer", tempPostion))
  {
    char buff[100];
    sprintf(buff, "This is a testing scenario.\n");
    buff[0] = '\0';
  }

  if (parser.compareParameter("cmdParser", tempPostion))
  {
    std::string valueToCheck = argv[tempPostion + 1], returnValue;
    parser.getParameterValue("cmdParser", returnValue);

    if (returnValue != valueToCheck)
    {
      return EXIT_FAILURE;
    }
    //std::string dirToWrite = argv[tempPostion + 1];
    //int i = 0;
    //const char *argv2[] = { "test-exe", "-1p", "1", "--secondParam", "2" };

    //cbica::CmdParser parser2 = cbica::CmdParser(i + 1, argv2);
    //parser2.setExeName("ParserClassTests");
    //parser2.addParameter("1p", "firstParam", cbica::Parameter::INTEGER, "1 to 10", "first parameter"); // defaults to optional parameters 
    //parser2.addRequiredParameter("2p", "secondParam", cbica::Parameter::FLOAT, "0.1 to 20", "second parameter", "description line 2", "description line 3", "description line 4", "description line 5");
    //parser2.addOptionalParameter("3p", "thirdParam", cbica::Parameter::STRING, "max length = 1024", "third parameter which is optional");

    //parser2.writeConfigFile(dirToWrite);
    //int tempPosition;

    //// parameter is found
    //if (!parser2.compareParameter("1p", tempPosition))
    //  return EXIT_FAILURE;

    //if (!parser2.compareParameter("2p", tempPosition))
    //  return EXIT_FAILURE;

    //// description can be read
    //if (parser2.getDescription("1p", false) .empty())
    //  return EXIT_FAILURE;

    //if (parser2.getDescription("2p", false) .empty())
    //  return EXIT_FAILURE;

    //// data type can be deduced as enum code
    //if (parser2.getDataTypeAsEnumCode("1p") != cbica::Parameter::INTEGER)
    //  return EXIT_FAILURE;

    //if (parser2.getDataTypeAsEnumCode("2p") != cbica::Parameter::FLOAT)
    //  return EXIT_FAILURE;

    //// data type can be deduced as string
    //if (parser2.getDataTypeAsString("1p") != "INTEGER")
    //  return EXIT_FAILURE;

    //if (parser2.getDataTypeAsString("2p") != "FLOAT")
    //  return EXIT_FAILURE;

  }

  if (parser.compareParameter("configFileReader", tempPostion))
  {
    std::string configFile = argv[tempPostion + 1];
    std::vector< cbica::Parameter > testParameters = parser.readConfigFile(configFile, true);

    size_t count = 0;

    for (size_t i = 0; i < testParameters.size(); i++)
    {
      // everything is put in a nested if-else loop so that the number of iterations can be reduced
      if (testParameters[i].verbose == "firstParam")
      {
        count++;
        if (testParameters[i].dataType_string != "INTEGER")
          return EXIT_FAILURE;

        if (testParameters[i].dataType_enumCode != cbica::Parameter::INTEGER)
          return EXIT_FAILURE;

        if (testParameters[i].dataRange != "1 to 10")
          return EXIT_FAILURE;

        if (testParameters[i].descriptionLine1.length() == 0)
          return EXIT_FAILURE;
      }
      else if (testParameters[i].verbose == "secondParam")
      {
        count++;
        if (testParameters[i].dataType_string != "FLOAT")
          return EXIT_FAILURE;

        if (testParameters[i].dataType_enumCode != cbica::Parameter::FLOAT)
          return EXIT_FAILURE;

        if (testParameters[i].dataRange != "0.1 to 20")
          return EXIT_FAILURE;

        if (testParameters[i].descriptionLine1.length() == 0)
          return EXIT_FAILURE;
      }
      else if (testParameters[i].verbose == "thirdParam")
      {
        count++;
        if (testParameters[i].dataType_string != "STRING")
          return EXIT_FAILURE;

        if (testParameters[i].dataType_enumCode != cbica::Parameter::STRING)
          return EXIT_FAILURE;

        if (testParameters[i].dataRange != "max length = 1024")
          return EXIT_FAILURE;

        if (testParameters[i].descriptionLine1.length() == 0)
          return EXIT_FAILURE;
      }
    }

    if (count < 3)
    {
      // return failure if the number of parameters that needed to be detected is less than 3
      return EXIT_FAILURE;
    }
  }

  if (parser.compareParameter("copyFile", tempPostion))
  {
    const std::string inputFile = argv[tempPostion + 1];
    std::string outputDir = cbica::createTemporaryDirectory(), path, base, ext;
    std::string outputFile = outputDir + "/example" + ext;
    cbica::splitFileName(inputFile, path, base, ext);

    if (!cbica::copyFile(inputFile, outputFile))
    {
      return EXIT_FAILURE;
    }

    if (!cbica::fileExists(outputFile))
    {
      return EXIT_FAILURE;
    }

    if (cbica::getFileSize(outputFile) != cbica::getFileSize(inputFile))
    {
      return EXIT_FAILURE;
    }

    cbica::deleteDir(outputDir);
  }

  if (parser.compareParameter("copyFolder", tempPostion))
  {
    const std::string inputFolder = argv[tempPostion + 1];
    std::string outputDir = cbica::createTemporaryDirectory();
    cbica::copyDir(inputFolder, outputDir);

    if (!cbica::copyDir(inputFolder, outputDir))
    {
      return EXIT_FAILURE;
    }

    if (!cbica::directoryExists(outputDir))
    {
      return EXIT_FAILURE;
    }

    if (cbica::getFolderSize(outputDir) != cbica::getFolderSize(inputFolder))
    {
      return EXIT_FAILURE;
    }

    cbica::deleteDir(outputDir);
  }

  if (parser.compareParameter("createFile", tempPostion))
  {
    std::ofstream file;
    file.open(argv[tempPostion + 1]);
    file << "test";
    file.close();
    if (!cbica::fileExists(argv[tempPostion + 1]))
      return EXIT_FAILURE;
    if (!cbica::exists(argv[tempPostion + 1]))
      return EXIT_FAILURE;

    std::remove(argv[tempPostion + 1]);
  }

  if (parser.compareParameter("createFolder", tempPostion))
  {
    cbica::createDir(argv[tempPostion + 1]);
    if (!cbica::directoryExists(argv[tempPostion + 1]))
      return EXIT_FAILURE;
    if (!cbica::exists(argv[tempPostion + 1]))
      return EXIT_FAILURE;

    cbica::deleteDir(argv[tempPostion + 1]);
  }

  if (parser.compareParameter("csvParser", tempPostion))
  {
    std::string csvFileName = argv[tempPostion + 1];

    std::vector< CSVDict > csv_test = cbica::parseCSVFile(csvFileName, "T2,FL,T1", "AGE", false);

    if (csv_test.size() != 8)
    {
      return EXIT_FAILURE;
    }
    if (csv_test[0].inputImages.size() != 3)
    {
      return EXIT_FAILURE;
    }
    if (csv_test[0].inputLabels.size() != 1)
    {
      return EXIT_FAILURE;
    }
  }

  if (parser.compareParameter("csvParserRaw", tempPostion))
  {
    const std::string csvFileName = std::string(argv[2]) + "/zscores.csv";
    auto parsedCsv = cbica::readCSVDataFile< double >(csvFileName);

    if (parsedCsv.size() != 100)
    {
      return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < parsedCsv.size(); i++)
    {
      if (parsedCsv[i].size() != 1)
      {
        return EXIT_FAILURE;
      }
    }

    // check column major implementation as well
    auto parsedCsv_col = cbica::readCSVDataFile< double >(csvFileName, true);

    if (parsedCsv_col.size() != 1)
    {
      return EXIT_FAILURE;
    }

    for (size_t i = 0; i < parsedCsv_col.size(); i++)
    {
      if (parsedCsv_col[i].size() != 100)
      {
        return EXIT_FAILURE;
      }
    }
  }

  if (parser.compareParameter("deleteFile", tempPostion))
  {
    std::ofstream file;
    file.open(argv[tempPostion + 1]);
    file << "test";
    file.close();
    if (std::remove(argv[tempPostion + 1]) != 0)
      return EXIT_FAILURE;
  }

  if (parser.compareParameter("deleteFolder", tempPostion))
  {
    cbica::createDir(argv[tempPostion + 1]);
    if (!cbica::deleteDir(argv[tempPostion + 1]))
      return EXIT_FAILURE;
  }

  if (parser.compareParameter("envName", tempPostion))
  {
    std::string random_variable = "RANDOM_VAR", random_val = "VAL";
    int val = 1;

    if (!cbica::setEnvironmentVariable(random_variable, random_val))
      return EXIT_FAILURE;
    std::string value = cbica::getEnvironmentVariableValue(random_variable);
    if (value != random_val)
      return EXIT_FAILURE;
    cbica::deleteEnvironmentVariable(random_variable);
    if (!cbica::setEnvironmentVariable(random_variable, std::to_string(val)))
      return EXIT_FAILURE;
    value = cbica::getEnvironmentVariableValue(random_variable);
    if (value != std::to_string(val))
      return EXIT_FAILURE;
    cbica::deleteEnvironmentVariable(random_variable);
  }

  if (parser.compareParameter("logging", tempPostion))
  {
    std::string dirName = cbica::createTmpDir();
    std::string fName = dirName + std::string("log.txt");

    cbica::Logging logger(fName, argv[tempPostion + 1] /*take free text from the console*/);

    if (!cbica::fileExists(fName))
    {
      return EXIT_FAILURE;
    }

    std::string ext, path, base;
    // check for certain conditions and fail if even one of them isn't met 
    if (!cbica::splitFileName(fName, path, base, ext) || // if splitFileName fails
      (ext != ".txt") || // if the extension is not txt
      (path .empty()) || // if path is empty
      (base .empty())) // if base is empty
    {
      return EXIT_FAILURE; // fail if detected extension is not a txt
    }

    std::ifstream logFile;
    logFile.open(fName.c_str());
    std::string content;
    if (logFile.is_open())
      while (!logFile.eof())
        logFile >> content;
    logFile.close();
    cbica::deleteDir(dirName); // delete the temporary folder and all its contents 

#if defined(_WIN32)
    std::string userName = getenv("USERNAME");
#else
    std::string userName = std::getenv("USER");
#endif
    if (content.find(userName) != std::string::npos)
      return EXIT_SUCCESS;
  }

  if (parser.compareParameter("makeTempDir", tempPostion))
  {
    std::string return_dir = cbica::createTmpDir();

    if (!cbica::isDir(return_dir))
      return EXIT_FAILURE;

    cbica::deleteDir(return_dir);
  }

  if (parser.compareParameter("noFile", tempPostion))
  {
    if (cbica::fileExists(argv[tempPostion + 1]))
      return EXIT_FAILURE;
    else if (cbica::exists(argv[tempPostion + 1]))
      return EXIT_FAILURE;
  }

  if (parser.compareParameter("noFolder", tempPostion))
  {
    if (cbica::directoryExists(argv[tempPostion + 1]))
      return EXIT_FAILURE;
    else if (cbica::exists(argv[tempPostion + 1]))
      return EXIT_FAILURE;
  }

  if (parser.compareParameter("symbolic", tempPostion))
  {
    std::string dirName = cbica::createTmpDir();
    std::string fName = dirName + std::string("log.txt");
    std::string fName_sym = dirName + std::string("log_sym.txt");
    cbica::Logging logger(fName, argv[tempPostion + 1]);

#if defined(_WIN32)
    // do nothing as Windows doesn't support symbolic linkage creatation for non-admins
#else
    cbica::makeSymbolicLink(fName, fName_sym);
    if (!cbica::isLink(fName_sym))
      return EXIT_FAILURE;
#endif // defined(WIN_32)

    cbica::deleteDir(dirName);
  }

  if (parser.compareParameter("subDir", tempPostion))
  {
    std::string return_dir = cbica::normPath(cbica::createTmpDir());

    if (!cbica::isDir(return_dir))
    {
      return EXIT_FAILURE;
    }

    if (!cbica::createDir(return_dir + "/random1/"))
    {
      cbica::deleteDir(return_dir);
      return EXIT_FAILURE;
    }

    if (!cbica::createDir(return_dir + "/random2/"))
    {
      cbica::deleteDir(return_dir);
      return EXIT_FAILURE;
    }

    if (!cbica::createDir(return_dir + "/random3/"))
    {
      cbica::deleteDir(return_dir);
      return EXIT_FAILURE;
    }

    if (!cbica::createDir(return_dir + "/random1/random11/"))
    {
      cbica::deleteDir(return_dir);
      return EXIT_FAILURE;
    }

    if (cbica::subdirectoriesInDirectory(return_dir, true).size() != 4)
    {
      cbica::deleteDir(return_dir);
      return EXIT_FAILURE;
    }

    if (!cbica::deleteDir(return_dir))
    {
      return EXIT_FAILURE;
    }
  }

#if (_MSC_VER >= 1800) || __GXX_EXPERIMENTAL_CXX0X__ || (__GNUC__ > 4)
  if (parser.compareParameter("variadic", tempPostion))
  {
    int a = 1, b = 1, c = 1, d = 1;
    if (!cbica::compareEqual(a, b, c, d))
    {
      return EXIT_FAILURE;
    }

    a = 5, b = 4, c = 3, d = 2;
    if (!cbica::compareGreater(a, b, c, d))
    {
      return EXIT_FAILURE;
    }

    a = 2, b = 3, c = 4, d = 5;
    if (!cbica::compareLesser(a, b, c, d))
    {
      return EXIT_FAILURE;
    }
  }
#endif

  if (parser.isPresent("roc"))
  {
    const std::string dataDir = argv[2];
    const std::string realLabelFile = dataDir + "/labels_real.csv";
    const std::string predLabelFile = dataDir + "/labels_predicted.csv";
    const std::string comparisonFile = dataDir + "/roc_compare.csv";

    auto realLabels = cbica::readCSVDataFile< float >(realLabelFile, true);
    auto predLabels = cbica::readCSVDataFile< float >(predLabelFile, true);
    auto comparisonValues = cbica::readCSVDataFile(comparisonFile);

    auto output = cbica::ROC_Values(realLabels[0], predLabels[0]);

    // fail if the output is empty
    if (output.empty())
    {
      return EXIT_FAILURE;
    }

    // fail if the same number of statistics haven't been generated - this means that the comparisonFile is out of date
    if (output.size() != comparisonValues.size() - 1)
    {
      return EXIT_FAILURE;
    }

    for (size_t i = 1; i < comparisonValues.size(); ++i)
    {
      auto meanSqDiff = std::pow((output[comparisonValues[i][0]] - std::atof(comparisonValues[i][1].c_str())), 2);
      if (meanSqDiff > 1e-5) // arbitrary threshold set here
      {
        return EXIT_FAILURE;
      }
    }
  }

  if (parser.isPresent("zscore"))
  {
    const std::string dataDir = argv[2];

    const std::string inputFile = dataDir + "/input.csv";
    const std::string compareFile = dataDir + "/zscores.csv";

    auto input = cbica::readCSVDataFile< float >(inputFile, true);
    auto compare = cbica::readCSVDataFile< float >(compareFile, true);

    cbica::Statistics< float > statistics(input[0]);
    auto output = statistics.GetZScores();
    
    if (output.empty())
    {
      return EXIT_FAILURE;
    }

    if (output.size() != compare[0].size())
    {
      return EXIT_FAILURE;
    }

    for (size_t i = 0; i < output.size(); i++)
    {
      auto meanSqDiff = std::pow((output[i] - compare[0][i]), 2);
      if (meanSqDiff > 1e-5) // arbitrary threshold set here
      {
        return EXIT_FAILURE;
      }
    }
    int blah = 1;
  }


  return EXIT_SUCCESS;
}
