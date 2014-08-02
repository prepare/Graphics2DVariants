
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
// $Id: AggOOPixelFormatConverter.h,v 1.4 2008/01/10 18:02:10 dratek Exp $
//
// PixelFormatConverter.h -- Class for converting buffers from one pixel format
//                           to another.
//
// Author       : Chad M. Draper
// Date         : September 14, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-14      Initial implementation
//

#ifndef AggOOPixelFormatConverter_H_
#define AggOOPixelFormatConverter_H_

#include "AggOOPixelFormats.h"
#include "AggOOImaging.h"

//#include <cmd/LogFileManager.h>

namespace AggOO
{
    /** @class BufferConverter

        This class is designed to convert a buffer from a given PixelFormat
        to another PixelFormat.  This will use AGG utility methods to
        perform the conversion, but the sources will be raw data.
    */
    class _AggOOExport BufferConverter
    {
    public:
        /** ConvertPixelFormat

            Convert a buffer from a given pixel format to a different pixel format.
        */
        static Status
        ConvertPixelFormat( const BitmapData* src, BitmapData* dest );

    private:

        // Convert a buffer of a given format to a buffer of the same format:
        static Status
        convertToSame( const BitmapData* src, BitmapData* dest );

        // Helper routines to convert to a destination format:
        static Status
        convertToGray8( const BitmapData* src, BitmapData* dest );

        static Status
        convertToGray16( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo16bppRGB555( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo16bppRGB565( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo24bppRGB( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo24bppBGR( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo32bppARGB( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo32bppRGBA( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo32bppBGRA( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo32bppABGR( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo48bppRGB( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo48bppBGR( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo64bppARGB( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo64bppRGBA( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo64bppBGRA( const BitmapData* src, BitmapData* dest );

        static Status
        convertTo64bppABGR( const BitmapData* src, BitmapData* dest );

        // Iterate over buffers, converting the source to the destination,
        // row-by-row.
        template< class CopyRow >
        static Status convertBuffers( const BitmapData* src, BitmapData* dest, CopyRow copyRowFunctor )
        {
            if ( !src || !dest || src->Stride == 0 || dest->Stride == 0 )
                return InvalidParameter;

            INT     bufWidth    = min( src->Width, dest->Width );
            INT     bufHeight   = min( src->Height, dest->Height );

            size_t y;
            size_t srcPixelWidth    = GetPixelFormatSize( src->PixelFormat ) / 8;
            size_t destPixelWidth   = GetPixelFormatSize( dest->PixelFormat ) / 8;
            const BYTE* srcBuf = (const BYTE*)src->Scan0;
            BYTE* destBuf = (BYTE*)dest->Scan0;
            if ( !srcBuf || !destBuf )
                return InvalidParameter;
            for ( y = 0; y < bufHeight; y++ )
            {
                // Need to make sure the pixel indices are in-bounds:

                // Source row:
                const BYTE* srcRow = &srcBuf[ ( y + src->Y ) * src->Stride + src->X * srcPixelWidth ];
                BYTE* destRow = &destBuf[ ( y + dest->Y ) * dest->Stride + dest->X * destPixelWidth ];
                agg::color_conv_row( destRow, srcRow, bufWidth, copyRowFunctor );
            }

            return Ok;
        }

    }; // class BufferConverter


    //==========================================================================

    /** @class PixelFormatConverter

        Class for converting pixel formats between AggOO and AGG
    */
    class _AggOOExport PixelFormatConverter
    {
    public:

        /** Convert an AggOO::PixelFormat value to a corresponding AGG value */
        static unsigned
        convertAggOOtoAGG( PixelFormat fmt );

        /** Convert an agg::pix_format_e value to a corresponding AggOO value */
        static PixelFormat
        convertAGGtoAggOO( unsigned fmt );


    }; // class PixelFormatConverter

} // namespace AggOO

#endif  // AggOOPixelFormatConverter_H_
