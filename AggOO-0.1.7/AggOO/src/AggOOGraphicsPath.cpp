
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
// $Id: AggOOGraphicsPath.cpp,v 1.5 2008/08/01 15:13:35 dratek Exp $
//
// AggOOGraphicsPath.h -- Header file for the GraphicsPath class
//

#include "AggOOGraphicsPath.h"
#include "AggOOMatrix.h"

#include "agg_arc.h"
#include "agg_bezier_arc.h"
#include "agg_conv_bspline.h"
#include "agg_conv_stroke.h"

namespace AggOO
{
    GraphicsPath::GraphicsPath( FillMode fillMode )
        : mFillMode( fillMode )
    {
        mLastStatus = SetStatus( Ok );
    }

    GraphicsPath::GraphicsPath( const Point* points, const BYTE* types, INT count, FillMode fillMode )
        : mFillMode( fillMode )
    {
        mLastStatus = SetStatus( NotImplemented );
    }

    GraphicsPath::GraphicsPath( const PointF* points, const BYTE* types, INT count, FillMode fillMode )
        : mFillMode( fillMode )
    {
        mLastStatus = SetStatus( Ok );
    }

    GraphicsPath::~GraphicsPath()
    {
    }

    //isPathClosed--------------------------------------------------------------
    bool GraphicsPath::isPathClosed() const
    {
        // If the path is empty, it is considered closed
        if ( !mPathStorage.total_vertices() )
            return true;

        // Get the last vertex in the path.  If the flag for this vertex
        // indicates that it is closed, the path is closed.
        double x, y;
        unsigned flag = mPathStorage.last_vertex( &x, &y );
        if ( flag & agg::path_flags_close )
            return true;
        return false;
    }

    //GetLastPoint--------------------------------------------------------------
    Status GraphicsPath::GetLastPoint( PointF* lastPoint )
    {
        if ( !lastPoint )
            return SetStatus( InvalidParameter );

        double x, y;
        unsigned flag = mPathStorage.last_vertex( &x, &y );
        lastPoint->X = x;
        lastPoint->Y = y;
        return SetStatus( Ok );
    }


    //copyFrom------------------------------------------------------------------
    void GraphicsPath::copyFrom( const GraphicsPath* src )
    {
        // Copy the points from src to this buffer
        mPathStorage = src->mPathStorage;
    }

    //Clone---------------------------------------------------------------------
    GraphicsPath* GraphicsPath::Clone(void) const
    {
        GraphicsPath* ret = new GraphicsPath( mFillMode );
        ret->copyFrom( this );
        return ret;
    }

    //StartFigure---------------------------------------------------------------
    Status GraphicsPath::StartFigure(void)
    {
        mPathStorage.start_new_path();
        return SetStatus( Ok );
    }

    //CloseAllFigures-----------------------------------------------------------
    Status GraphicsPath::CloseAllFigures(void)
    {
        // TODO: This isn't really closing all figures
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //CloseFigure---------------------------------------------------------------
    Status GraphicsPath::CloseFigure(void)
    {
        mPathStorage.close_polygon();
        return SetStatus( Ok );
    }

    //Reset---------------------------------------------------------------------
    Status GraphicsPath::Reset(void)
    {
        mPathStorage.remove_all();
        mFillMode = FillModeAlternate;
        return SetStatus( Ok );
    }

    //AddArc--------------------------------------------------------------------
    Status GraphicsPath::AddArc( const Rect& rect, REAL startAngle, REAL sweepAngle )
    {
        return AddArc( rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle );
    }

    //AddArc--------------------------------------------------------------------
    Status GraphicsPath::AddArc( const RectF& rect, REAL startAngle, REAL sweepAngle )
    {
        return AddArc( rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle );
    }

    //AddArc--------------------------------------------------------------------
    Status GraphicsPath::AddArc( INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle )
    {
        REAL rx = width / 2.0, ry = height / 2.0;
        agg::bezier_arc arc( x + rx, y + ry, rx, ry, agg::deg2rad( startAngle ), agg::deg2rad( sweepAngle ) );

        if ( isPathClosed() )
            mPathStorage.concat_path( arc, 0 );
        else
            mPathStorage.join_path( arc, 0 );

        return SetStatus( Ok );
    }

    //AddArc--------------------------------------------------------------------
    Status GraphicsPath::AddArc( REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle )
    {
        REAL rx = width / 2.0, ry = height / 2.0;
        agg::bezier_arc arc( x + rx, y + ry, rx, ry, agg::deg2rad( startAngle ), agg::deg2rad( sweepAngle ) );

        if ( isPathClosed() )
            mPathStorage.concat_path( arc, 0 );
        else
            mPathStorage.join_path( arc, 0 );

        return SetStatus( Ok );
    }

    //AddEllipse----------------------------------------------------------------
    Status GraphicsPath::AddEllipse( const Rect& rect )
    {
        return AddEllipse( rect.X, rect.Y, rect.Width, rect.Height );
    }

    //AddEllipse----------------------------------------------------------------
    Status GraphicsPath::AddEllipse( const RectF& rect )
    {
        return AddEllipse( rect.X, rect.Y, rect.Width, rect.Height );
    }

    //AddEllipse----------------------------------------------------------------
    Status GraphicsPath::AddEllipse( INT x, INT y, INT width, INT height )
    {
        REAL rx = width / 2.0, ry = height / 2.0;
        agg::bezier_arc arc( x + rx, y + ry, rx, ry, 0.0, 2.0 * agg::pi );

/*
        if ( isPathClosed() )
            mPathStorage.concat_path( arc, 0 );
        else
            mPathStorage.join_path( arc, 0 );
*/

        CloseAllFigures();
        mPathStorage.concat_path( arc, 0 );
        //--mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddEllipse----------------------------------------------------------------
    Status GraphicsPath::AddEllipse( REAL x, REAL y, REAL width, REAL height )
    {
        REAL rx = width / 2.0, ry = height / 2.0;
        agg::bezier_arc arc( x + rx, y + ry, rx, ry, 0.0, 2.0 * agg::pi );

/*
        if ( isPathClosed() )
            mPathStorage.concat_path( arc, 0 );
        else
            mPathStorage.join_path( arc, 0 );
*/

        CloseAllFigures();
        mPathStorage.concat_path( arc, 0 );
        //--mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddLine-------------------------------------------------------------------
    Status GraphicsPath::AddLine( const Point& pt1, const Point& pt2 )
    {
        return AddLine( pt1.X, pt1.Y, pt1.X, pt2.Y );
    }

    //AddLine-------------------------------------------------------------------
    Status GraphicsPath::AddLine( const PointF& pt1, const PointF& pt2 )
    {
        return AddLine( pt1.X, pt1.Y, pt1.X, pt2.Y );
    }

    //AddLine-------------------------------------------------------------------
    Status GraphicsPath::AddLine( INT x1, INT y1, INT x2, INT y2 )
    {
        if ( isPathClosed() )
            mPathStorage.move_to( x1, y1 );
        else
            mPathStorage.line_to( x1, y1 );
        mPathStorage.line_to( x2, y2 );

        return SetStatus( Ok );
    }

    //AddLine-------------------------------------------------------------------
    Status GraphicsPath::AddLine( REAL x1, REAL y1, REAL x2, REAL y2 )
    {
        if ( isPathClosed() )
            mPathStorage.move_to( x1, y1 );
        else
            mPathStorage.line_to( x1, y1 );
        mPathStorage.line_to( x2, y2 );

        return SetStatus( Ok );
    }

    //AddLines------------------------------------------------------------------
    Status GraphicsPath::AddLines( const Point* points, INT count )
    {
        if ( !points )
            return SetStatus( InvalidParameter );

        Status status = Ok;
        if ( isPathClosed() )
            mPathStorage.move_to( points[ 0 ].X, points[ 0 ].Y );
        else
            mPathStorage.line_to( points[ 0 ].X, points[ 0 ].Y );
        INT curPt = 1;
        while ( curPt < count )
        {
            mPathStorage.line_to( points[ curPt ].X, points[ curPt ].Y );
            curPt++;
        }
        //INT curPt = 0;
        //do
        //{
        //    status = AddLine( points[ curPt ].X, points[ curPt ].Y, points[ curPt + 1 ].X, points[ curPt + 1 ].Y );
        //    curPt += 2;
        //} while ( status == Ok && curPt < count );
        return status;
    }

    //AddLines------------------------------------------------------------------
    Status GraphicsPath::AddLines( const PointF* points, INT count )
    {
        if ( !points )
            return SetStatus( InvalidParameter );

        Status status = Ok;
        if ( isPathClosed() )
            mPathStorage.move_to( points[ 0 ].X, points[ 0 ].Y );
        else
            mPathStorage.line_to( points[ 0 ].X, points[ 0 ].Y );
        INT curPt = 1;
        while ( curPt < count )
        {
            mPathStorage.line_to( points[ curPt ].X, points[ curPt ].Y );
            curPt++;
        }
        //INT curPt = 0;
        //do
        //{
        //    status = AddLine( points[ curPt ].X, points[ curPt ].Y, points[ curPt + 1 ].X, points[ curPt + 1 ].Y );
        //    curPt += 2;
        //} while ( status == Ok && curPt < count );
        return status;
    }

    //AddPath-------------------------------------------------------------------
    Status GraphicsPath::AddPath( const GraphicsPath* addingPath, BOOL connect )
    {
        if ( !addingPath )
            return SetStatus( InvalidParameter );

        if ( connect && !isPathClosed() )
            mPathStorage.join_path( ((GraphicsPath*)addingPath)->mPathStorage );
        else
            mPathStorage.concat_path( ((GraphicsPath*)addingPath)->mPathStorage );

        return SetStatus( Ok );
    }

    //AddPie--------------------------------------------------------------------
    Status GraphicsPath::AddPie( const Rect& rect, REAL startAngle, REAL sweepAngle )
    {
        return AddPie( rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle );
    }

    //AddPie--------------------------------------------------------------------
    Status GraphicsPath::AddPie( const RectF& rect, REAL startAngle, REAL sweepAngle )
    {
        return AddPie( rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle );
    }

    //AddPie--------------------------------------------------------------------
    Status GraphicsPath::AddPie( INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle )
    {
        REAL rx = width / 2.0;
        REAL ry = height / 2.0;
        INT  cx = x + rx;
        INT  cy = y + ry;

        // The pie is a closed figure, so the path needs to be closed before it
        // is added:
        CloseAllFigures();

        // Add the arc first:
        agg::bezier_arc arc( cx, cy, rx, ry, agg::deg2rad( startAngle ), agg::deg2rad( sweepAngle ) );
        mPathStorage.concat_path( arc, 0 );

        // Draw a line to the center:
        mPathStorage.line_to( cx, cy );

        // Close the figure:
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddPie--------------------------------------------------------------------
    Status GraphicsPath::AddPie( REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle )
    {
        REAL rx = width / 2.0;
        REAL ry = height / 2.0;
        INT  cx = x + rx;
        INT  cy = y + ry;

        // The pie is a closed figure, so the path needs to be closed before it
        // is added:
        CloseAllFigures();

        // Add the arc first:
        agg::bezier_arc arc( cx, cy, rx, ry, agg::deg2rad( startAngle ), agg::deg2rad( sweepAngle ) );
        mPathStorage.concat_path( arc, 0 );

        // Draw a line to the center:
        mPathStorage.line_to( cx, cy );

        // Close the figure:
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddPolygon----------------------------------------------------------------
    Status GraphicsPath::AddPolygon( const Point* points, INT count )
    {
        if ( !points || count <= 0 )
            return SetStatus( InvalidParameter );

        // Add the first point:
        mPathStorage.move_to( points->X, points->Y );

        // Loop over the points, adding them to the path storage:
        for ( int i = 1; i < count; i++ )
        {
            ++points;
            mPathStorage.line_to( points->X, points->Y );
        }

        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddPolygon----------------------------------------------------------------
    Status GraphicsPath::AddPolygon( const PointF* points, INT count )
    {
        if ( !points || count <= 0 )
            return SetStatus( InvalidParameter );

        // Add the first point:
        mPathStorage.move_to( points->X, points->Y );

        // Loop over the points, adding them to the path storage:
        for ( int i = 1; i < count; i++ )
        {
            ++points;
            mPathStorage.line_to( points->X, points->Y );
        }

        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddRectangle--------------------------------------------------------------
    Status GraphicsPath::AddRectangle( const Rect& rect )
    {
        mPathStorage.move_to( rect.X, rect.Y );
        mPathStorage.hline_rel( rect.Width );
        mPathStorage.vline_rel( rect.Height );
        mPathStorage.hline_rel( -rect.Width );
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddRectangle--------------------------------------------------------------
    Status GraphicsPath::AddRectangle( const RectF& rect )
    {
        mPathStorage.move_to( rect.X, rect.Y );
        mPathStorage.hline_rel( rect.Width );
        mPathStorage.vline_rel( rect.Height );
        mPathStorage.hline_rel( -rect.Width );
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddRectangles-------------------------------------------------------------
    Status GraphicsPath::AddRectangles( const Rect* rects, INT count )
    {
        if ( !rects )
            return SetStatus( InvalidParameter );

        Status status = Ok;
        for ( int i = 0; i < count; i++ )
            status = AddRectangle( rects[ i ] );
        return status;
    }

    //AddRectangles-------------------------------------------------------------
    Status GraphicsPath::AddRectangles( const RectF* rects, INT count )
    {
        if ( !rects )
            return SetStatus( InvalidParameter );

        Status status = Ok;
        for ( int i = 0; i < count; i++ )
            status = AddRectangle( rects[ i ] );
        return status;
    }

    //AddQuadratic--------------------------------------------------------------
    Status GraphicsPath::AddQuadratic( const Point& pt1, const Point& pt2, const Point& pt3 )
    {
        return AddQuadratic( pt1.X, pt1.Y, pt2.X, pt2.Y, pt3.X, pt3.Y );
    }
    Status GraphicsPath::AddQuadratic( const PointF& pt1, const PointF& pt2, const PointF& pt3 )
    {
        return AddQuadratic( pt1.X, pt1.Y, pt2.X, pt2.Y, pt3.X, pt3.Y );
    }
    Status GraphicsPath::AddQuadratic( INT x1, INT y1, INT x2, INT y2, INT x3, INT y3 )
    {
        // Add the first point:
        if ( isPathClosed() )
            mPathStorage.move_to( x1, y1 );
        else
            mPathStorage.line_to( x1, y1 );

        // Add the spline:
        mPathStorage.curve4( x2, y2, x3, y3 );
        return SetStatus( Ok );
    }
    Status GraphicsPath::AddQuadratic( REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3 )
    {
        // Add the first point:
        if ( isPathClosed() )
            mPathStorage.move_to( x1, y1 );
        else
            mPathStorage.line_to( x1, y1 );

        // Add the spline:
        mPathStorage.curve4( x2, y2, x3, y3 );
        return SetStatus( Ok );
    }
    Status GraphicsPath::AddQuadratics( const Point* points, INT count )
    {
        if ( count < 3 )
            return SetStatus( InvalidParameter );

        // Add the first point:
        Point*  curPt       = (Point*)&points[ 0 ];
        int     curIndex    = 0;
        if ( isPathClosed() )
            mPathStorage.move_to( curPt->X, curPt->Y );
        else
            mPathStorage.line_to( curPt->X, curPt->Y );
        ++curPt;
        ++curIndex;

        // Add splines:
        while ( curIndex + 2 <= count )
        {
            mPathStorage.curve3( curPt[ 0 ].X, curPt[ 0 ].Y, curPt[ 1 ].X, curPt[ 1 ].Y );
            curPt += 2;
            curIndex += 2;
        }

        return SetStatus( Ok );
    }
    Status GraphicsPath::AddQuadratics( const PointF* points, INT count )
    {
        if ( count < 3 )
            return SetStatus( InvalidParameter );

        // Add the first point:
        Point*  curPt       = (Point*)&points[ 0 ];
        int     curIndex    = 0;
        if ( isPathClosed() )
            mPathStorage.move_to( curPt->X, curPt->Y );
        else
            mPathStorage.line_to( curPt->X, curPt->Y );
        ++curPt;
        ++curIndex;

        // Add splines:
        while ( curIndex + 2 <= count )
        {
            mPathStorage.curve3( curPt[ 0 ].X, curPt[ 0 ].Y, curPt[ 1 ].X, curPt[ 1 ].Y );
            curPt += 2;
            curIndex += 2;
        }

        return SetStatus( Ok );
    }

    //AddBezier-----------------------------------------------------------------
    Status GraphicsPath::AddBezier( const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4 )
    {
        return AddBezier( pt1.X, pt1.Y, pt2.X, pt2.Y, pt3.X, pt3.Y, pt4.X, pt4.Y );
    }

    Status GraphicsPath::AddBezier( const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4 )
    {
        return AddBezier( pt1.X, pt1.Y, pt2.X, pt2.Y, pt3.X, pt3.Y, pt4.X, pt4.Y );
    }

    Status GraphicsPath::AddBezier( INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4 )
    {
        // Add the first point:
        if ( isPathClosed() )
            mPathStorage.move_to( x1, y1 );
        else
            mPathStorage.line_to( x1, y1 );

        // Add the spline:
        mPathStorage.curve4( x2, y2, x3, y3, x4, y4 );
        return SetStatus( Ok );
    }

    Status GraphicsPath::AddBezier( REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4 )
    {
        // Add the first point:
        if ( isPathClosed() )
            mPathStorage.move_to( x1, y1 );
        else
            mPathStorage.line_to( x1, y1 );

        // Add the spline:
        mPathStorage.curve4( x2, y2, x3, y3, x4, y4 );
        return SetStatus( Ok );
    }

    //AddBeziers----------------------------------------------------------------
    Status GraphicsPath::AddBeziers( const Point* points, INT count )
    {
        if ( count < 4 )
            return SetStatus( InvalidParameter );

        // Add the first point:
        Point*  curPt       = (Point*)&points[ 0 ];
        int     curIndex    = 0;
        if ( isPathClosed() )
            mPathStorage.move_to( curPt->X, curPt->Y );
        else
            mPathStorage.line_to( curPt->X, curPt->Y );
        ++curPt;
        ++curIndex;

        // Add splines:
        while ( curIndex + 3 <= count )
        {
            mPathStorage.curve4( curPt[ 0 ].X, curPt[ 0 ].Y, curPt[ 1 ].X, curPt[ 1 ].Y, curPt[ 2 ].X, curPt[ 2 ].Y );
            curPt += 3;
            curIndex += 3;
        }

        return SetStatus( Ok );
    }

    Status GraphicsPath::AddBeziers( const PointF* points, INT count )
    {
        if ( count < 4 )
            return SetStatus( InvalidParameter );

        // Add the first point:
        PointF* curPt       = (PointF*)&points[ 0 ];
        int     curIndex    = 0;
        if ( isPathClosed() )
            mPathStorage.move_to( curPt->X, curPt->Y );
        else
            mPathStorage.line_to( curPt->X, curPt->Y );
        ++curPt;
        ++curIndex;

        // Add splines:
        while ( curIndex + 3 <= count )
        {
            mPathStorage.curve4( curPt[ 0 ].X, curPt[ 0 ].Y, curPt[ 1 ].X, curPt[ 1 ].Y, curPt[ 2 ].X, curPt[ 2 ].Y );
            curPt += 3;
            curIndex += 3;
        }

        return SetStatus( Ok );
    }

    //AddClosedCurve------------------------------------------------------------
    Status GraphicsPath::AddClosedCurve( const Point* points, INT count )
    {
        if ( !points || count <= 0 )
            return SetStatus( InvalidParameter );

        // Add the first point:
        agg::path_storage poly;
        poly.move_to( points[ 0 ].X, points[ 0 ].Y );

        // Loop over the points, adding them to the path storage:
        for ( int i = 1; i < count; i++ )
            poly.line_to( points[ i ].X, points[ i ].Y );

        poly.close_polygon();

        typedef agg::conv_bspline< agg::path_storage > conv_bspline_type;
        conv_bspline_type bspline( poly );
        bspline.interpolation_step( 1.0 / (double)( 20.0 ) );

        typedef agg::conv_stroke< conv_bspline_type > conv_stroke_type;
        conv_stroke_type stroke( bspline );

        // Add the spline to the path:
        mPathStorage.concat_path( stroke, 0 );
        return SetStatus( Ok );
    }

    Status GraphicsPath::AddClosedCurve( const PointF* points, INT count )
    {
        if ( !points || count <= 0 )
            return SetStatus( InvalidParameter );

        // Add the first point:
        agg::path_storage poly;
        poly.move_to( points[ 0 ].X, points[ 0 ].Y );

        // Loop over the points, adding them to the path storage:
        for ( int i = 1; i < count; i++ )
            poly.line_to( points[ i ].X, points[ i ].Y );

        poly.close_polygon();

        typedef agg::conv_bspline< agg::path_storage > conv_bspline_type;
        conv_bspline_type bspline( poly );
        bspline.interpolation_step( 1.0 / (double)( 20.0 ) );

        typedef agg::conv_stroke< conv_bspline_type > conv_stroke_type;
        conv_stroke_type stroke( bspline );

        // Add the spline to the path:
        mPathStorage.concat_path( stroke, 0 );
        return SetStatus( Ok );
    }

    //AddClosedCurve------------------------------------------------------------
    Status GraphicsPath::AddCurve( const Point* points, INT count )
    {
        if ( !points || count <= 0 )
            return SetStatus( InvalidParameter );

        // Add the first point:
        agg::path_storage poly;
        poly.move_to( points[ 0 ].X, points[ 0 ].Y );

        // Loop over the points, adding them to the path storage:
        for ( int i = 1; i < count; i++ )
            poly.line_to( points[ i ].X, points[ i ].Y );

        typedef agg::conv_bspline< agg::path_storage > conv_bspline_type;
        conv_bspline_type bspline( poly );
        bspline.interpolation_step( 1.0 / (double)( 20.0 ) );

        typedef agg::conv_stroke< conv_bspline_type > conv_stroke_type;
        conv_stroke_type stroke( bspline );

        // Add the spline to the path:
        mPathStorage.concat_path( stroke, 0 );
        return SetStatus( Ok );
    }

    Status GraphicsPath::AddCurve( const PointF* points, INT count )
    {
        if ( !points || count <= 0 )
            return SetStatus( InvalidParameter );

        // Add the first point:
        agg::path_storage poly;
        poly.move_to( points[ 0 ].X, points[ 0 ].Y );

        // Loop over the points, adding them to the path storage:
        for ( int i = 1; i < count; i++ )
            poly.line_to( points[ i ].X, points[ i ].Y );

        typedef agg::conv_bspline< agg::path_storage > conv_bspline_type;
        conv_bspline_type bspline( poly );
        bspline.interpolation_step( 1.0 / (double)( 20.0 ) );

        typedef agg::conv_stroke< conv_bspline_type > conv_stroke_type;
        conv_stroke_type stroke( bspline );

        // Add the spline to the path:
        mPathStorage.concat_path( stroke, 0 );
        return SetStatus( Ok );
    }

    //Status GraphicsPath::AddCurve( const Point* points, INT count, INT offset, INT numberOfSegments, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //Status GraphicsPath::AddCurve( const PointF* points, INT count, INT offset, INT numberOfSegments, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //Status GraphicsPath::AddCurve( const Point* points, INT count, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //Status GraphicsPath::AddCurve( const PointF* points, INT count, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //Transform-----------------------------------------------------------------
    Status GraphicsPath::Transform( const Matrix* matrix )
    {
        if ( matrix )
        {
            agg::path_storage path2( mPathStorage );
            agg::conv_transform< agg::path_storage > trans( path2, matrix->mMatrix );

            // Remove all points from the current path:
            mPathStorage.remove_all();
            mPathStorage.concat_path( trans );
        }
        return SetStatus( Ok );
    }

} // namespace AggOO
