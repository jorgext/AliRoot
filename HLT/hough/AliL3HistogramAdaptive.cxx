// @(#) $Id$

// Author: Anders Vestbo <mailto:vestbo@fi.uib.no>
//*-- Copyright &copy ALICE HLT Group

#include "AliL3StandardIncludes.h"
#include "AliL3Logging.h"
#include "AliL3HistogramAdaptive.h"
#include "AliL3Transform.h"
#include "AliL3Track.h"

#if GCCVERSION == 3
using namespace std;
#endif

//_____________________________________________________________
// AliL3HistogramAdaptive
//
// 2D histogram class adapted for kappa and psi as used in the Circle Hough Transform.
// The bins in kappa is not linear, but has a width which is specified by argument
// ptres in the constructor. This gives the relative pt resolution which should
// be kept throughout the kappa range. 

ClassImp(AliL3HistogramAdaptive)

AliL3HistogramAdaptive::AliL3HistogramAdaptive() : AliL3Histogram()
{
  fKappaBins=0;
}

  
AliL3HistogramAdaptive::AliL3HistogramAdaptive(Char_t *name,Double_t minpt,Double_t maxpt,Double_t ptres,
					       Int_t nybins,Double_t ymin,Double_t ymax)
{
  strcpy(fName,name);
  
  fPtres = ptres;
  fXmin = -1*AliL3Transform::GetBFact()*AliL3Transform::GetBField()/minpt;
  fXmax = AliL3Transform::GetBFact()*AliL3Transform::GetBField()/minpt;

  fMinPt = minpt;
  fMaxPt = maxpt;
  fNxbins = InitKappaBins();
  fNybins = nybins;
  
  fYmin = ymin;
  fYmax = ymax;
  fFirstXbin=1;
  fFirstYbin=1;
  fLastXbin = fNxbins;
  fLastYbin = fNybins;
  fNcells = (fNxbins+2)*(fNybins+2);

  fThreshold=0;
  fContent = new Int_t[fNcells];
  Reset();
}

AliL3HistogramAdaptive::~AliL3HistogramAdaptive()
{
  if(fKappaBins)
    delete [] fKappaBins;
}

Int_t AliL3HistogramAdaptive::InitKappaBins()
{
  //Here a LUT for the kappa values created. This has to be done since
  //the binwidth in kappa is not constant, but change according to the
  //set relative resolution in pt.
  //Since the kappa values are symmetric about origo, the size of the
  //LUT is half of the total number of bins in kappa direction.
  
  Double_t pt = fMinPt,delta_pt,local_pt;
  Int_t bin=0;
  
  while(pt < fMaxPt)
    {
      local_pt = pt;
      delta_pt = fPtres*local_pt*local_pt;
      pt += 2*delta_pt;
      bin++;
    }
  fKappaBins = new Double_t[bin+1];
  pt=fMinPt;
  bin=0;
  fKappaBins[bin] = AliL3Transform::GetBFact()*AliL3Transform::GetBField()/fMinPt; 
  while(pt < fMaxPt)
    {
      local_pt = pt;
      delta_pt = fPtres*local_pt*local_pt;
      pt += 2*delta_pt;                      //*2 because pt +- 1/2*deltapt is one bin
      bin++;
      fKappaBins[bin] = AliL3Transform::GetBFact()*AliL3Transform::GetBField()/pt;
    }
  return (bin+1)*2; //Both negative and positive kappa.
}


void AliL3HistogramAdaptive::Fill(Double_t x,Double_t y,Int_t weight)
{
  Int_t bin = FindBin(x,y);
  if(bin < 0)
    return;
  AddBinContent(bin,weight);

}

Int_t AliL3HistogramAdaptive::FindBin(Double_t x,Double_t y)
{
  
  Int_t xbin = FindXbin(x);
  Int_t ybin = FindYbin(y);
  
  if(!xbin || !ybin) 
    return -1;
  return GetBin(xbin,ybin);
}

Int_t AliL3HistogramAdaptive::FindXbin(Double_t x)
{

  if(x < fXmin || x > fXmax || fabs(x) < fKappaBins[(fNxbins/2-1)])
    return 0;
  
  //Remember that kappa value is decreasing with bin number!
  //Also, the bin numbering starts at 1 and ends at fNxbins,
  //so the corresponding elements in the LUT is bin - 1.

  Int_t bin=0;
  while(bin < fNxbins/2)
    {
      if(fabs(x) <= fKappaBins[bin] && fabs(x) > fKappaBins[bin+1])
	break;
      bin++;
    }
  if(x < 0)
    return bin + 1;
  else 
    return fNxbins - bin;
  
}

Int_t AliL3HistogramAdaptive::FindYbin(Double_t y)
{
  if(y < fYmin || y > fYmax)
    return 0;
  
  return 1 + (Int_t)(fNybins*(y-fYmin)/(fYmax-fYmin));
}

Double_t AliL3HistogramAdaptive::GetBinCenterX(Int_t xbin)
{
  if(xbin < fFirstXbin || xbin > fLastXbin)
    {
      LOG(AliL3Log::kWarning,"AliL3HistogramAdaptive::GetBinCenterX","Bin-value")
	<<"XBinvalue out of range "<<xbin<<ENDLOG;
      return 0;
    }
  
  //The bin numbers go from 1 to fNxbins, so the corresponding
  //element in the LUT is xbin - 1. This is the reason why we 
  //substract a 1 here:
  
  Int_t bin = xbin;
  bin -= 1;
  if(bin >= fNxbins/2)
    bin = fNxbins - 1 - bin;
  
  //Remember again that the kappa-values are _decreasing_ with bin number.
  
  Double_t binwidth = fKappaBins[bin] - fKappaBins[bin+1];
  Double_t kappa = fKappaBins[bin] - 0.5*binwidth;
  if(xbin < fNxbins/2)
    return -1.*kappa;
  else
    return kappa;

}

Double_t AliL3HistogramAdaptive::GetBinCenterY(Int_t ybin)
{
  if(ybin < fFirstYbin || ybin > fLastYbin)
    {
      LOG(AliL3Log::kError,"AliL3HistogramAdaptive::GetBinCenterY","ybin")
	<<"Bin-value out of range "<<ybin<<ENDLOG;
      return -1;
    }
  Double_t binwidth = (fYmax - fYmin) / fNybins;
  return fYmin + (ybin-0.5) * binwidth;

}


void AliL3HistogramAdaptive::Draw(Char_t *option)
{
#ifdef use_root
  if(!fRootHisto)
    CreateRootHisto();
  
  Double_t kappa,psi;
  Int_t content,bin;
  for(Int_t i=fFirstXbin; i<=fLastXbin; i++)
    {
      kappa = GetBinCenterX(i);
      for(Int_t j=fFirstYbin; j<=fLastYbin; j++)
	{
	  psi = GetBinCenterY(j);
	  bin = GetBin(i,j);
	  content = GetBinContent(bin);
	  fRootHisto->Fill(kappa,psi,content);
	}
    }
  fRootHisto->Draw(option);
  return;
#endif
  cerr<<"AliL3HistogramAdaptive::Draw : You need to compile with ROOT in order to draw histogram"<<endl;
}

void AliL3HistogramAdaptive::Print()
{
  cout<<"Printing content of histogram "<<fName<<endl;
  for(Int_t i=0; i<fNcells; i++)
    {
      if(GetBinContent(i)==0) continue;
      cout<<"Bin "<<i<<": "<<GetBinContent(i)<<endl;
    }

}
