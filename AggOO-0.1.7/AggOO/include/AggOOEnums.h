
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
// $Id: AggOOEnums.h,v 1.5 2008/02/28 20:55:48 dratek Exp $
//
// AggOOBase.h -- Enumerated constants for AggOO (mostly copied from GDI+.)
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

#ifndef AggOOEnums_H_
#define AggOOEnums_H_

#include "AggOOTypes.h"

#include "agg_math_stroke.h"

namespace AggOO
{

    //--------------------------------------------------------------------------
    // Default bezier flattening tolerance in device pixels.
    //--------------------------------------------------------------------------

    const float FlatnessDefault = 1.0f / 4.0f;

    //--------------------------------------------------------------------------
    // Graphics and Container State cookies
    //--------------------------------------------------------------------------

    typedef UINT     GraphicsState;
    typedef UINT     GraphicsContainer;

    //--------------------------------------------------------------------------
    // AGG uses either pre-multiplied or plain alpha for the colors.  Pre-
    // multiplied is faster, but plain is needed when rendering to a layer that
    // will be composited outside of AGG.  I'm not certain, but I think GDI+
    // uses the "plain" format.
    //--------------------------------------------------------------------------
    enum ColorFormat
    {
        ColorFormatDefault,         // Blend plain pixels to pre-multiplied.  Data will be converted to pre-multiplied.
        ColorFormatPremultiplied,   // Use pre-multiplied alpha.  Any source data is assumed to already be premultiplied.
        ColorFormatPlain            // Plain alpha.  No premultiplying.
    };

    //--------------------------------------------------------------------------
    // Fill mode constants
    //--------------------------------------------------------------------------

    enum FillMode
    {
        FillModeAlternate,        // 0
        FillModeWinding           // 1
    };

    // Convert a FillMode to an agg::fill_mode
    inline agg::filling_rule_e convertFillMode( FillMode mode )
    {
        return ( mode == FillModeAlternate ) ? agg::fill_even_odd : agg::fill_non_zero;
    }

    // Convert an agg::fill_mode to a FillMode
    inline FillMode convertFillMode( agg::filling_rule_e mode )
    {
        return ( mode == agg::fill_even_odd ) ? FillModeAlternate : FillModeWinding;
    }

    //--------------------------------------------------------------------------
    // Quality mode constants
    //--------------------------------------------------------------------------

    enum QualityMode
    {
        QualityModeInvalid   = -1,
        QualityModeDefault   = 0,
        QualityModeLow       = 1, // Best performance
        QualityModeHigh      = 2  // Best rendering quality
    };

    //--------------------------------------------------------------------------
    // Alpha Compositing mode constants
    //--------------------------------------------------------------------------

    enum CompositingMode
    {
        CompositingModeSourceOver,    // 0
        CompositingModeSourceCopy     // 1
    };

    //--------------------------------------------------------------------------
    // Alpha Compositing quality constants
    //--------------------------------------------------------------------------

    enum CompositingQuality
    {
        CompositingQualityInvalid          = QualityModeInvalid,
        CompositingQualityDefault          = QualityModeDefault,
        CompositingQualityHighSpeed        = QualityModeLow,
        CompositingQualityHighQuality      = QualityModeHigh,
        CompositingQualityGammaCorrected,
        CompositingQualityAssumeLinear
    };

    //--------------------------------------------------------------------------
    // Unit constants
    //--------------------------------------------------------------------------

    enum Unit
    {
        UnitWorld,      // 0 -- World coordinate (non-physical unit)
        UnitDisplay,    // 1 -- Variable -- for PageTransform only
        UnitPixel,      // 2 -- Each unit is one device pixel.
        UnitPoint,      // 3 -- Each unit is a printer's point, or 1/72 inch.
        UnitInch,       // 4 -- Each unit is 1 inch.
        UnitDocument,   // 5 -- Each unit is 1/300 inch.
        UnitMillimeter  // 6 -- Each unit is 1 millimeter.
    };

    //--------------------------------------------------------------------------
    // MetafileFrameUnit
    //
    // The frameRect for creating a metafile can be specified in any of these
    // units.  There is an extra frame unit value (MetafileFrameUnitGdi) so
    // that units can be supplied in the same units that GDI expects for
    // frame rects -- these units are in .01 (1/100ths) millimeter units
    // as defined by GDI.
    //--------------------------------------------------------------------------

    enum MetafileFrameUnit
    {
        MetafileFrameUnitPixel      = UnitPixel,
        MetafileFrameUnitPoint      = UnitPoint,
        MetafileFrameUnitInch       = UnitInch,
        MetafileFrameUnitDocument   = UnitDocument,
        MetafileFrameUnitMillimeter = UnitMillimeter,
        MetafileFrameUnitGdi                        // GDI compatible .01 MM units
    };

    //--------------------------------------------------------------------------
    // Coordinate space identifiers
    //--------------------------------------------------------------------------

    enum CoordinateSpace
    {
        CoordinateSpaceWorld,     // 0
        CoordinateSpacePage,      // 1
        CoordinateSpaceDevice     // 2
    };

    //--------------------------------------------------------------------------
    // Various wrap modes for brushes
    //--------------------------------------------------------------------------

    enum WrapMode
    {
        WrapModeTile,        // 0
        WrapModeTileFlipX,   // 1
        WrapModeTileFlipY,   // 2
        WrapModeTileFlipXY,  // 3
        WrapModeFlip        = WrapModeTileFlipXY,
        WrapModeClamp        // 4
    };

    //--------------------------------------------------------------------------
    // Various hatch styles
    //--------------------------------------------------------------------------

    enum HatchStyle
    {
        HatchStyleHorizontal,                   // 0
        HatchStyleVertical,                     // 1
        HatchStyleForwardDiagonal,              // 2
        HatchStyleBackwardDiagonal,             // 3
        HatchStyleCross,                        // 4
        HatchStyleDiagonalCross,                // 5
        HatchStyle05Percent,                    // 6
        HatchStyle10Percent,                    // 7
        HatchStyle20Percent,                    // 8
        HatchStyle25Percent,                    // 9
        HatchStyle30Percent,                    // 10
        HatchStyle40Percent,                    // 11
        HatchStyle50Percent,                    // 12
        HatchStyle60Percent,                    // 13
        HatchStyle70Percent,                    // 14
        HatchStyle75Percent,                    // 15
        HatchStyle80Percent,                    // 16
        HatchStyle90Percent,                    // 17
        HatchStyleLightDownwardDiagonal,        // 18
        HatchStyleLightUpwardDiagonal,          // 19
        HatchStyleDarkDownwardDiagonal,         // 20
        HatchStyleDarkUpwardDiagonal,           // 21
        HatchStyleWideDownwardDiagonal,         // 22
        HatchStyleWideUpwardDiagonal,           // 23
        HatchStyleLightVertical,                // 24
        HatchStyleLightHorizontal,              // 25
        HatchStyleNarrowVertical,               // 26
        HatchStyleNarrowHorizontal,             // 27
        HatchStyleDarkVertical,                 // 28
        HatchStyleDarkHorizontal,               // 29
        HatchStyleDashedDownwardDiagonal,       // 30
        HatchStyleDashedUpwardDiagonal,         // 31
        HatchStyleDashedHorizontal,             // 32
        HatchStyleDashedVertical,               // 33
        HatchStyleSmallConfetti,                // 34
        HatchStyleLargeConfetti,                // 35
        HatchStyleZigZag,                       // 36
        HatchStyleWave,                         // 37
        HatchStyleDiagonalBrick,                // 38
        HatchStyleHorizontalBrick,              // 39
        HatchStyleWeave,                        // 40
        HatchStylePlaid,                        // 41
        HatchStyleDivot,                        // 42
        HatchStyleDottedGrid,                   // 43
        HatchStyleDottedDiamond,                // 44
        HatchStyleShingle,                      // 45
        HatchStyleTrellis,                      // 46
        HatchStyleSphere,                       // 47
        HatchStyleSmallGrid,                    // 48
        HatchStyleSmallCheckerBoard,            // 49
        HatchStyleLargeCheckerBoard,            // 50
        HatchStyleOutlinedDiamond,              // 51
        HatchStyleSolidDiamond,                 // 52

        HatchStyleTotal,
        HatchStyleLargeGrid = HatchStyleCross,  // 4

        HatchStyleMin       = HatchStyleHorizontal,
        HatchStyleMax       = HatchStyleTotal - 1,
    };

    //--------------------------------------------------------------------------
    // Dash style constants
    //--------------------------------------------------------------------------

    enum DashStyle
    {
        DashStyleSolid,          // 0
        DashStyleDash,           // 1
        DashStyleDot,            // 2
        DashStyleDashDot,        // 3
        DashStyleDashDotDot,     // 4
        DashStyleCustom          // 5
    };

    //--------------------------------------------------------------------------
    // Dash cap constants
    //--------------------------------------------------------------------------

    enum DashCap
    {
        DashCapFlat             = 0,
        DashCapRound            = 2,
        DashCapTriangle         = 3
    };

    // Convert a DashCap to an agg::line_cap_e
    inline agg::line_cap_e convertDashCap( DashCap dashCap ) 
    {
        switch ( dashCap )
        {
        case DashCapRound:
            return agg::round_cap;

        case DashCapTriangle:
            return agg::square_cap;

        case DashCapFlat:
        default:
            return agg::butt_cap;
        }
    }

    // Convert an agg::line_cap_e to a DashCap
    inline DashCap convertDashCap( agg::line_cap_e dashCap ) 
    {
        switch ( dashCap )
        {
        case agg::round_cap:
            return DashCapRound;

        case agg::square_cap:
            return DashCapTriangle;

        case agg::butt_cap:
        default:
            return DashCapFlat;
        }
    }

    //--------------------------------------------------------------------------
    // Line cap constants (only the lowest 8 bits are used).
    //--------------------------------------------------------------------------

    enum LineCap
    {
        LineCapFlat             = 0,
        LineCapSquare           = 1,
        LineCapRound            = 2,
        LineCapTriangle         = 3,

        LineCapNoAnchor         = 0x10, // corresponds to flat cap
        LineCapSquareAnchor     = 0x11, // corresponds to square cap
        LineCapRoundAnchor      = 0x12, // corresponds to round cap
        LineCapDiamondAnchor    = 0x13, // corresponds to triangle cap
        LineCapArrowAnchor      = 0x14, // no correspondence

        LineCapCustom           = 0xff, // custom cap

        LineCapAnchorMask       = 0xf0  // mask to check for anchor or not.
    };

    // Convert a LineCap to an agg::line_cap_e
    inline agg::line_cap_e convertLineCap( LineCap lineCap ) 
    {
        switch ( lineCap )
        {
        case LineCapSquare:
            return agg::square_cap;

        case LineCapRound:
            return agg::round_cap;

        case LineCapTriangle:
            // Not actually defined...
            return agg::square_cap;

        case LineCapNoAnchor:
            return agg::butt_cap;

        case LineCapSquareAnchor:
            return agg::square_cap;

        case LineCapRoundAnchor:
            return agg::round_cap;

        case LineCapDiamondAnchor:
            // Not actually defined...
            return agg::square_cap;

        //case LineCapArrowAnchor:

        //case LineCapCustom:

        //case LineCapAnchorMask:

        case LineCapFlat:
        default:
            return agg::butt_cap;
        }
    }

    // Convert an agg::line_cap_e to a LineCap
    inline LineCap convertLineCap( agg::line_cap_e lineCap ) 
    {
        switch ( lineCap )
        {
        case agg::square_cap:
            return LineCapSquare;

        //case LineCapTriangle:
        //    // Not actually defined...
        //    return agg::square_cap;

        //case LineCapDiamondAnchor:
        //    // Not actually defined...
        //    return agg::square_cap;

        //case agg::square_cap:
        //    return LineCapSquareAnchor;

        case agg::round_cap:
            return LineCapRound;

        //case agg::round_cap:
        //    return LineCapRoundAnchor;

        //case LineCapArrowAnchor:

        //case LineCapCustom:

        //case LineCapAnchorMask:

        case agg::butt_cap:
            return LineCapNoAnchor;

        default:
            return LineCapFlat;
        }
    }

    //--------------------------------------------------------------------------
    // Custom Line cap type constants
    //--------------------------------------------------------------------------

    enum CustomLineCapType
    {
        CustomLineCapTypeDefault         = 0,
        CustomLineCapTypeAdjustableArrow = 1
    };

    //--------------------------------------------------------------------------
    // Line join constants
    //--------------------------------------------------------------------------

    enum LineJoin
    {
        LineJoinMiter        = 0,
        LineJoinBevel        = 1,
        LineJoinRound        = 2,
        LineJoinMiterClipped = 3
    };

    // Convert a LineJoin to an agg::line_join_e
    inline agg::line_join_e convertLineJoin( LineJoin lineJoin )
    {
        switch ( lineJoin )
        {
        case LineJoinMiter:
            return agg::miter_join;

        case LineJoinBevel:
            return agg::bevel_join;

        case LineJoinMiterClipped:
            return agg::miter_join_revert;

        case LineJoinRound:
        default:
            return agg::round_join;
        }
        return agg::round_join;
    }

    // Convert an agg::line_join_e to a LineJoin
    inline LineJoin convertLineJoin( agg::line_join_e lineJoin )
    {
        switch ( lineJoin )
        {
        case agg::miter_join:
            return LineJoinMiter;

        case agg::bevel_join:
            return LineJoinBevel;

        case agg::miter_join_revert:
            return LineJoinMiterClipped;
        }
        return LineJoinRound;
    }

    // Convert a LineJoin to an agg::inner_join_e
    inline agg::inner_join_e convertInnerJoin( LineJoin lineJoin ) 
    {
        switch ( lineJoin )
        {
        case LineJoinMiter:
            return agg::inner_miter;

        case LineJoinBevel:
            return agg::inner_bevel;

        case LineJoinMiterClipped:
            return agg::inner_jag;

        case LineJoinRound:
        default:
            return agg::inner_round;
        }
        return agg::inner_round;
    }

    // Convert an agg::inner_join_e to a LineJoin
    inline LineJoin convertInnerJoin( agg::inner_join_e lineJoin ) 
    {
        switch ( lineJoin )
        {
        case agg::inner_miter:
            return LineJoinMiter;

        case agg::inner_bevel:
            return LineJoinBevel;

        case agg::inner_jag:
            return LineJoinMiterClipped;
        }
        return LineJoinRound;
    }

    //--------------------------------------------------------------------------
    // Path point types (only the lowest 8 bits are used.)
    //  The lowest 3 bits are interpreted as point type
    //  The higher 5 bits are reserved for flags.
    //--------------------------------------------------------------------------

    enum PathPointType
    {
        PathPointTypeStart           = 0,    // move
        PathPointTypeLine            = 1,    // line
        PathPointTypeBezier          = 3,    // default Bezier (= cubic Bezier)
        PathPointTypePathTypeMask    = 0x07, // type mask (lowest 3 bits).
        PathPointTypeDashMode        = 0x10, // currently in dash mode.
        PathPointTypePathMarker      = 0x20, // a marker for the path.
        PathPointTypeCloseSubpath    = 0x80, // closed flag

        // Path types used for advanced path.

        PathPointTypeBezier3    = 3,         // cubic Bezier
    };


    //--------------------------------------------------------------------------
    // WarpMode constants
    //--------------------------------------------------------------------------

    enum WarpMode
    {
        WarpModePerspective,    // 0
        WarpModeBilinear        // 1
    };

    //--------------------------------------------------------------------------
    // LineGradient Mode
    //--------------------------------------------------------------------------

    enum LinearGradientMode
    {
        LinearGradientModeHorizontal,         // 0
        LinearGradientModeVertical,           // 1
        LinearGradientModeForwardDiagonal,    // 2
        LinearGradientModeBackwardDiagonal    // 3
    };

    /** AggOO exclusive

        GDI+ really only provides for linear or path gradients.  With AGG, we
        can provide addition styles, such as radial, XY, and conic.  All of
        these are handled with the same LinearGradientBrush, so be careful if
        porting from AggOO to GDI+.
    */
    enum GradientStyle
    {
        GradientStyleConic,             // 0
        GradientStyleDiamond,           // 1
        GradientStyleLinear,            // 2
        GradientStyleRadial,            // 3
        GradientStyleSqrtXY,            // 4
        GradientStyleXY,                // 5
        GradientStyleSpiral,            // 6
        GradientStyleRadialFocus,       // 7
        GradientStyleCircle             // 8
    };

    /** AggOO exclusive

        Allow for different gradient interpolation methods (how colors are
        interpolated between key positions.)
    */
    enum GradientInterpolation
    {
        GradientInterpolationLinear,    // 0
        GradientInterpolationStep       // 1
    };

    //--------------------------------------------------------------------------
    // Region Combine Modes
    //--------------------------------------------------------------------------

    enum CombineMode
    {
        CombineModeReplace,     // 0
        CombineModeIntersect,   // 1
        CombineModeUnion,       // 2
        CombineModeXor,         // 3
        CombineModeExclude,     // 4
        CombineModeComplement   // 5 (Exclude From)
    };

    //--------------------------------------------------------------------------
    // Image types
    //--------------------------------------------------------------------------

    enum ImageType
    {
        ImageTypeUnknown,   // 0
        ImageTypeBitmap,    // 1
        ImageTypeMetafile   // 2
    };

    //--------------------------------------------------------------------------
    // Interpolation modes
    //--------------------------------------------------------------------------

    enum InterpolationMode
    {
        InterpolationModeInvalid          = QualityModeInvalid,
        InterpolationModeDefault          = QualityModeDefault,
        InterpolationModeLowQuality       = QualityModeLow,
        InterpolationModeHighQuality      = QualityModeHigh,
        InterpolationModeBilinear,
        InterpolationModeBicubic,
        InterpolationModeNearestNeighbor,
        InterpolationModeHighQualityBilinear,
        InterpolationModeHighQualityBicubic
    };

    //--------------------------------------------------------------------------
    // Pen types
    //--------------------------------------------------------------------------

    enum PenAlignment
    {
        PenAlignmentCenter       = 0,
        PenAlignmentInset        = 1
    };

    //--------------------------------------------------------------------------
    // Brush types
    //--------------------------------------------------------------------------

    enum BrushType
    {
        BrushTypeSolidColor       = 0,
        BrushTypeHatchFill        = 1,
        BrushTypeTextureFill      = 2,
        BrushTypePathGradient     = 3,
        BrushTypeLinearGradient   = 4
    };

    //--------------------------------------------------------------------------
    // Pen's Fill types
    //--------------------------------------------------------------------------

    enum PenType
    {
        PenTypeSolidColor       = BrushTypeSolidColor,
        PenTypeHatchFill        = BrushTypeHatchFill,
        PenTypeTextureFill      = BrushTypeTextureFill,
        PenTypePathGradient     = BrushTypePathGradient,
        PenTypeLinearGradient   = BrushTypeLinearGradient,
        PenTypeUnknown          = -1
    };

    //--------------------------------------------------------------------------
    // Matrix Order
    //--------------------------------------------------------------------------

    enum MatrixOrder
    {
        MatrixOrderPrepend    = 0,
        MatrixOrderAppend     = 1
    };

    //---------------------------------------------------------------------------
    // Smoothing Mode
    //---------------------------------------------------------------------------

    enum SmoothingMode
    {
        SmoothingModeInvalid     = QualityModeInvalid,
        SmoothingModeDefault     = QualityModeDefault,
        SmoothingModeHighSpeed   = QualityModeLow,
        SmoothingModeHighQuality = QualityModeHigh,
        SmoothingModeNone,
        SmoothingModeAntiAlias
    };

    //---------------------------------------------------------------------------
    // Pixel Format Mode
    //---------------------------------------------------------------------------

    enum PixelOffsetMode
    {
        PixelOffsetModeInvalid     = QualityModeInvalid,
        PixelOffsetModeDefault     = QualityModeDefault,
        PixelOffsetModeHighSpeed   = QualityModeLow,
        PixelOffsetModeHighQuality = QualityModeHigh,
        PixelOffsetModeNone,    // No pixel offset
        PixelOffsetModeHalf     // Offset by -0.5, -0.5 for fast anti-alias perf
    };

    //--------------------------------------------------------------------------
    // Generic font families
    //--------------------------------------------------------------------------

    enum GenericFontFamily
    {
        GenericFontFamilySerif,
        GenericFontFamilySansSerif,
        GenericFontFamilyMonospace

    };

    //--------------------------------------------------------------------------
    // FontStyle: face types and common styles
    //--------------------------------------------------------------------------

    enum FontStyle
    {
        FontStyleRegular    = 0,
        FontStyleBold       = 1,
        FontStyleItalic     = 2,
        FontStyleBoldItalic = 3,
        FontStyleUnderline  = 4,
        FontStyleStrikeout  = 8
    };

    //---------------------------------------------------------------------------
    // Text Rendering Hint
    //---------------------------------------------------------------------------

    enum TextRenderingHint
    {
        TextRenderingHintSystemDefault = 0,            // Glyph with system default rendering hint
        TextRenderingHintSingleBitPerPixelGridFit,     // Glyph bitmap with hinting
        TextRenderingHintSingleBitPerPixel,            // Glyph bitmap without hinting
        TextRenderingHintAntiAliasGridFit,             // Glyph anti-alias bitmap with hinting
        TextRenderingHintAntiAlias,                    // Glyph anti-alias bitmap without hinting
        TextRenderingHintClearTypeGridFit              // Glyph CT bitmap with hinting
    };

    //---------------------------------------------------------------------------
    // String alignment flags
    //---------------------------------------------------------------------------

    enum StringAlignment
    {
        // Left edge for left-to-right text,
        // right for right-to-left text,
        // and top for vertical
        StringAlignmentNear   = 0,
        StringAlignmentCenter = 1,
        StringAlignmentFar    = 2
    };

    //---------------------------------------------------------------------------
    // DriverStringOptions
    //---------------------------------------------------------------------------

    enum DriverStringOptions
    {
        DriverStringOptionsCmapLookup             = 1,
        DriverStringOptionsVertical               = 2,
        DriverStringOptionsRealizedAdvance        = 4,
        DriverStringOptionsLimitSubpixel          = 8
    };


} // namespace AggOO

#endif  // AggOOEnums_H_
