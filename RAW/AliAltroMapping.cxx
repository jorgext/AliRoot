/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// This class handles the mapping of the Altro channels
// The mapping is read from an external mapping files
// The class is used as a base class by TPC,PHOS and FMD
// Author: C.Cheshkov

#include "AliAltroMapping.h"
#include "AliLog.h"
#include <Riostream.h>
//#include <stdlib.h>


ClassImp(AliAltroMapping)

//_____________________________________________________________________________
AliAltroMapping::AliAltroMapping():
  fIn(NULL),
  fNumberOfChannels(0),
  fMaxHWAddress(0)
{
  // Default constructor
}

//_____________________________________________________________________________
AliAltroMapping::AliAltroMapping(const char *mappingFile):
  fIn(NULL),
  fNumberOfChannels(0),
  fMaxHWAddress(0)
{
  // Constructor
  // Reads the mapping from an external file
  if (mappingFile)
    OpenMappingFile(mappingFile);
  else
    AliFatal("Mapping file not specified !");
}

//_____________________________________________________________________________
AliAltroMapping::~AliAltroMapping()
{
  // destructor
}

//_____________________________________________________________________________
AliAltroMapping::AliAltroMapping(const AliAltroMapping& mapping):
  TObject(mapping),
  fIn(mapping.fIn),
  fNumberOfChannels(mapping.fNumberOfChannels),
  fMaxHWAddress(mapping.fMaxHWAddress)
{
// Copy Constructor

  Fatal("AliAltroMapping", "copy constructor not implemented");
}

//_____________________________________________________________________________
AliAltroMapping& AliAltroMapping::operator = (const AliAltroMapping& /*mapping*/)
{
//Assigment operator

  Fatal("operator =", "assignment operator not implemented");
  return *this;
}

//_____________________________________________________________________________
Bool_t AliAltroMapping::OpenMappingFile(const char *mappingFile)
{
  // Initalizes the ALTRO mapping from a file
  // Look at the TPC module for the format of
  // the mapping file
  fIn = new ifstream(mappingFile);
  if (!*fIn) {
    AliFatal(Form("Missing mapping file (%s) !",mappingFile));
    return kFALSE;
  }
  if (!(*fIn >> fNumberOfChannels)) {
    AliFatal(Form("Syntax of the mapping file is wrong (%s) !",mappingFile));
    return kFALSE;
  }
  if (!(*fIn >> fMaxHWAddress)) {
    AliFatal(Form("Syntax of the mapping file is wrong (%s) !",mappingFile));
    return kFALSE;
  }

  return kTRUE;
}

//_____________________________________________________________________________
Bool_t AliAltroMapping::CloseMappingFile()
{
  // Closes the external mapping
  // file
  if (fIn) {
    fIn->close();
    fIn = NULL;
  }

  return kTRUE;
}
