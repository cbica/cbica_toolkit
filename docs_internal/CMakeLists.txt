#
# target "doc" allows building the documentation with doxygen/dot on WIN32 and Linux
# Creates .chm windows help file if MS HTML help workshop 
# (available from http://msdn.microsoft.com/workshop/author/htmlhelp)
# is installed with its DLLs in PATH.
#
#
# Please note, that the tools, e.g.:
# doxygen, dot, latex, dvips, makeindex, gswin32, etc.
# must be in path.
#
# Note about Visual Studio Projects: 
# MSVS hast its own path environment which may differ from the shell.
# See "Menu Tools/Options/Projects/VC++ Directories" in VS 7.1
#
# author Jan Woetzel 2004-2006
# www.mip.informatik.uni-kiel.de/~jw


FIND_PACKAGE(Doxygen REQUIRED)

# click+jump in Emacs and Visual Studio (for doxy.config) (jw)
IF    (CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
  SET(DOXY_WARN_FORMAT "\"$file($line) : $text \"")
ELSE  (CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
  SET(DOXY_WARN_FORMAT "\"$file:$line: $text \"")
ENDIF (CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")

# we need latex for doxygen because of the formulas
FIND_PACKAGE(LATEX)
IF    (NOT LATEX_COMPILER)
  MESSAGE(STATUS "latex command LATEX_COMPILER not found but usually required. You will probably get warnings and user interaction on doxy run.")
ENDIF (NOT LATEX_COMPILER)
IF    (NOT MAKEINDEX_COMPILER)
  MESSAGE(STATUS "makeindex command MAKEINDEX_COMPILER not found but usually required.")
ENDIF (NOT MAKEINDEX_COMPILER)
IF    (NOT DVIPS_CONVERTER)
  MESSAGE(STATUS "dvips command DVIPS_CONVERTER not found but usually required.")
ENDIF (NOT DVIPS_CONVERTER)

IF   (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in")
	#MESSAGE(STATUS "configured ${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in --> ${CMAKE_CURRENT_BINARY_DIR}/doxy.config")
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in 
    ${CMAKE_CURRENT_BINARY_DIR}/doxy.config
    @ONLY )
	#stuff for the web page
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/basic.css 
    ${CMAKE_CURRENT_BINARY_DIR}/basic.css COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doxygen.css 
    ${CMAKE_CURRENT_BINARY_DIR}/doxygen.css COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/master.css 
    ${CMAKE_CURRENT_BINARY_DIR}/master.css COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/sbia.css 
    ${CMAKE_CURRENT_BINARY_DIR}/sbia.css COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/screen.css 
    ${CMAKE_CURRENT_BINARY_DIR}/screen.css COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/style.css 
    ${CMAKE_CURRENT_BINARY_DIR}/tabs.css COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/tabs.css 
    ${CMAKE_CURRENT_BINARY_DIR}/style.css COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/header.html 
    ${CMAKE_CURRENT_BINARY_DIR}/header.html COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/footer.html 
    ${CMAKE_CURRENT_BINARY_DIR}/footer.html COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/img/pennlogo.gif 
    ${CMAKE_CURRENT_BINARY_DIR}/html/img/pennlogo.gif COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/img/SBIA.png 
    ${CMAKE_CURRENT_BINARY_DIR}/html/img/SBIA.png COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/img/pennlogo.gif 
    ${CMAKE_CURRENT_BINARY_DIR}/img/pennlogo.gif COPYONLY )
	CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/img/SBIA.png 
    ${CMAKE_CURRENT_BINARY_DIR}/img/SBIA.png COPYONLY )
	
	# use (configured) doxy.config from (out of place) BUILD tree:
	SET(DOXY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/doxy.config")
	ELSE (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in")
	# use static hand-edited doxy.config from SOURCE tree:
	SET(DOXY_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config")
		IF   (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config")
			MESSAGE(STATUS "WARNING: using existing ${CMAKE_CURRENT_SOURCE_DIR}/doxy.config instead of configuring from doxy.config.in file.")
		ELSE (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config")
		IF   (EXISTS "${CMAKE_MODULE_PATH}/doxy.config.in")
		  # using template doxy.config.in
			#MESSAGE(STATUS "configured ${CMAKE_CMAKE_MODULE_PATH}/doxy.config.in --> ${CMAKE_CURRENT_BINARY_DIR}/doxy.config")
			CONFIGURE_FILE(${CMAKE_MODULE_PATH}/doxy.config.in 
			${CMAKE_CURRENT_BINARY_DIR}/doxy.config
			@ONLY )
			CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/../README.md 
			${CMAKE_CURRENT_BINARY_DIR}/README.md
			@ONLY )
			SET(DOXY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/doxy.config")
		ELSE (EXISTS "${CMAKE_MODULE_PATH}/doxy.config.in")
			# failed completely...
			MESSAGE(SEND_ERROR "Please create ${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in (or doxy.config as fallback)")
		ENDIF(EXISTS "${CMAKE_MODULE_PATH}/doxy.config.in")

	ENDIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config")
ENDIF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in")

ADD_CUSTOM_TARGET(doc_internal ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG})

# create a windows help .chm file using hhc.exe
# HTMLHelp DLL must be in path!
# fallback: use hhw.exe interactively
IF    (WIN32)
  FIND_PACKAGE(HTMLHelp)
  IF   (HTML_HELP_COMPILER)      
    SET (TMP "${CMAKE_CURRENT_BINARY_DIR}\\docs\\html\\index.hhp")
    STRING(REGEX REPLACE "[/]" "\\\\" HHP_FILE ${TMP} )
    # MESSAGE(SEND_ERROR "DBG  HHP_FILE=${HHP_FILE}")
    
    IF (NOT TARGET_DOC_SKIP_INSTALL)
    # install windows help?
    # determine useful name for output file 
    # should be project and version unique to allow installing 
    # multiple projects into one global directory      
    IF   (EXISTS "${PROJECT_BINARY_DIR}/docs/html/index.chm")
      IF   (PROJECT_NAME)
        SET(OUT "${PROJECT_NAME}")
      ELSE (PROJECT_NAME)
        SET(OUT "Documentation") # default
      ENDIF(PROJECT_NAME)
      IF   (${PROJECT_NAME}_VERSION_MAJOR)
        SET(OUT "${OUT}-${${PROJECT_NAME}_VERSION_MAJOR}")
        IF   (${PROJECT_NAME}_VERSION_MINOR)
          SET(OUT  "${OUT}.${${PROJECT_NAME}_VERSION_MINOR}")
          IF   (${PROJECT_NAME}_VERSION_PATCH)
            SET(OUT "${OUT}.${${PROJECT_NAME}_VERSION_PATCH}")      
          ENDIF(${PROJECT_NAME}_VERSION_PATCH)
        ENDIF(${PROJECT_NAME}_VERSION_MINOR)
      ENDIF(${PROJECT_NAME}_VERSION_MAJOR)
      # keep suffix
      SET(OUT  "${OUT}.chm")
      
      #MESSAGE("DBG ${PROJECT_BINARY_DIR}/docs/html/index.chm \n${OUT}")
      # create target used by install and package commands 
      INSTALL(FILES "${PROJECT_BINARY_DIR}/docs/html/index.chm"
        DESTINATION "doc"
        RENAME "${OUT}"
      )
    ENDIF(EXISTS "${PROJECT_BINARY_DIR}/docs/html/index.chm")
    ENDIF(NOT TARGET_DOC_SKIP_INSTALL)

  ENDIF(HTML_HELP_COMPILER)
  # MESSAGE(SEND_ERROR "HTML_HELP_COMPILER=${HTML_HELP_COMPILER}")
ENDIF (WIN32) 

###copy the html files into doc on install
#~ IF    (UNIX)
  #~ FILE(GLOB htmlFiles "${CMAKE_CURRENT_BINARY_DIR}/html/*")
  #~ INSTALL(FILES ${htmlFiles} DESTINATION "doc/${PROJECT_NAME}_${PROJECT_VERSION}/html" )
 
  #~ FILE(GLOB latexFiles "${CMAKE_CURRENT_BINARY_DIR}/latex/*")
  #~ INSTALL(FILES ${latexFiles} DESTINATION "doc/${PROJECT_NAME}_${PROJECT_VERSION}/latex" )
 
