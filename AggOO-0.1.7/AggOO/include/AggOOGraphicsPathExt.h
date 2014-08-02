
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
// $Id: AggOOGraphicsPathExt.h,v 1.9 2009/02/26 22:13:38 dratek Exp $
//
// AggOOGraphicsPathExt.h -- Extension of the GraphicsPath class, with
//                           additional features not found in GDI+.
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

#ifndef AggOOGraphicsPathExt_H_
#define AggOOGraphicsPathExt_H_

#include "AggOOGraphicsPath.h"
//#include "AggOOSVGShapeData.h"
//#include "AggOOAIShapeData.h"

namespace AggOO
{
    //namespace svg
    //{
    //    class SVGShapeData;
    //}
    //namespace ai
    //{
    //    class AIShapeData;
    //}
    namespace vs
    {
        class VectorShapeData;
    }

    /** @class GraphicsPathExt

            Extension of AggOO::GraphicsPath class with methods not found in GDI+,
            but which may be of significant value.
    */
    class _AggOOExport GraphicsPathExt : public GraphicsPath
    {
    public:
        /** Constructor */
        GraphicsPathExt( FillMode fillMode = FillModeAlternate );
        /** Constructor */
        GraphicsPathExt( const Point* points, const BYTE* types, INT count, FillMode fillMode = FillModeAlternate );
        /** Constructor */
        GraphicsPathExt( const PointF* points, const BYTE* types, INT count, FillMode fillMode = FillModeAlternate );
        //~GraphicsPathExt(void);

        /** Add a rounded rectangle to the path */
        Status AddRoundRectangle( const Rect& rect, REAL radius );
        /** Add a rounded rectangle to the path */
        Status AddRoundRectangle( const RectF& rect, REAL radius );

        /** Add a regular polygon to the path */
        Status AddRegularPolygon( INT numPoints, const Point& center, INT radius );
        /** Add a regular polygon to the path */
        Status AddRegularPolygon( INT numPoints, const PointF& center, INT radius );

        /** Add a star to the path.
            @param  numPoints       Number of outer points on the star
            @param  center          Coordinate of the star's center
            @param  radius1         Inner radius of the star
            @param  radius2         Outer radius of the star
        */
        Status AddStar( INT numPoints, const Point& center, INT radius1, INT radius2 );
        /** Add a star to the path.
            @param  numPoints       Number of outer points on the star
            @param  center          Coordinate of the star's center
            @param  radius1         Inner radius of the star
            @param  radius2         Outer radius of the star
        */
        Status AddStar( INT numPoints, const PointF& center, REAL radius1, INT radius2 );

        /** Add a series of connected lines. */
        Status AddLineStrip( const Point* points, INT count );
        /** Add a series of connected lines. */
        Status AddLineStrip( const PointF* points, INT count );

        ///** Add a VectorShape to the path */
        //Status AddVectorShape( const vs::VectorShapeData& shape );
        ///** Add an array of VectorShapes to the path */
        //Status AddVectorShape( const vs::VectorShapeData* shape, INT count );

        /** The following methods reflect similar agg::path_base methods, which
            allow for generating the path in sequence.  This can simplify
            parsing files, as the data is usually fed in the form of
            move_to(x,y), line_to(x,y).
        */

        /** Move to an absolute position */
        Status MoveTo( REAL x, REAL y );
        /** Move to a position relative to the last point. */
        Status MoveToRel( REAL dx, REAL dy );

        /** Draw a line from the last point to an absolute position */
        Status LineTo( REAL x, REAL y );
        /** Draw a line from the last point to an relative offset */
        Status LineToRel( REAL dx, REAL dy );

        /** Draw a horizontal line from the last point to an absolute x
            coordinate.  The y coordinate will be the same as the previous point.
        */
        Status HLineTo( REAL x );
        /** Draw a horizontal line from the last point to an relative x
            offset.  The y coordinate will be the same as the previous point.
        */
        Status HLineToRel( REAL dx );

        /** Draw a vertical line from the last point to an absolute x
            coordinate.  The y coordinate will be the same as the previous point.
        */
        Status VLineTo( REAL y );
        /** Draw a vertical line from the last point to an relative x
            offset.  The y coordinate will be the same as the previous point.
        */
        Status VLineToRel( REAL dy );

        /** Draw an arc from the last point with a center at an absolute position.
            @param  rx          Horizontal radius of the arc
            @param  ry          Vertical radius of the arc
            @param  angle       Angle that the arc will sweep through (in degrees)
            @param  largeArc    If true, the arc sweeps through the larger angle to its endpoint
            @param  sweepFlag   Indicates which direction the arc will sweep
            @param  x           Absolute x-coordinate of the center of the arc
            @param  y           Absolute y-coordinate of the center of the arc
        */
        Status ArcTo( REAL rx, REAL ry, REAL angle, bool largeArc, bool sweepFlag, REAL x, REAL y );
        /** Draw an arc from the last point with a center at a position relative
            to the previous point.
            @param  rx          Horizontal radius of the arc
            @param  ry          Vertical radius of the arc
            @param  angle       Angle that the arc will sweep through (in degrees)
            @param  largeArc    If true, the arc sweeps through the larger angle to its endpoint
            @param  sweepFlag   Indicates which direction the arc will sweep
            @param  dx          Relative x-coordinate of the center of the arc
            @param  dy          Relative y-coordinate of the center of the arc
        */
        Status ArcToRel( REAL rx, REAL ry, REAL angle, bool largeArc, bool sweepFlag, REAL dx, REAL dy );

        /** Draw a cubic curve from the last point to an absolute position
            @param  xCtrl       Absolute position of the x-control point
            @param  yCtrl       Absolute position of the y-control point
            @param  x           Absolute position of the destination x-coordinate
            @param  y           Absolute position of the destination y-coordinate
        */
        Status Curve3To( REAL xCtrl, REAL yCtrl, REAL x, REAL y );
        /** Draw a cubic curve from the last point to a position relative to the
            previous point.
            @param  xCtrl       Relative position of the x-control point
            @param  yCtrl       Relative position of the y-control point
            @param  dx          Relative position of the destination x-coordinate
            @param  dy          Relative position of the destination y-coordinate
        */
        Status Curve3ToRel( REAL xCtrl, REAL yCtrl, REAL dx, REAL dy );

        /** Draw a cubic curve from the last point to an absolute position
            @param  x           Absolute position of the destination x-coordinate
            @param  y           Absolute position of the destination y-coordinate
        */
        Status Curve3To( REAL x, REAL y );
        /** Draw a cubic curve from the last point to a position relative to the
            previous point.
            @param  dx          Relative position of the destination x-coordinate
            @param  dy          Relative position of the destination y-coordinate
        */
        Status Curve3ToRel( REAL dx, REAL dy );

        /** Draw a quadratic curve from the last point to an absolute position
            @param  xCtrl1      Absolute position of the first x-control point
            @param  yCtrl1      Absolute position of the first y-control point
            @param  xCtrl2      Absolute position of the second x-control point
            @param  yCtrl2      Absolute position of the second y-control point
            @param  x           Absolute position of the destination x-coordinate
            @param  y           Absolute position of the destination y-coordinate
        */
        Status Curve4To( REAL xCtrl1, REAL yCtrl1, REAL xCtrl2, REAL yCtrl2, REAL x, REAL y );
        /** Draw a quadratic curve from the last point to a position relative to the
            previous point.
            @param  xCtrl2      Relative position of the second x-control point
            @param  yCtrl2      Relative position of the second y-control point
            @param  dx          Relative position of the destination x-coordinate
            @param  dy          Relative position of the destination y-coordinate
        */
        Status Curve4ToRel( REAL xCtrl1, REAL yCtrl1, REAL xCtrl2, REAL yCtrl2, REAL dx, REAL dy );

        /** Draw a quadratic curve from the last point to an absolute position
            @param  xCtrl2      Absolute position of the second x-control point
            @param  yCtrl2      Absolute position of the second y-control point
            @param  x           Absolute position of the destination x-coordinate
            @param  y           Absolute position of the destination y-coordinate
        */
        Status Curve4To( REAL xCtrl2, REAL yCtrl2, REAL x, REAL y );
        /** Draw a cubic curve from the last point to a position relative to the
            previous point.
            @param  xCtrl2      Relative position of the second x-control point
            @param  yCtrl2      Relative position of the second y-control point
            @param  dx          Relative position of the destination x-coordinate
            @param  dy          Relative position of the destination y-coordinate
        */
        Status Curve4ToRel( REAL xCtrl2, REAL yCtrl2, REAL dx, REAL dy );

        /** Start a new path */
        Status StartNewPath();

        /** End the polygon */
        Status EndPolygon();

        /** Close the polygon */
        Status ClosePolygon();
    };

} // namespace AggOO

#endif  // AggOOGraphicsPathExt_H_
