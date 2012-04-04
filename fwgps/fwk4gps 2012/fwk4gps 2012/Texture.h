#ifndef _TEXTURE_H_
#define _TEXTURE_H_

/* Texture Definition - Modelling Layer
 *
 * Texture.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "iTexture.h" // for the Texture Interface
#include <mbstring.h>


//-------------------------------- Texture ------------------------------------
//
// The Texture class defines the structure of a texture in the Modelling Layer
//
class iAPITexture;

class Texture : public iTexture {

	iAPITexture* apiTexture;   // points to the api texture

	//unsigned char opacity;              // new variable for texture opacity [\x00, \xff]

	Texture(const Texture&);
	virtual ~Texture();

  public:
	Texture(const wchar_t* file, unsigned filter = 0);
	//Texture(const wchar_t* file, unsigned char* opac, unsigned filter = 0);
	Texture& operator=(const Texture&);
    void* clone() const { return new Texture(*this); }
	// execution
	void attach() const;
    void setFilter(unsigned) const;
	//void setOpacity(const unsigned char* opac='\x00'){   _mbscpy_s(opacity, opac); };
	void detach();
	void render(const Rectf&, bool, int);

	// termination
	void suspend();
    void release();
};

#endif
