
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
// $Id: AggOOSVGShapeData.h,v 1.4 2008/01/10 18:02:10 dratek Exp $
//
// AggOOSVGShapeData.h -- Header file for the SVGShapeData class
//
// Author       : Chad M. Draper
// Date         : May 1, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2007-05-01      Initial implementation
//

#ifndef AggOOSVGShapeData_H_
#define AggOOSVGShapeData_H_

#include "AggOOBase.h"
#include "AggOOTypes.h"
#include "AggOOMath.h"
#include "AggOOEnums.h"
#include "AggOOColor.h"
#include "AggOOMatrix.h"
#include "svg/agg_svg_path_renderer.h"

#include <vector>

namespace AggOO
{
    namespace svg
    {
        /** @class SVGSubPath
            The Shape will consist of at least 1 sub path, but may contain more.
            The sub-path contains the vertices for the shape, while the shape
            itself contains the rest of the settings/information.
        */
        class _AggOOExport SVGSubPath : public Base
        {
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

        public:
            std::vector< PointF > mVertices;    /**< Vertices which define the path */
            REAL        mNormalizationFactor;   /**< Scale factor used to normalize the shape.  This may be needed to preserve stroke width ratios. */

            /** Constructor */
            SVGSubPath();

            /** Reset the shape */
            Status Reset();

            /** Assigment operator */
            SVGSubPath& operator=( const SVGSubPath& src )
            {
                mVertices.assign( src.mVertices.begin(), src.mVertices.end() );
                return *this;
            }

            /** Get the vertex count */
            size_t GetVertexCount() const
            {
                return mVertices.size();
            }

            /** Add a vertex to the shape
                @note   The vertex is added at the end of the vertex array.
            */
            Status AddVertex( REAL x, REAL y );
            Status AddVertex( const PointF& pt );

            /** Get a specified vertex */
            Status GetVertex( size_t idx, PointF* pt ) const;

            /** Set the vertex at a specified point.  This requires that there
                be a vertex at the index, and that vertex will be replaced.
            */
            Status SetVertex( size_t idx, const PointF& pt );

            /** Calculate the bounding rect for the shape */
            Status GetBoundingRect( RectF* bounds ) const;

            /** Normalize the shape by scaling the vertices so that the shape radius
                is 1, and centered at 0,0.
            */
            Status Normalize();

            /** Normalize a shape based on a bounding rect.
                @param  bounds          Bounding rect.  The shape will be centered in this rect, and scaled based on the rect's radius.
            */
            Status Normalize( const RectF& bounds );
        };

        /** @class SVGShapeData
            The SVGShapeData class defines a single shape stored in an SVG file.

            @note   The shape is stored as a polygon.
        */
        class _AggOOExport SVGShapeData : public Base
        {
        private:
            std::vector< SVGSubPath > mPaths;
//            std::vector< PointF >   mVertices;  /**< Vertices which define the shape */
            agg::svg::path_attributes mAttr;    /**< Attributes for the shape (color, fill mode, etc.) */

            Color       mFillColor;             /**< Fill color of the shape */
            Color       mStrokeColor;           /**< Outline color of the shape */
            REAL        mStrokeWidth;           /**< Width of the outline */
            bool        mFillFlag;              /**< Indicates if the shape is filled */
            bool        mEvenOddFlag;           /**< Indicates if the fill mode is even-odd, or alternate */
            bool        mStrokeFlag;            /**< Indicates if the outline is drawn */
            LineJoin    mLineJoin;              /**< Indicates how the lines making up the outline are joined */
            LineCap     mLineCap;               /**< Indicates how the outline segments are capped */
            REAL        mMiterLimit;            /**< Miter limit for the outline segments */
            Matrix      mMatrix;                /**< Transformation matrix for the shape */

            REAL        mNormalizationFactor;   /**< Scale factor used to normalize the shape.  This may be needed to preserve stroke width ratios. */

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

        public:
            friend class Graphics;
            friend class GraphicsExt;
            friend class GraphicsPath;
            friend class GraphicsPathExt;

            SVGShapeData();
            SVGShapeData( const agg::svg::path_attributes& attr );

            /** Reset the shape, clearing the vertices. */
            Status Reset();

            /** Get the number of vertices */
            size_t GetVertexCount() const;

            /** Add a vertex to the shape
                @note   The vertex is added at the end of the vertex array.
            */
            Status AddVertex( REAL x, REAL y, size_t shapeIdx = 0 );
            Status AddVertex( const PointF& pt, size_t shapeIdx = 0 );

            /** Get a specified vertex */
            Status GetVertex( size_t idx, PointF* pt, size_t shapeIdx = 0 ) const;

            /** Set the vertex at a specified point.  This requires that there
                be a vertex at the index, and that vertex will be replaced.
            */
            Status SetVertex( size_t idx, const PointF& pt, size_t shapeIdx = 0 );

            /** Assignment operator */
            SVGShapeData& operator=( const SVGShapeData& src )
            {
                mPaths.assign( src.mPaths.begin(), src.mPaths.end() );
                //mVertices.assign( src.mVertices.begin(), src.mVertices.end() );
                mAttr = src.mAttr;
                return *this;
            }

            /** Calculate the bounding rect for the shape */
            Status GetBoundingRect( RectF* bounds ) const;

            /** Normalize the shape by scaling the vertices so that the shape radius
                is 1, and centered at 0,0.
            */
            Status Normalize();

            /** Normalize a shape based on a bounding rect.
                @param  bounds          Bounding rect.  The shape will be centered in this rect, and scaled based on the rect's radius.
            */
            Status Normalize( const RectF& bounds );

            /** Add a sub-path to the shape */
            Status AddPath( const SVGSubPath& path );

            /** Get the number of sub-paths in the shape */
            INT GetPathCount() const    { return mPaths.size(); }

            //
            // Accessors
            //
            /** Get the fill color of the shape */
            const Color& GetFillColor() const           { return mFillColor; }
            /** Set the fill color */
            void SetFillColor( const Color& color )     { mFillColor = color; }

            /** Get the stroke color */
            const Color& GetStrokeColor() const         { return mStrokeColor; }
            /** Set the stroke color */
            void SetStrokeColor( const Color& color )   { mStrokeColor = color; }

            /** Get the stroke width */
            REAL GetStrokeWidth() const                 { return mStrokeWidth; }
            /** Set the stroke width */
            void SetStrokeWidth( REAL width )           { mStrokeWidth = width; }

            /** Get the fill flag */
            bool GetFillFlag() const                    { return mFillFlag; }
            /** Set the fill flag */
            void SetFillFlag( bool flag )               { mFillFlag = flag; }

            /** Get the fill mode */
            bool IsEvenOddFill() const                  { return mEvenOddFlag; }
            /** Set the fill mode */
            void SetEvenOddFill( bool flag )            { mEvenOddFlag = flag; }

            /** Get the stroking flag */
            bool GetStrokeFlag() const                  { return mStrokeFlag; }
            /** Set the stroke flag */
            void SetStrokeFlag( bool flag )             { mStrokeFlag = flag; }

            /** Get the line join type */
            LineJoin GetLineJoin() const                { return mLineJoin; }
            /** Set the line join type */
            void SetLineJoin( LineJoin mode )           { mLineJoin = mode; }

            /** Get the line cap type */
            LineCap GetLineCap() const                  { return mLineCap; }
            /** Set the line cap type */
            void SetLineCap( LineCap mode )             { mLineCap = mode; }

            /** Get the miter limit */
            REAL GetMiterLimit() const                  { return mMiterLimit; }
            /** Set the miter limit */
            void SetMiterLimit( REAL limit )            { mMiterLimit = limit; }

            /** Get the transform matrix */
            Status GetTransform( Matrix* matrix ) const
            {
                Status status;
                REAL m[ 6 ];
                if ( ( status = mMatrix.GetElements( m ) ) != Ok )
                    return SetStatus( status );
                return SetStatus( matrix->SetElements( m[ 0 ], m[ 1 ], m[ 2 ], m[ 3 ], m[ 4 ], m[ 5 ] ) );
            }
            /** Set the transform matrix */
            Status SetTransform( const Matrix* matrix )
            {
                Status status;
                REAL m[ 6 ];
                if ( ( status = matrix->GetElements( m ) ) != Ok )
                    return SetStatus( status );
                return SetStatus( mMatrix.SetElements( m[ 0 ], m[ 1 ], m[ 2 ], m[ 3 ], m[ 4 ], m[ 5 ] ) );
            }

            /** Get the normalization scale */
            REAL GetNormalizationScale() const          { return mNormalizationFactor; }
        };
    } // namespace svg
} // namespace AggOO

#endif  // AggOOSVGShapeData_H_
