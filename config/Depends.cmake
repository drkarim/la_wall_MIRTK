if (MIRTK_Image_WITH_VTK)
  basis_find_package(VTK 7|6 REQUIRED COMPONENTS vtkCommonCore vtkCommonDataModel)
endif ()
