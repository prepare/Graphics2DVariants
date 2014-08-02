
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
// $Id: AggOOImage.cpp,v 1.11 2009/03/26 21:58:10 dratek Exp $
//
// AggOOImage.cpp
//

#include "AggOOConfig.h"
#include "AggOOBrush.h"
#include "AggOOImage.h"
#include "AggOOGraphics.h"
#include "AggOOPixelFormatConverter.h"
//#include "AggOOVectorShapeData.h"
//#include "AggOOVectorShapePath.h"
//#include "AggOOVectorShapeParser.h"

#include <assert.h>
#include <math.h>
#include <vector>

//#include <cmd/LogFileManager.h>
//using cmd::LogFileManager;
//using cmd::LogFileSection;
#include <fstream>

#if ( AGGOO_ENABLE_IMAGE_LOADING == 1 )
    // NOTE: DevIL MUST be installed and configured
#   include <IL/il.h>
#endif
#if ( AGGOO_ENABLE_VECTOR_LOADING == 1 )
    // NOTE: VectorFileParse MUST be installed and configured
#   include "FileParse.h"
#   include "AggOOFileParseRenderer.h"
#endif

namespace AggOO
{
    Image::Image( ImageType type, Status status )
        : mType( type ),
          mUnit( UnitPixel )
    {
        memset( &mActiveBitmap, 0, sizeof( BitmapData ) );
        mLastStatus = SetStatus( status );
    }

#if ( AGGOO_ENABLE_IMAGE_LOADING == 1 || AGGOO_ENABLE_VECTOR_LOADING == 1 )
    Image::Image( const CHAR* filename, BOOL useEmbeddedColorManagement /*= FALSE*/ )
    {
        mLastStatus = SetStatus( loadImage( filename, useEmbeddedColorManagement ) );
    }

    Image::Image( const CHAR* filename, INT destW, INT destH, PixelFormat format, BOOL useEmbeddedColorManagement )
    {
        mLastStatus = SetStatus( loadImage( filename, destW, destH, format, useEmbeddedColorManagement ) );
    }

    Image::Image( const WCHAR* filename, BOOL useEmbeddedColorManagement /*= FALSE*/ )
    {
        // Convert the filename to a char*
        char* mbString = new char [ MB_CUR_MAX ];
        wcstombs( mbString, filename, MB_CUR_MAX );
        
        mLastStatus = SetStatus( loadImage( mbString, useEmbeddedColorManagement ) );

        delete [] mbString;
    }

    Image::Image( const WCHAR* filename, INT destW, INT destH, PixelFormat format, BOOL useEmbeddedColorManagement )
    {
        // Convert the filename to a char*
        char* mbString = new char [ MB_CUR_MAX ];
        wcstombs( mbString, filename, MB_CUR_MAX );
        
        mLastStatus = SetStatus( loadImage( mbString, destW, destH, format, useEmbeddedColorManagement ) );

        delete [] mbString;
    }

    //loadImage-----------------------------------------------------------------
    Status Image::loadImage( const CHAR* filename, INT destW, INT destH, PixelFormat destFormat, BOOL useEmbeddedColorManagement )
    {
        Status status = Ok;

        // Use DevIL to load the image:
        if ( ilGetInteger( IL_VERSION_NUM ) < IL_VERSION )
            status = InvalidLibraryVersion;
        else
        {
            ilInit();
            ILuint imageID;
            ilGenImages( 1, &imageID );
            ilBindImage( imageID );

            // Load the image and convert it to 32bpp RGBA
            if ( ilLoadImage( filename ) && ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE ) )
            {
                // If using the default dimensions and pixel format, load the
                // image directly into this object.  Otherwise, use a temporary
                // image to load into, then draw to this object.
                if ( destW <= 0 && destH <= 0 && destFormat == PixelFormat32bppRGBA )
                {
                    loadImage( filename, useEmbeddedColorManagement );
                }
                else
                {
                    // Load the full-size image
                    Image* tempImage = new Image( filename, useEmbeddedColorManagement );

                    // Create the destination image:
                    memset( &mActiveBitmap, 0, sizeof( BitmapData ) );
                    mActiveBitmap.Width     = destW;
                    mActiveBitmap.Height    = destH;
                    mActiveBitmap.Stride    = GetRowWidth( destW, GetPixelFormatSize( destFormat ) );
                    mActiveBitmap.PixelFormat = destFormat;
                    mActiveBitmap.Flags     = ImageFlagOwnScan0;
                    mActiveBitmap.Scan0     = new BYTE [ mActiveBitmap.Stride * mActiveBitmap.Height ];
                    if ( !mActiveBitmap.Scan0 )
                        status = OutOfMemory;
                    else
                    {
                        // Draw the image to this object:
                        Graphics gfx( this );
                        Rect destRect( 0, 0, destW, destH );
                        gfx.DrawImage( tempImage, destRect );
                    }
                }
            }
            else
            {
                // The image is not recognized by devIL...  Since this is also
                // supposed to load vector files, see if it can be loaded as a
                // vector file.
                status = loadVectorImage( filename, destW, destH, destFormat );
            }
        }
        return status;
    }

    //loadImage-----------------------------------------------------------------
    Status Image::loadImage( const CHAR* filename, BOOL useEmbeddedColorManagement )
    {
        Status status = Ok;

        // Use DevIL to load the image:
        if ( ilGetInteger( IL_VERSION_NUM ) < IL_VERSION )
            status = InvalidLibraryVersion;
        else
        {
            ilInit();
            ILuint imageID;
            ilGenImages( 1, &imageID );
            ilBindImage( imageID );

            // Load the image and convert it to 32bpp RGBA
            if ( ilLoadImage( filename ) && ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE ) )
            {
                // Create the bitmap to hold the image:
                memset( &mActiveBitmap, 0, sizeof( BitmapData ) );
                mActiveBitmap.Width     = ilGetInteger( IL_IMAGE_WIDTH );
                mActiveBitmap.Height    = ilGetInteger( IL_IMAGE_HEIGHT );
                mActiveBitmap.Stride    = mActiveBitmap.Width * 4;
                mActiveBitmap.PixelFormat = PixelFormat32bppRGBA;
                mActiveBitmap.Flags     = ImageFlagOwnScan0;
                mActiveBitmap.Scan0     = new BYTE [ mActiveBitmap.Stride * mActiveBitmap.Height ];
                if ( !mActiveBitmap.Scan0 )
                    status = OutOfMemory;
                else
                {
                    //memset( mActiveBitmap.Scan0, 255, sizeof( BYTE ) * mActiveBitmap.Stride * mActiveBitmap.Height );
                    memcpy( mActiveBitmap.Scan0, ilGetData(), sizeof( BYTE ) * mActiveBitmap.Stride * mActiveBitmap.Height );
                }
            }
            else
            {
                // The image is not recognized by devIL...  Since this is also
                // supposed to load vector files, see if it can be loaded as a
                // vector file.
                status = loadVectorImage( filename );
            }
        }
        return status;
    }

    //loadVectorImage-----------------------------------------------------------
    Status Image::loadVectorImage( const CHAR* filename, PixelFormat destFormat )
    {
        Status status = NotImplemented;

#if ( AGGOO_ENABLE_VECTOR_LOADING == 1 )
//#if ( 1 )
        status = Ok;

        ::FileParse::Scene    scene;
        ::FileParse::ParserBase* parser = ::FileParse::ParserBase::CreateParser( filename );

        // Parser the file:
        if ( parser )
        {
            // Parse the scene data
            scene.Clear();
            parser->Parse( filename, &scene );
            delete parser;

            // Create the bitmap to hold the image:
            memset( &mActiveBitmap, 0, sizeof( BitmapData ) );
            ::FileParse::RectF bounds = scene.GetBoundingBox();
            //mActiveBitmap.Width     = 440;
            //mActiveBitmap.Height    = 400;
            mActiveBitmap.Width     = bounds.Width;
            mActiveBitmap.Height    = bounds.Height;
            mActiveBitmap.Stride    = GetRowWidth( mActiveBitmap.Width, GetPixelFormatSize( destFormat ) );
            mActiveBitmap.PixelFormat = destFormat;
            mActiveBitmap.Flags     = ImageFlagOwnScan0;
            mActiveBitmap.Scan0     = new BYTE [ mActiveBitmap.Stride * mActiveBitmap.Height ];
            if ( !mActiveBitmap.Scan0 )
                status = OutOfMemory;
            else
            {
                // Create a Graphics object to render this image:
                Graphics gfx( this );
                gfx.SetSmoothingMode( SmoothingModeAntiAlias );
                //gfx.Clear( Color( 255, 0, 0, 255 ) );
                //gfx.TranslateTransform( -bounds.X, -bounds.Y );

                ::FileParse::Matrix fpMatrix, oldMatrix = scene.GetMatrix();
                fpMatrix.Translate( -bounds.X, -bounds.Y );
                scene.SetMatrix( &fpMatrix );
                scene.MultiplyMatrix( &oldMatrix );

                SolidBrush br( Color::DarkOliveGreen );
                //gfx.FillRectangle( &br, bounds.X, bounds.Y, 100.0f, 100.0f );
                ::FileParse::RendererBase* renderer = new RendererAggOO( &gfx );
                renderer->Render( &scene );
                delete renderer;
/*
                if ( path.mFlipVertical )
                {
                    gfx.TranslateTransform( 0, bounds.Height / 2.0 );
                    gfx.ScaleTransform( 1.0, -1.0 );
                    gfx.TranslateTransform( 0, -bounds.Height / 2.0 );
                }
                if ( path.mFlipHorizontal )
                {
                    gfx.TranslateTransform( bounds.Width / 2.0, 0.0 );
                    gfx.ScaleTransform( -1.0, 1.0 );
                    gfx.TranslateTransform( -bounds.Width / 2.0, 0.0 );
                }
                gfx.TranslateTransform( -bounds.X, -bounds.Y );
                path.Render( &gfx );
*/
            }
        }
        else
        {
            status = OutOfMemory;
        }

/*
        vs::VectorShapePath path;
        vs::VectorShapeParser parser( &path );
        if ( parser.Parse( filename ) == Ok )
        {
            // Get the bounding rect of the image
            RectF   bounds;
            path.BoundingRect( &bounds );

            // Create the bitmap to hold the image:
            memset( &mActiveBitmap, 0, sizeof( BitmapData ) );
            //mActiveBitmap.Width     = 440;
            //mActiveBitmap.Height    = 400;
            mActiveBitmap.Width     = bounds.Width;
            mActiveBitmap.Height    = bounds.Height;
            mActiveBitmap.Stride    = GetRowWidth( mActiveBitmap.Width, GetPixelFormatSize( destFormat ) );
            mActiveBitmap.PixelFormat = destFormat;
            mActiveBitmap.Flags     = ImageFlagOwnScan0;
            mActiveBitmap.Scan0     = new BYTE [ mActiveBitmap.Stride * mActiveBitmap.Height ];
            if ( !mActiveBitmap.Scan0 )
                status = OutOfMemory;
            else
            {
                // Create a Graphics object to render this image:
                Graphics gfx( this );
                gfx.SetSmoothingMode( SmoothingModeAntiAlias );
                gfx.Clear( Color( 0, 0, 0, 0 ) );
                if ( path.mFlipVertical )
                {
                    gfx.TranslateTransform( 0, bounds.Height / 2.0 );
                    gfx.ScaleTransform( 1.0, -1.0 );
                    gfx.TranslateTransform( 0, -bounds.Height / 2.0 );
                }
                if ( path.mFlipHorizontal )
                {
                    gfx.TranslateTransform( bounds.Width / 2.0, 0.0 );
                    gfx.ScaleTransform( -1.0, 1.0 );
                    gfx.TranslateTransform( -bounds.Width / 2.0, 0.0 );
                }
                gfx.TranslateTransform( -bounds.X, -bounds.Y );
                path.Render( &gfx );
            }

            //Matrix mtx;
            //std::vector< AIShapeData > shapes;
            //aiPath.RenderShapeData( mtx, &shapes );
        }
        else
            status = parser.GetLastStatus();
            //status = UnknownImageFormat;
*/

#endif  // AGGOO_ENABLE_VECTOR_LOADING

        return status;
    }

    //loadVectorImage-----------------------------------------------------------
    Status Image::loadVectorImage( const CHAR* filename, INT destW, INT destH, PixelFormat destFormat )
    {
        Status status = NotImplemented;

#if ( AGGOO_ENABLE_VECTOR_LOADING == 1 )
        status = Ok;

        // If this uses default values, then we can use the default loader:
        if ( destW <= 0 && destH <= 0 && destFormat == PixelFormat32bppRGBA )
            return loadVectorImage( filename, destFormat );

        ::FileParse::Scene    scene;
        ::FileParse::ParserBase* parser = ::FileParse::ParserBase::CreateParser( filename );

        // Parser the file:
        if ( parser )
        {

            // Parse the scene data
            scene.Clear();
            parser->Parse( filename, &scene );
            delete parser;

            // Create the bitmap to hold the image:
            memset( &mActiveBitmap, 0, sizeof( BitmapData ) );
            ::FileParse::RectF bounds = scene.GetBoundingBox();
            //mActiveBitmap.Width     = 440;
            //mActiveBitmap.Height    = 400;
            mActiveBitmap.Width     = ( destW > 0 ) ? destW : bounds.Width;
            mActiveBitmap.Height    = ( destH > 0 ) ? destH : bounds.Height;
            mActiveBitmap.Stride    = GetRowWidth( mActiveBitmap.Width, GetPixelFormatSize( destFormat ) );
            mActiveBitmap.PixelFormat = destFormat;
            mActiveBitmap.Flags     = ImageFlagOwnScan0;
            mActiveBitmap.Scan0     = new BYTE [ mActiveBitmap.Stride * mActiveBitmap.Height ];
            if ( !mActiveBitmap.Scan0 )
                status = OutOfMemory;
            else
            {
                // Create a Graphics object to render this image:
                Graphics gfx( this );
                gfx.SetSmoothingMode( SmoothingModeAntiAlias );
                gfx.Clear( Color( 0, 0, 0, 0 ) );

                // Transform the render surface to accomodate the desired dimensions
                REAL destAspect = mActiveBitmap.Width / REAL( mActiveBitmap.Height );
                REAL imageAspect = bounds.Width / bounds.Height;
                PointF scale( mActiveBitmap.Width / bounds.Width, mActiveBitmap.Height / bounds.Height );
                ::FileParse::Matrix fpMatrix, oldMatrix = scene.GetMatrix();
                if ( imageAspect < destAspect )
                {
                    fpMatrix.Translate( ( mActiveBitmap.Width - bounds.Width * scale.Y ) * 0.5, 0.0 );
                    fpMatrix.Scale( scale.Y, scale.Y );
                }
                else
                {
                    fpMatrix.Translate( 0.0, ( mActiveBitmap.Height - bounds.Height * scale.X ) * 0.5 );
                    fpMatrix.Scale( scale.X, scale.X );
                }
                fpMatrix.Translate( -bounds.X, -bounds.Y );
                //fpMatrix.Multiply( &oldMatrix );
                //scene.SetMatrix( &fpMatrix );
                scene.SetMatrix( &fpMatrix );
                scene.MultiplyMatrix( &oldMatrix );

                // Render the image
                ::FileParse::RendererBase* renderer = new RendererAggOO( &gfx );
                renderer->Render( &scene );
                delete renderer;
            }
        }
        else
            return OutOfMemory;

#endif  // AGGOO_ENABLE_VECTOR_LOADING == 1

/*
        // The process is the same for all vector files:
        // 1. Parse the file
        // 2. Get the bounding rect for the shape
        // 3. Scale the shape to fit the image
        // 4. Render the shape

        vs::VectorShapePath path;
        vs::VectorShapeParser parser( &path );
        parser.Parse( filename );
        //if ( parser.Parse( filename ) == Ok )
        //{
            path.ArrangeOrientations();

            // Get the bounding rect of the image
            RectF   bounds;
            path.BoundingRect( &bounds );

            // Create the bitmap to hold the image:
            memset( &mActiveBitmap, 0, sizeof( BitmapData ) );
            //mActiveBitmap.Width     = 440;
            //mActiveBitmap.Height    = 400;
            mActiveBitmap.Width     = ( destW > 0 ) ? destW : bounds.Width;
            mActiveBitmap.Height    = ( destH > 0 ) ? destH : bounds.Height;
            mActiveBitmap.Stride    = GetRowWidth( mActiveBitmap.Width, GetPixelFormatSize( destFormat ) );
            mActiveBitmap.PixelFormat = destFormat;
            mActiveBitmap.Flags     = ImageFlagOwnScan0;
            mActiveBitmap.Scan0     = new BYTE [ mActiveBitmap.Stride * mActiveBitmap.Height ];

            if ( !mActiveBitmap.Scan0 )
                status = OutOfMemory;
            else
            {
                REAL destAspect = mActiveBitmap.Width / REAL( mActiveBitmap.Height );
                REAL imageAspect = bounds.Width / bounds.Height;
                PointF scale( mActiveBitmap.Width / bounds.Width, mActiveBitmap.Height / bounds.Height );

                // Create a Graphics object to render this image:
                Graphics gfx( this );
                gfx.SetSmoothingMode( SmoothingModeAntiAlias );
                gfx.Clear( Color( 0, 0, 0, 0 ) );
                if ( imageAspect < destAspect )
                {
                    gfx.TranslateTransform( ( mActiveBitmap.Width - bounds.Width * scale.Y ) * 0.5, 0.0 );
                    gfx.ScaleTransform( scale.Y, scale.Y );
                }
                else
                {
                    gfx.TranslateTransform( 0.0, ( mActiveBitmap.Height - bounds.Height * scale.X ) * 0.5 );
                    gfx.ScaleTransform( scale.X, scale.X );
                }
                if ( path.mFlipVertical )
                {
                    gfx.TranslateTransform( 0, bounds.Height / 2.0 );
                    gfx.ScaleTransform( 1.0, -1.0 );
                    gfx.TranslateTransform( 0, -bounds.Height / 2.0 );
                }
                if ( path.mFlipHorizontal )
                {
                    gfx.TranslateTransform( bounds.Width / 2.0, 0.0 );
                    gfx.ScaleTransform( -1.0, 1.0 );
                    gfx.TranslateTransform( -bounds.Width / 2.0, 0.0 );
                }
                gfx.TranslateTransform( -bounds.X, -bounds.Y );
                path.Render( &gfx );
                //gfx.Clear( Color::Red );
            }

*/

        return status;
    }
#endif // AGGOO_ENABLE_IMAGE_LOADING || AGGOO_ENABLE_VECTOR_LOADING

    Image::~Image(void)
    {
        destroy();
    }

#if ( AGGOO_ENABLE_IMAGE_LOADING == 1 || AGGOO_ENABLE_VECTOR_LOADING == 1 )
    //FromFile------------------------------------------------------------------
    Image* Image::FromFile( const CHAR* filename, BOOL useEmbeddedColorManagement /*= FALSE*/ )
    {
        return new Image( filename, useEmbeddedColorManagement );
    }

    //FromFile------------------------------------------------------------------
    Image* Image::FromFile( const CHAR* filename, INT destW, INT destH, PixelFormat destFormat, BOOL useEmbeddedColorManagement )
    {
        return new Image( filename, destW, destH, destFormat, useEmbeddedColorManagement );
    }

    //FromFile------------------------------------------------------------------
    Image* Image::FromFile( const WCHAR* filename, BOOL useEmbeddedColorManagement /*= FALSE*/ )
    {
        return new Image( filename, useEmbeddedColorManagement );
    }

    //FromFile------------------------------------------------------------------
    Image* Image::FromFile( const WCHAR* filename, INT destW, INT destH, PixelFormat destFormat, BOOL useEmbeddedColorManagement )
    {
        return new Image( filename, destW, destH, destFormat, useEmbeddedColorManagement );
    }

    //saveImage-----------------------------------------------------------------
    Status Image::saveImage( const CHAR* filename )
    {
        Status status = Ok;

        // Use DevIL to save the image:
        if ( ilGetInteger( IL_VERSION_NUM ) < IL_VERSION )
            status = InvalidLibraryVersion;
        else
        {
            ilInit();
            ILuint imageID;
            ilGenImages( 1, &imageID );
            ilBindImage( imageID );
            ilEnable(IL_FILE_OVERWRITE);

            // Convert the image to 32bpp RGBA:
            BYTE* scan0 = new BYTE [ mActiveBitmap.Width * 4 * mActiveBitmap.Height ];
            BitmapData destBmp(
                            Rect( 0, 0, mActiveBitmap.Width, mActiveBitmap.Height ),
                            mActiveBitmap.Width * 4,
                            PixelFormat32bppRGBA,
                            ImageFlagOwnScan0, 
                            scan0 );
            BufferConverter::ConvertPixelFormat( &mActiveBitmap, &destBmp );

            // Create the il image:
            if ( !ilTexImage(
                        destBmp.Width,
                        destBmp.Height,
                        1,                      // Depth
                        4,                      // Num. Channels (RGBA)
                        IL_RGBA,                // Format
                        IL_UNSIGNED_BYTE,       // Pixel type
                        destBmp.Scan0 ) )
            {
                status = GenericError;
            }

            // Save the image
            if ( !ilSaveImage( filename ) )
                status = UnknownImageFormat;
        }
        return status;
    }

    //Save----------------------------------------------------------------------
    Status Image::Save( const CHAR* filename )
    {
        Status status = Ok;

        return SetStatus( saveImage( filename ) );
    }

    //Save----------------------------------------------------------------------
    Status Image::Save( const WCHAR* filename )
    {
        Status status = Ok;

        // Convert the filename to a char*
        char* mbString = new char [ MB_CUR_MAX ];
        wcstombs( mbString, filename, MB_CUR_MAX );

        return SetStatus( saveImage( mbString ) );
    }
#endif // AGGOO_ENABLE_IMAGE_LOADING || AGGOO_ENABLE_VECTOR_LOADING

    //destroy-------------------------------------------------------------------
    void Image::destroy()
    {
        if ( mActiveBitmap.Scan0 && ( mActiveBitmap.Flags & ImageFlagOwnScan0 ) )
        {
            delete [] mActiveBitmap.Scan0;
        }

        SetStatus( Ok );
        //SetStatus( WrongState );
        memset( &mActiveBitmap, 0, sizeof( BitmapData ) );
        //mPixelBuffer = 0;
        //mWidth = mHeight = 0;
        //mStride = 0;
    }

    //copyFrom------------------------------------------------------------------
    void Image::copyFrom( const Image* src )
    {
        destroy();

        // Copy the data from this image to the new image:
        mActiveBitmap.X         = src->mActiveBitmap.X;
        mActiveBitmap.Y         = src->mActiveBitmap.Y;
        mActiveBitmap.Width     = src->mActiveBitmap.Width;
        mActiveBitmap.Height    = src->mActiveBitmap.Height;
        mActiveBitmap.Stride    = src->mActiveBitmap.Stride;
        mActiveBitmap.PixelFormat = src->mActiveBitmap.PixelFormat;
        mActiveBitmap.Flags     = src->mActiveBitmap.Flags;

        if ( mActiveBitmap.Flags & ImageFlagOwnScan0 )
        {
            // Allocate the buffer and copy the pixel data:
            UINT rowLength = GetRowWidth( mActiveBitmap.Width, GetPixelFormatSize( mActiveBitmap.PixelFormat ) );
            assert( (UINT)abs( mActiveBitmap.Stride ) == rowLength );
            mActiveBitmap.Scan0 = new BYTE [ rowLength * mActiveBitmap.Height ];
            memcpy( mActiveBitmap.Scan0, src->mActiveBitmap.Scan0, rowLength * mActiveBitmap.Height );
        }
        else
        {
            UINT rowLength = GetRowWidth( mActiveBitmap.Width, GetPixelFormatSize( mActiveBitmap.PixelFormat ) );
            mActiveBitmap.Scan0 = new BYTE [ rowLength * mActiveBitmap.Height ];
            if ( !mActiveBitmap.Scan0, src->mActiveBitmap.Scan0 )
                destroy();
            else
                memcpy( mActiveBitmap.Scan0, src->mActiveBitmap.Scan0, rowLength * mActiveBitmap.Height );
        }
    }

    //Clone---------------------------------------------------------------------
    Image* Image::Clone() const
    {
        Image* ret = new Image( mType, Ok );

        ret->copyFrom( this );

        return ret;
    }

    //--------------------------------------------------------------------------
    // Helper functios for the rotate-flip method.  Each one will handle a
    // specific rotate/flip.
    //--------------------------------------------------------------------------
    Status Image::rotateNoneFlipNone( Image* image )
    {
        // The image doesn't change
        return Ok;
    }

    Status Image::rotate90FlipNone( Image* image )
    {
        if ( !image ) return InvalidParameter;

        // Create a new Image where the dimensions are transposed
        Image* tempImg = new Image( image->mType, Ok );
        if ( !tempImg ) return OutOfMemory;
        BitmapData* srcBmp  = &image->mActiveBitmap;
        tempImg->mActiveBitmap.Width     = srcBmp->Height;
        tempImg->mActiveBitmap.Height    = srcBmp->Width;
        tempImg->mActiveBitmap.PixelFormat = srcBmp->PixelFormat;
        tempImg->mActiveBitmap.Stride    = GetRowWidth( srcBmp->Width, GetPixelFormatSize( srcBmp->PixelFormat ) );
        tempImg->mActiveBitmap.Flags     = srcBmp->Flags;
        tempImg->mActiveBitmap.X         = srcBmp->X;
        tempImg->mActiveBitmap.Y         = srcBmp->Y;

        // Allocate the buffer:
        tempImg->mActiveBitmap.Scan0 = new BYTE [ tempImg->mActiveBitmap.Stride * tempImg->mActiveBitmap.Height ];
        if ( !tempImg->mActiveBitmap.Scan0 )
            return InsufficientBuffer;

        // BYTES per pixel
        UINT bytesPerPixel = srcBmp->Stride / srcBmp->Width;

        // Copy the source data to the new image, rotated 90 degrees:
        BYTE* srcBuffer  = static_cast< BYTE* >( srcBmp->Scan0 );
        BYTE* destBuffer = static_cast< BYTE* >( tempImg->mActiveBitmap.Scan0 );
        BYTE* destPix = &destBuffer[ 0 ];
        for ( UINT y = 0; y < tempImg->mActiveBitmap.Height; y++ )
        {
            UINT lookX = y;
            for ( UINT x = 0; x < tempImg->mActiveBitmap.Width; x++ )
            {
                UINT lookY = image->GetHeight() - 1 - x;
                BYTE* srcPix = &srcBuffer[ lookY * srcBmp->Stride + lookX * bytesPerPixel ];
                for ( UINT curByte = 0; curByte < bytesPerPixel; curByte++ )
                {
                    *destPix = *srcPix;
                    ++destPix;
                    ++srcPix;
                }
            }
        }

        // Copy the temporary image to the source image:
        image->copyFrom( tempImg );

        // Destroy the temp image:
        tempImg->destroy();

        return Ok;
    }

    Status Image::rotate180FlipNone( Image* image )
    {
        if ( !image ) return InvalidParameter;

        // BYTES per pixel
        UINT bytesPerPixel = image->mActiveBitmap.Stride / image->mActiveBitmap.Width;

        // Don't need to allocate a new buffer.  Just loop over the top half of
        // the image and swap pixels accordingly.
        BYTE* buffer = static_cast< BYTE* >( image->mActiveBitmap.Scan0 );
        BYTE* curPix = &buffer[ 0 ];
        for ( UINT y = 0; y < image->GetHeight() / 2; y++ )
        {
            UINT lookY = image->GetHeight() - 1 - y;
            for ( UINT x = 0; x < image->GetWidth(); x++ )
            {
                UINT lookX = image->GetWidth() - 1 - x;
                BYTE* pix2 = &buffer[ lookY * image->mActiveBitmap.Stride + lookX * bytesPerPixel ];
                for ( UINT curByte = 0; curByte < bytesPerPixel; curByte++ )
                {
                    // Swap the pixels:
                    BYTE tempByte = *curPix;
                    *curPix = *pix2;
                    *pix2 = tempByte;

                    ++curPix;
                    ++pix2;
                }
            }
        }

        return Ok;
    }

    Status Image::rotate270FlipNone( Image* image )
    {
        if ( !image ) return InvalidParameter;

        // Create a new Image where the dimensions are transposed
        Image* tempImg = new Image( image->mType, Ok );
        if ( !tempImg ) return OutOfMemory;
        BitmapData* srcBmp = &image->mActiveBitmap;
        tempImg->mActiveBitmap.Width     = srcBmp->Height;
        tempImg->mActiveBitmap.Height    = srcBmp->Width;
        tempImg->mActiveBitmap.PixelFormat = srcBmp->PixelFormat;
        tempImg->mActiveBitmap.Stride    = GetRowWidth( srcBmp->Width, GetPixelFormatSize( tempImg->mActiveBitmap.PixelFormat ) );
        tempImg->mActiveBitmap.Flags     = srcBmp->Flags;
        tempImg->mActiveBitmap.X         = srcBmp->X;
        tempImg->mActiveBitmap.Y         = srcBmp->Y;

        // Allocate the buffer:
        tempImg->mActiveBitmap.Scan0 = new BYTE [ tempImg->mActiveBitmap.Stride * tempImg->mActiveBitmap.Height ];
        if ( !tempImg->mActiveBitmap.Scan0 )
            return InsufficientBuffer;

        // BYTES per pixel
        UINT bytesPerPixel = srcBmp->Stride / srcBmp->Width;

        // Copy the source data to the new image, rotated 90 degrees:
        BYTE* srcBuffer  = static_cast< BYTE* >( srcBmp->Scan0 );
        BYTE* destBuffer = static_cast< BYTE* >( tempImg->mActiveBitmap.Scan0 );
        BYTE* destPix = &destBuffer[ 0 ];
        for ( UINT y = 0; y < tempImg->mActiveBitmap.Height; y++ )
        {
            UINT lookX = image->GetWidth() - 1 - y;
            for ( UINT x = 0; x < tempImg->mActiveBitmap.Width; x++ )
            {
                UINT lookY = x;
                BYTE* srcPix = &srcBuffer[ lookY * image->mActiveBitmap.Stride + lookX * bytesPerPixel ];
                for ( UINT curByte = 0; curByte < bytesPerPixel; curByte++ )
                {
                    *destPix = *srcPix;
                    ++destPix;
                    ++srcPix;
                }
            }
        }

        // Copy the temporary image to the source image:
        image->copyFrom( tempImg );

        // Destroy the temp image:
        tempImg->destroy();

        return Ok;
    }

    Status Image::rotateNoneFlipX( Image* image )
    {
        if ( !image ) return InvalidParameter;

        // BYTES per pixel
        UINT bytesPerPixel = image->mActiveBitmap.Stride / image->mActiveBitmap.Width;

        // Don't need to allocate a new buffer.  Just loop over the half of
        // the image and swap pixels accordingly.
        BYTE* buffer = static_cast< BYTE* >( image->mActiveBitmap.Scan0 );
        BYTE* curPix = &buffer[ 0 ];
        for ( UINT y = 0; y < image->GetHeight(); y++ )
        {
            UINT lookY = y;
            for ( UINT x = 0; x < image->GetWidth() / 2; x++ )
            {
                UINT lookX = image->GetWidth() - 1 - x;
                BYTE* pix2 = &buffer[ lookY * image->mActiveBitmap.Stride + lookX * bytesPerPixel ];
                for ( UINT curByte = 0; curByte < bytesPerPixel; curByte++ )
                {
                    // Swap the pixels:
                    BYTE tempByte = *curPix;
                    *curPix = *pix2;
                    *pix2 = tempByte;

                    ++curPix;
                    ++pix2;
                }
            }
        }

        return Ok;
    }

    Status Image::rotate90FlipX( Image* image )
    {
        if ( !image ) return InvalidParameter;

        // Create a new Image where the dimensions are transposed
        Image* tempImg = new Image( image->mType, Ok );
        if ( !tempImg ) return OutOfMemory;
        BitmapData* srcBmp  = &image->mActiveBitmap;
        tempImg->mActiveBitmap.Width     = srcBmp->Height;
        tempImg->mActiveBitmap.Height    = srcBmp->Width;
        tempImg->mActiveBitmap.PixelFormat = srcBmp->PixelFormat;
        tempImg->mActiveBitmap.Stride    = GetRowWidth( tempImg->mActiveBitmap.Width, GetPixelFormatSize( tempImg->mActiveBitmap.PixelFormat ) );
        tempImg->mActiveBitmap.Flags     = srcBmp->Flags;
        tempImg->mActiveBitmap.X         = srcBmp->X;
        tempImg->mActiveBitmap.Y         = srcBmp->Y;

        // Allocate the buffer:
        tempImg->mActiveBitmap.Scan0 = new BYTE [ tempImg->mActiveBitmap.Stride * tempImg->mActiveBitmap.Height ];
        if ( !tempImg->mActiveBitmap.Scan0 )
            return InsufficientBuffer;

        // BYTES per pixel
        UINT bytesPerPixel = srcBmp->Stride / srcBmp->Width;

        // Copy the source data to the new image, rotated 90 degrees:
        BYTE* srcBuffer  = static_cast< BYTE* >( srcBmp->Scan0 );
        BYTE* destBuffer = static_cast< BYTE* >( tempImg->mActiveBitmap.Scan0 );
        BYTE* destPix = &destBuffer[ 0 ];
        for ( UINT y = 0; y < tempImg->mActiveBitmap.Height; y++ )
        {
            UINT lookX = y;
            for ( UINT x = 0; x < tempImg->mActiveBitmap.Width; x++ )
            {
                UINT lookY = x;
                BYTE* srcPix = &srcBuffer[ lookY * srcBmp->Stride + lookX * bytesPerPixel ];
                for ( UINT curByte = 0; curByte < bytesPerPixel; curByte++ )
                {
                    *destPix = *srcPix;
                    ++destPix;
                    ++srcPix;
                }
            }
        }

        // Copy the temporary image to the source image:
        image->copyFrom( tempImg );

        // Destroy the temp image:
        tempImg->destroy();

        return Ok;
    }

    Status Image::rotate180FlipX( Image* image )
    {
        // This is the same as RotateNoneFlipY, so use that handy tidbit...
        if ( !image ) return InvalidParameter;

        // BYTES per pixel
        UINT bytesPerPixel = image->mActiveBitmap.Stride / image->mActiveBitmap.Width;

        // Don't need to allocate a new buffer.  Just loop over the half of
        // the image and swap pixels accordingly.
        BYTE* buffer = static_cast< BYTE* >( image->mActiveBitmap.Scan0 );
        BYTE* curPix = &buffer[ 0 ];
        for ( UINT y = 0; y < image->GetHeight() / 2; y++ )
        {
            UINT lookY = image->GetHeight() - 1 - y;
            for ( UINT x = 0; x < image->GetWidth(); x++ )
            {
                UINT lookX = x;
                BYTE* pix2 = &buffer[ lookY * image->mActiveBitmap.Stride + lookX * bytesPerPixel ];
                for ( UINT curByte = 0; curByte < bytesPerPixel; curByte++ )
                {
                    // Swap the pixels:
                    BYTE tempByte = *curPix;
                    *curPix = *pix2;
                    *pix2 = tempByte;

                    ++curPix;
                    ++pix2;
                }
            }
        }

        return Ok;
    }

    Status Image::rotate270FlipX( Image* image )
    {
        if ( !image ) return InvalidParameter;

        // Create a new Image where the dimensions are transposed
        Image* tempImg = new Image( image->mType, Ok );
        if ( !tempImg ) return OutOfMemory;
        BitmapData* srcBmp  = &image->mActiveBitmap;
        tempImg->mActiveBitmap.Width     = srcBmp->Height;
        tempImg->mActiveBitmap.Height    = srcBmp->Width;
        tempImg->mActiveBitmap.PixelFormat = srcBmp->PixelFormat;
        tempImg->mActiveBitmap.Stride    = GetRowWidth( tempImg->mActiveBitmap.Width, GetPixelFormatSize( tempImg->mActiveBitmap.PixelFormat ) );
        tempImg->mActiveBitmap.Flags     = srcBmp->Flags;
        tempImg->mActiveBitmap.X         = srcBmp->X;
        tempImg->mActiveBitmap.Y         = srcBmp->Y;

        // Allocate the buffer:
        tempImg->mActiveBitmap.Scan0 = new BYTE [ srcBmp->Stride * srcBmp->Height ];
        if ( !tempImg->mActiveBitmap.Scan0 )
            return InsufficientBuffer;

        // BYTES per pixel
        UINT bytesPerPixel = srcBmp->Stride / srcBmp->Width;

        // Copy the source data to the new image, rotated 90 degrees:
        BYTE* srcBuffer  = static_cast< BYTE* >( srcBmp->Scan0 );
        BYTE* destBuffer = static_cast< BYTE* >( tempImg->mActiveBitmap.Scan0 );
        BYTE* destPix = &destBuffer[ 0 ];
        for ( UINT y = 0; y < tempImg->mActiveBitmap.Height; y++ )
        {
            UINT lookX = image->GetWidth() - 1 - y;
            for ( UINT x = 0; x < tempImg->mActiveBitmap.Width; x++ )
            {
                UINT lookY = image->GetHeight() - 1 - x;
                BYTE* srcPix = &srcBuffer[ lookY * srcBmp->Stride + lookX * bytesPerPixel ];
                for ( UINT curByte = 0; curByte < bytesPerPixel; curByte++ )
                {
                    *destPix = *srcPix;
                    ++destPix;
                    ++srcPix;
                }
            }
        }

        // Copy the temporary image to the source image:
        image->copyFrom( tempImg );

        // Destroy the temp image:
        tempImg->destroy();

        return Ok;
    }

    //RotateFlip----------------------------------------------------------------
    Status Image::RotateFlip( RotateFlipType rotateFlipType )
    {
        typedef Status (*RotateFlipHelper)( Image* );
        static const RotateFlipHelper helperFuncs[] =
        {
            &Image::rotateNoneFlipNone,
            &Image::rotate90FlipNone,
            &Image::rotate180FlipNone,
            &Image::rotate270FlipNone,
            &Image::rotateNoneFlipX,
            &Image::rotate90FlipX,
            &Image::rotate180FlipX,
            &Image::rotate270FlipX
        };
        Status status = helperFuncs[ rotateFlipType ]( this );
        return SetStatus( status );
    }

    //GetBounds-----------------------------------------------------------------
    Status Image::GetBounds( RectF* rect, Unit* srcUnit )
    {
        if ( !rect || !srcUnit )
            return InvalidParameter;

        rect->X = 0;
        rect->Y = 0;
        rect->Width  = mActiveBitmap.Width;
        rect->Height = mActiveBitmap.Height;

        *srcUnit = mUnit;

        return mLastStatus;
    }

    //GetFlags------------------------------------------------------------------
    UINT Image::GetFlags() const
    {
        return mActiveBitmap.Flags;
    }

    //GetWidth------------------------------------------------------------------
    UINT Image::GetWidth() const
    {
        return mActiveBitmap.Width;
    }

    //GetHeight-----------------------------------------------------------------
    UINT Image::GetHeight() const
    {
        return mActiveBitmap.Height;
    }

    ////GetStride-----------------------------------------------------------------
    //int Image::getStride() const
    //{
    //    return mActiveBitmap.Stride;
    //}
    ////GetImageBuffer------------------------------------------------------------
    //void* Image::getImageBuffer() const
    //{
    //    return mActiveBitmap.Scan0;
    //}

    ////GetBitmapData-------------------------------------------------------------
    //BitmapData* Image::getBitmapData()
    //{
    //    return &mActiveBitmap;
    //}

    //GetHorizontalResolution---------------------------------------------------
    REAL Image::GetHorizontalResolution(void)
    {

        return 0;
    }

    //GetVerticalResolution-----------------------------------------------------
    REAL Image::GetVerticalResolution(void)
    {

        return 0;
    }

    //GetPhysicalDimension------------------------------------------------------
    Status Image::GetPhysicalDimension( SizeF* size )
    {

        return mLastStatus;
    }

    //GetPixelFormat------------------------------------------------------------
    PixelFormat Image::GetPixelFormat(void) const
    {
        return mActiveBitmap.PixelFormat;
    }

/*
    //calcRowLength-------------------------------------------------------------
    UINT Image::calcRowLength( UINT w, UINT bpp )
    {
        unsigned n = w;
        unsigned k;

        switch ( bpp )
        {
        case 1:
            k = n;
            n = n >> 3;
            if ( k & 7 ) ++n;
            break;

        case 4:
            k = n;
            n >> 1;
            if ( k & 3 ) ++n;
            break;

        case 8:
            break;

        case 16:
            n *= 2;
            break;

        case 24:
            n *= 3;
            break;

        case 32:
            n *= 4;
            break;

        case 48:
            n *= 6;
            break;

        case 64:
            n *= 8;
            break;

        default:
            n = 0;
            break;
        }
        return ( ( n + 3 ) >> 2 ) << 2;
    }
*/

} // namespace AggOO
