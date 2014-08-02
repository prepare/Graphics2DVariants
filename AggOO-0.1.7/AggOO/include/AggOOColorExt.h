
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
// $Id: AggOOColorExt.h,v 1.6 2008/02/28 20:55:48 dratek Exp $
//
// AggOOColorExt.h -- Extended color class with additional features not
//                       found in GDI+.
//
// Author       : Chad M. Draper
// Date         : October 19, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-10-19      Initial implementation
//

#ifndef AggOOColorExt_H_
#define AggOOColorExt_H_

#include "AggOOColor.h"

#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"

namespace AggOO
{
    /** @class ColorExt

            Extension of AggOO::Color class with methods not found in GDI+, but
            which may be of significant value.
    */
    class _AggOOExport ColorExt : public Color
    {
    public:
        /** Constructor */
        ColorExt() : Color()    { }
        /** Constructor */
        ColorExt( BYTE r, BYTE g, BYTE b ) : Color( r, g, b )
        {
        }
        /** Constructor */
        ColorExt( BYTE a, BYTE r, BYTE g, BYTE b ) : Color( a, r, g, b )
        {
        }
        /** Constructor */
        ColorExt( ARGB argb ) : Color( argb )  { }

        /** Get the alpha component from an ARGB value */
        static BYTE GetA( ARGB argb )   { return (BYTE)( ( argb >> AlphaShift ) ); }
        /** Get the red component from an ARGB value */
        static BYTE GetR( ARGB argb )   { return (BYTE)( ( argb >> RedShift ) ); }
        /** Get the green component from an ARGB value */
        static BYTE GetG( ARGB argb )   { return (BYTE)( ( argb >> GreenShift ) ); }
        /** Get the blue component from an ARGB value */
        static BYTE GetB( ARGB argb )   { return (BYTE)( ( argb >> BlueShift ) ); }

        /** Assignment operator */
        ColorExt& operator=( const Color& src )
        {
            mARGB = src.GetValue();
            return *this;
        }

        /** Return a random color value with full alpha */
        static Color RandomRGB()
        {
            Color ret;
            ret = Color( 255, IRangeRandom( 0, 255 ), IRangeRandom( 0, 255 ), IRangeRandom( 0, 255 ) );
            return ret;
        }
        /** Return a random color value with a random alpha */
        static Color RandomARGB()
        {
            Color ret;
            ret = Color( IRangeRandom( 0, 255 ), IRangeRandom( 0, 255 ), IRangeRandom( 0, 255 ), IRangeRandom( 0, 255 ) );
            return ret;
        }

        /** Linearly interpolate between this color and another based on a ratio */
        Color Lerp( REAL ratio, const Color& color2 ) const
        {
            return Color(
                            ILerp( ratio, GetAlpha(), color2.GetA() ),
                            ILerp( ratio, GetRed(),   color2.GetR() ),
                            ILerp( ratio, GetGreen(), color2.GetG() ),
                            ILerp( ratio, GetBlue(),  color2.GetB() )
                        );
        }

        /** Linearly interpolate between two colors based on a ratio */
        static Color Lerp( REAL ratio, const Color& start, const Color& end )
        {
            return Color(
                            ILerp( ratio, start.GetA(), end.GetA() ),
                            ILerp( ratio, start.GetR(), end.GetR() ),
                            ILerp( ratio, start.GetG(), end.GetG() ),
                            ILerp( ratio, start.GetB(), end.GetB() )
                        );
        }

        /** Generate this color randomly (full alpha) */
        VOID FromRandomRGB()
        {
            *this = RandomRGB();
        }
        /** Generate this color randomly (random alpha) */
        VOID FromRandomARGB()
        {
            *this = RandomARGB();
        }

        /** Convert an AggOO::Color value to an agg::rgba8 value */
        static agg::rgba8 toRGBA8( const Color& color )
        {
            return agg::rgba8( color.GetR(), color.GetG(), color.GetB(), color.GetA() );
        }

        /** Convert an AggOO::Color value to an rgba value */
        static agg::rgba toRGBA( const Color& color )
        {
            double inv255 = 1.0 / 255.0;
            return agg::rgba( color.GetR() * inv255, color.GetG() * inv255, color.GetB() * inv255, color.GetA() * inv255 );
        }

        /** Convert an agg::rgba8 to an AggOO::Color */
        static AggOO::Color convertColor( const agg::rgba8& color )
        {
            return AggOO::Color( color.a, color.r, color.g, color.b );
        }

    }; // class ColorExt

    ////////////////////////////////////////////////////////////////////////////

    /** @class GradientColorItem
        Structure to define a color item in the gradient, consisting of a color
        and percent position.
    */
    class GradientColorItem
    {
    public:
        Color   mColor;         /**< Color of the item */
        double  mPosition;      /**< Position of the item [0..1] */

        /** Default constructor */
        GradientColorItem() : mColor( Color( 0, 0, 0, 0 ) ), mPosition( 0.0 )
        {
        }

        /** Constructor which fills the item data
            @param  col     Color value
            @param  pos     Position value
        */
        GradientColorItem( agg::rgba8 col, double pos )
            : mColor( ColorExt::convertColor( col ) ), mPosition( pos )
        {
        }

        /** Constructor which fills the item data
            @param  col     Color value
            @param  pos     Position value
        */
        GradientColorItem( const Color& col, double pos )
            : mColor( col ), mPosition( pos )
        {
        }

        bool operator<( const GradientColorItem& item ) const
        {
            return mPosition < item.mPosition;
        }

    }; // class GradientColorItem

} //namespace AggOO

#endif  // AggOOColorExt_H_
