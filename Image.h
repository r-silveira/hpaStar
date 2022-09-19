#ifndef _GL_WIDGETS_IMAGE_H_
#define _GL_WIDGETS_IMAGE_H_

//-----------------------------------------------------------------------------
//-- includes -----------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifdef _UNICODE
#undef _UNICODE
#endif //_UNICODE
#include <IL/il.h>
#include <string>
//-----------------------------------------------------------------------------
using std::string;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//-- classes ------------------------------------------------------------------
//-----------------------------------------------------------------------------
class Image
{
private:
  static int m_nRefCount;
  ILuint m_idImage;

public:
  Image();
  Image(const string& strFilename); // ESSE CONSTRUTOR DÁ PROBLEMA NO GCC !!!
  ~Image();

  const bool LoadFromFilename(const string& strFilename);
  const int GetWidth();
  const int GetHeight();
  const int GetPixelDepth();
  const unsigned char* GetData();
};
//-----------------------------------------------------------------------------

#endif //_GL_WIDGETS_IMAGE_H_
