
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
are(met)

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

/*! $Id: AggOOAbstractPixelFormat.cpp,v 1.2 2009/06/19 23:12:36 dratek Exp $
 *  @file   AggOOAbstractPixelFormat.cpp
 *  
 */

#include "AggOOAbstractPixelFormat.h"

#include "agg_pixfmt_gray.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgb_packed.h"
#include "agg_pixfmt_rgba.h"

namespace AggOO
{
    // Fill in the array for creating the pixel format
    const AbstractPixelFormat::CreateFunc AbstractPixelFormat::createPixelFormatArray[ PixelFormatMax ] = {\
        &AbstractPixelFormat::create1bppIndexed,        \
        &AbstractPixelFormat::create4bppIndexed,        \
        &AbstractPixelFormat::create8bppIndexed,        \
        &AbstractPixelFormat::create8bppGrayScale,      \
        &AbstractPixelFormat::create16bppGrayScale,     \
        &AbstractPixelFormat::create16bppRGB555,        \
        &AbstractPixelFormat::create16bppRGB565,        \
        &AbstractPixelFormat::create16bppARGB1555,      \
        &AbstractPixelFormat::create24bppRGB,           \
        &AbstractPixelFormat::create32bppRGB,           \
        &AbstractPixelFormat::create32bppARGB,          \
        &AbstractPixelFormat::create32bppPARGB,         \
        &AbstractPixelFormat::create48bppRGB,           \
        &AbstractPixelFormat::create64bppARGB,          \
        &AbstractPixelFormat::create64bppPARGB,         \
        &AbstractPixelFormat::create24bppBGR,           \
        &AbstractPixelFormat::create30bppRGB,           \
        &AbstractPixelFormat::create30bppBGR,           \
        &AbstractPixelFormat::create32bppBGR,           \
        &AbstractPixelFormat::create32bppRGBA,          \
        &AbstractPixelFormat::create32bppBGRA,          \
        &AbstractPixelFormat::create32bppABGR,          \
        &AbstractPixelFormat::create48bppBGR,           \
        &AbstractPixelFormat::create64bppRGBA,          \
        &AbstractPixelFormat::create64bppABGR,          \
        &AbstractPixelFormat::create64bppBGRA           \
    };

    AbstractPixelFormat* AbstractPixelFormat::CreatePixelFormat( agg::rendering_buffer& buf, PixelFormat format, ColorFormat colorFormat /*= ColorFormatPlain*/ )
    {
        int index = GetPixelFormatIndex( format );
        AbstractPixelFormat* pixFormat = ( index < 0 || index >= PixelFormatMax ) ? create32bppRGBA( buf ) : (*createPixelFormatArray[ index ])( buf, colorFormat );
        pixFormat->mAggOOPixelFormat = format;
        pixFormat->mAggOOColorFormat = colorFormat;
        return pixFormat;
        //if ( index < 0 || index >= PixelFormatMax )
        //    return create32bppRGBA( buf );
        //return (*createPixelFormatArray[ index ])( buf, colorFormat );

/*
        // Return a specific pixel format, based on the given format.  Use
        // PixelFormat32bppRGBA as a default (if no other valid format is
        // given.)
        switch ( format )
        {
        //case PixelFormat1bppIndexed:
        //    break;

        //case PixelFormat4bppIndexed:
        //    break;

        //case PixelFormat8bppIndexed:
        //    break;

        case PixelFormat8bppGrayScale:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormat< agg::pixfmt_gray8_pre >( buf );
            else
                return new SpecificPixelFormat< agg::pixfmt_gray8 >( buf );

        case PixelFormat16bppGrayScale:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormat< agg::pixfmt_gray16_pre >( buf );
            else
                return new SpecificPixelFormat< agg::pixfmt_gray16 >( buf );

        case PixelFormat16bppRGB555:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormat< agg::pixfmt_rgb555_pre >( buf );
            else
                return new SpecificPixelFormat< agg::pixfmt_rgb555 >( buf );

        case PixelFormat16bppRGB565:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormat< agg::pixfmt_rgb565_pre >( buf );
            else
                return new SpecificPixelFormat< agg::pixfmt_rgb565 >( buf );

        //case PixelFormat16bppARGB1555:
        //    break;

        case PixelFormat24bppRGB:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGB< agg::pixfmt_rgb24_pre >( buf );
            else
                return new SpecificPixelFormatRGB< agg::pixfmt_rgb24 >( buf );

        case PixelFormat24bppBGR:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGB< agg::pixfmt_bgr24_pre >( buf );
            else
                return new SpecificPixelFormatRGB< agg::pixfmt_bgr24 >( buf );

        case PixelFormat30bppRGB:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGB< agg::pixfmt_rgbAAA_pre >( buf );
            else
                return new SpecificPixelFormatRGB< agg::pixfmt_rgbAAA >( buf );

        case PixelFormat30bppBGR:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGB< agg::pixfmt_bgrAAA_pre >( buf );
            else
                return new SpecificPixelFormatRGB< agg::pixfmt_bgrAAA >( buf );

        case PixelFormat32bppRGB:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGB< agg::pixfmt_rgbBBA_pre >( buf );
            else
                return new SpecificPixelFormatRGB< agg::pixfmt_rgbBBA >( buf );

        case PixelFormat32bppBGR:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGB< agg::pixfmt_bgrABB_pre >( buf );
            else
                return new SpecificPixelFormatRGB< agg::pixfmt_bgrABB >( buf );

        case PixelFormat32bppBGRA:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGBA< agg::pixfmt_bgra32_pre >( buf );
            else if ( colorFormat == ColorFormatPlain )
                return new SpecificPixelFormatRGBA< agg::pixfmt_bgra32_plain >( buf );
            else
                return new SpecificPixelFormatRGBA< agg::pixfmt_bgra32 >( buf );

        case PixelFormat32bppARGB:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGBA< agg::pixfmt_argb32_pre >( buf );
            else if ( colorFormat == ColorFormatPlain )
                return new SpecificPixelFormatRGBA< agg::pixfmt_argb32_plain >( buf );
            else
                return new SpecificPixelFormatRGBA< agg::pixfmt_argb32 >( buf );

        case PixelFormat32bppABGR:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGBA< agg::pixfmt_abgr32_pre >( buf );
            else if ( colorFormat == ColorFormatPlain )
                return new SpecificPixelFormatRGBA< agg::pixfmt_abgr32_plain >( buf );
            else
                return new SpecificPixelFormatRGBA< agg::pixfmt_abgr32 >( buf );

        case PixelFormat32bppPARGB:
            return new SpecificPixelFormatRGBA< agg::pixfmt_argb32_pre >( buf );

        case PixelFormat48bppRGB:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGB< agg::pixfmt_rgb48_pre >( buf );
            else
                return new SpecificPixelFormatRGB< agg::pixfmt_rgb48 >( buf );

        case PixelFormat48bppBGR:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGB< agg::pixfmt_bgr48_pre >( buf );
            else
                return new SpecificPixelFormatRGB< agg::pixfmt_bgr48 >( buf );

        case PixelFormat64bppRGBA:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGBA< agg::pixfmt_rgba64_pre >( buf );
            else
                return new SpecificPixelFormatRGBA< agg::pixfmt_rgba64 >( buf );

        case PixelFormat64bppABGR:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGBA< agg::pixfmt_abgr64_pre >( buf );
            else
                return new SpecificPixelFormatRGBA< agg::pixfmt_abgr64 >( buf );

        case PixelFormat64bppARGB:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGBA< agg::pixfmt_argb64_pre >( buf );
            else
                return new SpecificPixelFormatRGBA< agg::pixfmt_argb64 >( buf );

        case PixelFormat64bppPARGB:
            return new SpecificPixelFormatRGBA< agg::pixfmt_argb64_pre >( buf );

        case PixelFormat32bppRGBA:
        default:
            if ( colorFormat == ColorFormatPremultiplied )
                return new SpecificPixelFormatRGBA< agg::pixfmt_rgba32_pre >( buf );
            else if ( colorFormat == ColorFormatPlain )
                return new SpecificPixelFormatRGBA< agg::pixfmt_rgba32_plain >( buf );
            else
                return new SpecificPixelFormatRGBA< agg::pixfmt_rgba32 >( buf );
        }
*/
    }

    AbstractPixelFormat* AbstractPixelFormat::create1bppIndexed( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        return NULL;
    }

    AbstractPixelFormat* AbstractPixelFormat::create4bppIndexed( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        return NULL;
    }

    AbstractPixelFormat* AbstractPixelFormat::create8bppIndexed( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        return NULL;
    }

    AbstractPixelFormat* AbstractPixelFormat::create8bppGrayScale( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatGray< agg::pixfmt_gray8_pre >( buf );
        else
            return new SpecificPixelFormatGray< agg::pixfmt_gray8 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create16bppGrayScale( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatGray< agg::pixfmt_gray16_pre >( buf );
        else
            return new SpecificPixelFormatGray< agg::pixfmt_gray16 >( buf );
    }
    AbstractPixelFormat* AbstractPixelFormat::create16bppRGB555( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGB< agg::pixfmt_rgb555_pre >( buf );
        else
            return new SpecificPixelFormatRGB< agg::pixfmt_rgb555 >( buf );
    }
    AbstractPixelFormat* AbstractPixelFormat::create16bppRGB565( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGB< agg::pixfmt_rgb565_pre >( buf );
        else
            return new SpecificPixelFormatRGB< agg::pixfmt_rgb565 >( buf );
    }
    AbstractPixelFormat* AbstractPixelFormat::create16bppARGB1555( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        return NULL;
    }

    AbstractPixelFormat* AbstractPixelFormat::create24bppRGB( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGB< agg::pixfmt_rgb24_pre >( buf );
        else
            return new SpecificPixelFormatRGB< agg::pixfmt_rgb24 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create24bppBGR( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGB< agg::pixfmt_bgr24_pre >( buf );
        else
            return new SpecificPixelFormatRGB< agg::pixfmt_bgr24 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create30bppRGB( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGB< agg::pixfmt_rgbAAA_pre >( buf );
        else
            return new SpecificPixelFormatRGB< agg::pixfmt_rgbAAA >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create30bppBGR( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGB< agg::pixfmt_bgrAAA_pre >( buf );
        else
            return new SpecificPixelFormatRGB< agg::pixfmt_bgrAAA >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create32bppRGB( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGB< agg::pixfmt_rgbBBA_pre >( buf );
        else
            return new SpecificPixelFormatRGB< agg::pixfmt_rgbBBA >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create32bppBGR( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGB< agg::pixfmt_bgrABB_pre >( buf );
        else
            return new SpecificPixelFormatRGB< agg::pixfmt_bgrABB >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create32bppBGRA( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGBA< agg::pixfmt_bgra32_pre >( buf );
        else if ( colorFormat == ColorFormatPlain )
            return new SpecificPixelFormatRGBA< agg::pixfmt_bgra32_plain >( buf );
        else
            return new SpecificPixelFormatRGBA< agg::pixfmt_bgra32 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create32bppARGB( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGBA< agg::pixfmt_argb32_pre >( buf );
        else if ( colorFormat == ColorFormatPlain )
            return new SpecificPixelFormatRGBA< agg::pixfmt_argb32_plain >( buf );
        else
            return new SpecificPixelFormatRGBA< agg::pixfmt_argb32 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create32bppABGR( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGBA< agg::pixfmt_abgr32_pre >( buf );
        else if ( colorFormat == ColorFormatPlain )
            return new SpecificPixelFormatRGBA< agg::pixfmt_abgr32_plain >( buf );
        else
            return new SpecificPixelFormatRGBA< agg::pixfmt_abgr32 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create32bppPARGB( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        return new SpecificPixelFormatRGBA< agg::pixfmt_argb32_pre >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create32bppRGBA( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGBA< agg::pixfmt_rgba32_pre >( buf );
        else if ( colorFormat == ColorFormatPlain )
            return new SpecificPixelFormatRGBA< agg::pixfmt_rgba32_plain >( buf );
        else
            return new SpecificPixelFormatRGBA< agg::pixfmt_rgba32 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create48bppRGB( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGB< agg::pixfmt_rgb48_pre >( buf );
        else
            return new SpecificPixelFormatRGB< agg::pixfmt_rgb48 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create48bppBGR( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGB< agg::pixfmt_bgr48_pre >( buf );
        else
            return new SpecificPixelFormatRGB< agg::pixfmt_bgr48 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create64bppRGBA( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGBA< agg::pixfmt_rgba64_pre >( buf );
        else
            return new SpecificPixelFormatRGBA< agg::pixfmt_rgba64 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create64bppABGR( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGBA< agg::pixfmt_abgr64_pre >( buf );
        else
            return new SpecificPixelFormatRGBA< agg::pixfmt_abgr64 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create64bppARGB( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGBA< agg::pixfmt_argb64_pre >( buf );
        else
            return new SpecificPixelFormatRGBA< agg::pixfmt_argb64 >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create64bppPARGB( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        return new SpecificPixelFormatRGBA< agg::pixfmt_argb64_pre >( buf );
    }

    AbstractPixelFormat* AbstractPixelFormat::create64bppBGRA( agg::rendering_buffer& buf, ColorFormat colorFormat )
    {
        if ( colorFormat == ColorFormatPremultiplied )
            return new SpecificPixelFormatRGBA< agg::pixfmt_bgra64_pre >( buf );
        else
            return new SpecificPixelFormatRGBA< agg::pixfmt_bgra64 >( buf );
    }

} // namespace AggOO
