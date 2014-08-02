
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
// $Id: AggOOColor.h,v 1.6 2010/03/12 04:20:57 dratek Exp $
//
// AggOOColor.h -- Defines the color type for AggOO
//
// Author       : Chad M. Draper
// Date         : September 7, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-07      Initial implementation
//

#ifndef AggOOColor_H_
#define AggOOColor_H_

#include "AggOOPrereqs.h"
//#include "AggOOTypes.h"
#include "AggOOPixelFormats.h"
//#include "agg_color_rgba.h"
#include "AggOOMath.h"

#include "AggOOStringUtil.h"
#include <string>

namespace AggOO
{

    //----------------------------------------------------------------------------
    // Color mode
    //----------------------------------------------------------------------------
    enum ColorMode
    {
        ColorModeARGB32 = 0,
        ColorModeARGB64 = 1
    };

    //----------------------------------------------------------------------------
    // Color Channel flags
    //----------------------------------------------------------------------------
    enum ColorChannelFlags
    {
        ColorChannelFlagsC = 0,
        ColorChannelFlagsM,
        ColorChannelFlagsY,
        ColorChannelFlagsK,
        ColorChannelFlagsLast
    };

    /**
        @class Color

        This provides a GDI+ compatible color class which (currently) converts to an
        agg::rgba8 structure.  GDI+ stores the color data using 8 bits per
        component, so it is logical to use a similar method here.  It may be
        possible in the future to provide support for, say, agg::rgba16, but that is
        not a priority now (and it would probably go against seamless replacement of
        GDI+.)
    */
    class _AggOOExport Color
    {
    protected:
        ARGB    mARGB;

    public:
        // Pre-defined colors (copied from GDI+):
        enum
        {
            AliceBlue            = 0xFFF0F8FF,
            AntiqueWhite         = 0xFFFAEBD7,
            Aqua                 = 0xFF00FFFF,
            Aquamarine           = 0xFF7FFFD4,
            Azure                = 0xFFF0FFFF,
            Beige                = 0xFFF5F5DC,
            Bisque               = 0xFFFFE4C4,
            Black                = 0xFF000000,
            BlanchedAlmond       = 0xFFFFEBCD,
            Blue                 = 0xFF0000FF,
            BlueViolet           = 0xFF8A2BE2,
            Brown                = 0xFFA52A2A,
            BurlyWood            = 0xFFDEB887,
            CadetBlue            = 0xFF5F9EA0,
            Chartreuse           = 0xFF7FFF00,
            Chocolate            = 0xFFD2691E,
            Coral                = 0xFFFF7F50,
            CornflowerBlue       = 0xFF6495ED,
            Cornsilk             = 0xFFFFF8DC,
            Crimson              = 0xFFDC143C,
            Cyan                 = 0xFF00FFFF,
            DarkBlue             = 0xFF00008B,
            DarkCyan             = 0xFF008B8B,
            DarkGoldenrod        = 0xFFB8860B,
            DarkGray             = 0xFFA9A9A9,
            DarkGreen            = 0xFF006400,
            DarkKhaki            = 0xFFBDB76B,
            DarkMagenta          = 0xFF8B008B,
            DarkOliveGreen       = 0xFF556B2F,
            DarkOrange           = 0xFFFF8C00,
            DarkOrchid           = 0xFF9932CC,
            DarkRed              = 0xFF8B0000,
            DarkSalmon           = 0xFFE9967A,
            DarkSeaGreen         = 0xFF8FBC8B,
            DarkSlateBlue        = 0xFF483D8B,
            DarkSlateGray        = 0xFF2F4F4F,
            DarkTurquoise        = 0xFF00CED1,
            DarkViolet           = 0xFF9400D3,
            DeepPink             = 0xFFFF1493,
            DeepSkyBlue          = 0xFF00BFFF,
            DimGray              = 0xFF696969,
            DodgerBlue           = 0xFF1E90FF,
            Firebrick            = 0xFFB22222,
            FloralWhite          = 0xFFFFFAF0,
            ForestGreen          = 0xFF228B22,
            Fuchsia              = 0xFFFF00FF,
            Gainsboro            = 0xFFDCDCDC,
            GhostWhite           = 0xFFF8F8FF,
            Gold                 = 0xFFFFD700,
            Goldenrod            = 0xFFDAA520,
            Gray                 = 0xFF808080,
            Green                = 0xFF008000,
            GreenYellow          = 0xFFADFF2F,
            Honeydew             = 0xFFF0FFF0,
            HotPink              = 0xFFFF69B4,
            IndianRed            = 0xFFCD5C5C,
            Indigo               = 0xFF4B0082,
            Ivory                = 0xFFFFFFF0,
            Khaki                = 0xFFF0E68C,
            Lavender             = 0xFFE6E6FA,
            LavenderBlush        = 0xFFFFF0F5,
            LawnGreen            = 0xFF7CFC00,
            LemonChiffon         = 0xFFFFFACD,
            LightBlue            = 0xFFADD8E6,
            LightCoral           = 0xFFF08080,
            LightCyan            = 0xFFE0FFFF,
            LightGoldenrodYellow = 0xFFFAFAD2,
            LightGray            = 0xFFD3D3D3,
            LightGreen           = 0xFF90EE90,
            LightPink            = 0xFFFFB6C1,
            LightSalmon          = 0xFFFFA07A,
            LightSeaGreen        = 0xFF20B2AA,
            LightSkyBlue         = 0xFF87CEFA,
            LightSlateGray       = 0xFF778899,
            LightSteelBlue       = 0xFFB0C4DE,
            LightYellow          = 0xFFFFFFE0,
            Lime                 = 0xFF00FF00,
            LimeGreen            = 0xFF32CD32,
            Linen                = 0xFFFAF0E6,
            Magenta              = 0xFFFF00FF,
            Maroon               = 0xFF800000,
            MediumAquamarine     = 0xFF66CDAA,
            MediumBlue           = 0xFF0000CD,
            MediumOrchid         = 0xFFBA55D3,
            MediumPurple         = 0xFF9370DB,
            MediumSeaGreen       = 0xFF3CB371,
            MediumSlateBlue      = 0xFF7B68EE,
            MediumSpringGreen    = 0xFF00FA9A,
            MediumTurquoise      = 0xFF48D1CC,
            MediumVioletRed      = 0xFFC71585,
            MidnightBlue         = 0xFF191970,
            MintCream            = 0xFFF5FFFA,
            MistyRose            = 0xFFFFE4E1,
            Moccasin             = 0xFFFFE4B5,
            NavajoWhite          = 0xFFFFDEAD,
            Navy                 = 0xFF000080,
            OldLace              = 0xFFFDF5E6,
            Olive                = 0xFF808000,
            OliveDrab            = 0xFF6B8E23,
            Orange               = 0xFFFFA500,
            OrangeRed            = 0xFFFF4500,
            Orchid               = 0xFFDA70D6,
            PaleGoldenrod        = 0xFFEEE8AA,
            PaleGreen            = 0xFF98FB98,
            PaleTurquoise        = 0xFFAFEEEE,
            PaleVioletRed        = 0xFFDB7093,
            PapayaWhip           = 0xFFFFEFD5,
            PeachPuff            = 0xFFFFDAB9,
            Peru                 = 0xFFCD853F,
            Pink                 = 0xFFFFC0CB,
            Plum                 = 0xFFDDA0DD,
            PowderBlue           = 0xFFB0E0E6,
            Purple               = 0xFF800080,
            Red                  = 0xFFFF0000,
            RosyBrown            = 0xFFBC8F8F,
            RoyalBlue            = 0xFF4169E1,
            SaddleBrown          = 0xFF8B4513,
            Salmon               = 0xFFFA8072,
            SandyBrown           = 0xFFF4A460,
            SeaGreen             = 0xFF2E8B57,
            SeaShell             = 0xFFFFF5EE,
            Sienna               = 0xFFA0522D,
            Silver               = 0xFFC0C0C0,
            SkyBlue              = 0xFF87CEEB,
            SlateBlue            = 0xFF6A5ACD,
            SlateGray            = 0xFF708090,
            Snow                 = 0xFFFFFAFA,
            SpringGreen          = 0xFF00FF7F,
            SteelBlue            = 0xFF4682B4,
            Tan                  = 0xFFD2B48C,
            Teal                 = 0xFF008080,
            Thistle              = 0xFFD8BFD8,
            Tomato               = 0xFFFF6347,
            Transparent          = 0x00FFFFFF,
            Turquoise            = 0xFF40E0D0,
            Violet               = 0xFFEE82EE,
            Wheat                = 0xFFF5DEB3,
            White                = 0xFFFFFFFF,
            WhiteSmoke           = 0xFFF5F5F5,
            Yellow               = 0xFFFFFF00,
            YellowGreen          = 0xFF9ACD32
        };

        // Shift count and bit mask for A, R, G, B components
        enum
        {
            AlphaShift  = 24,
            RedShift    = 16,
            GreenShift  = 8,
            BlueShift   = 0
        };

        enum
        {
            AlphaMask   = 0xff000000,
            RedMask     = 0x00ff0000,
            GreenMask   = 0x0000ff00,
            BlueMask    = 0x000000ff
        };

    public:
        /** Constructor */
        Color() : mARGB( Black )    { }
        /** Constructor */
        Color( BYTE r, BYTE g, BYTE b )
        {
            mARGB = MakeARGB( 255, r, g, b );
        }
        /** Constructor */
        Color( BYTE a, BYTE r, BYTE g, BYTE b )
        {
            mARGB = MakeARGB( a, r, g, b );
        }
        /** Constructor */
        Color( ARGB argb ) : mARGB( argb )  { }

        /** Generate an ARGB value from the individual components */
        static ARGB MakeARGB( BYTE a, BYTE r, BYTE g, BYTE b )
        {
            return ARGB( ( a << AlphaShift ) | ( r << RedShift ) | ( g << GreenShift ) | ( b << BlueShift ) );
        }

        /** Get the alpha component */
        BYTE GetA() const       { return (BYTE)( ( mARGB >> AlphaShift ) ); }
        /** Get the alpha component */
        BYTE GetAlpha() const   { return (BYTE)( ( mARGB >> AlphaShift ) ); }

        /** Get the red component */
        BYTE GetR() const       { return (BYTE)( ( mARGB >> RedShift ) ); }
        /** Get the red component */
        BYTE GetRed() const     { return (BYTE)( ( mARGB >> RedShift ) ); }

        /** Get the green component */
        BYTE GetG() const       { return (BYTE)( ( mARGB >> GreenShift ) ); }
        /** Get the green component */
        BYTE GetGreen() const   { return (BYTE)( ( mARGB >> GreenShift ) ); }

        /** Get the blue component */
        BYTE GetB() const       { return (BYTE)( ( mARGB >> BlueShift ) ); }
        /** Get the blue component */
        BYTE GetBlue() const    { return (BYTE)( ( mARGB >> BlueShift ) ); }

        /** Get the ARGB value */
        ARGB GetValue() const   { return mARGB; }

        /** Set the ARGB value */
        VOID SetValue( ARGB argb )  { mARGB = argb; }

        /** Set the color from a COLORREF value
            @param  cr          Source color as a COLORREF (DWORD)
            @remarks
                Since COLORREF is defined on Windows as an unsigned long, and it
                has been defined in AggOOPixelFormats.h, this should be ok, even
                on non-Windows platforms.
        */
        VOID SetFromCOLORREF( COLORREF cr )
        {
            MakeARGB( 255, GetRValue( cr ), GetGValue( cr ), GetBValue( cr ) );
        }

        /** Convert the color to a COLORREF value
        */
        COLORREF ToCOLORREF() const
        {
            return RGB( GetR(), GetG(), GetB() );
        }

        /** Convert the color to a specified format, returning the result as a
            UINT.  The result will be packed in a way that the useful
            information can be masked out of the least significant bits.
        */
        UINT ToPixelFormat( PixelFormat PixelFormat ) const
        {
            UINT ret = 0;

            // The color is already in 32bpp ARGB format

            switch ( PixelFormat )
            {
            case PixelFormat8bppGrayScale:
                ret = UINT( ( GetR() * 0.299 ) + ( GetG() * 0.587 ) + ( GetB() * 0.114 ) );
                break;

            case PixelFormat16bppGrayScale:
                {
                    REAL gray8 = ( ( GetR() * 0.299 ) + ( GetG() * 0.587 ) + ( GetB() * 0.114 ) ) / 255.0;
                    ret = (UINT)( gray8 * 65535 );
                }
                break;

            case PixelFormat16bppRGB555:
                {
                    BYTE r = BYTE( ( GetR() / 255.0 ) * 31 );
                    BYTE g = BYTE( ( GetG() / 255.0 ) * 31 );
                    BYTE b = BYTE( ( GetB() / 255.0 ) * 31 );
                    ret = ( r << 10 ) | ( g << 5 ) | b;
                }
                break;

            case PixelFormat16bppRGB565:
                {
                    BYTE r = BYTE( ( GetR() / 255.0 ) * 31 );
                    BYTE g = BYTE( ( GetG() / 255.0 ) * 63 );
                    BYTE b = BYTE( ( GetB() / 255.0 ) * 31 );
                    ret = ( r << 11 ) | ( g << 5 ) | b;
                }
                break;

            case PixelFormat16bppARGB1555:
                {
                    BYTE r = BYTE( ( GetR() / 255.0 ) * 31 );
                    BYTE g = BYTE( ( GetG() / 255.0 ) * 31 );
                    BYTE b = BYTE( ( GetB() / 255.0 ) * 31 );
                    BYTE a = ( GetA() ) ? 1 : 0;
                    ret = ( a << 15 ) | ( r << 10 ) | ( g << 5 ) | b;
                }
                break;

            case PixelFormat24bppRGB:
                ret = ( GetR() << 16 ) | ( GetG() << 8 ) | GetB();
                break;

            case PixelFormat24bppBGR:
                ret = ( GetB() << 16 ) | ( GetG() << 8 ) | GetR();
                break;

            case PixelFormat30bppRGB:
                {
                    UINT r = UINT( GetR() / 255.0 * 1023 );
                    UINT g = UINT( GetG() / 255.0 * 1023 );
                    UINT b = UINT( GetB() / 255.0 * 1023 );
                    ret = ( r << 20 ) | ( g << 10 ) | b;
                }
                break;

            case PixelFormat30bppBGR:
                {
                    UINT r = UINT( GetR() / 255.0 * 1023 );
                    UINT g = UINT( GetG() / 255.0 * 1023 );
                    UINT b = UINT( GetB() / 255.0 * 1023 );
                    ret = ( b << 20 ) | ( g << 10 ) | r;
                }
                break;

            case PixelFormat32bppRGB:
                {
                    // BBA
                    UINT r = UINT( GetR() / 255.0 * 1023 );
                    UINT g = UINT( GetG() / 255.0 * 2047 );
                    UINT b = UINT( GetB() / 255.0 * 2047 );
                    ret = ( r << 22 ) | ( g << 11 ) | b;
                }
                break;

            case PixelFormat32bppBGR:
                {
                    // BBA
                    UINT r = UINT( GetR() / 255.0 * 2047 );
                    UINT g = UINT( GetG() / 255.0 * 2047 );
                    UINT b = UINT( GetB() / 255.0 * 1023 );
                    ret = ( b << 22 ) | ( g << 11 ) | r;
                }
                break;

            case PixelFormat32bppARGB:
                ret = mARGB;
                break;

            case PixelFormat32bppABGR:
                ret = ( GetA() << 24 ) | ( GetB() << 16 ) | ( GetG() << 8 ) + GetR();
                break;

            case PixelFormat32bppRGBA:
                ret = ( GetR() << 24 ) | ( GetG() << 16 ) | ( GetB() << 8 ) + GetA();
                break;

            case PixelFormat32bppBGRA:
                ret = ( GetB() << 24 ) | ( GetG() << 16 ) | ( GetR() << 8 ) + GetA();
                break;

/*
            // TODO: Can't left-shift UINTs 32 bits...

            case PixelFormat48bppRGB:
                {
                    UINT r = UINT( GetR() / 255.0 * 65535 );
                    UINT g = UINT( GetG() / 255.0 * 65535 );
                    UINT b = UINT( GetB() / 255.0 * 65535 );
                    ret = ( r << 32 ) | ( g << 16 ) | b;
                }
                break;

            case PixelFormat48bppBGR:
                {
                    UINT r = UINT( GetR() / 255.0 * 65535 );
                    UINT g = UINT( GetG() / 255.0 * 65535 );
                    UINT b = UINT( GetB() / 255.0 * 65535 );
                    ret = ( b << 32 ) | ( g << 16 ) | r;
                }
                break;

            case PixelFormat64bppARGB:
                {
                    UINT r = UINT( GetR() / 255.0 * 65535 );
                    UINT g = UINT( GetG() / 255.0 * 65535 );
                    UINT b = UINT( GetB() / 255.0 * 65535 );
                    UINT a = UINT( GetA() / 255.0 * 65535 );
                    ret = ( a << 48 ) | ( r << 32 ) | ( g << 16 ) | b;
                }
                break;

            case PixelFormat64bppABGR:
                {
                    UINT r = UINT( GetR() / 255.0 * 65535 );
                    UINT g = UINT( GetG() / 255.0 * 65535 );
                    UINT b = UINT( GetB() / 255.0 * 65535 );
                    UINT a = UINT( GetA() / 255.0 * 65535 );
                    ret = ( a << 48 ) | ( b << 32 ) | ( g << 16 ) | r;
                }
                break;

            case PixelFormat64bppRGBA:
                {
                    UINT r = UINT( GetR() / 255.0 * 65535 );
                    UINT g = UINT( GetG() / 255.0 * 65535 );
                    UINT b = UINT( GetB() / 255.0 * 65535 );
                    UINT a = UINT( GetA() / 255.0 * 65535 );
                    ret = ( r << 48 ) | ( g << 32 ) | ( b << 16 ) | a;
                }
                break;

            case PixelFormat64bppBGRA:
                {
                    UINT r = UINT( GetR() / 255.0 * 65535 );
                    UINT g = UINT( GetG() / 255.0 * 65535 );
                    UINT b = UINT( GetB() / 255.0 * 65535 );
                    UINT a = UINT( GetA() / 255.0 * 65535 );
                    ret = ( b << 48 ) | ( g << 32 ) | ( r << 16 ) | a;
                }
                break;
*/

            }

            return ret;
        }

        /** Get the color as a string.  This can be used by the stream output
            operator, as well as used as in index for color mapping.
        */
        STRING GetColorString() const
        {
            std::string str = "Color( " +
                StringUtil::ToString( (int)GetA() ) + ", " +
                StringUtil::ToString( (int)GetR() ) + ", " +
                StringUtil::ToString( (int)GetG() ) + ", " +
                StringUtil::ToString( (int)GetB() ) + " )";
            return str;
        }

        //! Write to a stream
        inline friend std::ostream& operator<<( std::ostream& stream, const Color& src )
        {
            std::ostream::sentry opfx(stream);
            if ( !opfx )
                return stream;
            return ( stream << src.GetColorString() );
            //return ( stream << "Color( " << (int)src.GetA() << ", " << (int)src.GetR() << ", " << (int)src.GetG() << ", " << (int)src.GetB() << " )" );
        }

    }; // class Color

} // namespace AggOO

#endif  // AggOOColor_H_
