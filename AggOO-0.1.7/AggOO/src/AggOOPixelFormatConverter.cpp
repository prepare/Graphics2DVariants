
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
// $Id: AggOOPixelFormatConverter.cpp,v 1.2 2007/10/03 14:13:51 dratek Exp $
//
// PixelFormatConverter.cpp

#include "AggOOPixelFormatConverter.h"
#include "platform/agg_platform_support.h"

namespace AggOO
{
    //ConvertPixelFormat--------------------------------------------------------
    Status BufferConverter::ConvertPixelFormat( const BitmapData* src, BitmapData* dest )
    {
        if ( !src || !dest )
            return InvalidParameter;

        //// Make sure the regions are valid:
        //if ( ( dest->X + dest->Width > src->Width ) || ( dest->Y + dest->Height > src->Height ) )
        //    return InvalidParameter;

        // If the source and destination formats are the same, conversion is
        // fairly easy:
//        const int sourceBytesPerPixel = GetPixelFormatSize( src->PixelFormat ) / 8;
//        const int destBytesPerPixel   = GetPixelFormatSize( dest->PixelFormat ) / 8;
        if ( src->PixelFormat == dest->PixelFormat )
            return convertToSame( src, dest );

        // TODO: A number of the conversions are not yet implemented

        // There are a lot of possible conversions.  To simplify things a bit,
        // first look at the destination format, then call an appropriate method
        // to convert to that from the source format.
        switch ( dest->PixelFormat )
        {
        case PixelFormat1bppIndexed:
            return NotImplemented;

        case PixelFormat4bppIndexed:
            return NotImplemented;

        case PixelFormat8bppIndexed:
            return NotImplemented;

        case PixelFormat8bppGrayScale:
            return convertToGray8( src, dest );

        case PixelFormat16bppGrayScale:
            return convertToGray16( src, dest );

        case PixelFormat16bppRGB555:
            return convertTo16bppRGB555( src, dest );

        case PixelFormat16bppRGB565:
            return convertTo16bppRGB565( src, dest );

        case PixelFormat16bppARGB1555:
            return NotImplemented;

        case PixelFormat24bppRGB:
            return convertTo24bppRGB( src, dest );

        case PixelFormat24bppBGR:
            return convertTo24bppBGR( src, dest );

        case PixelFormat32bppARGB:
            return convertTo32bppARGB( src, dest );

        case PixelFormat32bppRGBA:
            return convertTo32bppRGBA( src, dest );

        case PixelFormat32bppBGRA:
            return convertTo32bppBGRA( src, dest );

        case PixelFormat32bppABGR:
            return convertTo32bppABGR( src, dest );

        case PixelFormat48bppRGB:
            return convertTo48bppRGB( src, dest );

        case PixelFormat48bppBGR:
            return convertTo48bppRGB( src, dest );

        case PixelFormat64bppARGB:
            return convertTo64bppARGB( src, dest );

        case PixelFormat64bppRGBA:
            return convertTo64bppRGBA( src, dest );

        case PixelFormat64bppBGRA:
            return convertTo64bppBGRA( src, dest );

        case PixelFormat64bppABGR:
            return convertTo64bppABGR( src, dest );

        }

        return GenericError;
    }

    // ConvertSame--------------------------------------------------------------
    Status BufferConverter::convertToSame( const BitmapData* src, BitmapData* dest )
    {
        const int sourceBytesPerPixel = GetPixelFormatSize( src->PixelFormat ) >> 3;
        switch ( sourceBytesPerPixel )
        {
        case 1:
            return convertBuffers( src, dest, agg::color_conv_same< 1 >() );
            break;

        case 2:
            return convertBuffers( src, dest, agg::color_conv_same< 2 >() );
            break;

        case 3:
            return convertBuffers( src, dest, agg::color_conv_same< 3 >() );
            break;

        case 4:
            return convertBuffers( src, dest, agg::color_conv_same< 4 >() );
            break;

        case 6:
            return convertBuffers( src, dest, agg::color_conv_same< 6 >() );
            break;

        case 8:
            return convertBuffers( src, dest, agg::color_conv_same< 8 >() );
            break;

        default:
            return NotImplemented;
        }
    }


    //--------------------------------------------------------------------------
    // Helper routines to convert to a destination format:

    Status BufferConverter::convertToGray8( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat16bppGrayScale:
            return convertBuffers( src, dest, agg::color_conv_gray16_to_gray8() );

        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_gray8() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_gray8() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertToGray16( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_gray16() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_gray16() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo16bppRGB555( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat16bppRGB565:
            return convertBuffers( src, dest, agg::color_conv_rgb565_to_rgb555() );

        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_rgb555() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_rgb555() );

        case PixelFormat32bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba32_to_rgb555() );

        case PixelFormat32bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb32_to_rgb555() );

        case PixelFormat32bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra32_to_rgb555() );

        case PixelFormat32bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr32_to_rgb555() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo16bppRGB565( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat16bppRGB565:
            return convertBuffers( src, dest, agg::color_conv_rgb555_to_rgb565() );

        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_rgb565() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_rgb565() );

        case PixelFormat32bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba32_to_rgb565() );

        case PixelFormat32bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb32_to_rgb565() );

        case PixelFormat32bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra32_to_rgb565() );

        case PixelFormat32bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr32_to_rgb565() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo24bppRGB( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat16bppRGB555:
            return convertBuffers( src, dest, agg::color_conv_rgb555_to_rgb24() );

        case PixelFormat16bppRGB565:
            return convertBuffers( src, dest, agg::color_conv_rgb565_to_rgb24() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_rgb24() );

        case PixelFormat32bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb32_to_rgb24() );

        case PixelFormat32bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba32_to_rgb24() );

        case PixelFormat32bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra32_to_rgb24() );

        case PixelFormat32bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr32_to_rgb24() );

        case PixelFormat30bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgbAAA_to_rgb24() );

        case PixelFormat30bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgrAAA_to_rgb24() );

        case PixelFormat32bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgbBBA_to_rgb24() );

        case PixelFormat32bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgrABB_to_rgb24() );

        case PixelFormat48bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb48_to_rgb24() );

        case PixelFormat48bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr48_to_rgb24() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo24bppBGR( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat16bppRGB555:
            return convertBuffers( src, dest, agg::color_conv_rgb555_to_bgr24() );

        case PixelFormat16bppRGB565:
            return convertBuffers( src, dest, agg::color_conv_rgb565_to_bgr24() );

        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_bgr24() );

        case PixelFormat32bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb32_to_bgr24() );

        case PixelFormat32bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba32_to_bgr24() );

        case PixelFormat32bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra32_to_bgr24() );

        case PixelFormat32bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr32_to_bgr24() );

        case PixelFormat30bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgbAAA_to_bgr24() );

        case PixelFormat30bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgrAAA_to_bgr24() );

        case PixelFormat32bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgbBBA_to_bgr24() );

        case PixelFormat32bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgrABB_to_bgr24() );

        case PixelFormat48bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb48_to_bgr24() );

        case PixelFormat48bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr48_to_bgr24() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo32bppARGB( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat16bppRGB555:
            return convertBuffers( src, dest, agg::color_conv_rgb555_to_argb32() );

        case PixelFormat16bppRGB565:
            return convertBuffers( src, dest, agg::color_conv_rgb565_to_argb32() );

        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_argb32() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_argb32() );

        case PixelFormat32bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba32_to_argb32() );

        case PixelFormat32bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra32_to_argb32() );

        case PixelFormat32bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr32_to_argb32() );

        case PixelFormat64bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb64_to_argb32() );

        case PixelFormat64bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba64_to_argb32() );

        case PixelFormat64bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra64_to_argb32() );

        case PixelFormat64bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr64_to_argb32() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo32bppRGBA( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat16bppRGB555:
            return convertBuffers( src, dest, agg::color_conv_rgb555_to_rgba32() );

        case PixelFormat16bppRGB565:
            return convertBuffers( src, dest, agg::color_conv_rgb565_to_rgba32() );

        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_rgba32() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_rgba32() );

        case PixelFormat32bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb32_to_rgba32() );

        case PixelFormat32bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra32_to_rgba32() );

        case PixelFormat32bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr32_to_rgba32() );

        case PixelFormat64bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb64_to_rgba32() );

        case PixelFormat64bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba64_to_rgba32() );

        case PixelFormat64bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra64_to_rgba32() );

        case PixelFormat64bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr64_to_rgba32() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo32bppBGRA( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat16bppRGB555:
            return convertBuffers( src, dest, agg::color_conv_rgb555_to_bgra32() );

        case PixelFormat16bppRGB565:
            return convertBuffers( src, dest, agg::color_conv_rgb565_to_bgra32() );

        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_bgra32() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_bgra32() );

        case PixelFormat32bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba32_to_bgra32() );

        case PixelFormat32bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb32_to_bgra32() );

        case PixelFormat32bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr32_to_bgra32() );

        case PixelFormat64bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb64_to_bgra32() );

        case PixelFormat64bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba64_to_bgra32() );

        case PixelFormat64bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra64_to_bgra32() );

        case PixelFormat64bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr64_to_bgra32() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo32bppABGR( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat16bppRGB555:
            return convertBuffers( src, dest, agg::color_conv_rgb555_to_abgr32() );

        case PixelFormat16bppRGB565:
            return convertBuffers( src, dest, agg::color_conv_rgb565_to_abgr32() );

        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_abgr32() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_abgr32() );

        case PixelFormat32bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba32_to_abgr32() );

        case PixelFormat32bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra32_to_abgr32() );

        case PixelFormat32bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb32_to_abgr32() );

        case PixelFormat64bppARGB:
            return convertBuffers( src, dest, agg::color_conv_argb64_to_abgr32() );

        case PixelFormat64bppRGBA:
            return convertBuffers( src, dest, agg::color_conv_rgba64_to_abgr32() );

        case PixelFormat64bppBGRA:
            return convertBuffers( src, dest, agg::color_conv_bgra64_to_abgr32() );

        case PixelFormat64bppABGR:
            return convertBuffers( src, dest, agg::color_conv_abgr64_to_abgr32() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo48bppRGB( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_rgb48() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_rgb48() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo48bppBGR( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_bgr48() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_bgr48() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo64bppARGB( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_argb64() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_argb64() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo64bppRGBA( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_rgba64() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_rgba64() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo64bppBGRA( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_bgra64() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_bgra64() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    Status BufferConverter::convertTo64bppABGR( const BitmapData* src, BitmapData* dest )
    {
        // Convert the buffers based on the source format.  Conversion might not
        // be possible for all formats.
        switch ( src->PixelFormat )
        {
        case PixelFormat24bppRGB:
            return convertBuffers( src, dest, agg::color_conv_rgb24_to_abgr64() );

        case PixelFormat24bppBGR:
            return convertBuffers( src, dest, agg::color_conv_bgr24_to_abgr64() );

        default:
            return InvalidParameter;
        }

        // fail-safe:
        return GenericError;
    }

    //convertAggOOtoAGG---------------------------------------------------------
    unsigned PixelFormatConverter::convertAggOOtoAGG( PixelFormat fmt )
    {
        switch ( fmt )
        {
        case PixelFormat1bppIndexed:
            return agg::pix_format_bw;

        case PixelFormat4bppIndexed:
            return agg::pix_format_gray8;

        case PixelFormat8bppIndexed:
        case PixelFormat8bppGrayScale:
            return agg::pix_format_gray8;

        case PixelFormat16bppGrayScale:
            return agg::pix_format_gray16;

        case PixelFormat16bppRGB555:
            return agg::pix_format_rgb555;

        case PixelFormat16bppRGB565:
            return agg::pix_format_rgb565;

        case PixelFormat16bppARGB1555:
            return agg::pix_format_rgb555;

        case PixelFormat24bppRGB:
            return agg::pix_format_rgb24;

        case PixelFormat24bppBGR:
            return agg::pix_format_bgr24;

        case PixelFormat30bppRGB:
            return agg::pix_format_rgbAAA;

        case PixelFormat30bppBGR:
            return agg::pix_format_bgrAAA;

        case PixelFormat32bppRGB:
            return agg::pix_format_rgbBBA;

        case PixelFormat32bppBGR:
            return agg::pix_format_bgrABB;

        case PixelFormat32bppARGB:
        case PixelFormat32bppPARGB:
            return agg::pix_format_argb32;

        case PixelFormat32bppABGR:
            return agg::pix_format_abgr32;

        case PixelFormat32bppRGBA:
            return agg::pix_format_rgba32;

        case PixelFormat32bppBGRA:
            return agg::pix_format_bgra32;

        case PixelFormat48bppRGB:
            return agg::pix_format_rgb48;

        case PixelFormat48bppBGR:
            return agg::pix_format_bgr48;

        case PixelFormat64bppARGB:
            return agg::pix_format_argb64;

        case PixelFormat64bppPARGB:
            return agg::pix_format_argb64;

        case PixelFormat64bppABGR:
            return agg::pix_format_abgr64;

        case PixelFormat64bppRGBA:
            return agg::pix_format_rgba64;

        case PixelFormat64bppBGRA:
            return agg::pix_format_bgra64;

        default:
            return agg::pix_format_undefined;
        }
    }

    //convertAGGtoAggOO---------------------------------------------------------
    PixelFormat PixelFormatConverter::convertAGGtoAggOO( unsigned fmt )
    {
        switch ( fmt )
        {
        case agg::pix_format_bw:
            return PixelFormat1bppIndexed;

        case agg::pix_format_gray8:
            return PixelFormat4bppIndexed;

        case agg::pix_format_gray16:
            return PixelFormat16bppGrayScale;

        case agg::pix_format_rgb555:
            return PixelFormat16bppRGB555;

        case agg::pix_format_rgb565:
            return PixelFormat16bppRGB565;

        case agg::pix_format_rgb24:
            return PixelFormat24bppRGB;

        case agg::pix_format_bgr24:
            return PixelFormat24bppBGR;

        case agg::pix_format_rgbAAA:
            return PixelFormat30bppRGB;

        case agg::pix_format_bgrAAA:
            return PixelFormat30bppBGR;

        case agg::pix_format_rgbBBA:
            return PixelFormat32bppRGB;

        case agg::pix_format_bgrABB:
            return PixelFormat32bppBGR;

        case agg::pix_format_argb32:
            return PixelFormat32bppARGB;

        case agg::pix_format_abgr32:
            return PixelFormat32bppABGR;

        case agg::pix_format_rgba32:
            return PixelFormat32bppRGBA;

        case agg::pix_format_bgra32:
            return PixelFormat32bppBGRA;

        case agg::pix_format_rgb48:
            return PixelFormat48bppRGB;

        case agg::pix_format_bgr48:
            return PixelFormat48bppBGR;

        case agg::pix_format_argb64:
            return PixelFormat64bppARGB;

        case agg::pix_format_abgr64:
            return PixelFormat64bppABGR;

        case agg::pix_format_rgba64:
            return PixelFormat64bppRGBA;

        case agg::pix_format_bgra64:
            return PixelFormat64bppBGRA;

        default:
            return PixelFormatMax;
        }
    }

    //--------------------------------------------------------------------------

} // namespace AggOO
