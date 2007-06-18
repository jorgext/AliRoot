// $Header$

#include "StraightLineSetGL.h"
#include <Reve/StraightLineSet.h>
#include <Reve/GLUtilNS.h>

#include <TGLDrawFlags.h>

#ifdef WIN32
#include "Windows4root.h"
#endif
#include <GL/gl.h>
#include <GL/glu.h>

using namespace Reve;

//______________________________________________________________________
// StraightLineSetGL
//

ClassImp(StraightLineSetGL)

StraightLineSetGL::StraightLineSetGL() : TGLObject(), fM(0)
{
  // fCached = false; // Disable display list.
}

StraightLineSetGL::~StraightLineSetGL()
{}

/**************************************************************************/

Bool_t StraightLineSetGL::SetModel(TObject* obj)
{
  if(SetModelCheckClass(obj, StraightLineSet::Class())) {
    fM = dynamic_cast<StraightLineSet*>(obj);
    return kTRUE;
  }
  return kFALSE;
}

void StraightLineSetGL::SetBBox()
{
  // !! This ok if master sub-classed from TAttBBox
  SetAxisAlignedBBox(((StraightLineSet*)fExternalObj)->AssertBBox());
}

//______________________________________________________________________________
Bool_t StraightLineSetGL::ShouldCache(const TGLDrawFlags & flags) const
{
   // Override from TGLDrawable.
   // To account for large point-sizes we modify the projection matrix
   // during selection and thus we need a direct draw.

   if (flags.Selection()) return kFALSE;
   return fCached;
}

/**************************************************************************/

void StraightLineSetGL::DirectDraw(const TGLDrawFlags & flags) const
{
  // printf("StraightLineSetGL::DirectDraw Style %d, LOD %d\n", flags.Style(), flags.LOD());

  StraightLineSet& mL = * fM;

  glPushAttrib(GL_POINT_BIT | GL_LINE_BIT | GL_ENABLE_BIT);
  GLUtilNS::GL_Capability_Switch lights_off(GL_LIGHTING, false);
 
  if(mL.fRnrLines && mL.fLinePlex.Size() > 0)
  {
    UChar_t color[4];
    ColorFromIdx(mL.GetMainColor(), color);
    glColor4ubv(color);

    VoidCPlex::iterator li(mL.fLinePlex);
    if(flags.SecSelection()) 
    {  
      GLuint name = 0;
      glPushName(1);
      glPushName(0);  
      while (li.next()) 
      {
	StraightLineSet::Line& l = * (StraightLineSet::Line*) li();
	glLoadName(name);
	{
	  glBegin(GL_LINES);  
	  glVertex3f(l.fV1[0], l.fV1[1], l.fV1[2]);
	  glVertex3f(l.fV2[0], l.fV2[1], l.fV2[2]);
	  glEnd();
	}
	name ++;
      }  
      glPopName(); 
      glPopName();
    } 
    else 
    {
      glBegin(GL_LINES);    
      while (li.next()) 
      {
	StraightLineSet::Line& l = * (StraightLineSet::Line*) li();
	glVertex3f(l.fV1[0], l.fV1[1], l.fV1[2]);
	glVertex3f(l.fV2[0], l.fV2[1], l.fV2[2]);
      }    
      glEnd();
    }
  }

  if(mL.fRnrMarkers && mL.fMarkerPlex.Size() > 0)
  {
    UChar_t color[4];
    ColorFromIdx(mL.GetMarkerColor(), color);
    glColor4ubv(color);

    VoidCPlex::iterator mi(mL.fMarkerPlex);
    Float_t* pnts = new Float_t[mL.fMarkerPlex.Size()*3];
    Float_t* pnt  = pnts;
    Int_t lidx = -1; 
    while (mi.next()) 
    {
      StraightLineSet::Marker& m = * (StraightLineSet::Marker*) mi();
      lidx = m.fLineID;
      StraightLineSet::Line& l = * (StraightLineSet::Line*) mL.fLinePlex.Atom(lidx);
      pnt[0] = l.fV1[0] + (l.fV2[0] - l.fV1[0])*m.fPos;
      pnt[1] = l.fV1[1] + (l.fV2[1] - l.fV1[1])*m.fPos;
      pnt[2] = l.fV1[2] + (l.fV2[2] - l.fV1[2])*m.fPos;;
      pnt   += 3;
    }
    if(flags.SecSelection()) glPushName(2);
    GLUtilNS::RenderPolyMarkers((TAttMarker&)mL, pnts, mL.fMarkerPlex.Size(),
				flags.Selection(), flags.SecSelection());
    if(flags.SecSelection()) glPopName();
    delete [] pnts;
  }

  glPopAttrib();
}

/**************************************************************************/

void StraightLineSetGL::ProcessSelection(UInt_t* ptr, TGLViewer*, TGLScene*)
{ 
  if (ptr[0] != 3) return;
  ptr += 3; // skip n, zmin, zmax
  if(ptr[1] == 1)
  {
    printf("selected line %d\n", ptr[2]);
  }
  else 
  {
   StraightLineSet::Marker& m = * (StraightLineSet::Marker*) fM->fMarkerPlex.Atom(ptr[2]);
   printf("Selected point %d on line %d\n", ptr[2], m.fLineID);
  }
}
