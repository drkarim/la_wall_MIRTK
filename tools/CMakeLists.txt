##############################################################################
# @file  CMakeLists.txt
# @brief Build configuration of MIRTK SliceDice tools.
##############################################################################

mirtk_add_executable(
  evaluate-la-wall
  DEPENDS
    LibCommon
    LibNumerics
    LibImage
    LibIO
)
