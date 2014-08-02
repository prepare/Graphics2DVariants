
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
// $Id: AggOOGraphicsPath.h,v 1.5 2008/08/01 15:13:33 dratek Exp $
//
// AggOOGraphicsPath.h -- Header file for the GraphicsPath class
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

#ifndef AggOOGraphicsPath_H_
#define AggOOGraphicsPath_H_

#include "AggOOBase.h"
#include "AggOOEnums.h"
#include "AggOOTypes.h"

#include "agg_path_storage.h"

namespace AggOO
{
    class Matrix;

    /**
        @class GraphicsPath

            The GraphicsPath stores a sequence of lines, shapes, and curves.  The
            entire sequence can be drawn at once using the Graphics::DrawPath
            method.
    */
    class _AggOOExport GraphicsPath : public Base
    {
    public:
        friend class Graphics;

    protected:
        mutable Status  mLastStatus;

        /** Set the status of the pen
        */
        Status SetStatus( Status status ) const
        {
            if ( status != Ok )
                return ( mLastStatus = status );
            return status;
        }

        FillMode        mFillMode;

        void copyFrom( const GraphicsPath* src );

        agg::path_storage  mPathStorage;

        bool isPathClosed() const;

    public:
        /** Constructor */
        GraphicsPath( FillMode fillMode = FillModeAlternate );
        /** Constructor */
        GraphicsPath( const Point* points, const BYTE* types, INT count, FillMode fillMode = FillModeAlternate );
        /** Constructor */
        GraphicsPath( const PointF* points, const BYTE* types, INT count, FillMode fillMode = FillModeAlternate );
        /** Destructor */
        virtual ~GraphicsPath(void);

        // Return a pointer to the path storage
        agg::path_storage* getPathStorage()     { return &mPathStorage; }

        /** Add an AGG path */
        template< class VertexSource >
        Status addAggPath( const VertexSource& path, unsigned pathID = 0 )
        {
            double x, y;
            unsigned cmd, curVert = 0;
            PointF* pts = new PointF [ path.total_vertices() ];
            path.rewind( pathID );
            cmd = path.vertex( &x, &y );
            if ( !is_stop( cmd ) )
            {
                pts[ curVert++ ] = PointF( x, y );
                while ( !is_stop( cmd = path.vertex( &x, &y ) ) )
                    pts[ curVert++ ] = PointF( x, y );
            }
            AddLines( pts, curVert );
            delete pts;
            return Ok;
        }

        /** Return the previously set status */
        Status GetLastStatus() const    { return mLastStatus; }

        /** Create a new clone of the path */
        GraphicsPath* Clone(void) const;

        /** Start a new shape without closing the previous one */
        Status StartFigure(void);
        /** Close all shapes in the path */
        Status CloseAllFigures(void);
        /** Close the current shape in the path */
        Status CloseFigure(void);

        /** Empty the path and set the fill mode to FillModeAlternate */
        Status Reset(void);

        /** Add an arc to the graphics path */
        Status AddArc( const Rect& rect, REAL startAngle, REAL sweepAngle );
        /** Add an arc to the graphics path */
        Status AddArc( const RectF& rect, REAL startAngle, REAL sweepAngle );
        /** Add an arc to the graphics path */
        Status AddArc( INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle );
        /** Add an arc to the graphics path */
        Status AddArc( REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle );

        /** Add a quadratic curve to the path */
        Status AddQuadratic( const Point& pt1, const Point& pt2, const Point& pt3 );
        /** Add a quadratic curve to the path */
        Status AddQuadratic( const PointF& pt1, const PointF& pt2, const PointF& pt3 );
        /** Add a quadratic curve to the path */
        Status AddQuadratic( INT x1, INT y1, INT x2, INT y2, INT x3, INT y3 );
        /** Add a quadratic curve to the path */
        Status AddQuadratic( REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3 );

        /** Add a sequence of quadratic curves to the path
            @remarks
                Each quadratic spline is made of 3 points.  The first and last
                points are the end-points, and the second is the
                control point.  When adding multiple splines, the third point
                of the previous spline is used as the first point of the current
                spline.  In other words, the first 3 points in the array are
                used for the first spline, the next 2, along with the last point
                from the first spline, are used for the second spline, and so on.
        */
        Status AddQuadratics( const Point* points, INT count );
        /** Add a sequence of quadratic curves to the path
            @remarks
                Each quadratic spline is made of 3 points.  The first and last
                points are the end-points, and the second is the
                control point.  When adding multiple splines, the third point
                of the previous spline is used as the first point of the current
                spline.  In other words, the first 3 points in the array are
                used for the first spline, the next 2, along with the last point
                from the first spline, are used for the second spline, and so on.
        */
        Status AddQuadratics( const PointF* points, INT count );

        /** Add a bezier curve to the path */
        Status AddBezier( const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4 );
        /** Add a bezier curve to the path */
        Status AddBezier( const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4 );
        /** Add a bezier curve to the path */
        Status AddBezier( INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4 );
        /** Add a bezier curve to the path */
        Status AddBezier( REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4 );

        /** Add a sequence of bezier curve to the path
            @remarks
                Each bezier spline is made of 4 points.  The first and fourth
                points are the end-points, and the second and third are the
                control points.  When adding multiple splines, the fourth point
                of the previous spline is used as the first point of the current
                spline.  In other words, the first 4 points in the array are
                used for the first spline, the next 3, along with the last point
                from the first spline, are used for the second spline, and so on.
        */
        Status AddBeziers( const Point* points, INT count );
        /** Add a sequence of bezier curve to the path
            @remarks
                Each bezier spline is made of 4 points.  The first and fourth
                points are the end-points, and the second and third are the
                control points.  When adding multiple splines, the fourth point
                of the previous spline is used as the first point of the current
                spline.  In other words, the first 4 points in the array are
                used for the first spline, the next 3, along with the last point
                from the first spline, are used for the second spline, and so on.
        */
        Status AddBeziers( const PointF* points, INT count );

        /** Add a closed spline to the path */
        Status AddClosedCurve( const Point* points, INT count );
        /** Add a closed spline to the path */
        Status AddClosedCurve( const PointF* points, INT count );

        /** Add a spline to the path */
        Status AddCurve( const Point* points, INT count );
        /** Add a spline to the path */
        Status AddCurve( const PointF* points, INT count );
        /** Add a spline to the path */
        //Status AddCurve( const Point* points, INT count, INT offset, INT numberOfSegments, REAL tension );
        /** Add a spline to the path */
        //Status AddCurve( const PointF* points, INT count, INT offset, INT numberOfSegments, REAL tension );
        /** Add a spline to the path */
        //Status AddCurve( const Point* points, INT count, REAL tension );
        /** Add a spline to the path */
        //Status AddCurve( const PointF* points, INT count, REAL tension );

        /** Add an ellipse to the path */
        Status AddEllipse( const Rect& rect );
        /** Add an ellipse to the path */
        Status AddEllipse( const RectF& rect );
        /** Add an ellipse to the path */
        Status AddEllipse( INT x, INT y, INT width, INT height );
        /** Add an ellipse to the path */
        Status AddEllipse( REAL x, REAL y, REAL width, REAL height );

        /** Add a line to the path */
        Status AddLine( const Point& pt1, const Point& pt2 );
        /** Add a line to the path */
        Status AddLine( const PointF& pt1, const PointF& pt2 );
        /** Add a line to the path */
        Status AddLine( INT x1, INT y1, INT x2, INT y2 );
        /** Add a line to the path */
        Status AddLine( REAL x1, REAL y1, REAL x2, REAL y2 );

        /** Add a sequence of lines to the path */
        Status AddLines( const Point* points, INT count );
        /** Add a sequence of lines to the path */
        Status AddLines( const PointF* points, INT count );

        /** Add a path to the existing path */
        Status AddPath( const GraphicsPath* addingPath, BOOL connect );

        /** Add a pie to the path */
        Status AddPie( const Rect& rect, REAL startAngle, REAL sweepAngle );
        /** Add a pie to the path */
        Status AddPie( const RectF& rect, REAL startAngle, REAL sweepAngle );
        /** Add a pie to the path */
        Status AddPie( INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle );
        /** Add a pie to the path */
        Status AddPie( REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle );

        /** Add a polygon to the path */
        Status AddPolygon( const Point* points, INT count );
        /** Add a polygon to the path */
        Status AddPolygon( const PointF* points, INT count );

        /** Add a rectangle to the path */
        Status AddRectangle( const Rect& rect );
        /** Add a rectangle to the path */
        Status AddRectangle( const RectF& rect );

        /** Add a sequence of rectangles to the path */
        Status AddRectangles( const Rect* rects, INT count );
        /** Add a sequence of rectangles to the path */
        Status AddRectangles( const RectF* rects, INT count );

        /** Get the fill mode for the path */
        FillMode GetFillMode(void) const    { return mFillMode; }

        /** Set the fill mode for the path */
        Status SetFillMode( FillMode fillMode )
        {
            mFillMode = fillMode;
            return SetStatus( Ok );
        }

        /** Get the last point in the path */
        Status GetLastPoint( PointF* lastPoint );

        ///** Get an array of points and point types that define the path
        //*/
        //Status GetPathData( PathData* pathData );

        ///** Get an array of the path's points.  The points are the end points
        //    and control points of the lines and bezier curves.
        //*/
        //Status GetPathPoints( Point* points, INT count );
        //Status GetPathPoints( PointF* points, INT count );

        ///** Get an array of point types used in the path
        //*/
        //Status GetPathTypes( BYTE* types, INT count );

        ///** Get the number of points in the path's array of data points.  This
        //    is the same as the number of point types.
        //*/
        //INT GetPointCount(void);

        ///** Reverse the order of the points that define this path
        //*/
        //Status Reverse(void);

        /** Transform the path by a given matrix */
        Status Transform( const Matrix* matrix );

    };

    /** @remarks
        // TODO: The following methods exist in GDI+, but not AggOO (yet):

        Status AddString( const WCHAR* string, INT length, const FontFamily* family, INT style, REAL emSize, const Rect& layoutRect, const StringFormat* format );
        Status AddString( const WCHAR* string, INT length, const FontFamily* family, INT style, REAL emSize, const RectF& layoutRect, const StringFormat* format );
        Status AddString( const WCHAR* string, INT length, const FontFamily* family, INT style, REAL emSize, const Point& origin, const StringFormat* format );
        Status AddString( const WCHAR* string, INT length, const FontFamily* family, INT style, REAL emSize, const PointF& origin, const StringFormat* format );

        Status ClearMarkers(void);
        Status SetMarker(void);

        Status Flatten( const Matrix* matrix = 0, REAL flatness = FlatnessDefault );

        Status GetBounds( Rect* bounds, const Matrix* matrix, const Pen* pen );
        Status GetBounds( RectF* bounds, const Matrix* matrix, const Pen* pen );

        BOOL IsOutlineVisible( const Point& point, const Pen* pen, const Graphics* g );
        BOOL IsOutlineVisible( const PointF& point, const Pen* pen, const Graphics* g );
        BOOL IsOutlineVisible( INT x, INT y, const Pen* pen, const Graphics* g );
        BOOL IsOutlineVisible( REAL x, REAL y, const Pen* pen, const Graphics* g );

        BOOL IsVisible( const Point& point, const Graphics* g );
        BOOL IsVisible( const PointF& point, const Graphics* g );
        BOOL IsVisible( INT x, INT y, const Graphics* g );
        BOOL IsVisible( REAL x, REAL y, const Graphics* g );

        Status Outline( const Matrix* matrix, REAL flatness );

        Status Warp( const PointF* destPoints, INT count, const RectF& srcRect, const Matrix* matrix, WarpMode warpMode, REAL flatness );

        Status Widen( const Pen* pen, const Matrix* matrix, REAL flatness );

    */

} // namespace AggOO

#endif  // AggOOGraphicsPath_H_
