## PARENT CONFIGURATION FILE

CMAKE_MINIMUM_REQUIRED( VERSION 3.7 )

SET( 
  PROJECT_NAME CBICATk # change this for branches, etc.
  #VERSION 0.0.2 
  ) # change project name here

PROJECT( ${PROJECT_NAME} )
SET(PROJECT_NAME_EXTENDED "CBICA Toolkit")
ADD_DEFINITIONS(-DPROJECT_NAME_EXTENDED="${PROJECT_NAME_EXTENDED}")
ADD_DEFINITIONS(-DPROJECT_NAME="${PROJECT_NAME}")
SET( PROJECT_VERSION_MAJOR 2 )
SET( PROJECT_VERSION_MINOR 0 )
SET( PROJECT_VERSION_PATCH 0 )
SET( PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
ADD_DEFINITIONS(-DPROJECT_VERSION="${PROJECT_VERSION}")
SET(PROJECT_VENDOR "CBICA - UPenn")
SET(LICENSE_FILE "${PROJECT_SOURCE_DIR}/licenses/COPYING.txt")
SET(README_FILE  "${PROJECT_SOURCE_DIR}/README.txt")
SET(PROJECT_DOMAIN "https://www.cbica.upenn.edu/sbia/software/")
SET(PROJECT_CONTACT "software@cbica.upenn.edu")

CMAKE_POLICY(SET CMP0048 NEW) # only with CMake 3
CMAKE_POLICY(SET CMP0025 NEW) # only with CMake 3

SET(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard to be used" )
SET(CMAKE_CXX_STANDARD_REQUIRED YES) 
SET_PROPERTY( GLOBAL PROPERTY USE_FOLDERS ON )

SET( OPENCV_VERSION "3.4.5" CACHE STRING "Superbuild OpenCV version to build CaPTk against" FORCE )

SET( CACHED_INCLUDE_DIRS
  ${CACHED_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/classes
  ${CMAKE_CURRENT_SOURCE_DIR}/src/classes/itk
  ${CMAKE_CURRENT_SOURCE_DIR}/src/classes/itk/IO/inc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/classes/opencv
  ${CMAKE_CURRENT_SOURCE_DIR}/src/classes/torch
  CACHE STRING "All include directories"
)
#MESSAGE( STATUS "[DEBUG] CACHED_INCLUDE_DIRS@Macro: ${CACHED_INCLUDE_DIRS}" )

FILE( GLOB_RECURSE CURRENT_APPLICATION_DEPENDS "${PROJECT_SOURCE_DIR}/src/depends/*" )
FILE( GLOB CURRENT_APPLICATION_SOURCES "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}*" )

SET( APPLICATION_SOURCES ${CURRENT_APPLICATION_SOURCES} CACHE STRING "All application sources" FORCE )
SET( APPLICATION_DEPENDS ${CURRENT_APPLICATION_DEPENDS} CACHE STRING "All application depends" FORCE )

# set the install path
IF( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
  SET( CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install CACHE PATH "Changed the default install path" FORCE  )
ENDIF( )

#  set cmake up
IF(WIN32)
  SET(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS 1)
  SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)
  SET(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES 1)
  SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1)
  SET(CMAKE_C_RESPONSE_FILE_LINK_FLAG "@")
  SET(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")
  SET(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>") 
  SET(CMAKE_CXX_CREATE_STATIC_LIBRARY "<CMAKE_AR> rc <TARGET> <LINK_FLAGS> <OBJECTS>") 
ENDIF()

IF(NOT WIN32)
  SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDIF()

# find openmp flags
IF(APPLE)
  SET(OPENMP_LIBRARIES "${CMAKE_C_COMPILER}/../../lib")
  SET(OPENMP_INCLUDES "${CMAKE_C_COMPILER}/../../include")
  
  # MESSAGE( AUTHOR_WARNING "Apple_C_Compiler: ${CMAKE_C_COMPILER}")
  
  SET(OpenMP_C "${CMAKE_C_COMPILER}")
  SET(OpenMP_C_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
  SET(OpenMP_C_LIB_NAMES "libomp" "libgomp" "libiomp5")
  SET(OpenMP_libomp_LIBRARY ${OpenMP_C_LIB_NAMES})
  SET(OpenMP_libgomp_LIBRARY ${OpenMP_C_LIB_NAMES})
  SET(OpenMP_libiomp5_LIBRARY ${OpenMP_C_LIB_NAMES})
  SET(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
  SET(OpenMP_CXX_FLAGS "-fopenmp=libomp -Wno-unused-command-line-argument")
  SET(OpenMP_CXX_LIB_NAMES "libomp" "libgomp" "libiomp5")
  SET(OpenMP_libomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
  SET(OpenMP_libgomp_LIBRARY ${OpenMP_CXX_LIB_NAMES})
  SET(OpenMP_libiomp5_LIBRARY ${OpenMP_CXX_LIB_NAMES})
  
  INCLUDE_DIRECTORIES("${OPENMP_INCLUDES}")
  LINK_DIRECTORIES("${OPENMP_LIBRARIES}")

ELSE()
  # ensure OpenMP flag is enabled throughout the project 
  FIND_PACKAGE(OpenMP REQUIRED)
  SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}" )
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" )
ENDIF()

SET( CMAKE_CXX_STANDARD 14 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )

#MESSAGE(AUTHOR_WARNING "OpenMP is enabled, you may see issues on Azure")

# add a few compiler options for GCC and MSVC
IF( CMAKE_COMPILER_IS_GNUCXX )
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -O3" )
ENDIF()

if(APPLE)
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-dev -Wno-narrowing -Wall" )
endif()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake_modules/)

# update submodule
IF( NOT EXISTS "${PROJECT_SOURCE_DIR}/CmdParser/CMakeLists.txt" )
  EXECUTE_PROCESS(COMMAND git submodule update --init --recursive ${PROJECT_SOURCE_DIR}/CmdParser
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  RESULT_VARIABLE git_result
  OUTPUT_VARIABLE git_ver)
ENDIF()
ADD_SUBDIRECTORY( CmdParser )
SET( CACHED_INCLUDE_DIRS
  ${CACHED_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/CmdParser/include
  CACHE STRING "All include directories" FORCE
)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/CmdParser/include)

SET_TARGET_PROPERTIES( CmdParser PROPERTIES FOLDER "CBICA_Toolkit")

## Do you want to build the ITK classes as well?
SET( BUILD_CBICA_ITK_CLASSES ON CACHE BOOL "Do you want to build the CBICA ITK classes (needs ITK 4.10.0+)?" )
IF( BUILD_CBICA_ITK_CLASSES )
  ADD_DEFINITIONS( -DBUILD_CBICA_ITK_CLASSES=1 )
  
  FIND_PACKAGE(ITK)
  
  IF(NOT EXISTS ${ITK_USE_FILE})
    
    IF( EXISTS ${PROJECT_BINARY_DIR}/ITK-build/ITKConfig.cmake )
    
      MESSAGE( STATUS "Found ITK in the build directory; using it" )
    
      SET( ITK_DIR "${PROJECT_BINARY_DIR}/ITK-build/" CACHE STRING "ITK_DIR Path for use in other builds" FORCE )
      SET( ENV{CMAKE_PREFIX_PATH} "${PROJECT_BINARY_DIR}/ITK-build/:${PROJECT_BINARY_DIR}/OpenCV-build/:$ENV{CMAKE_PREFIX_PATH}" CACHE PATH "" FORCE )
      #MESSAGE( AUTHOR_WARNING "=== [DEBUG] ENV{CMAKE_PREFIX_PATH}=$ENV{CMAKE_PREFIX_PATH}" )

    ELSE()
      MESSAGE( STATUS "Starting Superbuild" )
    
      IF( NOT EXISTS ${PROJECT_BINARY_DIR}/ITK-build/ITKConfig.cmake )
      
        PROJECT(${PROJECT_NAME}_Depends)
        INCLUDE( ${PROJECT_SOURCE_DIR}/cmake_modules/Superbuild.cmake )
        return()

      ENDIF()
    ENDIF()
    
  ENDIF()
  
ELSE()
  ADD_DEFINITIONS( -DBUILD_CBICA_ITK_CLASSES=0 )
ENDIF()

## Do you want to build the OpenCV classes as well?
SET( BUILD_CBICA_OPENCV_CLASSES OFF CACHE BOOL "Do you want to build the CBICA OpenCV classes (needs OpenCV 3.0+)?" )
IF( BUILD_CBICA_OPENCV_CLASSES )
  ADD_DEFINITIONS( -BUILD_CBICA_OPENCV_CLASSES=1 )
  
  # Try to find OpenCV; if not found, perform superbuild
  FIND_PACKAGE( OpenCV )
  IF( OpenCV_FOUND )

    SET( CACHED_INCLUDE_DIRS
        ${CACHED_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS} 
      )
      
  ELSE()
    IF( NOT EXISTS ${PROJECT_BINARY_DIR}/OpenCV-build/OpenCVConfig.cmake )
    
      MESSAGE( STATUS "Starting Superbuild" )
  
      PROJECT(${PROJECT_NAME}_Depends)
      INCLUDE( ${PROJECT_SOURCE_DIR}/cmake_modules/Superbuild.cmake )
      return()
    ENDIF()
  ENDIF()

ELSE()
  ADD_DEFINITIONS( -BUILD_CBICA_OPENCV_CLASSES=0 )
ENDIF()

# Find all *.h and *.cpp files in a folder -> useful when the application is purely command line based
SET( SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src )
SET( BASIC_CLASS_DIRECTORY ${SOURCE_DIRECTORY}/classes )
SET( THIRDPARTY_LIB_PATH ${SOURCE_DIRECTORY}/thirdparty )
INCLUDE_DIRECTORIES( ${THIRDPARTY_LIB_PATH} )

# macro to find all sub-directories 
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children 
    RELATIVE ${curdir} ${curdir}/* 
    PATTERN "svn" EXCLUDE
  )
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

### third party libary processing
# find and include all third party libraries
SUBDIRLIST(SUBDIRECTORIES ${THIRDPARTY_LIB_PATH})

FOREACH(thirdParty_include ${SUBDIRECTORIES})

  ADD_SUBDIRECTORY( ${THIRDPARTY_LIB_PATH}/${thirdParty_include} )
  SET( CACHED_INCLUDE_DIRS
      ${CACHED_INCLUDE_DIRS}
      ${THIRDPARTY_LIB_PATH}/${thirdParty_include}
    )
    
  IF( EXISTS "${THIRDPARTY_LIB_PATH}/${thirdParty_include}/include" )
    INCLUDE_DIRECTORIES( ${THIRDPARTY_LIB_PATH}/${thirdParty_include}/include )
  ENDIF()
  #SUBDIRLIST(SUBDIRECTORIES_INT ${THIRDPARTY_LIB_PATH})
  #FOREACH(thirdParty_int_include ${THIRDPARTY_LIB_PATH}/${thirdParty_include})
  #  INCLUDE_DIRECTORIES( ${THIRDPARTY_LIB_PATH}/${thirdParty_include}/${thirdParty_int_include} )
  #ENDFOREACH()
  
ENDFOREACH()

### third party libary processing

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

IF( BUILD_CBICA_ITK_CLASSES )

	#ADD_DEFINITIONS( -DBUILD_CBICA_ITK=1 )	
	FIND_PACKAGE( ITK REQUIRED )
  INCLUDE("${ITK_USE_FILE}")
  SET(ITK_FOUND ON CACHE BOOL "ITK was found" )
  ADD_DEFINITIONS(-DITK_FOUND=${ITK_FOUND})

ENDIF( BUILD_CBICA_ITK_CLASSES ) 

IF( BUILD_CBICA_OPENCV_CLASSES )

	#ADD_DEFINITIONS( -DBUILD_CBICA_OPENCV=1 )	
	FIND_PACKAGE( OpenCV 3.0 REQUIRED )
  INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
  
ENDIF( BUILD_CBICA_OPENCV_CLASSES ) 

## Build test(s)
SET( BUILD_TESTING OFF CACHE BOOL "Build testing" )
IF( BUILD_TESTING )
  ENABLE_TESTING()
ENDIF()

ADD_SUBDIRECTORY( ${BASIC_CLASS_DIRECTORY} )

ADD_SUBDIRECTORY( ${SOURCE_DIRECTORY} )

# Done for temporary testing
FILE( GLOB 
  MainSources
  "${SOURCE_DIRECTORY}/*.h"
  "${SOURCE_DIRECTORY}/*.cpp"
  "${BASIC_CLASS_DIRECTORY}/*.h"
  "${BASIC_CLASS_DIRECTORY}/*.cpp"
)

# If Windows is detected, add getopt for command line parsing. Will be deprecated in favor of cbica::CmdParser
IF( WIN32 )
	SET( MainSources
		${MainSources}
		${SOURCE_DIRECTORY}/windows/getopt.h
		${SOURCE_DIRECTORY}/windows/getopt.cpp
		${SOURCE_DIRECTORY}/windows/dirent.h
	)
ENDIF( WIN32 )

SET( INCLUDE_DIR_CONFIG "\"${PROJECT_SOURCE_DIR}/src/classes\"" )
#GET_TARGET_PROPERTY( LIBRARY_CONFIG CBICA_Classes LOCATION )
SET( LIBRARY_CONFIG "\"${LIBRARY_CONFIG}\"" )

# Building the documentation
SET( BUILD_DOCUMENTATION OFF CACHE BOOL "Do you want to build Documentation (needs Doxygen)?" )
IF( BUILD_DOCUMENTATION )

  SET( CBICA_UPDATE_DOCS OFF CACHE BOOL "Do you want to update the documentation for the website?" )

  ADD_SUBDIRECTORY( docs_simple ) # this is plain doxygen
  #ADD_SUBDIRECTORY( docs ) # this is the one that uses Sphinx
  #ADD_SUBDIRECTORY( docs_internal ) # only used for internal documentation 
ENDIF ()

OPTION( SKIP_INSTALL "Whether to perform the installation for the config files or not: not needed if getting built as submodule" OFF )

### Python wrapping
SET( BUILD_PYTHON_WRAP OFF CACHE BOOL "Do you want to wrap classes (needs SWIG 3.0.4)? Ensure build option matches Python lib build." )
IF( BUILD_PYTHON_WRAP )
  IF(WIN32)
    MESSAGE( STATUS "If Python has not been built on Debug mode (i.e., pythonXX_d.lib is NOT present), then only build library in Release." )
  ENDIF()
  ADD_SUBDIRECTORY( wrapping )
ENDIF ()

INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${BASIC_CLASS_DIRECTORY}
	${SOURCE_DIRECTORY}
  ${PROJECT_SOURCE_DIR}/src/thirdparty/yaml-cpp	
	#${Boost_INCLUDE_DIRS}
)

SET( EXE_NAME ${PROJECT_NAME} CACHE STRING "Executable name" FORCE)

SET( PACKAGE_NAME ${PROJECT_NAME} )

IF( WIN32 )
  SET( INSTALL_CONFIG_DIR "cmake" )
ELSE()
  SET( INSTALL_CONFIG_DIR "lib" )
ENDIF()

FILE( RELATIVE_PATH DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_CONFIG_DIR}" "${CMAKE_INSTALL_PREFIX}" )
STRING( REGEX REPLACE "/$|\\$" "" DIR "${DIR}" )

SET( INSTALL_DIR_CONFIG "${CMAKE_INSTALL_PREFIX}" CACHE INTERNAL "Install location")

#SET( INCLUDE_DIR_CONFIG "\"\${CMAKE_CURRENT_LIST_DIR}/include\"" )
SET( INCLUDE_DIR_CONFIG "\"${INSTALL_DIR_CONFIG}/include/\"" )
#GET_TARGET_PROPERTY( LIBRARY_CONFIG ${PROJECT_NAME} LOCATION )
GET_FILENAME_COMPONENT( LIBRARY_CONFIG "${LIBRARY_CONFIG}" NAME )
SET( LIBRARY_CONFIG "\"${INSTALL_DIR_CONFIG}/lib/${LIBRARY_CONFIG}\"" )

CONFIGURE_FILE( "config/Config.cmake.in" "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}InstallConfig.cmake" @ONLY )
CONFIGURE_FILE( "config/Use.cmake.in" "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}Use.cmake" @ONLY )
CONFIGURE_FILE( "config/ConfigVersion.cmake.in" "${PROJECT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake" @ONLY )

IF( NOT SKIP_INSTALL )

  INSTALL(
    FILES       ${PROJECT_BINARY_DIR}/${PACKAGE_NAME}InstallConfig.cmake 
    DESTINATION ${INSTALL_DIR_CONFIG}
    RENAME      ${PACKAGE_NAME}Config.cmake
  )

  INSTALL(
    FILES       ${PROJECT_BINARY_DIR}/${PACKAGE_NAME}Use.cmake
    DESTINATION ${INSTALL_DIR_CONFIG}
  )

  INSTALL(
    FILES       ${PROJECT_BINARY_DIR}/${PACKAGE_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_DIR_CONFIG}
  )

ENDIF()
