
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


/*! $Id: AggOOVectorShapeData.cpp,v 1.4 2008/08/13 22:08:31 dratek Exp $
 *  @file   AggOOVectorShapeData.cpp
 *  @author Chad M. Draper
 *  @date   November 26, 2007
 *  
 */

#include "AggOOVectorShapeData.h"

//#include <cmd/LogFileManager.h>

namespace AggOO
{
    namespace vs
    {
        //
        // VectorSubPath
        //

        VectorSubPath::VectorSubPath()
        {
            Reset();
        }

        //Reset-----------------------------------------------------------------
        Status VectorSubPath::Reset()
        {
            mNormalizationFactor = 1.0;
            mIsClosed = false;
            mVertices.clear();
            return SetStatus( Ok );
        }

        //AddVertex-------------------------------------------------------------
        Status VectorSubPath::AddVertex( REAL x, REAL y )
        {
            mVertices.push_back( PointF( x, y ) );
            return SetStatus( Ok );
        }

        //AddVertex-------------------------------------------------------------
        Status VectorSubPath::AddVertex( const PointF& pt )
        {
            mVertices.push_back( pt );
            return SetStatus( Ok );
        }

        //GetVertex-------------------------------------------------------------
        Status VectorSubPath::GetVertex( size_t idx, PointF* pt ) const
        {
            if ( idx >= mVertices.size() || !pt )   return SetStatus( InvalidParameter );
            try
            {
                *pt = mVertices.at( idx );
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //SetVertex-------------------------------------------------------------
        Status VectorSubPath::SetVertex( size_t idx, const PointF& pt )
        {
            if ( idx >= mVertices.size() )   return SetStatus( InvalidParameter );
            try
            {
                mVertices[ idx ] = pt;
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //TransformVertices-----------------------------------------------------
        Status VectorSubPath::TransformVertices( Matrix* matrix )
        {
            return Ok;
            Status status = matrix->TransformPoints( &mVertices[ 0 ], mVertices.size() );
            return status;
        }

        /** Calculate the bounding rect for the shape */
        Status VectorSubPath::GetBoundingRect( RectF* bounds ) const
        {
            if ( !bounds )  return SetStatus( InvalidParameter );
            try
            {
                size_t vertIdx = 0, numVerts = GetVertexCount();
                PointF topLeft, bottomRight;
                Status status = GetVertex( vertIdx++, &topLeft );
                if ( status != Ok ) return SetStatus( status );
                bottomRight = topLeft;
                while ( vertIdx < numVerts )
                {
                    PointF pt;
                    if ( ( status = GetVertex( vertIdx++, &pt ) ) != Ok )
                        return SetStatus( status );
                    if ( pt.X < topLeft.X )             topLeft.X = pt.X;
                    else if ( pt.X > bottomRight.X )    bottomRight.X = pt.X;

                    if ( pt.Y < topLeft.Y )             topLeft.Y = pt.Y;
                    else if ( pt.Y > bottomRight.Y )    bottomRight.Y = pt.Y;
                }
                *bounds = RectF( topLeft.X, topLeft.Y, bottomRight.X - topLeft.X, bottomRight.Y - topLeft.Y );
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        /** Normalize the shape by scaling the vertices so that the shape radius
            is 1, and centered at 0,0.
        */
        Status VectorSubPath::Normalize()
        {
            try
            {
                // First, find the bounding rect
                RectF bounds;
                Status status = GetBoundingRect( &bounds );
                if ( status != Ok ) return SetStatus( status );

                // Center:
                PointF center( bounds.X + bounds.Width / 2.0, bounds.Y + bounds.Height / 2.0 );

                // Center the vertices around 0,0.  In the process, find the vertex
                // that is furthest from the center (actually, we just need the
                // distance.)
                REAL maxDist = 0;
                PointF origin( 0, 0 );
                std::vector< PointF >::iterator iter;
                for ( iter = mVertices.begin(); iter != mVertices.end(); iter++ )
                {
                    *iter -= center;

                    REAL dist = PointExtF::Distance( *iter, origin );
                    if ( maxDist < dist )
                        maxDist = dist;
                }

                // Calculate the scale:
                mNormalizationFactor = 1.0 / maxDist;

                // Scale the vertices:
                for ( iter = mVertices.begin(); iter != mVertices.end(); iter++ )
                {
                    *iter *= mNormalizationFactor;
                }
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        /** Normalize a shape based on a bounding rect.
            @param  bounds          Bounding rect.  The shape will be centered in this rect, and scaled based on the rect's radius.
        */
        Status VectorSubPath::Normalize( const RectF& bounds )
        {
            try
            {
                // Center:
                PointF center( bounds.X + bounds.Width / 2.0, bounds.Y + bounds.Height / 2.0 );

                // Bounds radius:
                REAL radius = Sqrt( Sqr( bounds.Width / 2.0 ) + Sqr( bounds.Height / 2.0 ) );

                // Calculate the scale:
                mNormalizationFactor = 1.0 / radius;

                // Scale the vertices:
                std::vector< PointF >::iterator iter;
                for ( iter = mVertices.begin(); iter != mVertices.end(); iter++ )
                {
                    *iter = ( *iter - center ) * mNormalizationFactor;
                }
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }


        //
        // VectorShapeData
        //

        VectorShapeData::VectorShapeData(void)
            : Base()
        {
            Reset();
            SetStatus( Ok );
        }

        //VectorShapeData::VectorShapeData( const path_attributes& attr )
        //    : Base()
        //{
        //    Reset();
        //    mAttr = attr;
        //    SetStatus( Ok );
        //}

        //Reset-----------------------------------------------------------------
        Status VectorShapeData::Reset()
        {
            mPaths.clear();
            //mVertices.clear();
            //path_attributes newAttr;
            //mAttr = newAttr;

            mFillColor      = Color::White;
            mStrokeColor    = Color::Gray;
            mStrokeWidth    = 1.0;
            mFillFlag       = false;
            mEvenOddFlag    = true;
            mStrokeFlag     = false;
            mLineJoin       = LineJoinRound;
            mLineCap        = LineCapRound;
            mMiterLimit     = 1.0;
            mNormalizationFactor = 1.0;
            mMatrix.Reset();

            return SetStatus( Ok );
        }

        //GetVertexCount--------------------------------------------------------
        size_t VectorShapeData::GetVertexCount() const
        {
            size_t vertCount = 0;
            std::vector< VectorSubPath >::const_iterator iter = mPaths.begin();
            while ( iter != mPaths.end() )
            {
                vertCount += iter->GetVertexCount();
                iter++;
            }
            return vertCount;
        }


        //AddVertex-------------------------------------------------------------
        Status VectorShapeData::AddVertex( REAL x, REAL y, size_t shapeIdx )
        {
            try
            {
                mPaths.at( shapeIdx ).AddVertex( PointF( x, y ) );
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //AddVertex-------------------------------------------------------------
        Status VectorShapeData::AddVertex( const PointF& pt, size_t shapeIdx )
        {
            try
            {
                mPaths.at( shapeIdx ).AddVertex( pt );
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //GetVertex-------------------------------------------------------------
        Status VectorShapeData::GetVertex( size_t idx, PointF* pt, size_t shapeIdx ) const
        {
            if ( !pt )   return SetStatus( InvalidParameter );
            try
            {
                mPaths.at( shapeIdx ).GetVertex( idx, pt );
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //SetVertex-------------------------------------------------------------
        Status VectorShapeData::SetVertex( size_t idx, const PointF& pt, size_t shapeIdx )
        {
            try
            {
                mPaths.at( shapeIdx ).SetVertex( idx, pt );
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //TransformVertices-----------------------------------------------------
        Status VectorShapeData::TransformVertices()
        {
            Status status = Ok;
return status;
            if ( !mMatrix.IsIdentity() )
            {
                std::vector< VectorSubPath >::iterator pathIter = mPaths.begin();
                while ( pathIter != mPaths.end() )
                {
                    // Sometimes, the shape has a transformation matrix assigned to
                    // it, which can cause problems when normalizing the shapes.
                    // (The vertices get normalized, but and translation is still
                    // in effect, so the points are farther apart.)  What we will
                    // do, is apply the transformation before normalizing the points,
                    // then reset the shape matrix.
                    status = pathIter->TransformVertices( &mMatrix );
                    if ( status != Ok )
                        return status;
                    pathIter++;
                }
                mMatrix.Reset();
            }
            return status;
        }

        //GetBoundingRect-------------------------------------------------------
        Status VectorShapeData::GetBoundingRect( RectF* bounds ) 
        {
            if ( !bounds )  return SetStatus( InvalidParameter );
            try
            {
                TransformVertices();

                std::vector< VectorSubPath >::const_iterator iter = mPaths.begin();
                *bounds = RectF( 0, 0, 0, 0 );
                if ( iter != mPaths.end() )
                {
                    iter->GetBoundingRect( bounds );
                    iter++;
                }
                while ( iter != mPaths.end() )
                {
                    RectF subBounds;
                    iter->GetBoundingRect( &subBounds );

                    bounds->Union( *bounds, *bounds, subBounds );
                    iter++;
                }

                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //Normalize-------------------------------------------------------------
        Status VectorShapeData::Normalize()
        {
            try
            {
                // Before calculating the bounding rect, transform all vertices
                // by the shape matrix (and reset the shape matrix.)  This will
                // prevent problems with translation.
                TransformVertices();

                // First, find the bounding rect
                RectF bounds;
                Status status = GetBoundingRect( &bounds );
                if ( status != Ok ) return SetStatus( status );

                // Center:
                PointF center( bounds.X + bounds.Width / 2.0, bounds.Y + bounds.Height / 2.0 );

                // Center the vertices around 0,0.  In the process, find the vertex
                // that is furthest from the center (actually, we just need the
                // distance.)
                REAL maxDist = 0;
                PointF origin( 0, 0 );
                std::vector< VectorSubPath >::iterator pathIter = mPaths.begin();
                while ( pathIter != mPaths.end() )
                {
                    // Find the distance to each of the vertices:
                    std::vector< PointF >::iterator vertIter = pathIter->mVertices.begin();
                    while ( vertIter != pathIter->mVertices.end() )
                    {
                        *vertIter -= center;

                        REAL dist = PointExtF::Distance( *vertIter, origin );
                        if ( maxDist < dist )
                            maxDist = dist;
                        vertIter++;
                    }
                    pathIter++;
                }

                //std::vector< PointF >::iterator iter;
                //for ( iter = mVertices.begin(); iter != mVertices.end(); iter++ )
                //{
                //    *iter -= center;

                //    REAL dist = PointExtF::Distance( *iter, origin );
                //    if ( maxDist < dist )
                //        maxDist = dist;
                //}

                // Calculate the scale:
                mNormalizationFactor = 1.0 / maxDist;

                // Scale the vertices:
                pathIter = mPaths.begin();
                while ( pathIter != mPaths.end() )
                {
                    // Find the distance to each of the vertices:
                    std::vector< PointF >::iterator vertIter = pathIter->mVertices.begin();
                    while ( vertIter != pathIter->mVertices.end() )
                    {
                        *vertIter *= mNormalizationFactor;
                        vertIter++;
                    }
                    pathIter++;
                }
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //Normalize-------------------------------------------------------------
        Status VectorShapeData::Normalize( const RectF& bounds )
        {
            try
            {
                // Center:
                PointF center( bounds.X + bounds.Width / 2.0, bounds.Y + bounds.Height / 2.0 );

                // Bounds radius:
                REAL radius = Sqrt( Sqr( bounds.Width / 2.0 ) + Sqr( bounds.Height / 2.0 ) );

                // Calculate the scale:
                mNormalizationFactor = 1.0 / radius;

                // Scale the vertices:
                std::vector< VectorSubPath >::iterator iter = mPaths.begin();
                while ( iter != mPaths.end() )
                {
                    // Sometimes, the shape has a transformation matrix assigned to
                    // it, which can cause problems when normalizing the shapes.
                    // (The vertices get normalized, but and translation is still
                    // in effect, so the points are farther apart.)  What we will
                    // do, is apply the transformation before normalizing the points,
                    // then reset the shape matrix.
                    iter->TransformVertices( &mMatrix );

                    iter->Normalize( bounds );
                    iter++;
                }

                // Reset the matrix:
                mMatrix.Reset();

                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //AddPath---------------------------------------------------------------
        Status VectorShapeData::AddPath( const VectorSubPath& path )
        {
            mPaths.push_back( path );
            return SetStatus( Ok );
        }
    } // namespace vs
} // namespace AggOO
