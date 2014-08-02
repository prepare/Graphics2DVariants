
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
// $Id: AggOOBitmap.cpp,v 1.6 2009/06/19 23:12:36 dratek Exp $
//
// AggOOBitmap.cpp
//

#include "AggOOBitmap.h"
#include "AggOOPixelFormatConverter.h"

namespace AggOO
{
    Bitmap::Bitmap( INT width, INT height, PixelFormat format ) : Image( ImageTypeBitmap )
    {
        // Determine the row length:
        UINT rowLength = GetRowWidth( width, GetPixelFormatSize( format ) );

        // Allocate the pixel buffer:
        mActiveBitmap.Scan0 = new BYTE [ rowLength * height ];
        if ( !mActiveBitmap.Scan0 )
            mLastStatus = OutOfMemory;
        else
        {
            memset( mActiveBitmap.Scan0, 0, rowLength * height );
            mActiveBitmap.Width = width;
            mActiveBitmap.Height = height;
            mActiveBitmap.Stride = rowLength;
            mActiveBitmap.PixelFormat = format;
            mActiveBitmap.Flags |= ImageFlagOwnScan0;

            mLastStatus = Ok;
        }
    }

    Bitmap::Bitmap( INT width, INT height, INT stride, PixelFormat format, BYTE* scan0 ) : Image( ImageTypeBitmap )
    {
        // Determine the row length:
        UINT rowLength = GetRowWidth( width, GetPixelFormatSize( format ) );

        // Assign the pixel data to the buffer:
        if ( scan0 )
        {
            if ( stride < 0 )
                mActiveBitmap.Scan0 = scan0 + ( height - 1 ) * stride;
            else
                mActiveBitmap.Scan0 = scan0;

            mActiveBitmap.Width = width;
            mActiveBitmap.Height = height;
            mActiveBitmap.Stride = stride;
            mActiveBitmap.PixelFormat = format;
            mActiveBitmap.Flags &= ~ImageFlagOwnScan0;

            mLastStatus = Ok;
        }
        else
            mLastStatus = InvalidParameter;
    }

    Bitmap::Bitmap( INT width, INT height, Graphics* target ) : Image( ImageTypeBitmap )
    {
    }

    Bitmap::~Bitmap(void)
    {
    }

#if ( AGGOO_ENABLE_IMAGE_LOADING == 1 || AGGOO_ENABLE_VECTOR_LOADING == 1 )
    Bitmap::Bitmap( const CHAR* filename, BOOL useIcm )
        : Image( filename, useIcm )
    {
    }

    Bitmap::Bitmap( const CHAR* filename, INT destW, INT destH, PixelFormat destFormat, BOOL useEmbeddedColorManagement )
        : Image( filename, destW, destH, destFormat, useEmbeddedColorManagement )
    {
    }

    Bitmap::Bitmap( const WCHAR* filename, BOOL useIcm )
        : Image( filename, useIcm )
    {
    }

    Bitmap::Bitmap( const WCHAR* filename, INT destW, INT destH, PixelFormat destFormat, BOOL useEmbeddedColorManagement )
        : Image( filename, destW, destH, destFormat, useEmbeddedColorManagement )
    {
    }

    //FromFile------------------------------------------------------------------
    Bitmap* Bitmap::FromFile( const CHAR* filename, BOOL useEmbeddedColorManagement )
    {
        return new Bitmap( filename, useEmbeddedColorManagement );
    }

    //FromFile------------------------------------------------------------------
    Bitmap* Bitmap::FromFile( const CHAR* filename, INT destW, INT destH, PixelFormat format, BOOL useEmbeddedColorManagement )
    {
        return new Bitmap( filename, destW, destH, format, useEmbeddedColorManagement );
    }

    //FromFile------------------------------------------------------------------
    Bitmap* Bitmap::FromFile( const WCHAR* filename, BOOL useEmbeddedColorManagement )
    {
        return new Bitmap( filename, useEmbeddedColorManagement );
    }

    //FromFile------------------------------------------------------------------
    Bitmap* Bitmap::FromFile( const WCHAR* filename, INT destW, INT destH, PixelFormat format, BOOL useEmbeddedColorManagement )
    {
        return new Bitmap( filename, destW, destH, format, useEmbeddedColorManagement );
    }
#endif // AGGOO_ENABLE_IMAGE_LOADING || AGGOO_ENABLE_VECTOR_LOADING

    //copyFrom------------------------------------------------------------------
    void Bitmap::copyPixelDataFrom( const Rect& rect, const Bitmap* src )
    {
        BitmapData outData, srcData;

        // Lock this bitmap for writing:
        Rect outRect( 0, 0, rect.Width, rect.Height );
        Rect srcRect( rect );
        LockBits( &outRect, ImageLockModeWrite, GetPixelFormat(), &outData );

        // Lock the source bitmap for reading:
        LockBits( &srcRect, ImageLockModeRead, GetPixelFormat(), &srcData );

        // Copy the pixel data:
        BYTE* outRow = (BYTE*)outData.Scan0;
        BYTE* srcRow = (BYTE*)srcData.Scan0;
        for ( int y = 0; y < min( outData.Height, srcData.Height ); y++ )
        {
            // Copy the row data:
            memcpy( outRow, srcRow, outData.Stride );

            outRow += outData.Stride;
            srcRow += srcData.Stride;
        }

        // Unlock the bitmaps:
        UnlockBits( &srcData );
        UnlockBits( &outData );
    }

    //Clone---------------------------------------------------------------------
    Bitmap* Bitmap::Clone() const
    {
        Bitmap* ret = new Bitmap( GetWidth(), GetHeight(), this->GetPixelFormat() );
        ret->copyPixelDataFrom( Rect( 0, 0, GetWidth(), GetHeight() ), this );
        return ret;
    }

    Bitmap* Bitmap::Clone( const Rect& rect, PixelFormat format ) const
    {
        Bitmap* ret = new Bitmap( rect.Width, rect.Height, this->GetPixelFormat() );
        ret->copyPixelDataFrom( rect, this );
        return ret;
    }

    Bitmap* Bitmap::Clone( const RectF& rect, PixelFormat format ) const
    {
        Bitmap* ret = new Bitmap( rect.Width, rect.Height, this->GetPixelFormat() );
        ret->copyPixelDataFrom( rect, this );
        return ret;
    }

    Bitmap* Bitmap::Clone( INT x, INT y, INT width, INT height, PixelFormat format ) const
    {
        Bitmap* ret = new Bitmap( width, height, this->GetPixelFormat() );
        ret->copyPixelDataFrom( Rect( x, y, width, height ), this );
        return ret;
    }

    Bitmap* Bitmap::Clone( REAL x, REAL y, REAL width, REAL height, PixelFormat format ) const
    {
        Bitmap* ret = new Bitmap( width, height, this->GetPixelFormat() );
        ret->copyPixelDataFrom( Rect( x, y, width, height ), this );
        return ret;
    }

    //GetPixel------------------------------------------------------------------
    Status Bitmap::GetPixel( INT x, INT y, Color* color )
    {
        // If the buffer is locked, it cannot be plotted to:
        if ( x < 0 || y < 0 || x >= mActiveBitmap.Width || y >= mActiveBitmap.Height || mActiveBitmap.Flags & ImageFlagLocked )
            return InvalidParameter;

        // TODO: Indexed and extended colors are not supported yet
        if ( IsIndexedPixelFormat( mActiveBitmap.PixelFormat ) || IsExtendedPixelFormat( mActiveBitmap.PixelFormat ) )
            return NotImplemented;

        // We have to examine the color format of the bitmap to know how to
        // adjust the color bytes.
        BYTE* row = (BYTE*)( mActiveBitmap.Scan0 ) + y * abs( mActiveBitmap.Stride );
        switch ( mActiveBitmap.PixelFormat )
        {
        case PixelFormat8bppGrayScale:
            color->MakeARGB( 255, row[ x ], row[ x ], row[ x ] );
            break;

        case PixelFormat16bppGrayScale:
            {
                unsigned short* scan = (unsigned short*)row;
                BYTE col = scan[ x ] / 65535.0 * 255;
                color->MakeARGB( 255, col, col, col );
            }
            break;

        case PixelFormat16bppRGB555:
            {
                unsigned short* scan = (unsigned short*)row;
                unsigned short col = scan[ x ];
                BYTE r = BYTE( ( ( col >> 10 ) & 0x5 ) / 31.0 * 255 );
                BYTE g = BYTE( ( ( col >> 5  ) & 0x5 ) / 31.0 * 255 );
                BYTE b = BYTE( ( ( col       ) & 0x5 ) / 31.0 * 255 );
                color->MakeARGB( 255, r, g, b );
            }
            break;

        case PixelFormat16bppRGB565:
            {
                unsigned short* scan = (unsigned short*)row;
                unsigned short col = scan[ x ];
                BYTE r = BYTE( ( ( col >> 11 ) & 0x5 ) / 31.0 * 255 );
                BYTE g = BYTE( ( ( col >> 6  ) & 0x6 ) / 63.0 * 255 );
                BYTE b = BYTE( ( ( col       ) & 0x5 ) / 31.0 * 255 );
                color->MakeARGB( 255, r, g, b );
            }
            break;

        case PixelFormat16bppARGB1555:
            {
                unsigned short* scan = (unsigned short*)row;
                unsigned short col = scan[ x ];
                BYTE a = ( col >> 15 ) ? 255 : 0;
                BYTE r = BYTE( ( ( col >> 10 ) & 0x5 ) / 31.0 * 255 );
                BYTE g = BYTE( ( ( col >> 5  ) & 0x5 ) / 31.0 * 255 );
                BYTE b = BYTE( ( ( col       ) & 0x5 ) / 31.0 * 255 );
                color->MakeARGB( a, r, g, b );
            }
            break;

        case PixelFormat24bppRGB:
            {
                BYTE r = row[ x * 3 ];
                BYTE g = row[ x * 3 + 1 ];
                BYTE b = row[ x * 3 + 2 ];
                color->MakeARGB( 255, r, g, b );
            }
            break;

        case PixelFormat24bppBGR:
            {
                BYTE b = row[ x * 3 ];
                BYTE g = row[ x * 3 + 1 ];
                BYTE r = row[ x * 3 + 2 ];
                color->MakeARGB( 255, r, g, b );
            }
            break;

        case PixelFormat32bppARGB:
            color->MakeARGB( row[ x * 4 ], row[ x * 4 + 1 ], row[ x * 4 + 2 ], row[ x * 4 + 3 ] );
            break;

        case PixelFormat32bppABGR:
            color->MakeARGB( row[ x * 4 ], row[ x * 4 + 3 ], row[ x * 4 + 2 ], row[ x * 4 + 1 ] );
            break;

        case PixelFormat32bppRGBA:
            color->MakeARGB( row[ x * 4 + 1 ], row[ x * 4 + 2 ], row[ x * 4 + 3 ], row[ x * 4 ] );
            break;

        case PixelFormat32bppBGRA:
            color->MakeARGB( row[ x * 4 + 3 ], row[ x * 4 + 2 ], row[ x * 4 + 1 ], row[ x * 4 ] );
            break;

        }

        return SetStatus( Ok );
    }

    //SetPixel------------------------------------------------------------------
    Status Bitmap::SetPixel( INT x, INT y, const Color& color )
    {
        // If the buffer is locked, it cannot be plotted to:
        if ( x < 0 || y < 0 || x >= mActiveBitmap.Width || y >= mActiveBitmap.Height || mActiveBitmap.Flags & ImageFlagLocked )
            return InvalidParameter;

        // TODO: Indexed colors are not supported yet
        if ( IsIndexedPixelFormat( mActiveBitmap.PixelFormat ) || IsExtendedPixelFormat( mActiveBitmap.PixelFormat ) )
            return NotImplemented;

        // Set the color:
        BYTE* row = (BYTE*)( mActiveBitmap.Scan0 ) + y * abs( mActiveBitmap.Stride );
        switch ( mActiveBitmap.PixelFormat )
        {
        case PixelFormat8bppGrayScale:
            row[ x ] = ( color.GetR() * 0.299 ) + ( color.GetG() * 0.587 ) + ( color.GetB() * 0.114 );
            break;

        case PixelFormat16bppGrayScale:
            {
                unsigned short* scan = (unsigned short*)row;
                unsigned short col = ( ( color.GetR() * 0.299 ) + ( color.GetG() * 0.587 ) + ( color.GetB() * 0.114 ) ) / 255.0 * 65535;
                scan[ x ] = col;
            }
            break;

        case PixelFormat16bppRGB555:
            {
                unsigned short* scan = (unsigned short*)row;
                unsigned short r = (unsigned short)( ( color.GetR() / 255.0 ) * 31 );
                unsigned short g = (unsigned short)( ( color.GetG() / 255.0 ) * 31 );
                unsigned short b = (unsigned short)( ( color.GetB() / 255.0 ) * 31 );
                scan[ x ] = ( r << 10 ) | ( g << 5 ) | b;
            }
            break;

        case PixelFormat16bppRGB565:
            {
                unsigned short* scan = (unsigned short*)row;
                unsigned short r = (unsigned short)( ( color.GetR() / 255.0 ) * 31 );
                unsigned short g = (unsigned short)( ( color.GetG() / 255.0 ) * 63 );
                unsigned short b = (unsigned short)( ( color.GetB() / 255.0 ) * 31 );
                scan[ x ] = ( r << 10 ) | ( g << 5 ) | b;
            }
            break;

        case PixelFormat16bppARGB1555:
            {
                unsigned short* scan = (unsigned short*)row;
                unsigned short a = ( color.GetA() ) ? 1 : 0;
                unsigned short r = (unsigned short)( ( color.GetR() / 255.0 ) * 31 );
                unsigned short g = (unsigned short)( ( color.GetG() / 255.0 ) * 63 );
                unsigned short b = (unsigned short)( ( color.GetB() / 255.0 ) * 31 );
                scan[ x ] = ( r << 10 ) | ( g << 5 ) | b;
            }
            break;

        case PixelFormat24bppRGB:
            {
                row[ x * 3     ] = color.GetR();
                row[ x * 3 + 1 ] = color.GetG();
                row[ x * 3 + 2 ] = color.GetB();
            }
            break;

        case PixelFormat24bppBGR:
            {
                row[ x * 3     ] = color.GetB();
                row[ x * 3 + 1 ] = color.GetG();
                row[ x * 3 + 2 ] = color.GetR();
            }
            break;

        case PixelFormat32bppARGB:
            row[ x * 4 + 0 ] = color.GetA();
            row[ x * 4 + 1 ] = color.GetR();
            row[ x * 4 + 2 ] = color.GetG();
            row[ x * 4 + 3 ] = color.GetB();
            break;

        case PixelFormat32bppABGR:
            row[ x * 4 + 0 ] = color.GetA();
            row[ x * 4 + 1 ] = color.GetB();
            row[ x * 4 + 2 ] = color.GetG();
            row[ x * 4 + 3 ] = color.GetR();
            break;

        case PixelFormat32bppRGBA:
            row[ x * 4 + 0 ] = color.GetR();
            row[ x * 4 + 1 ] = color.GetG();
            row[ x * 4 + 2 ] = color.GetB();
            row[ x * 4 + 3 ] = color.GetA();
            break;

        case PixelFormat32bppBGRA:
            row[ x * 4 + 0 ] = color.GetB();
            row[ x * 4 + 1 ] = color.GetG();
            row[ x * 4 + 2 ] = color.GetR();
            row[ x * 4 + 3 ] = color.GetA();
            break;

        }

        return SetStatus( Ok );
    }

    Status Bitmap::LockBits( const Rect* rect, UINT flags, PixelFormat format, BitmapData* lockedBitmapData )
    {
        // TODO:
        // Not fully implemented yet.

        INT             destBPP;
        INT             destSize;
        INT             destStride;
        unsigned char*  destScan0;

        // Make sure the parameters are valid:
        if ( !rect || !lockedBitmapData || !flags )
            return SetStatus( InvalidParameter );

        Rect            destRect( 0, 0, rect->Width, rect->Height );
        BitmapData*     rootData = &mActiveBitmap;

        // If the bitmap is already locked, then it cannot be locked again
        if ( rootData->Flags & ImageFlagLocked )
            return SetStatus( InvalidParameter );

        // Indexed formats are not currently supported:
        if ( IsIndexedPixelFormat( rootData->PixelFormat ) || IsIndexedPixelFormat( format ) )
            return SetStatus( NotImplemented );

        // Make sure the source rect is valid:
        if ( rect->GetRight() > rootData->Width || rect->GetBottom() > rootData->Height )
            return SetStatus( InvalidParameter );

        // TODO: Test that the format is supported

        // Set flags to indicate how the data is locked:
        if ( flags & ImageLockModeWrite )
        {
            lockedBitmapData->Flags |= ImageFlagWriteOk;
        }
        else
        {
            lockedBitmapData->Flags &= ~ImageFlagWriteOk;
        }

        lockedBitmapData->Flags |= ImageFlagLocked | ImageFlagOwnScan0;
        rootData->Flags         |= ImageFlagLocked;

        destBPP     = GetPixelFormatSize( format );
        destStride  = GetRowWidth( rect->Width, destBPP );
        destSize    = destStride * rect->Height;

        // If the user is supplying the buffer, then make sure the buffer is
        // valid and clear the 'OwnScan0' flag:
        if ( flags & ImageLockModeUserInputBuf )
        {
            if ( !lockedBitmapData->Scan0 )
                return SetStatus( InvalidParameter );

            lockedBitmapData->Flags &= ~ImageFlagOwnScan0;
        }
        else
        {
            // GDI+ does this a bit differently.  If the pixel format of the
            // locked data is the same as the source, it basically returns a
            // pointer to the source buffer, regardless of what the flags are.
            // This is faster, as it doesn't require allocating/deallocating the
            // buffer.  However, it seems pointless in that it is possible to
            // lock the bitmap for reading, but still be able to modify the
            // pixels.  AggOO will always allocate a temporary buffer, which
            // will be copied to the bitmap during UnlockBits.

            // Allocate the buffer:
            lockedBitmapData->Scan0 = (VOID*) new BYTE [ destSize ];
            if ( !lockedBitmapData->Scan0 )
                return SetStatus( OutOfMemory );
        }

        lockedBitmapData->X         = rect->X;
        lockedBitmapData->Y         = rect->Y;
        lockedBitmapData->Width     = rect->Width;
        lockedBitmapData->Height    = rect->Height;
        lockedBitmapData->Stride    = destStride;
        lockedBitmapData->PixelFormat = format;

        // If the locked data is to be readable, copy the bitmap data to the
        // locked buffer:
        Status status = Ok;
        if ( !( flags & ImageFlagWriteOk ) )    // If its not writable, it is considered readable.
        {
            // Convert the source data to the destination data:
            status = BufferConverter::ConvertPixelFormat( rootData, lockedBitmapData );
        }

        return SetStatus( status );
    }

    Status Bitmap::UnlockBits( BitmapData* lockedBitmapData )
    {
        // Make sure the bitmap data is valid, and that it is locked:
        if ( !lockedBitmapData || !( lockedBitmapData->Flags & ImageFlagLocked ) || !( mActiveBitmap.Flags & ImageFlagLocked ) )
            return SetStatus( InvalidParameter );

        BitmapData* rootData = &mActiveBitmap;

        // Make sure the locked data's size is valid:
        if ( ( lockedBitmapData->Width > rootData->Width ) || ( lockedBitmapData->Height > rootData->Height ) )
            return SetStatus( InvalidParameter );

        // If the locked data was writable, copy the data back to the bitmap:
        if ( lockedBitmapData->Flags & ImageFlagWriteOk )
        {
            Rect    srcRect( 0, 0, lockedBitmapData->Width, lockedBitmapData->Height );
            Rect    dstRect( lockedBitmapData->X, lockedBitmapData->Y, lockedBitmapData->Width, lockedBitmapData->Height );

            // Copy the pixel data (doing a format conversion if necessary)
            BufferConverter::ConvertPixelFormat( lockedBitmapData, rootData );

        }

        // If LockBits allocated the buffer, release it now:
        if ( lockedBitmapData->Flags & ImageFlagOwnScan0 )
        {
            delete [] lockedBitmapData->Scan0;
            lockedBitmapData->Scan0 = 0;
            lockedBitmapData->Flags &= ~ImageFlagOwnScan0;
        }



        // Unlock the bitmap:
        lockedBitmapData->Flags &= ~ImageFlagLocked;
        rootData->Flags         &= ~ImageFlagLocked;

        return SetStatus( Ok );
    }

} // namespace AggOO
