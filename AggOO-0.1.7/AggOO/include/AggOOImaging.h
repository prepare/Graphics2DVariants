
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
// $Id: AggOOImaging.h,v 1.5 2008/02/28 20:55:48 dratek Exp $
//
// AggOOImaging.h -- Provides values needed for image classes
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

#ifndef AggOOImaging_H_
#define AggOOImaging_H_

#include "AggOOPixelFormats.h"
#include "AggOOMath.h"

#include <iostream>

//#include <cmd/LogFileManager.h>
//using cmd::LogFileManager;
//using cmd::LogFile;
//using cmd::LogFileSection;

namespace AggOO
{
    //--------------------------------------------------------------------------
    // Information flags about image codecs
    //--------------------------------------------------------------------------

    enum ImageCodecFlags
    {
        ImageCodecFlagsEncoder            = 0x00000001,
        ImageCodecFlagsDecoder            = 0x00000002,
        ImageCodecFlagsSupportBitmap      = 0x00000004,
        ImageCodecFlagsSupportVector      = 0x00000008,
        ImageCodecFlagsSeekableEncode     = 0x00000010,
        ImageCodecFlagsBlockingDecode     = 0x00000020,

        ImageCodecFlagsBuiltin            = 0x00010000,
        ImageCodecFlagsSystem             = 0x00020000,
        ImageCodecFlagsUser               = 0x00040000
    };

    //---------------------------------------------------------------------------
    // Access modes used when calling Image::LockBits
    //---------------------------------------------------------------------------

    enum ImageLockMode
    {
        ImageLockModeRead        = 0x0001,
        ImageLockModeWrite       = 0x0002,
        ImageLockModeUserInputBuf= 0x0004
    };

    // Flags used when locking a bitmap
    enum ImageFlag
    {
        ImageFlagOwnScan0       = ( 1 << 8 ),
        ImageFlagWriteOk        = ( 1 << 9 ),
        ImageFlagLocked         = ( 1 << 10 )
    };

    //---------------------------------------------------------------------------
    // Information about image pixel data
    //---------------------------------------------------------------------------

    /** @class BitmapData

        Defines a bitmap.
    */
    class _AggOOExport BitmapData
    {
        friend class Image;
        friend class Bitmap;
        friend class Graphics;
        friend class BufferConverter;
        friend class AbstractPixelFormat;
        template< class PixFmt > friend class SpecificPixelFormat;
        template< class PixFmt > friend class GraphicsRenderer;

    public:
        UINT Width;
        UINT Height;
        INT Stride;
        PixelFormat PixelFormat;
        VOID* Scan0;
        UINT_PTR Reserved;

        BitmapData()
            : X( 0 ), Y( 0 ), Width( 0 ), Height( 0 ),
              Stride( 0 ), PixelFormat( 0 ), Flags( 0 ), Scan0( 0 ), Reserved( 0 )
        {
        }

        // Deconstructor
        ~BitmapData()
        {
            if ( Scan0 && ( Flags & ImageFlagOwnScan0 ) )
            {
                delete [] Scan0;
                Scan0 = 0;
            }
        }

    private:

        // Used internally
        UINT Flags;
        UINT X, Y;

        // Constructor:
        BitmapData( const Rect& rect, INT stride, AggOO::PixelFormat format, UINT flags, VOID* scan0 )
        {
            X       = rect.X;
            Y       = rect.Y;
            Width   = IMinClamp( rect.Width, 0 );
            Height  = IMinClamp( rect.Height, 0 );
            Stride  = stride;
            PixelFormat = format;
            Flags   = flags;
            Scan0   = scan0;
            Reserved = 0;

            if ( rect.X < 0 )
            {
                X       = 0;
                Width   = IMinClamp( rect.Width + rect.X, 0 );
            }
            if ( rect.Y < 0 )
            {
                Y       = 0;
                Height  = IMinClamp( rect.Height + rect.Y, 0 );
            }
        }

        //! Write to a stream
        inline friend std::ostream& operator<<( std::ostream& stream, const BitmapData& src )
        {
            std::ostream::sentry opfx(stream);
            if ( !opfx )
                return stream;
            stream << "BitmapData( \n";
            stream << "    Rect = ( " << src.X << ", " << src.Y << ", " << src.Width << ", " << src.Height << " )\n";
            stream << "    Stride = " << src.Stride << std::endl;
            stream << "    PixelFormat = " << src.PixelFormat << std::endl;
            stream << "    Flags = " << src.Flags << std::endl;
            stream << ")\n";
            return stream;
        }
    };

/*
    //! Write to a stream
    inline std::ostream& operator<<( std::ostream& stream, const BitmapData& src )
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;
        stream << "BitmapData( \n";
        stream << "    Rect = ( " << src.X << ", " << src.Y << ", " << src.Width << ", " << src.Height << " )\n";
        stream << "    Stride = " << src.Stride << std::endl;
        stream << "    PixelFormat = " << src.PixelFormat << std::endl;
        stream << "    Flags = " << src.Flags << std::endl;
        stream << ")\n";
        return stream;
    }
*/

    //---------------------------------------------------------------------------
    // Image flags
    //---------------------------------------------------------------------------

    enum ImageFlags
    {
        ImageFlagsNone                = 0,

        // Low-word: shared with SINKFLAG_x

        ImageFlagsScalable            = 0x0001,
        ImageFlagsHasAlpha            = 0x0002,
        ImageFlagsHasTranslucent      = 0x0004,
        ImageFlagsPartiallyScalable   = 0x0008,

        // Low-word: color space definition

        ImageFlagsColorSpaceRGB       = 0x0010,
        ImageFlagsColorSpaceCMYK      = 0x0020,
        ImageFlagsColorSpaceGRAY      = 0x0040,
        ImageFlagsColorSpaceYCBCR     = 0x0080,
        ImageFlagsColorSpaceYCCK      = 0x0100,

        // Low-word: image size info

        ImageFlagsHasRealDPI          = 0x1000,
        ImageFlagsHasRealPixelSize    = 0x2000,

        // High-word

        ImageFlagsReadOnly            = 0x00010000,
        ImageFlagsCaching             = 0x00020000
    };

    enum RotateFlipType
    {
        RotateNoneFlipNone = 0,
        Rotate90FlipNone   = 1,
        Rotate180FlipNone  = 2,
        Rotate270FlipNone  = 3,

        RotateNoneFlipX    = 4,
        Rotate90FlipX      = 5,
        Rotate180FlipX     = 6,
        Rotate270FlipX     = 7,

        RotateNoneFlipY    = Rotate180FlipX,
        Rotate90FlipY      = Rotate270FlipX,
        Rotate180FlipY     = RotateNoneFlipX,
        Rotate270FlipY     = Rotate90FlipX,

        RotateNoneFlipXY   = Rotate180FlipNone,
        Rotate90FlipXY     = Rotate270FlipNone,
        Rotate180FlipXY    = RotateNoneFlipNone,
        Rotate270FlipXY    = Rotate90FlipNone
    };

} // namespace AggOO

#endif  // AggOOImaging_H_
