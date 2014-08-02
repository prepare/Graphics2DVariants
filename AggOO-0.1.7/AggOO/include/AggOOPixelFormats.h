
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
// $Id: AggOOPixelFormats.h,v 1.3 2008/02/28 20:55:48 dratek Exp $
//
// AggOOPixelFormats.h -- Defines the pixel formats that can be used in AggOO.
//
// Author       : Chad M. Draper
// Date         : September 6, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-06      Initial implementation
//

#ifndef AggOOPixelFormats_H_
#define AggOOPixelFormats_H_

/** @remarks

    GDI+ uses #define to specify the pixel formats.  The problem with this is
    that it does not protect them by the namespace, so they can be a global
    nuisance.  I'd rather define them within an enum, which is namespace
    protected, which means that the AggOO files MUST be included before GDI+.
    There's probably still a chance for problems, but hopefully this will work
    better.  The main AggOO header will check for the presence of GDI+, but as
    a fail-safe, check for the presence of PixelFormat (since AGG+ also defines
    that) here, and issue an error warning.

    Note that even with this check, the #defines can make life difficult.  When
    using a pixel format that is supported by GDI+, use the GDI+ value.  In
    other words, don't use the AggOO namespace.  This only applies when using
    GDI+ with AggOO.
*/
#ifdef PixelFormatUndefined
#error GDI+ pixel formats detected.  Make sure AggOO is being included before GDI+ and other similar libraries (such as AggPlus.)
#endif

#include "AggOOPrereqs.h"
#include "AggOOTypes.h"
//#include "AggOOImaging.h"
#include "util/agg_color_conv_rgb8.h"
#include "util/agg_color_conv_rgb16.h"

namespace AggOO
{
    enum
    {
        ALPHA_SHIFT = 24,
        RED_SHIFT   = 16,
        GREEN_SHIFT = 8,
        BLUE_SHIFT  = 0
    };

    inline INT ALPHA_MASK() { return (ARGB)0xff << ALPHA_SHIFT; }
//    #define MASK_ALPHA  ( (ARGB)0xff << SHIFT_ALPHA )
//#define ALPHA_MASK ( (ARGB)0xff << SHIFT_ALPHA )

    // GDI+ (and AggPlus) use #defines for the pixel formats.  That's probably
    // an easier way, but it is not protected by the namespace.  As such, the
    // names had to be changed here in order to avoid clashing when using this
    // with GDI+.  The values will be consistent, so it should work just as well
    // or better.
    enum
    {
        PixelFormatIndexed      = 0x00010000,       // Indexes into a palette
        PixelFormatGDI          = 0x00020000,       // Format is GDI-supported
        PixelFormatAlpha        = 0x00040000,       // Format has alpha
        PixelFormatPAlpha       = 0x00080000,       // Format has premultiplied alpha
        PixelFormatExtended     = 0x00100000,       // Extended color (16 bits / channel)
        PixelFormatCanonical    = 0x00200000,

        PixelFormatUndefined    = 0,
        PixelFormatDontCare     = 0
    };

    /** Supported pixel formats
    */
    typedef INT PixelFormat;
    enum
    {
        PixelFormat1bppIndexed      = (  1 | (  1 << 8 ) | PixelFormatIndexed | PixelFormatGDI ),
        PixelFormat4bppIndexed      = (  2 | (  4 << 8 ) | PixelFormatIndexed | PixelFormatGDI ),
        PixelFormat8bppIndexed      = (  3 | (  8 << 8 ) | PixelFormatIndexed | PixelFormatGDI ),
        PixelFormat8bppGrayScale    = (  4 | (  8 << 8 ) | PixelFormatExtended ),
        PixelFormat16bppGrayScale   = (  5 | ( 16 << 8 ) | PixelFormatExtended ),
        PixelFormat16bppRGB555      = (  6 | ( 16 << 8 ) | PixelFormatGDI ),
        PixelFormat16bppRGB565      = (  7 | ( 16 << 8 ) | PixelFormatGDI ),
        PixelFormat16bppARGB1555    = (  8 | ( 16 << 8 ) | PixelFormatAlpha | PixelFormatGDI ),
        PixelFormat24bppRGB         = (  9 | ( 24 << 8 ) | PixelFormatGDI ),
        PixelFormat32bppRGB         = ( 10 | ( 32 << 8 ) | PixelFormatGDI ),
        PixelFormat32bppARGB        = ( 11 | ( 32 << 8 ) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical ),
        PixelFormat32bppPARGB       = ( 12 | ( 32 << 8 ) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatGDI ),
        PixelFormat48bppRGB         = ( 13 | ( 48 << 8 ) | PixelFormatExtended ),
        PixelFormat64bppARGB        = ( 14 | ( 64 << 8 ) | PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended ),
        PixelFormat64bppPARGB       = ( 15 | ( 64 << 8 ) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatExtended ),

        // AGG can support additional formats:
        PixelFormat24bppBGR         = ( 16 | ( 24 << 8 ) | PixelFormatCanonical ),
        PixelFormat30bppRGB         = ( 17 | ( 30 << 8 ) | PixelFormatExtended ),
        PixelFormat30bppBGR         = ( 18 | ( 30 << 8 ) | PixelFormatExtended ),
        PixelFormat32bppBGR         = ( 19 | ( 32 << 8 ) | PixelFormatExtended ),
        PixelFormat32bppRGBA        = ( 20 | ( 32 << 8 ) | PixelFormatAlpha | PixelFormatCanonical ),
        PixelFormat32bppBGRA        = ( 21 | ( 32 << 8 ) | PixelFormatAlpha | PixelFormatCanonical ),
        PixelFormat32bppABGR        = ( 22 | ( 32 << 8 ) | PixelFormatAlpha | PixelFormatCanonical ),
        PixelFormat48bppBGR         = ( 23 | ( 48 << 8 ) | PixelFormatExtended ),
        PixelFormat64bppRGBA        = ( 24 | ( 64 << 8 ) | PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended ),
        PixelFormat64bppABGR        = ( 25 | ( 64 << 8 ) | PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended ),
        PixelFormat64bppBGRA        = ( 26 | ( 64 << 8 ) | PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended ),

        PixelFormatMax              = ( 27 )
    };

    inline _AggOOExport unsigned int
    GetPixelFormatSize( PixelFormat PixelFormat )
    {
        return (PixelFormat >> 8) & 0xff;
    }

    inline _AggOOExport unsigned int
    GetPixelFormatIndex( PixelFormat PixelFormat )
    {
        return ( PixelFormat & 0xff ) - 1;
    }

    inline _AggOOExport unsigned int
    GetRowWidth( INT width, INT bpp )
    {
        unsigned n = width;
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

    inline _AggOOExport BOOL
    IsIndexedPixelFormat( PixelFormat PixelFormat )
    {
        return (PixelFormat & PixelFormatIndexed) != 0;
    }

    inline _AggOOExport BOOL IsAlphaPixelFormat( PixelFormat PixelFormat )
    {
        return (PixelFormat & PixelFormatAlpha) != 0;
    }

    inline _AggOOExport BOOL
    IsExtendedPixelFormat( PixelFormat PixelFormat )
    {
        return (PixelFormat & PixelFormatExtended) != 0;
    }

    //--------------------------------------------------------------------------
    // Determine if the Pixel Format is Canonical format:
    //   PixelFormat32bppARGB
    //   PixelFormat32bppPARGB
    //   PixelFormat64bppARGB
    //   PixelFormat64bppPARGB
    //--------------------------------------------------------------------------

    inline _AggOOExport BOOL
    IsCanonicalPixelFormat( PixelFormat PixelFormat )
    {
        return (PixelFormat & PixelFormatCanonical) != 0;
    }



} // namespace AggOO

#endif  // AggOOPixelFormats_H_
