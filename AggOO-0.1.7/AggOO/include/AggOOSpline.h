
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

/*! $Id: AggOOSpline.h,v 1.3 2009/02/26 22:13:38 dratek Exp $
 *  @file   AggOOSpline.h
 *  @author Chad M. Draper
 *  @date   March 12, 2008
 *  @brief  Defines a spline.
 *
 *  @note   As of March 28, 2008, knot and segment removal is NOT implemented.
 *          There are methods in place (with some code) but the functionality
 *          is not sound, and will require more work to get implement.
 *  
 */

#ifndef AggOOSpline_H_
#define AggOOSpline_H_

#include "AggOOBase.h"
#include "AggOOEnums.h"
#include "AggOOTypes.h"
#include "AggOOKnot.h"
#include "AggOOSharedPtr.h"

#include <vector>
#include <utility>

namespace AggOO
{
    /** @class Spline
        Defines a spline, which can be used for such uses as a path.
    */
    class _AggOOExport Spline : public Base
    {
    public:

        /** @class Segment
            Defines a segment between points.  The segment will keep track of
            its own length, and the distance to the end of the segment from the
            start of the spline.  It will also handle rendering the segment
            (which in turn renders the spline) based on type (linear, quadratic,
            or cubic.)
        */
        class _AggOOExport Segment
        {
            friend class Spline;

        public:
            enum SegmentType { Linear, Quadratic, Cubic };

        private:
            REAL    mLength;            /**< Length of the segment */
            size_t  mStartPoint;        /**< Index of the first point of the segment */
            size_t  mEndPoint;          /**< Index of the last point of the segment */
            SegmentType mSegmentType;   /**< Type of segment (linear, quadratic, or cubic */
            Spline* mSpline;            /**< Pointer to the spline which contains this segment (so the segment can access the point data) */

        protected:
            typedef REAL (*lengthFunc)( const Segment* segment, REAL delta );
            //lengthFunc mLengthFunc[ SegmentTypeCount ];
            static std::vector< lengthFunc > mLengthFuncs;

            /** Calculate the length of a linear segment */
            static REAL lengthLinear( const Segment* segment, REAL delta = 0.001 );
            /** Calculate the length of a quadratic segment */
            static REAL lengthQuadratic( const Segment* segment, REAL delta = 0.001 );
            /** Calculate the length of a cubic segment */
            static REAL lengthCubic( const Segment* segment, REAL delta = 0.001 );

            typedef PointF (*positionFunc)( const Segment* segment, REAL ratio );
            static std::vector< positionFunc > mPositionFuncs;
            //static positionFunc mPositionFunc[ SegmentTypeCount ];

            /** Calculate the position along a linear segment */
            static PointF positionLinear( const Segment* segment, REAL ratio );
            /** Calculate the position along a quadratic segment */
            static PointF positionQuadratic( const Segment* segment, REAL ratio );
            /** Calculate the position along a cubic segment */
            static PointF positionCubic( const Segment* segment, REAL ratio );

            typedef PointF (*tangentFunc)( const Segment* segment, REAL ratio );
            static std::vector< tangentFunc > mTangentFuncs;
            //static tangentFunc mTangentFunc[ SegmentTypeCount ];

            /** Calculate the tangent along a linear segment */
            static PointF tangentLinear( const Segment* segment, REAL ratio );
            /** Calculate the tangent along a quadratic segment */
            static PointF tangentQuadratic( const Segment* segment, REAL ratio );
            /** Calculate the tangent along a cubic segment */
            static PointF tangentCubic( const Segment* segment, REAL ratio );

        public:
            static std::vector< lengthFunc >    initLengthFunctions();
            static std::vector< positionFunc >  initPositionFunctions();
            static std::vector< tangentFunc >   initTangentFunctions();

        public:
            Segment();
            Segment( Spline* spline, size_t startIdx, size_t endIdx, SegmentType type = Cubic );

            /** Constructor which creates a segment by merging two existing segments */
            Segment( const Segment& seg1, const Segment& seg2 );

            /** Merge this segment with another segment, storing the merger
                in this segment.
            */
            Segment& Merge( const Segment& seg2 );

            /** Merge this segment with another segment.  The returned segment
                is a new segment and the originals are unaltered.
            */
            Segment CreateMerger( const Segment& seg2 ) const;

            /** Assignment operator */
            Segment& operator=( const Segment& src );

            /** Get the length of the segment */
            REAL Length() const             { return mLength; }

            /** Set the segment type */
            void SetSegmentType( SegmentType type );

            /** Get the position along the segment at a given ratio */
            PointF Position( REAL ratio ) const;

            /** Get the tangent along the segment at a given ratio */
            PointF Tangent( REAL ratio ) const;
        };

    private:
        /** Array of knots which are the nodes in the spline */
        std::vector< Knot >     mKnots;

        /** @typedef SegmentSplineLength
            A std::pair which has the segment in first, and the length of the
            spline up to the end of the segment in second.
        */
        typedef std::pair< Segment, REAL >  SegmentSplineLength;

        /** Array of SegmentSplineLength, which defines the segments of the spline. */
        std::vector< SegmentSplineLength >  mSegments;

        AggOO::REAL mInterpolationDelta;        /**< Size of difference when calculating the segment length */
        bool        mIsClosed;                  /**< Indicates if the spline is closed */

    protected:
        /** Merge two segments by combining their start and end knots.  This
            will remove any segments/points within the range of segIndex1 and
            segIndex2.
        */
        void mergeSegments( size_t segIndex1, size_t segIndex2 );

    public:
        /** Constructor */
        Spline(void);
        /** Destructor */
        ~Spline(void);

        /** Set the interpolation delta, which is the distance between segments
            when interpolating the curve.
        */
        void InterpolationDelta( AggOO::REAL delta )    { mInterpolationDelta = delta; }
        /** Get the interpolation delta */
        AggOO::REAL InterpolationDelta() const          { return mInterpolationDelta; }

        /** Move to a point.  If this is the first point, it is the start of
            the spline.  Otherwise, it is treated as a LineTo.
            @param  pos     Position of the point
            @param  rel     If true, and this is not the first point, then this
                            is an offset from the previous point to which to
                            draw the line.  The initial MoveTo will always be
                            absolute.
        */
        void MoveTo( const PointF& pos, bool rel = false );

        /** Draw a straight line to a point.
            @param  pos     End position of the line
            @param  rel     Indicates if pos is an absolute position, or
                            relative to the previous position.
        */
        void LineTo( const PointF& pos, bool rel = false );

        /** Draw a quadratic curve to a point.
            @param  pos     End position of the curve
            @param  ctrlPt  Control point of the curve
            @param  rel     Indicates if pos and ctrlPt are absolute positions,
                            or relative to the previous position.
        */
        void Curve3( const PointF& pos, const PointF& ctrlPt, bool rel = false );

        /** Draw a cubic curve to a point.
            @param  pos     End position of the curve
            @param  ctrlPt1 First control point of the curve
            @param  ctrlPt2 Second control point of the curve
            @param  rel     Indicates if pos, ctrlPt1, and ctrlPt are absolute
                            positions, or relative to the previous position.
        */
        void Curve4( const PointF& pos, const PointF& ctrlPt1, const PointF& ctrlPt2, bool rel = false );

        /** Remove a point from the spline at a given index */
        size_t  RemoveKnot( size_t index );

        /** Clear all points from the spline */
        void    Clear();

        /** Get a control point at a given index in the spline */
        Knot GetKnot( size_t index ) const;

        /** Get a pointer to a given knot.  This can be used to modify the knot. */
        Knot* GetKnotPtr( size_t index );

        /** Get the number of points in the spline */
        size_t  GetKnotCount() const                    { return mKnots.size(); }

        /** Get the number of segments in the spline */
        size_t  GetSegmentCount() const                 { return mSegments.size(); }

        /** Return the length of the spline */
        AggOO::REAL Length() const;

        /** Return the length of a segment at a given index */
        AggOO::REAL SegmentLength( size_t segment ) const;
        /** Find a position on the spline at a given ratio (0..1, where 0 is the
            start of the spline, and 1 is the end of the spline.)
        */
        AggOO::PointF Position( AggOO::REAL ratio ) const;

        /** Check if the spline is closed */
        bool IsClosed() const;

        /** Close the spline (if it is not already closed.)  This cannot be
            undone, but since all it really does is add a point to the end of
            the spline (making a segment that goes back to the original point,)
            deleting the last point in the spline would be equivalent to un-
            doing this action.
        */
        size_t Close();

        /** Specify the type for a given node
            @param  index       Index of the node to modify (-1 to modify all nodes)
            @param  type        New type for the node
        */
        void SetKnotType( int index, Knot::KnotType type );

        /** Specify the type for a given segment
            @param  index       Index of the segment to modify (-1 to modify all nodes)
            @param  type        New type for the segment
        */
        void SetSegmentType( int index, Segment::SegmentType type );

        /** Write to a stream */
        inline friend std::ostream& operator<<( std::ostream& stream, const Spline& src )
        {
            src.ToStream( stream );
            return stream;
        }

        std::ostream& ToStream( std::ostream& stream ) const;

    }; // namespace Spline

} // namespace AggOO

#endif  // AggOOSpline_H_
