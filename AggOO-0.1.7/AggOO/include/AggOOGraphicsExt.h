
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
// $Id: AggOOGraphicsExt.h,v 1.8 2009/02/26 22:13:38 dratek Exp $
//
// AggOOGraphicsExt.h -- Extended rendering class with additional features not
//                       found in GDI+.
//
// Author       : Chad M. Draper
// Date         : September 27, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-27      Initial implementation
//

#ifndef AggOOGraphicsExt_H_
#define AggOOGraphicsExt_H_

#include "AggOOGraphics.h"
//#include "AggOOVectorShapeData.h"

namespace AggOO
{

    /** @class GraphicsExt

            Extension of AggOO::Graphics class with methods not found in GDI+, but
            which may be of significant value.
    */
    class _AggOOExport GraphicsExt : public Graphics
    {
    public:
        /** Constructor */
        GraphicsExt( Image* image, ColorFormat colFmt = ColorFormatPlain );
//        ~GraphicsExt(void);


        /** Draw a rounded rectangle */
        Status DrawRoundRectangle( const Pen* pen, const Rect& rect, REAL radius );
        /** Draw a rounded rectangle */
        Status DrawRoundRectangle( const Pen* pen, const RectF& rect, REAL radius );
        /** Draw a rounded rectangle */
        Status DrawRoundRectangle( const Pen* pen, INT x, INT y, INT width, INT height, REAL radius );
        /** Draw a rounded rectangle */
        Status DrawRoundRectangle( const Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL radius );

        /** Fill a rounded rectangle */
        Status FillRoundRectangle( const Brush* brush, const Rect& rect, REAL radius );
        /** Fill a rounded rectangle */
        Status FillRoundRectangle( const Brush* brush, const RectF& rect, REAL radius );
        /** Fill a rounded rectangle */
        Status FillRoundRectangle( const Brush* brush, INT x, INT y, INT width, INT height, REAL radius );
        /** Fill a rounded rectangle */
        Status FillRoundRectangle( const Brush* brush, REAL x, REAL y, REAL width, REAL height, REAL radius );

        /** Draw a regular polygon */
        Status DrawRegularPolygon( const Pen* pen, INT numPoints, const Point& center, REAL radius );
        /** Draw a regular polygon */
        Status DrawRegularPolygon( const Pen* pen, INT numPoints, const PointF& center, REAL radius );

        /** Fill a regular polygon */
        Status FillRegularPolygon( const Brush* brush, INT numPoints, const Point& center, REAL radius );
        /** Fill a regular polygon */
        Status FillRegularPolygon( const Brush* brush, INT numPoints, const PointF& center, REAL radius );
        /** Fill a regular polygon */
        Status FillRegularPolygon( const Brush* brush, INT numPoints, const Point& center, REAL radius, FillMode fillMode );
        /** Fill a regular polygon */
        Status FillRegularPolygon( const Brush* brush, INT numPoints, const PointF& center, REAL radius, FillMode fillMode );

        /** Draw a star */
        Status DrawStar( const Pen* pen, INT numPoints, const Point& center, INT radius1, INT radius2 );
        /** Draw a star */
        Status DrawStar( const Pen* pen, INT numPoints, const PointF& center, REAL radius1, REAL radius2 );

        /** Fill a star */
        Status FillStar( const Brush* brush, INT numPoints, const Point& center, INT radius1, INT radius2 );
        /** Fill a star */
        Status FillStar( const Brush* brush, INT numPoints, const PointF& center, REAL radius1, REAL radius2 );
        /** Fill a star */
        Status FillStar( const Brush* brush, INT numPoints, const Point& center, INT radius1, INT radius2, FillMode fillMode );
        /** Fill a star */
        Status FillStar( const Brush* brush, INT numPoints, const PointF& center, REAL radius1, REAL radius2, FillMode fillMode );

        /** Draw a series of connected lines */
        Status DrawLineStrip( const Pen* pen, const Point* points, INT count );
        /** Draw a series of connected lines */
        Status DrawLineStrip( const Pen* pen, const PointF* points, INT count );

        /** Draw an SVGShape */
        //Status DrawVectorShape( const Pen* pen, const vs::VectorShapeData& shape );

        /** Fill an SVGShape */
        //Status FillVectorShape( const Brush* brush, const vs::VectorShapeData& shape );
        //Status FillVectorShape( const Brush* brush, const vs::VectorShapeData& shape, FillMode fillMode );


        //// Convert an AggOO LineJoin to an AGG line_join_e
        //static agg::line_join_e convertLineJoin( LineJoin lineJoin )
        //{
        //    switch ( lineJoin )
        //    {
        //    case LineJoinMiter:
        //        return agg::miter_join;

        //    case LineJoinBevel:
        //        return agg::bevel_join;

        //    case LineJoinMiterClipped:
        //        return agg::miter_join_revert;

        //    case LineJoinRound:
        //    default:
        //        return agg::round_join;
        //    }
        //    return agg::round_join;
        //}

        //// Convert an AggOO LineJoin to an AGG inner_join_e
        //static agg::inner_join_e convertInnerJoin( LineJoin lineJoin ) 
        //{
        //    switch ( lineJoin )
        //    {
        //    case LineJoinMiter:
        //        return agg::inner_miter;

        //    case LineJoinBevel:
        //        return agg::inner_bevel;

        //    case LineJoinMiterClipped:
        //        return agg::inner_jag;

        //    case LineJoinRound:
        //    default:
        //        return agg::inner_round;
        //    }
        //    return agg::inner_round;
        //}

        //// Convert an AggOO LineCap to an AGG line_cap_e
        //static agg::line_cap_e convertLineCap( LineCap lineCap ) 
        //{
        //    switch ( lineCap )
        //    {
        //    case LineCapSquare:
        //        return agg::square_cap;

        //    case LineCapRound:
        //        return agg::round_cap;

        //    case LineCapTriangle:
        //        // Not actually defined...
        //        return agg::square_cap;

        //    case LineCapNoAnchor:
        //        return agg::butt_cap;

        //    case LineCapSquareAnchor:
        //        return agg::square_cap;

        //    case LineCapRoundAnchor:
        //        return agg::round_cap;

        //    case LineCapDiamondAnchor:
        //        // Not actually defined...
        //        return agg::square_cap;

        //    //case LineCapArrowAnchor:

        //    //case LineCapCustom:

        //    //case LineCapAnchorMask:

        //    case LineCapFlat:
        //    default:
        //        return agg::butt_cap;
        //    }
        //}

        //// Convert an AggOO DashCap to an AGG line_cap_e
        //static agg::line_cap_e convertDashCap( DashCap dashCap ) 
        //{
        //    switch ( dashCap )
        //    {
        //    case DashCapRound:
        //        return agg::round_cap;

        //    case DashCapTriangle:
        //        return agg::square_cap;

        //    case DashCapFlat:
        //    default:
        //        return agg::butt_cap;
        //    }
        //}
    };

} // namespace AggOO

#endif  // AggOOGraphicsExt_H_
