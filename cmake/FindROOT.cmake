# **************************************************************************
# * Copyright(c) 1998-2014, ALICE Experiment at CERN, All rights reserved. *
# *                                                                        *
# * Author: The ALICE Off-line Project.                                    *
# * Contributors are mentioned in the code where appropriate.              *
# *                                                                        *
# * Permission to use, copy, modify and distribute this software and its   *
# * documentation strictly for non-commercial purposes is hereby granted   *
# * without fee, provided that the above copyright notice appears in all   *
# * copies and that both the copyright notice and this permission notice   *
# * appear in the supporting documentation. The authors make no claims     *
# * about the suitability of this software for any purpose. It is          *
# * provided "as is" without express or implied warranty.                  *
# **************************************************************************

# Checking for a proper ROOT installation based on the ROOTSYS variable
# received during configuration
# If proper root installation it is setting the following variables
# - ROOT_VERSION - ROOT version as reported by root-config
# - ROOT_VERSION_MAJOR
# - ROOT_VERSIOM_MINOR
# - ROOT_VERSION_PATCH
# - ROOT_CONFIG - path to root-config script
# - ROOT_CINT - path to rootcint executable
# - ROOT_FEATURES - list of build features for ROOT
# - ROOT_LIBDIR - full path to ROOT library folder
# - ROOT_LIBRARIES - libraries needed for the package to be used
# - ROOT_INCLUDE_DIRS - full path to ROOT include folder
# - ROOT_GLIBS - regular + GUI ROOT libraries + path to be used during linking
# - ROOT_HASALIEN - ROOT was built with AliEn support
# - ROOT_HASXML - ROOT was built with XML support


if(ROOTSYS)
    message(STATUS "Checking for a proper ROOT installation in ${ROOTSYS}.")

    # Setting the LD_LiBRARY_PATH to point to ROOT lib folder
    set(ROOT_LIBDIR ${ROOTSYS}/lib)
    set(ROOT_INCLUDE_DIRS ${ROOTSYS}/include)

    # Check for root-config scripts
    find_program(ROOT_CONFIG NAMES root-config PATHS ${ROOTSYS}/bin NO_DEFAULT_PATH)

    if(ROOT_CONFIG)
        message(STATUS "Found root-config: ${ROOT_CONFIG}")
    else()
        message(FATAL_ERROR "Could not find root-config script.")
    endif(ROOT_CONFIG)

    # Check for rootcint
    find_program(ROOT_CINT NAMES rootcint PATHS ${ROOTSYS}/bin NO_DEFAULT_PATH)
    if(ROOT_CINT)
        message(STATUS "Found ${ROOT_CINT}")
    else()
        message(FATAL_ERROR "Could not find rootcint executable.")
    endif(ROOT_CINT)

    # Checking ROOT version
    execute_process(COMMAND ${ROOT_CONFIG} --version OUTPUT_VARIABLE ROOT_VERSION ERROR_VARIABLE error OUTPUT_STRIP_TRAILING_WHITESPACE )
    if(error)
        message(FATAL_ERROR "Error retrieving ROOT version : ${error}")
    endif(error)

    # Extract major, minor, and patch versions from
    string(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+.*" "\\1" ROOT_VERSION_MAJOR "${ROOT_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" ROOT_VERSION_MINOR "${ROOT_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+).*" "\\1" ROOT_VERSION_PATCH "${ROOT_VERSION}")
    message(STATUS "Found ROOT version ${ROOT_VERSION_MAJOR}.${ROOT_VERSION_MINOR}.${ROOT_VERSION_PATCH}")

    # Print ROOT features
    execute_process(COMMAND ${ROOT_CONFIG} --features OUTPUT_VARIABLE ROOT_FEATURES ERROR_VARIABLE error OUTPUT_STRIP_TRAILING_WHITESPACE )
    if(error)
        message(FATAL_ERROR "Error retrieving ROOT features : ${error}")
    else()
        message(STATUS "ROOT was build with the following features: ${ROOT_FEATURES}")
    endif(error)

    # Checking for ROOT libdir
    execute_process(COMMAND ${ROOT_CONFIG} --libdir OUTPUT_VARIABLE ROOT_LIBDIR ERROR_VARIABLE error OUTPUT_STRIP_TRAILING_WHITESPACE )
    if(error)
        message(FATAL_ERROR "Error retrieving ROOT libdir: ${error}")
    endif(error)

    # Checking for ROOT incdir
    execute_process(COMMAND ${ROOT_CONFIG} --incdir OUTPUT_VARIABLE ROOT_INCDIR ERROR_VARIABLE error OUTPUT_STRIP_TRAILING_WHITESPACE )
    if(error)
        message(FATAL_ERROR "Error retrieving ROOT incdir: ${error}")
    endif(error)

    # Checking for glibs
    execute_process(COMMAND ${ROOT_CONFIG} --glibs OUTPUT_VARIABLE ROOT_GLIBS ERROR_VARIABLE error OUTPUT_STRIP_TRAILING_WHITESPACE )
    if(error)
        message(FATAL_ERROR "Error retrieving ROOT glibs: ${error}")
    endif(error)

    # Checking for glibs
    execute_process(COMMAND ${ROOT_CONFIG} --has-alien OUTPUT_VARIABLE ROOT_HASALIEN ERROR_VARIABLE error OUTPUT_STRIP_TRAILING_WHITESPACE )
    if(error)
        message(FATAL_ERROR "Error checking if ROOT was build with AliEn support: ${error}")
    endif(error)

    execute_process(COMMAND ${ROOT_CONFIG} --has-xml OUTPUT_VARIABLE ROOT_HASXML ERROR_VARIABLE error OUTPUT_STRIP_TRAILING_WHITESPACE )
    if(error)
        message(FATAL_ERROR "Error checking if ROOT was build with xml support: ${error}")
    endif(error)

    set(ROOT_FOUND TRUE)
else()
    message(FATAL_ERROR "ROOT installation not found! Please point to the ROOT installation using -DROOTSYS=ROOT_INSTALL_DIR.")
endif(ROOTSYS)