
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
// $Id: AggOOGraphicsPathExt.cpp,v 1.11 2009/02/26 22:13:38 dratek Exp $
//
// AggOOGraphicsPathExt.cpp
//

#include "AggOOGraphicsPathExt.h"
#include "agg_rounded_rect.h"
#include "AggOOMath.h"
//#include "AggOOVectorShapeData.h"

//#include <cmd/LogFileManager.h>

namespace AggOO
{
    GraphicsPathExt::GraphicsPathExt( FillMode fillMode )
        : GraphicsPath( fillMode )
    {
    }

    GraphicsPathExt::GraphicsPathExt( const Point* points, const BYTE* types, INT count, FillMode fillMode )
        : GraphicsPath( points, types, count, fillMode )
    {
    }

    GraphicsPathExt::GraphicsPathExt( const PointF* points, const BYTE* types, INT count, FillMode fillMode )
        : GraphicsPath( points, types, count, fillMode )
    {
    }

    //GraphicsPathExt::~GraphicsPathExt(void)
    //{
    //}

    //AddRoundRectangle---------------------------------------------------------
    Status GraphicsPathExt::AddRoundRectangle( const Rect& rect, REAL radius )
    {
        // Make sure the path is closed:
        CloseAllFigures();

        // Add the rounded rect:
        agg::rounded_rect r( rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height, radius );
        r.approximation_scale( 100 );
        mPathStorage.concat_path( r, 0 );

        // Close the rect:
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    Status GraphicsPathExt::AddRoundRectangle( const RectF& rect, REAL radius )
    {
        // Make sure the path is closed:
        CloseAllFigures();

        // Add the rounded rect:
        agg::rounded_rect r( rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height, radius );
        r.approximation_scale( 100 );
        mPathStorage.concat_path( r, 0 );

        // Close the rect:
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddRegularPolygon---------------------------------------------------------
    Status GraphicsPathExt::AddRegularPolygon( INT numPoints, const Point& center, INT radius )
    {
        // Close the path initially
        CloseAllFigures();

        // Generate and add the star's vertices
        REAL vertAngle  = two_pi / REAL( numPoints );
        REAL curAngle   = 0.0;

        // Start with the first point...
        mPathStorage.move_to( center.X + radius, center.Y );
        curAngle += vertAngle;

        // Generate the remaining vertices:
        for ( int i = 1; i < numPoints; i++ )
        {
            // Get the vertex coordinate:
            REAL x, y;
            PolarToRect( curAngle, radius, &x, &y, center.X, center.Y );

            mPathStorage.line_to( x, y );
            curAngle += vertAngle;
        }

        // Close the path:
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    Status GraphicsPathExt::AddRegularPolygon( INT numPoints, const PointF& center, INT radius )
    {
        // Close the path initially
        CloseAllFigures();

        // Generate and add the star's vertices
        REAL vertAngle  = two_pi / REAL( numPoints );
        REAL curAngle   = 0.0;

        // Start with the first point...
        mPathStorage.move_to( center.X + radius, center.Y );
        curAngle += vertAngle;

        // Generate the remaining vertices:
        for ( int i = 1; i < numPoints; i++ )
        {
            // Get the vertex coordinate:
            REAL x, y;
            PolarToRect( curAngle, radius, &x, &y, center.X, center.Y );

            mPathStorage.line_to( x, y );
            curAngle += vertAngle;
        }

        // Close the path:
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddStar-------------------------------------------------------------------
    Status GraphicsPathExt::AddStar( INT numPoints, const Point& center, INT radius1, INT radius2 )
    {
        // Close the path initially
        CloseAllFigures();

        // Generate and add the star's vertices
        INT totalPoints = numPoints * 2;
        REAL vertAngle  = two_pi / REAL( totalPoints );
        REAL curAngle   = 0.0;

        // Start with the first point...
        mPathStorage.move_to( center.X + radius1, center.Y );
        curAngle += vertAngle;

        // Generate the remaining vertices:
        INT radius[] = { radius1, radius2 };
        for ( int i = 1; i < totalPoints; i++ )
        {
            // Get the vertex coordinate:
            REAL x, y;
            PolarToRect( curAngle, radius[ i % 2 ], &x, &y, center.X, center.Y );

            mPathStorage.line_to( x, y );
            curAngle += vertAngle;
        }

        // Close the path:
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    Status GraphicsPathExt::AddStar( INT numPoints, const PointF& center, REAL radius1, INT radius2 )
    {
        // Close the path initially
        CloseAllFigures();

        // Generate and add the star's vertices
        INT totalPoints = numPoints * 2;
        REAL vertAngle  = two_pi / REAL( totalPoints );
        REAL curAngle   = 0.0;

        // Start with the first point...
        mPathStorage.move_to( center.X + radius1, center.Y );
        curAngle += vertAngle;

        // Generate the remaining vertices:
        REAL radius[] = { radius1, radius2 };
        for ( int i = 1; i < totalPoints; i++ )
        {
            // Get the vertex coordinate:
            REAL x, y;
            PolarToRect( curAngle, (REAL)radius[ i % 2 ], &x, &y, center.X, center.Y );

            mPathStorage.line_to( x, y );
            curAngle += vertAngle;
        }

        // Close the path:
        mPathStorage.close_polygon();

        return SetStatus( Ok );
    }

    //AddLineStrip--------------------------------------------------------------
    Status GraphicsPathExt::AddLineStrip( const Point* points, INT count )
    {
        if ( !points || count < 1 )
            return SetStatus( InvalidParameter );

        CloseAllFigures();
        Status status = Ok;
        INT curPt = 0;

        mPathStorage.move_to( points[ 0 ].X, points[ 0 ].Y );

        for ( int i = 1; i < count; i++ )
        {
            mPathStorage.line_to( points[ i ].X, points[ i ].Y );
        }

        return status;
    }

    Status GraphicsPathExt::AddLineStrip( const PointF* points, INT count )
    {
        if ( !points || count < 1 )
            return SetStatus( InvalidParameter );

        CloseAllFigures();
        Status status = Ok;
        INT curPt = 0;

        mPathStorage.move_to( points[ 0 ].X, points[ 0 ].Y );

        for ( int i = 1; i < count; i++ )
        {
            mPathStorage.line_to( points[ i ].X, points[ i ].Y );
        }

        return status;
    }

    ////AddVectorShape----------------------------------------------------------------
    //Status GraphicsPathExt::AddVectorShape( const vs::VectorShapeData& shape )
    //{
    //    CloseAllFigures();
    //    Status status = Ok;

    //    std::vector< vs::VectorSubPath >::const_iterator pathIter = shape.mPaths.begin();
    //    while ( pathIter != shape.mPaths.end() && status == Ok )
    //    {
    //        INT vertCount = pathIter->GetVertexCount();
    //        std::vector< PointF > transVerts( pathIter->mVertices.begin(), pathIter->mVertices.end() );
    //        Matrix shapeMtx;
    //        status = shape.GetTransform( &shapeMtx );
    //        status = shapeMtx.TransformPoints( &transVerts[ 0 ], transVerts.size() );
    //        if ( pathIter->IsClosed() )
    //            status = AddPolygon( &transVerts[ 0 ], transVerts.size() );
    //            //status = AddPolygon( &pathIter->mVertices[ 0 ], vertCount );
    //        else
    //            status = AddLines( &transVerts[ 0 ], transVerts.size() );
    //            //status = AddLines( &pathIter->mVertices[ 0 ], vertCount );
    //        pathIter++;
    //    }
    //    //if ( status == Ok )
    //    //{
    //    //    Matrix shapeMtx;
    //    //    shape.GetTransform( &shapeMtx );
    //    //    status = Transform( &shapeMtx );
    //    //}

    //    return status;
    //}

    ////AddVectorShape----------------------------------------------------------------
    //Status GraphicsPathExt::AddVectorShape( const vs::VectorShapeData* shapes, INT count )
    //{
    //    if ( !shapes || count < 1 )
    //        return SetStatus( InvalidParameter );

    //    Status status = Ok;
    //    INT curShape = count;
    //    while ( curShape-- )
    //    {
    //        if ( ( status = AddVectorShape( shapes[ curShape ] ) ) != Ok )
    //            return status;
    //    }
    //    return status;
    //}

    //MoveTo--------------------------------------------------------------------
    Status GraphicsPathExt::MoveTo( REAL x, REAL y )
    {
        mPathStorage.move_to( x, y );
        return Ok;
    }

    //MoveToRel-----------------------------------------------------------------
    Status GraphicsPathExt::MoveToRel( REAL dx, REAL dy )
    {
        mPathStorage.move_rel( dx, dy );
        return Ok;
    }

    //LineTo--------------------------------------------------------------------
    Status GraphicsPathExt::LineTo( REAL x, REAL y )
    {
        mPathStorage.line_to( x, y );
        return Ok;
    }

    //LineToRel-----------------------------------------------------------------
    Status GraphicsPathExt::LineToRel( REAL dx, REAL dy )
    {
        mPathStorage.line_rel( dx, dy );
        return Ok;
    }

    //HLineTo-------------------------------------------------------------------
    Status GraphicsPathExt::HLineTo( REAL x )
    {
        mPathStorage.hline_to( x );
        return Ok;
    }

    //HLineToRel----------------------------------------------------------------
    Status GraphicsPathExt::HLineToRel( REAL dx )
    {
        mPathStorage.hline_rel( dx );
        return Ok;
    }

    //VLineTo-------------------------------------------------------------------
    Status GraphicsPathExt::VLineTo( REAL y )
    {
        mPathStorage.vline_to( y );
        return Ok;
    }

    //VLineToRel----------------------------------------------------------------
    Status GraphicsPathExt::VLineToRel( REAL dy )
    {
        mPathStorage.vline_rel( dy );
        return Ok;
    }

    //ArcTo---------------------------------------------------------------------
    Status GraphicsPathExt::ArcTo( REAL rx, REAL ry, REAL angle, bool largeArc, bool sweepFlag, REAL x, REAL y )
    {
        mPathStorage.arc_to( rx, ry, angle, largeArc, sweepFlag, x, y );
        return Ok;
    }

    //ArcToRel------------------------------------------------------------------
    Status GraphicsPathExt::ArcToRel( REAL rx, REAL ry, REAL angle, bool largeArc, bool sweepFlag, REAL dx, REAL dy )
    {
        mPathStorage.arc_rel( rx, ry, angle, largeArc, sweepFlag, dx, dy );
        return Ok;
    }

    //Curve3To------------------------------------------------------------------
    Status GraphicsPathExt::Curve3To( REAL xCtrl, REAL yCtrl, REAL x, REAL y )
    {
        mPathStorage.curve3( xCtrl, yCtrl, x, y );
        return Ok;
    }

    //Curve3ToRel---------------------------------------------------------------
    Status GraphicsPathExt::Curve3ToRel( REAL xCtrl, REAL yCtrl, REAL dx, REAL dy )
    {
        mPathStorage.curve3_rel( xCtrl, yCtrl, dx, dy );
        return Ok;
    }

    //Curve3To------------------------------------------------------------------
    Status GraphicsPathExt::Curve3To( REAL x, REAL y )
    {
        mPathStorage.curve3( x, y );
        return Ok;
    }

    //Curve3ToRel---------------------------------------------------------------
    Status GraphicsPathExt::Curve3ToRel( REAL dx, REAL dy )
    {
        mPathStorage.curve3_rel( dx, dy );
        return Ok;
    }

    //Curve4To------------------------------------------------------------------
    Status GraphicsPathExt::Curve4To( REAL xCtrl1, REAL yCtrl1, REAL xCtrl2, REAL yCtrl2, REAL x, REAL y )
    {
        mPathStorage.curve4( xCtrl1, yCtrl1, xCtrl1, yCtrl1, x, y );
        return Ok;
    }

    //Curve4ToRel---------------------------------------------------------------
    Status GraphicsPathExt::Curve4ToRel( REAL xCtrl1, REAL yCtrl1, REAL xCtrl2, REAL yCtrl2, REAL dx, REAL dy )
    {
        mPathStorage.curve4_rel( xCtrl1, yCtrl1, xCtrl1, yCtrl1, dx, dy );
        return Ok;
    }

    //Curve4To------------------------------------------------------------------
    Status GraphicsPathExt::Curve4To( REAL xCtrl2, REAL yCtrl2, REAL x, REAL y )
    {
        mPathStorage.curve4( xCtrl2, yCtrl2, x, y );
        return Ok;
    }

    //Curve4ToRel---------------------------------------------------------------
    Status GraphicsPathExt::Curve4ToRel( REAL xCtrl2, REAL yCtrl2, REAL dx, REAL dy )
    {
        mPathStorage.curve4_rel( xCtrl2, yCtrl2, dx, dy );
        return Ok;
    }

    //StartNewPath--------------------------------------------------------------
    Status GraphicsPathExt::StartNewPath()
    {
        mPathStorage.start_new_path();
        return Ok;
    }

    //EndPolygon----------------------------------------------------------------
    Status GraphicsPathExt::EndPolygon()
    {
        mPathStorage.end_poly();
        return Ok;
    }

    //ClosePolygon--------------------------------------------------------------
    Status GraphicsPathExt::ClosePolygon()
    {
        mPathStorage.close_polygon();
        return Ok;
    }

} // namespace AggOO
