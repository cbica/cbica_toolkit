\mainpage CBICA Toolkit

Section of Biomedical Image Analysis <br>
Department of Radiology <br>
University of Pennsylvania <br>
3600 Market Street, Suite 380 <br>
Philadelphia, PA 19104 <br>

<pre>
Web  : http://www.cbica.upenn.edu/sbia/
Email: <a href="mailto:sbia-software@uphs.upenn.edu">sbia-software at uphs.upenn.edu</a> 
</pre>

Copyright (c) 2015 University of Pennsylvania. All rights reserved. <br>
See http://www.cbica.upenn.edu/sbia/software/license.html or COPYING file.

<b> Authors: </b><br>
Sarthak – primary developer, integration of everything <br>
Drew    – DTI algorithms <br>
Michael – sort + rename <br>
Martin  – sort + rename <br>
Jimit   – Overall testing <br>
Guray   – Overall testing <br>


===============
1. INTRODUCTION
===============

This project provides a common interface for some popular functions/methods used in the CBICA research center. It is actively maintained and tested on
Windows x64 and Linux (CentOS6) x64. 

All the functions are standalone in nature; i.e., they are header-only and can be exported individually for use/distribution in projects.

Currently, the Toolkit has been tested to be working on Windows x64 using MS Visual Studio 2012 while linking against ITK/4.4.2 built with options
specified in SBIA internal Wiki (http://sbia-wiki.uphs.upenn.edu/wiki/index.php/Third-party_Software#ITK).

<b>TO DO:</b>
- Compatibility with ITK/4.7.1
- Individual executables for functions to make Python wrapping easier

[Request a new function via eLog.]

==========
2. INSTALL    
==========

----------------
2.1 Dependencies
----------------

- C++ complier (MSVC/12, GCC/4.9.2)
- CMake 2.8.12.1
- Doxygen (for documentation only)
- GIT (for superbuild system only) -- ?
- ITK 4.7.0 (required for ITK based classes) - check cmake/ExternalITK for parameters

Make sure all dependencies are met before proceeding with install.

-------------------------------------
2.2 Generating standalone executables
-------------------------------------

Use CMake to generate files for the compiler in use (MSVC/GCC) and then run accordingly. The installation directory for both Python and CPP classes
can be customized. To use the Python class, make sure your Python executable has the install path in its find modules path.

An example under UNIX using CMake command line:

Go to the <b>build</b> directory:<br>
\verbatim
cd build
\endverbatim

Run CMake (note you can use <code>ccmake ../</code> to configure the installation directory):<br>
\verbatim
cmake ../
\endverbatim

Build and install:<br>
\verbatim
make 
make install
\endverbatim

----------------------------
2.3 Generating documentation
----------------------------

Generate the project files. Then follow appropriate instructions:

2.3.1 For Internal use (integrates with SBIA website):

Enable the option <b>BUILD_DOCUMENTATION_INTERNAL</b> while running cmake.<br>
UNIX: <code>make doc_internal</code><br>
WINDOWS: Build the <code>doc_internal</code> project.

The documentation is generated into the binary directory by default (<code>"BIN_DIR/doc_internal/"</code>) and it required Doxygen to run.

<b>NOTE:</b> The folder <code>"docs_internal"</code> should be never be present in a tag. It is for internal use only.

2.3.1 For External use (to be published along with source code):

Enable the option <b>BUILD_DOCUMENTATION</b> while running cmake.<br>
UNIX: <code>make doc</code><br>
WINDOWS: Build the <code>doc</code> project.

The documentation is generated into the binary directory by default (<code>"BIN_DIR/docs/"</code>) and it required Doxygen to run.

-----------
2.4 Testing
-----------

After generating project files, run the following commands:<br>
UNIX: <code>make tests</code><br>
WINDOWS: Build the <code>RUN_TESTS</code> project.

The test executables (if any) are generated into the binary directory by default (<code>"BIN_DIR/testing/"</code>).


==============================
3. AVAILABLE FUNCTIONS/CLASSES    
==============================

-------------------
3.1 Basic Functions
-------------------

Folder/File operations:

- <b>fileExists</b>: Check if specified file exists or not
- <b>isFile</b>: Wrap of <code>fileExists</code>
- <b>directoryExists</b>: Check if specified directory exists or not
- <b>isDir</b>: Wrap of <code>directoryExists</code>
- <b>createTmpDir</b>: Creates a temporary directory
- <b>createTemporaryDirectory</b>: Wrap of <code>createTmpDir</code>
- <b>makeTemporaryDirectory</b>: Wrap of <code>createTmpDir</code>
- <b>createDir</b>: Creates a directory
- <b>makeDir</b>: Wrap for <code>createDir</code>
- <b>removeDirectoryRecursively</b>: Removes a directory and all its contents
- <b>removeDir</b>: Wrap for <code>removeDirectoryRecursively</code>
- <b>deleteDir</b>: Wrap for <code>removeDirectoryRecursively</code>
- <b>numberOfRowsInFile</b>: Number of rows in CSV file
- <b>numberOfColsInFile</b>: Number of cols in CSV file
- <b>parseCSVFile</b>: Parse a list CSV file which contains file names and labels

OS operations:

- <b>getFilenameExtension</b>: Get the extension of the supplied file name. Wrap of <code>splitFileName</code>
- <b>getFilenameBase</b>: Get the extension of the supplied file name. Wrap of <code>splitFileName</code>
- <b>getFilenamePath</b>: Get the extension of the supplied file name. Wrap of <code>splitFileName</code>
- <b>getExecutableName</b>: Get current executable name
- <b>getFullPath</b>: Get path of current executable
- <b>getUserName</b>: Get current user name
- <b>getCWD</b>: Get current working directory
- <b>normalizePath</b>: Wrap for normPath
- <b>relativePath</b>: Wrap for relPath
- <b>realPath</b>: Reimplementation of python's "os.path.realpath"
- <b>isLink</b>: Reimplementation of python's "os.path.islink"
- <b>isSymbolicLink</b>: Wrap for <code>isLink</code>
- <b>makeSymbolicLink</b>: Make a symbolic link of a file
- <b>setEnvironmentVariable</b>: Sets the environment variable
- <b>deleteEnvironmentVariable</b>: Delete the environment variable
- <b>filesInDirectory</b>: Find all files in the first level of directory
- <b>subdirectoriesInDirectory</b>: Find all directories. Recursive turned off by default.

Python re-implementations: 

- <b>isFile</b>: Reimplementation of "os.path.isfile"--Return True if path is an existing regular file
- <b>isDir</b>: Reimplementation of "os.path.isdir"--Return True if path is an existing directory
- <b>relPath</b>: Reimplementation of "os.path.relpath"--Return a relative filepath to path
- <b>realPath</b>: Reimplementation of "os.path.realpath"--Return the canonical path of the specified filename
- <b>exists</b>: Reimplementation of "os.path.exists"--Return True if path exists and false for broken symbolic links
- <b>isLink</b>: Reimplementation of "os.path.islink"--Return True if path refers to a directory entry that is a symbolic link
- <b>normPath</b>: Reimplementation of "os.path.normpath"--Normalize a pathname by collapsing redundant separators and up-level references

String operations:

- <b>splitFileName</b>: Splits a supplied file name between path, base file name and extension
- <b>stringSplit</b>: Split the supplied string with the deliminator and store results in a vector
- <b>replaceString</b>: Replaces specified string with another string in a larger input string
- <b>constCharToChar</b>: Converts from "const char*" to "char*"
- <b>findInVector</b>: Checks for an element in a vector and returns true or false along with its position in vector (-1 if not found)
- <b>convertString</b>: Convert a character to its corresponding ASCII code
- <b>convertCharacter</b>: Convert first character to integer, double, unsigned int, etc.

MISC:

- <b>compareEqual</b>: Compare if multiple inputs are equal
- <b>compareGreater</b>: Compare if first input is greater than rest
- <b>compareLesser</b>: Compare if first input is lesser than rest


STD Overrides (these are present for GCC < 5):

- <b>round</b>: std::round wrap
- <b>to_string</b>: std::round wrap

TO DO:


[Request a new function via eLog.]

-------------------
3.2 Basic Classes
-------------------

* <b>Logging</b>: A logging class which generates machine-parseable logs. This can output to both console (default behavior) and a file.

Generated log is in the format: <br>
<CODE><4 digit year>:<2 digit month>:<2 digit date>,<2 digit 24 hour>:<2 digit minute>:<2 digit second>;<exe name>;<user name></CODE>

Available functions:

** <b>EnableTextLogging</b>: Switches from console to specific text file
** <b>Write</b>: Writes free text to console/file as std::cout
** <b>WriteError</b>: Writes free text to console/file as std::cerr - used for error reporting
** <b>UseNewFile</b>: Change Logging file after initializing class


* <b>CmdParser</b>: Universal command line parser. Add parameters, descriptions and call on them from the command line. Details in header file. It picks up exe name using getExecutableName() but it can be set in the class. The parameters "u/usage", "v/version" and "h/help" are automatically set. 'usage' is basic usage details, 'help' is verbose help and 'version' is version details (picked up automatically from parent CMake file).
  
Available functions: 

** <b>addRequiredParameter</b>: Add required parameter (no '-' is necessary) - exception is thrown if a required parameter is missing
** <b>addOptionalParameter</b>: Add optional parameter (no '-' is necessary)
** <b>addParameter</b>: Defaults to addOptionalParameter()
** <b>compareParameter</b>: Check if specific parameter is present in the argv
** <b>getDescription</b>: Get description of the specific parameter 
** <b>exampleUsage</b>: Add an example usage which comes up in echoHelp()
** <b>setExeName</b>: Set a custom executable name
** <b>writeConfigFile</b>: Writes a machine readable configuration file for the executable
** <b>echoUsage</b>: Based on the parameters added, it automatically constructs an echoUsage() console output which includes contact information and copyright details
** <b>echoHelp</b>: Based on the parameters added, it automatically constructs an echoHelp() console output which includes contact information and copyright details
  
TO DO:


[Request a new function via eLog.]

-----------------
3.3 ITK Classes
-----------------

* <b>ImageInfo</b>: obtains the data regarding spacing and dimensions of specified image in 
<code>itk::ImageIOBase</code>; look in class documentation for details.

Available functions: 
** <b>getImageIOBase</b>: Get the ImageIOBase class wrapped around a smart pointer<br>
** <b>getImageDimensions</b>: Get the dimensions<br>
** <b>getImageSpacings</b>: Get the spacings<br>

* <b>CommonHolder</b>: Common interface class for all algorithmic classes.
  
* <b>ComputeAverageMap</b>: Computes the average of a series of images and writes the output. Set input files as a vector of strings
and the output directory in the constructor itself.

* <b>ComputeDtiScalars</b>: Computes and saves the specified scalar image for an image. Input image and output (director/file) are 
specified in the constructor.
  
Available options: 
** FA   : FA images
** EIG  : Eigensystem images
** GEO  : Geoemtric features
** SKEW : Tensor Skewness
** KURT : Tensor kurtosis
** RADAX: Radial and Axial diffusivity
** GORDR: Gordon's R measures
** GORDK: Gordon's K measures
** ALL  : All of the above (default)
  
* <b>ComputeVarianceMap</b>: Computes the variance and saves the output for a single image (can be changed easily by modifying
the class to be more in line with ComputeAverageMap).

NOTE: There are a lot of back-end classes which have been implemented for compatibility between itk versions. 
These are not meant to be for standalone usage.

TO DO:
- dti Stuff

[Request a new function via eLog.]


=======================
4 Developer Information
=======================

If you want to write a class which works with the CBICA Toolkit, please make sure you follow the set guidelines of the code. A brief summary:

<b>
For basic classes:
</b>
- As of now, there is no "default" interface (since functionality differs so wildly).
- Make sure your class is well documented and a basic usage example is provided in the documentation (see cbica::CmdParser or cbica::Logging for examples)
- Let's say your class' header file is <code>cbicaXYZ.h</code>, add it to the list of headers in <code>itk_dev/src/classes/CMakeLists.txt</code>
(also add all other applicable files under headers or sources - this process of manual addition is intentional and in line with CMake guidelines).
- Include <code>cbicaITK_XYZ.h</code> in the test executable <code>itk_dev/src/classes/testing/BasicFunctionTests.cxx</code> and write a specific test 
and debug the class as required (suggested method is to hard code the path for debugging).

    
<b>
For ITK classes:
</b>

- If you are writing a class which takes a file as input, make sure you inherit from the cbica::CommonHolder class, which has a lot of pre-defined
functionality and also a basic default interface.
- Let's say your class' header file is <code>cbicaITK_XYZ.h</code>, add it to the list of headers in <code>itk_dev/src/classes/itk/CMakeLists.txt</code>
(also add all other applicable files under headers or sources - this process of manual addition is intentional and in line with CMake guidelines).
- Include <code>cbicaITK_XYZ.h</code> in the test executable <code>itk_dev/src/classes/itk/testing/ItkFunctionTests.cxx</code> and write a specific test 
and debug the class as required (suggested method is to hard code the path for debugging).

Please adhere to the package structure.


===========
5 LICENSING
===========

  See http://www.cbica.upenn.edu/sbia/software/license.html or "licences/COPYING" file.