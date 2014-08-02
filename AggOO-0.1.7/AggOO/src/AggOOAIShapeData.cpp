
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
// $Id: AggOOAIShapeData.cpp,v 1.2 2007/09/06 21:50:43 dratek Exp $
//
// AggOOAIShapeData.cpp
//

#include "AggOOAIShapeData.h"
#include "AggOOColor.h"
#include "AggOOMatrix.h"

namespace AggOO
{
    namespace ai
    {
        AIShapeData::AIShapeData(void)
            : Base()
        {
            Reset();
            SetStatus( Ok );
        }

        //Reset-----------------------------------------------------------------
        Status AIShapeData::Reset()
        {
            mVertices.clear();

            mShapeIndex     = -1;
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

        //AddVertex-------------------------------------------------------------
        Status AIShapeData::AddVertex( REAL x, REAL y )
        {
            try
            {
                mVertices.push_back( PointF( x, y ) );
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //AddVertex-------------------------------------------------------------
        Status AIShapeData::AddVertex( const PointF& pt )
        {
            try
            {
                mVertices.push_back( pt );
                return SetStatus( Ok );
            }
            catch ( std::exception& e )
            {
                return SetStatus( GenericError );
            }
        }

        //GetVertex-------------------------------------------------------------
        Status AIShapeData::GetVertex( size_t idx, PointF* pt ) const
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
        Status AIShapeData::SetVertex( size_t idx, const PointF& pt )
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

        //GetBoundingRect-------------------------------------------------------
        Status AIShapeData::GetBoundingRect( RectF* bounds )
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

        //Normalize-------------------------------------------------------------
        Status AIShapeData::Normalize()
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

        //Normalize-------------------------------------------------------------
        Status AIShapeData::Normalize( const RectF& bounds )
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

    } // namespace ai
} // namespace AggOO
