// @(#) $Id$

#ifndef ALILEVEL3_H
#define ALILEVEL3_H

#ifndef no_root
#include <TObject.h>
#include <TFile.h>
#endif

#include "AliL3DigitData.h"
#include "AliL3RootTypes.h"

class AliL3SpacePointData;
class AliL3DigitRowData;
class AliL3TrackSegmentData;
class AliL3DigitData;
class AliL3ConfMapper;
class AliL3Vertex;
class AliL3VertexFinder;
class AliL3TrackMerger;
class AliL3GlobalMerger;
#ifndef no_root
class TDirectory;
#endif
class AliL3ClustFinderNew;
class AliL3Merger;
class AliL3InterMerger;

//#ifdef use_aliroot
class AliL3FileHandler;
//#else
class AliL3MemHandler;
//#endif
class AliL3Benchmark;

#ifdef no_root
class AliLevel3 {
#else
class AliLevel3 : public TObject {
#endif

 private:
  UInt_t fNTrackData;
  AliL3TrackSegmentData* fTrackData; //!
  AliL3ConfMapper *fTracker; //!
  AliL3Vertex *fVertex; //! 
  AliL3VertexFinder *fVertexFinder; //!
  AliL3TrackMerger *fTrackMerger; //!
  AliL3GlobalMerger *fGlobalMerger; //!
  AliL3InterMerger *fInterMerger; //!
  AliL3ClustFinderNew *fClusterFinder; //! 
  //#ifdef use_aliroot
  //AliL3FileHandler *fFileHandler; //!
  ///#else
  AliL3MemHandler *fFileHandler; //!
  //#endif
  AliL3Benchmark *fBenchmark;//!

  Int_t fEvent;
  Int_t fNPatch;
  Int_t fRow[6][2];
  Float_t fEta[2];
  
#ifdef no_root
  FILE *fInputFile;
#else
  Char_t *fInputFile;//!
  TDirectory *savedir;
  //TFile *fInputFile;
#endif

  Char_t fPath[256];
  Char_t fWriteOutPath[256];
  
  Bool_t fDoRoi;
  Bool_t fFindVertex;
  Bool_t fDoNonVertex;
  Bool_t fPileUp;
  Bool_t fNoCF;
  
  Bool_t fUseBinary;
  Bool_t fWriteOut;

  Bool_t fClusterDeconv;
  Float_t fXYClusterError;
  Float_t fZClusterError;


  void WriteSpacePoints(UInt_t npoints,AliL3SpacePointData *points,
                                          Int_t slice,Int_t patch);
  Int_t WriteTracks(char *filename,AliL3Merger *merger,char opt='o');  
  void WriteResults();
  void FitGlobalTracks();
  void SetPath(char *p){sprintf(fPath,"%s",p);}

 public:
  AliLevel3 ();
  AliLevel3(Char_t *infile);
  //AliLevel3(TFile *in);
  virtual ~AliLevel3();
  enum EFileType {kBinary, kBinary8, kRoot, kRaw};
  void Init(Char_t *path,EFileType filetype=kBinary,Int_t npatches=6);
  //void Init(Char_t *path,Bool_t binary=kTRUE,Int_t npatches=6);
  void SetMergerParameters(Double_t maxy=1.2,Double_t maxz=1.6,Double_t maxkappa=0.003,
			   Double_t maxpsi=0.02,Double_t maxtgl=0.03);
  void SetTrackerParam(Int_t phi_segments=50,Int_t eta_segments=100,
		       Int_t trackletlength=3,Int_t tracklength=5,
		       Int_t rowscopetracklet=2,Int_t rowscopetrack=3,
		       Double_t min_pt_fit=0,Double_t maxangle=1.31,
		       Double_t goodDist=5,Double_t hitChi2Cut=10,
		       Double_t goodHitChi2=20,Double_t trackChi2Cut=50,
		       Int_t maxdist=50,Double_t maxphi=0.1,Double_t maxeta=0.1,
                       Bool_t vertexconstraint=kTRUE);
  void SetClusterFinderParam(Float_t fXYError=0.2,Float_t fZError=0.3,Bool_t deconv=kTRUE);

  void ProcessEvent(Int_t first,Int_t last,Int_t event=0);
  void ProcessSlice(Int_t slice);

  void DoMc(char* file="point_mc.dat");
  void DoNonVertexTracking() {fDoNonVertex=kTRUE;}
  void FindVertex() {fFindVertex=kTRUE;}
  void DoBench(char* name="benchmark");
  void DoPileup() {fPileUp = kTRUE;}
  void NoCF() {fNoCF=kTRUE;}
  void DoRoi(Float_t e0=0.4,Float_t e1=0.5){fEta[0]=e0;fEta[1]=e1;fDoRoi=kTRUE;}
  void WriteFiles(Char_t *path="./"){fWriteOut = kTRUE; sprintf(fWriteOutPath,"%s",path);}
  //void UseBinaryInput(char *path){SetPath(path);fUseBinary=kTRUE;}

  ClassDef(AliLevel3,1) //Interface class for Level3-tracking
};

#endif





