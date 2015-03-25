/**
\file  cbicaCmdParser.h

\brief Declaration of the cbicaCmdParser class. Does simple command line parsing

This is a pure c++ implementation. An example of usage is shown below:

<code>
// Executable name is automaticall picked up by the @PROJECT_NAME@ tag from the CMakeLists file
cbica::CmdParser parser();

// add paramters to the variable
parser.addParameter("-u","--usage", "prints usage", "second line");
parser.addParameter("-h","--help", "gets help from Justice League");
parser.addParameter("-m","--marvel", "prefer Marvel instead");

// compare and do something if true
if( parser.compareParamter(argv[2], "u") )
{
  parser.echoUsage();
  int u_test = 0;
}
</code>

https://www.cbica.upenn.edu/sbia/software/
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#include <string>
#include <stdio.h>
#include <vector>
#include <tuple>
#include <iostream>
#include <iterator>
#include <algorithm>

#define NEWLINE "\n"

namespace cbica
{
  class CmdParser
  {
  public:
    /**
    \brief The Constructor
    */
    explicit CmdParser(/*const std::string &exe_name*/);
    
    /**
    \brief The Constructor

    \param exe_name Name of the executable
    */
    explicit CmdParser(const std::string &exe_name);
    /**
    \brief The Destructor
    */
    virtual ~CmdParser();

    /**

    */
    void setExeName(const std::string exeName){m_exeName = exeName;};

    /**
    \brief Adding parameters

    \param laconic The laconic variant
    \param verbose The verbose variant
    \param description_line1 The single line of description for parameters
    */
    void addParameter( const std::string &laconic, const std::string &verbose, 
                                const std::string &description_line1 );
    
    /**
    \brief Adding parameters

    \param laconic The laconic variant
    \param verbose The verbose variant
    \param description_line1 The first line of description for parameters
    \param description_line2 The second line of description for parameters
    */
    void addParameter(const std::string &laconic, const std::string &verbose, 
                      const std::string &description_line1, const std::string &description_line2);

    /**
    \brief Display the usage
    */
    void echoUsage();
    
    /**
    \brief Display the version details
    */
    void echoVersion();

    /**
    \brief Check ONLY for laconic paramters
    */
    bool compareParamter(const std::string &inputParamToCheck, const std::string &execParamToCheck);

    /**
    \brief Get the description analogous with the paramter

    \param paramter Parameter whose description is requested
    \return Description of parameter
    */
    std::string getDescription(const std::string &parameter );

  private:
    //! Executable name
    std::string m_exeName;

    //! The parameters are stored as tuples. description_line2 is empty if not initialized
    std::vector< std::tuple< std::string,std::string,std::string,std::string, int > > m_parameters;

    ////! Store laconic paramters along with their position in m_paramters
    //std::vector< std::tuple< std::string, int > > m_laconicParamters;
    
    //! Store laconic paramters along with their position in m_paramters
    std::vector< std::string > m_laconicParamters;
    
    //! Store laconic paramters along with their position in m_paramters
    std::vector< std::tuple< std::string, int > > m_verboseParamters;

    //! Max length of parameters for echoUsage()
    int m_maxLength;

    bool checkMaxLen;
  };
}