
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
// $Id: AggOOBitmap.h,v 1.8 2010/03/12 04:20:57 dratek Exp $
//
// AggOOBitmap.h -- Defines a bitmap class, which is an image where the pixels
//                  can be manipulated.
//
// Author       : Chad M. Draper
// Date         : September 8, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-08      Initial implementation
//

#ifndef AggOOBitmap_H_
#define AggOOBitmap_H_

#include "AggOOImage.h"

namespace AggOO
{
    /**
        @class Bitmap

        The Bitmap class is an image where the pixels data can be modified
        (plotted.)
    */
    class _AggOOExport Bitmap : public Image
    {
    protected:
        /** Constructor */
        Bitmap() : Image( ImageTypeBitmap, Ok )     { }

    protected:
        Bitmap( const Bitmap& src ) : Image( src ) { }
        Bitmap& operator=( const Bitmap& src ) { return *this; }

        /** Copy a region of pixels from a source Bitmap
        */
        void copyPixelDataFrom( const Rect& rect, const Bitmap* src );

    public:
        /** Constructor */
        Bitmap( INT width, INT height, PixelFormat format = PixelFormat32bppARGB );
        /** Constructor */
        Bitmap( INT width, INT height, INT stride, PixelFormat format, BYTE* scan0 );
        /** Constructor */
        Bitmap( INT width, INT height, Graphics* target );

        /** Destructor */
        ~Bitmap(void);

#if ( AGGOO_ENABLE_IMAGE_LOADING == 1 || AGGOO_ENABLE_VECTOR_LOADING == 1 )
        /** GDI+ does not have image loading with regular ascii strings, but
            DevIL uses them to load images, so why not provide handlers for them
            directly?
            @param  filename    Path to the file to load
            @param  useIcm      (Currently unused in AggOO.  Default is FALSE.)
        */
        Bitmap( const CHAR* filename, BOOL useIcm = FALSE );

        /** Load an image to a specific sized Image.
            @param  filename    Path to the file to load
            @param  destW       Width of the created Image
            @param  destH       Height of the created Image
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)

            @note   GDI+ does not use this method
        */
        Bitmap( const CHAR* filename, INT destW, INT destH, PixelFormat destFormat, BOOL useEmbeddedColorManagement = FALSE );

        /** Create a Bitmap from a file
            @param  filename    Path to the file to load
            @param  useIcm      (Currently unused in AggOO.  Default is FALSE.)
        */
        Bitmap( const WCHAR* filename, BOOL useIcm = FALSE );

        /** Load an image to a specific sized Image.
            @param  filename    Path to the file to load
            @param  destW       Width of the created Image
            @param  destH       Height of the created Image
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)

            @note   GDI+ does not use this method
        */
        Bitmap( const WCHAR* filename, INT destW, INT destH, PixelFormat destFormat, BOOL useEmbeddedColorManagement = FALSE );

        /** Create a Bitmap from a file
            @param  filename    Path to the file to load
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)
            @return Pointer to the new Bitmap
        */
        static Bitmap* FromFile( const CHAR* filename, BOOL useEmbeddedColorManagement = FALSE );

        /** Load an image file into a specific sized Image
            @param  filename    Path to the file to load
            @param  destW       Width of the created Image
            @param  destH       Height of the created Image
            @param  format      Pixel format of the destination image (Default is PixelFormat32bppRGBA.)
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)

            @return Pointer to the new Image object
        */
        static Bitmap* FromFile( const CHAR* filename, INT destW, INT destH, PixelFormat format = PixelFormat32bppRGBA, BOOL useEmbeddedColorManagement = FALSE );

        /** Create a Bitmap from a file
            @param  filename    Path to the file to load
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)
            @return Pointer to the new Bitmap
        */
        static Bitmap* FromFile( const WCHAR* filename, BOOL useEmbeddedColorManagement = FALSE );

        /** Load an image file into a specific sized Image
            @param  filename    Path to the file to load
            @param  destW       Width of the created Image
            @param  destH       Height of the created Image
            @param  format      Pixel format of the destination image (Default is PixelFormat32bppRGBA.)
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)

            @return Pointer to the new Image object
        */
        static Bitmap* FromFile( const WCHAR* filename, INT destW, INT destH, PixelFormat format = PixelFormat32bppRGBA, BOOL useEmbeddedColorManagement = FALSE );
#endif // AGGOO_ENABLE_IMAGE_LOADING || AGGOO_ENABLE_VECTOR_LOADING

        /** Create a clone of this bitmap */
        virtual Bitmap* Clone() const;
        /** Create a clone of this bitmap */
        virtual Bitmap* Clone( const Rect& rect, PixelFormat format ) const;
        /** Create a clone of this bitmap */
        virtual Bitmap* Clone( const RectF& rect, PixelFormat format ) const;
        /** Create a clone of this bitmap */
        virtual Bitmap* Clone( INT x, INT y, INT width, INT height, PixelFormat format ) const;
        /** Create a clone of this bitmap */
        virtual Bitmap* Clone( REAL x, REAL y, REAL width, REAL height, PixelFormat format ) const;

        /** Fetch the color at a specified position
        */
        Status GetPixel( INT x, INT y, Color* color );

        /** Set the color at a specified position
        */
        Status SetPixel( INT x, INT y, const Color& color );

        /** Lock the bitmap for direct access to the pixel data
        */
        Status LockBits( const Rect* rect, UINT flags, PixelFormat format, BitmapData* lockedBitmapData );

        /** Unlock the bitmap data
        */
        Status UnlockBits( BitmapData* lockedBitmapData );
    };

    /** @remarks
        // TODO: The following methods exist in GDI+, but not AggOO (yet):
                 Note that some of these use Windows objects, such as HBITMAP
                 and HICON.  In order to implement these on other platforms, it
                 will probably be necessary to create those structures in AggOO,
                 or simply ignore them.  We can use the preprocessor command
                    #if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
                 to exclude them, or to specify that the local structures should
                 be used.

                 Bitmap( const BITMAPINFO* gdiBitmapInfo, VOID* gdiBitmapData );
                 Bitmap( HBITMAP hbm, HPALETTE hpal );
                 Bitmap( HICON hicon );
                 Bitmap( HINSTANCE hInstance, const WCHAR* bitmapName );
                 Bitmap( IStream* stream, BOOL useIcm );
                 Bitmap( IDirectDrawSurface7* surface );
                 static Bitmap* FromBITMAPINFO( const BITMAPINFO* gdiBitmapInfo, VOID* gdiBitmapData );
                 static Bitmap* FromDirectDrawSurface7( IDirectDrawSurface7* surface );
                 static Bitmap* FromHBITMAP( HBITMAP hbm, HPALETTE hpal );
                 static Bitmap* FromHICON( HICON hicon );
                 static Bitmap* FromResource( HINSTANCE hInstance, const WCHAR* bitmapName );
                 static Bitmap* FromStream( IStream* stream, BOOL useEmbeddedColorManagement );
                 Status GetHBITMAP( const Color& colorBackground, HBITMAP* hbmReturn );
                 Status GetHICON( HICON* hicon );
                 Status SetResolution( REAL xdpi, REAL ydpi );
    */

} // namespace AggOO

#endif  // AggOOBitmap_H_
