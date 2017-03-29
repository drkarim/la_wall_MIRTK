################################################################################
# @file  BasisProject.cmake
# @brief Sets basic information about the MIRTK module and calls basis_project().
#
# This file defines basic information about a project by calling 
# the basis_project() function. This basic information, also known as metadata, 
# is used by CMake BASIS to setup the project. The dependencies to other modules
# have to be specified here such that the top-level IRTK project can analyze the
# inter-module dependencies, as well as dependencies on third-party libraries.
#
# @sa https://cmake-basis.github.io/standard/modules.html
#
# @ingroup BasisSettings
################################################################################

# Note: The #<*> dependency patterns are required by the basisproject tool and
#       should be kept on a separate line as last commented argument of the
#       corresponding options of the basis_project() command. The TEMPLATE
#       option and set argument are also required by this tool and should not
#       be changed manually. The argument is updated by basisproject --update.

basis_project (

  # ----------------------------------------------------------------------------
  # meta-data
  NAME        "SliceDice"
  VERSION     "0.0.0" # version of this module
  SOVERSION   "0"     # API yet unstable
  PACKAGE     "MIRTK"
  AUTHORS     "Rashed Karim"
  DESCRIPTION "MIRTK command to compute slice by slice Dice overlap of binary masks."
  COPYRIGHT   "2017 Rashed Karim"
  LICENSE     "Apache License Version 2.0"
  CONTACT     "Rashed Karim <rashed.karim@kcl.ac.uk>"
  TEMPLATE    "mirtk-module/1.0"

  # ----------------------------------------------------------------------------
  # dependencies
  TOOLS_DEPENDS
    MIRTK{Common,Numerics,Image,IO}
  OPTIONAL_TOOLS_DEPENDS
    TBB{tbb}
)
