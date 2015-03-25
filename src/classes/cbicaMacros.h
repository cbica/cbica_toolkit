/**
\file  cbicaMarcos.h

\brief Contains common macros to be used

https://www.cbica.upenn.edu/sbia/software/
sbia-software@uphs.upenn.edu

Copyright (c) 2015 University of Pennsylvania. All rights reserved.
See COPYING file or https://www.cbica.upenn.edu/sbia/software/license.html

*/
#pragma once

#define cbicaEchoVersionMacro( N, R, SVN_R, SVN_FV ) \
{ \
  std::cout << std::endl << N << std::endl << \
  INDENT << " Release          : " << R << std::endl << \
  INDENT << " Svn Revision     : " << SVN_R << std::endl << \
  INDENT << " Svn File verions : " << SVN_FV << std::endl \
  << std::endl; \
}
