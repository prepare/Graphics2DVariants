
/*******************************************************************\

This file is part of the AggOO library.  AggOO provides a way to emulate
GDI+ using Anti-Grain Geometry (http://www.antigrain.com) as the rendering
backend.

AggOO - Version 0.1
Copyright (c) 2006 Dratek Software

Anti-Grain Geometry - Version 2.4 
Copyright (C) 2002-2005 Maxim Shemanarev (McSeem) 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

  1. Redistributions of source code must retain the above copyright 
     notice, this list of conditions and the following disclaimer. 

  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in 
     the documentation and/or other materials provided with the 
     distribution. 

  3. The name of the author may not be used to endorse or promote 
     products derived from this software without specific prior 
     written permission. 

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.

\*******************************************************************/

/////////////////////////////////////////////////////////////////////
// $Id: AggOOBitmapExt.h,v 1.6 2008/10/24 14:36:31 dratek Exp $
//
// AggOOBitmapExt.h -- An extended version of the Bitmap class
//
// Author       : Chad M. Draper
// Date         : October 26, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-10-26      Initial implementation
//

#ifndef AggOOBitmapExt_H_
#define AggOOBitmapExt_H_

#include "AggOOBitmap.h"
#include "AggOOColor.h"

namespace AggOO
{
    /** @class BitmapExt

        BitmapExt is an extended version of the Bitmap class, including methods
        not in Gdiplus::Bitmap.
    */
    class _AggOOExport BitmapExt : public Bitmap
    {
    public:
        /** Constructor */
        BitmapExt( INT width, INT height, PixelFormat format = PixelFormat32bppARGB );
        /** Constructor */
        BitmapExt( INT width, INT height, INT stride, PixelFormat format, BYTE *scan0 );
        /** Constructor */
        BitmapExt( INT width, INT height, Graphics *target );

#if ( AGGOO_ENABLE_IMAGE_LOADING == 1 || AGGOO_ENABLE_VECTOR_LOADING == 1 )
        BitmapExt( const CHAR* filename, BOOL useIcm = FALSE ) : Bitmap( filename, useIcm ) { }
        BitmapExt( const WCHAR* filename, BOOL useIcm = FALSE ) : Bitmap( filename, useIcm ) { }
#endif // AGGOO_ENABLE_IMAGE_LOADING || AGGOO_ENABLE_VECTOR_LOADING

        /** Clear the bitmap to a specified color */
        Status Clear( const Color& bgCol = Color( 0, 0, 0, 0 ) );

        /** Invert the RGB values in the bitmap */
        Status InvertRGB();

        /** Invert the RGBA values in the bitmap */
        Status InvertRGBA();

        /** Mask a source bitmap with a mask bitmap and store the result here

            @param  src                     Pointer to the source image
            @param  mask                    Pointer to the mask image
            @param  invertMask              If true, then wherever the mask has alpha > 0,
                                            the source pixel is kept.  If false, keep the
                                            source pixel where alpha == 0.
        */
        Status Mask( Bitmap *src, Bitmap *mask, BOOL invertMask = 0 );
        /** Mask a source bitmap with a mask bitmap and store the result here

            @param  src                     Pointer to the source image
            @param  mask                    Pointer to the mask image
            @param  x                       X-coordinate of the region to mask
            @param  y                       Y-coordinate of the region to mask
            @param  width                   Width of the mask region
            @param  height                  Height of the mask region
            @param  invertMask              If true, then wherever the mask has alpha > 0,
                                            the source pixel is kept.  If false, keep the
                                            source pixel where alpha == 0.
        */
        Status Mask( Bitmap *src, Bitmap *mask, unsigned int x, unsigned int y, unsigned int width, unsigned int height, BOOL invertMask = 0 );

        /** @brief Flood fill a region

            Replaces all adjacent pixels, starting at the specified point, with
            the new color.

            @param  x                       X coordinate of the location to start filling
            @param  y                       Y coordinate of the location to start filling
            @param  newColor                Color to fill the region with
        */
        Status FloodFill( INT x, INT y, const Color& newColor );

    private:
        /** Helper function for recursively filling a region. */
        Status floodFill( BitmapData* bmpData, INT x, INT y, const Color& newColor, const Color& oldColor );
        /** Set a pixel in an ARGB buffer */
        Status setPixelARGB( BitmapData* bmpData, INT x, INT y, const Color& col );
        /** Get a pixel from an ARGB buffer */
        Status getPixelARGB( const BitmapData* bmpData, INT x, INT y, Color* col );

        /** @remarks

        // TODO:
        BitmapExt( const WCHAR* filename, BOOL useIcm = FALSE ) : Bitmap( filename, useIcm )    { }
        BitmapExt( const BITMAPINFO* gdiBitmapInfo, void * gdiBitmapData ) : Bitmap( gdiBitmapInfo, gdiBitmapData )     { }
        BitmapExt( HBITMAP hbm, HPALETTE hpal ) : Bitmap(hbm, hpal)                             { }
        BitmapExt( HICON hicon ) : Bitmap(hicon)                                                { }
        BitmapExt( HINSTANCE hInstance, const WCHAR* bitmapName ) : Bitmap(hInstance, bitmapName)   { }
        BitmapExt( IStream* stream, BOOL useIcm = FALSE ) : Bitmap(stream, useIcm)              { }
        BitmapExt( IDirectDrawSurface7* surface ) : Bitmap(surface)                             { }

        */

    };

} // namespace AggOO

#endif  // AggOOBitmapExt_H_
