
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
// $Id: AggOOImage.h,v 1.8 2010/03/12 04:20:57 dratek Exp $
//
// AggOOImage.h -- Basic image class.
//
// Author       : Chad M. Draper
// Date         : September 8, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-08      Initial implementation
//      2007-09-13      Added support for loading/saving images using DevIL.
//

#ifndef AggOOImage_H_
#define AggOOImage_H_

#include "AggOOPixelFormats.h"
#include "AggOOEnums.h"
#include "AggOOConfig.h"
#include "AggOOBase.h"
#include "AggOOTypes.h"
#include "AggOOPrereqs.h"
#include "AggOOColor.h"
#include "AggOOImaging.h"


// TODO: I am initially going to add just what I need for my uses.  Additional
//       code will be added as I encounter uses for it.  The Image class has
//       a lot of uses I've never needed, so I won't get much done with it
//       myself.

namespace AggOO
{
    /** @class Image

        The image class is where the pixel data is stored.  It can be sub-
        classed for more specific uses, such as a Bitmap.
    */
    class _AggOOExport Image : public Base
    {
    public:
        friend class Graphics;
        friend class GraphicsRendererBase;
        friend class TextureBrush;

    protected:
        Image( ImageType type = ImageTypeUnknown, Status status = NotImplemented );

        mutable Status  mLastStatus;
        Unit            mUnit;              // Measurement unit of the image
/*
        BOOL            mExternalBuffer;    // Indicates if the image was allocated
                                            // internally (such as by a Bitmap) or
                                            // externally (loaded from a file.)
*/
    public:
        BitmapData      mActiveBitmap;      // Image data

        //UINT            mWidth, mHeight;    // Dimensions of the image
        //INT             mStride;            // Row width
        //PixelFormat     mPixFormat;         // Format of the image
        //void*           mPixelBuffer;       // Array of pixel data

    private:
        ImageType   mType;

    protected:
        Image( const Image& src ) { }
        Image& operator=( const Image& src ) { return *this; }

#if ( AGGOO_ENABLE_IMAGE_LOADING == 1 || AGGOO_ENABLE_VECTOR_LOADING == 1 )
        /** Helper method for loading an image
            @param  filename            Name if the image file to load
            @param  destW               Width of the destination image
            @param  destH               Height of the destination image
            @param  destFormat          Pixel format of the destination image

            @note   If destW or destH is -1, then the dimension of the image is
                    used instead.  Otherwise, the image dimension is overridden.
        */
        Status loadImage( const CHAR* filename, INT destW, INT destH, PixelFormat destFormat, BOOL useEmbeddedColorManagement = FALSE );

        /** Helper method for loading an image
            @param  filename            Name if the image file to load
        */
        Status loadImage( const CHAR* filename, BOOL useEmbeddedColorManagement = FALSE );

        /** Helper to load a vector image
            @param  filename            Name if the image file to load
            @param  destW               Width of the destination image
            @param  destH               Height of the destination image
            @param  destFormat          Pixel format of the destination image

            @note   If destW or destH is <= 0, then the actual image size is
                    used.  Otherwise, the image is scaled to fit the requested
                    size and pixel format.
        */
        Status loadVectorImage( const CHAR* filename, INT destW, INT destH, PixelFormat destFormat = PixelFormat32bppRGBA );
        Status loadVectorImage( const CHAR* filename, PixelFormat destFormat = PixelFormat32bppRGBA );

        // Helper method for saving an image
        Status saveImage( const CHAR* filename );
#endif // AGGOO_ENABLE_IMAGE_LOADING || AGGOO_ENABLE_VECTOR_LOADING

    public:
        /** Destructor */
        virtual ~Image(void);

#if ( AGGOO_ENABLE_IMAGE_LOADING == 1 || AGGOO_ENABLE_VECTOR_LOADING == 1 )
        /** GDI+ does not have image loading with regular ascii strings, but
            DevIL uses them to load images, so why not provide handlers for them
            directly?
            @param  filename    Path to the file to load
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)
        */
        Image( const CHAR* filename, BOOL useEmbeddedColorManagement = FALSE );

        /** Load an image to a specific sized Image.
            @param  filename    Path to the file to load
            @param  destW       Width of the created Image
            @param  destH       Height of the created Image
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)

            @note   GDI+ does not use this method
        */
        Image( const CHAR* filename, INT destW, INT destH, PixelFormat destFormat, BOOL useEmbeddedColorManagement = FALSE );

        /** Create a new Image from a file
            @param  filename    Path to the file to load
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)
        */
        Image( const WCHAR* filename, BOOL useEmbeddedColorManagement = FALSE );

        /** Load an image to a specific sized Image.
            @param  filename    Path to the file to load
            @param  destW       Width of the created Image
            @param  destH       Height of the created Image
            @param  destFormat  Pixel format of the destination image
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)

            @note   GDI+ does not use this method
        */
        Image( const WCHAR* filename, INT destW, INT destH, PixelFormat destFormat, BOOL useEmbeddedColorManagement = FALSE );

        /** Create a new Image from a file
            @param  filename    Path to the file to load
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)

            @return Pointer to the new Image object
        */
        static Image* FromFile( const CHAR* filename, BOOL useEmbeddedColorManagement = FALSE );

        /** Load an image file into a specific sized Image
            @param  filename    Path to the file to load
            @param  destW       Width of the created Image
            @param  destH       Height of the created Image
            @param  format      Pixel format of the destination image (Default is PixelFormat32bppRGBA.)
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)

            @return Pointer to the new Image object
        */
        static Image* FromFile( const CHAR* filename, INT destW, INT destH, PixelFormat format = PixelFormat32bppRGBA, BOOL useEmbeddedColorManagement = FALSE );

        /** Create a new Image from a file
            @param  filename    Path to the file to load
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)

            @return Pointer to the new Image object
        */
        static Image* FromFile( const WCHAR* filename, BOOL useEmbeddedColorManagement = FALSE );

        /** Load an image file into a specific sized Image
            @param  filename    Path to the file to load
            @param  destW       Width of the created Image
            @param  destH       Height of the created Image
            @param  format      Pixel format of the destination image (Default is PixelFormat32bppRGBA.)
            @param  useEmbeddedColorManagement  (Currently unused in AggOO.  Default is FALSE.)

            @return Pointer to the new Image object
        */
        static Image* FromFile( const WCHAR* filename, INT destW, INT destH, PixelFormat format = PixelFormat32bppRGBA, BOOL useEmbeddedColorManagement = FALSE );

        /** Save the image to a file
            @note   This is a basic save method.  It does not exist in GDI+.
            @param  filename    Path to the file to save
        */
        Status Save( const CHAR* filename );

        /** Save the image to a file
            @note   This is a basic save method.  It does not exist in GDI+.
            @param  filename    Path to the file to save
        */
        Status Save( const WCHAR* filename );
#endif // AGGOO_ENABLE_IMAGE_LOADING || AGGOO_ENABLE_VECTOR_LOADING

        /** Create a clone of this image */
        virtual Image* Clone() const;

        /** Rotate and/or flip the image */
        Status RotateFlip( RotateFlipType rotateFlipType );

        /** Get the bounding rectangle and measurement unit for the image */
        Status GetBounds( RectF* rect, Unit* srcUnit );

        /** Get flags about this image */
        UINT GetFlags() const;

        /** Get the image width */
        UINT GetWidth() const;
        /** Get the image height */
        UINT GetHeight() const;
        ///** Get the image stride */
        //int getStride() const;
        ///** Get a pointer to the image data */
        //void* getImageBuffer() const;
        ///** Get the BitmapData */
        //BitmapData* getBitmapData();


        /** Get the horizontal resolution of the image in dots-per-inch */
        REAL GetHorizontalResolution(void);
        /** Get the vertical resolution of the image in dots-per-inch */
        REAL GetVerticalResolution(void);
        /** Get the dimensions of the image */
        Status GetPhysicalDimension( SizeF* size );

        /** Retrieve the pixel format of the image */
        PixelFormat GetPixelFormat(void) const;



        /** Retrieve the image type (i.e. Bitmap or Metadata) */
        ImageType GetType() const       { return mType; }

        /** Set the status of the brush */
        Status SetStatus( Status status ) const
        {
            if ( status != Ok )
                return ( mLastStatus = status );
            return status;
        }

        /** Retrieve the last status message set */
        Status GetLastStatus() const    { return mLastStatus; }

    protected:
        //! Calculate the total row length (positive stride)
//        UINT calcRowLength( UINT w, UINT bpp );

    private:
        // Release all memory and reset the image
        void destroy();

        // Copy an image into this image
        void copyFrom( const Image* src );


        //----------------------------------------------------------------------
        // Create static methods for handling the rotate-flip routine.  These
        // will be put in an array, and called according to the rotate-flip
        // style.
        //----------------------------------------------------------------------
        static Status rotateNoneFlipNone( Image* image );
        static Status rotate90FlipNone( Image* image );
        static Status rotate180FlipNone( Image* image );
        static Status rotate270FlipNone( Image* image );
        static Status rotateNoneFlipX( Image* image );
        static Status rotate90FlipX( Image* image );
        static Status rotate180FlipX( Image* image );
        static Status rotate270FlipX( Image* image );
    };

    /** @remarks
    // TODO: The following methods are in GDI+, but not in AggOO (yet):

             Image( IStream* stream, BOOL useEmbeddedColorManagement );
             static Image* FromStream( IStream* stream, BOOL useEmbeddedColorManagement );
             Status GetAllPropertyItems( UINT totalBufferSize, UINT numProperties, PropertyItem* allItems );
             Status GetEncoderParameterList( const CLSID* clsidEncoder, UINT size, EncoderParameters* buffer );
             UINT GetEncoderParameterListSize( const CLSID* clsidEncoder );
             UINT GetFrameCount( const GUID* dimensionID );
             UINT GetFrameDimensionsCount(void);
             Status GetFrameDimensionsList( GUID* dimensionIDs, UINT count );
             Status GetPalette( ColorPalette* palette, INT size );
             INT GetPaletteSize(void);
             UINT GetPropertyCount(void);
             Status GetPropertyIdList( UINT numOfProperty, PROPID* list );
             Status GetPropertyItem( PROPID propId, UINT propSize, PropertyItem* buffer );
             UINT GetPropertyItemSize( PROPID propId );
             Status GetPropertySize( UINT* totalBufferSize, UINT* numProperties );
             Status GetRawFormat( GUID* format );
             Image* GetThumbnailImage( UINT thumbWidth, UINT thumbHeight, GetThumbnailImageAbort callback, VOID* callbackData );
             Status RemovePropertyItem( PROPID propId );
             Status Save( const WCHAR* filename, const CLSID* clsidEncoder, const EncoderParameters* encoderParams );
             Status Save( IStream* stream, const CLSID* clsidEncoder, const EncoderParameters* encoderParams );
             Status SaveAdd( const EncoderParameters* encoderParams );
             Status SaveAdd( Image* newImage, const EncoderParameters* encoderParams );
             Status SelectActiveFrame( const GUID* dimensionID, UINT frameIndex );
             Status SetPalette( const ColorPalette* palette );
             Status SetPropertyItem( const PropertyItem* item );
     */

} // namespace AggOO

#endif  // AggOOImage_H_
