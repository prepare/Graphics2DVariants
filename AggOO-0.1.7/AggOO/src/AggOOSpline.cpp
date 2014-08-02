
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


/*! $Id: AggOOSpline.cpp,v 1.3 2009/02/26 22:13:38 dratek Exp $
 *  @file   AggOOSpline.cpp
 *  
 */

#include "AggOOSpline.h"

namespace AggOO
{
    // Initialize function arrays:
    std::vector< Spline::Segment::lengthFunc >      Spline::Segment::mLengthFuncs   = initLengthFunctions();
    std::vector< Spline::Segment::positionFunc >    Spline::Segment::mPositionFuncs = initPositionFunctions();
    std::vector< Spline::Segment::tangentFunc >     Spline::Segment::mTangentFuncs  = initTangentFunctions();

    std::vector< Spline::Segment::lengthFunc >    Spline::Segment::initLengthFunctions()
    {
        std::vector< Spline::Segment::lengthFunc > tempVec;
        tempVec.push_back( &Spline::Segment::lengthLinear );
        tempVec.push_back( &Spline::Segment::lengthQuadratic );
        tempVec.push_back( &Spline::Segment::lengthCubic );

        return tempVec;
    }
    std::vector< Spline::Segment::positionFunc >  Spline::Segment::initPositionFunctions()
    {
        std::vector< Spline::Segment::positionFunc > tempVec;
        tempVec.push_back( &Spline::Segment::positionLinear );
        tempVec.push_back( &Spline::Segment::positionQuadratic );
        tempVec.push_back( &Spline::Segment::positionCubic );

        return tempVec;
    }
    std::vector< Spline::Segment::tangentFunc >   Spline::Segment::initTangentFunctions()
    {
        std::vector< Spline::Segment::tangentFunc > tempVec;
        tempVec.push_back( &Spline::Segment::tangentLinear );
        tempVec.push_back( &Spline::Segment::tangentQuadratic );
        tempVec.push_back( &Spline::Segment::tangentCubic );

        return tempVec;
    }

    //--------------------------------------------------------------------------
    // Spline::Segment
    //--------------------------------------------------------------------------
    Spline::Segment::Segment()
        : mSpline( 0 ),
          mStartPoint( 0 ),
          mEndPoint( 0 ),
          mSegmentType( Spline::Segment::Cubic )
    {
    }

    Spline::Segment::Segment( Spline* spline, size_t startIdx, size_t endIdx, Spline::Segment::SegmentType type /*= Cubic*/ )
        : mSpline( spline ),
          mStartPoint( startIdx ),
          mEndPoint( endIdx ),
          mSegmentType( type )
    {
        // Fill in the function arrays
        Spline::Segment::mLengthFuncs.push_back( &Spline::Segment::lengthLinear );
        Spline::Segment::mLengthFuncs.push_back( &Spline::Segment::lengthQuadratic );
        Spline::Segment::mLengthFuncs.push_back( &Spline::Segment::lengthCubic );

        Spline::Segment::mPositionFuncs.push_back( &Spline::Segment::positionLinear );
        Spline::Segment::mPositionFuncs.push_back( &Spline::Segment::positionQuadratic );
        Spline::Segment::mPositionFuncs.push_back( &Spline::Segment::positionCubic );

        Spline::Segment::mTangentFuncs.push_back( &Spline::Segment::positionLinear );
        Spline::Segment::mTangentFuncs.push_back( &Spline::Segment::positionQuadratic );
        Spline::Segment::mTangentFuncs.push_back( &Spline::Segment::positionCubic );

        // Calculate the length
        mLength = mLengthFuncs[ mSegmentType ]( this, 0.001 );
    }

    Spline::Segment::Segment( const Segment& seg1, const Segment& seg2 )
    {
        *this = seg1.CreateMerger( seg2 );
    }

    //Merge---------------------------------------------------------------------
    Spline::Segment& Spline::Segment::Merge( const Segment& seg2 )
    {
        if ( !mSpline )     mSpline = seg2.mSpline;
        mEndPoint   = seg2.mEndPoint;

        // The knots will need to be combined as well.
        if ( mSpline && seg2.mSpline )
        {
            // Combine the first knots:
            Knot*   knot1a  = mSpline->GetKnotPtr( mStartPoint );
            Knot*   knot1b  = seg2.mSpline->GetKnotPtr( seg2.mStartPoint );
            PointF  outVec  = ( knot1a->GetOutVector() + knot1b->GetOutVector() ) * 0.5;

            // Combine the last knots:
            Knot*   knot2a  = mSpline->GetKnotPtr( mEndPoint );
            Knot*   knot2b  = seg2.mSpline->GetKnotPtr( seg2.mEndPoint );
            PointF  inVec   = ( knot2a->GetInVector() + knot2b->GetInVector() ) * 0.5;

            Knot*   knot1   = mSpline->GetKnotPtr( mStartPoint );
            Knot*   knot2   = mSpline->GetKnotPtr( mEndPoint );
            knot1->SetOutVector( outVec );
            knot2->SetInVector( inVec );
        }

        return *this;
    }

    //CreateMerger--------------------------------------------------------------
    Spline::Segment Spline::Segment::CreateMerger( const Segment& seg2 ) const
    {
        Segment newSeg = *this;
        newSeg.Merge( seg2 );
        return newSeg;
/*
        newSeg.mSpline      = ( mSpline ) ? mSpline : seg2.mSpline;
        newSeg.mStartPoint  = mStartPoint;
        newSeg.mEndPoint    = seg2.mEndPoint;
        newSeg.mSegmentType = mSegmentType;

        // The knots will need to be combined as well.
        if ( mSpline && seg2.mSpline )
        {
            // Combine the first knots:
            Knot*   knot1a  = mSpline->GetKnotPtr( mStartPoint );
            Knot*   knot1b  = seg2.mSpline->GetKnotPtr( seg2.mStartPoint );
            PointF  outVec  = ( knot1a->GetOutVector() + knot1b->GetOutVector() ) * 0.5;

            // Combine the last knots:
            Knot*   knot2a  = mSpline->GetKnotPtr( mLastPoint );
            Knot*   knot2b  = seg2.mSpline->GetKnotPtr( seg2.mLastPoint );
            PointF  inVec  = ( knot2a->GetInVector() + knot2b->GetInVector() ) * 0.5;

            Knot*   knot1   = newSeg.mSpline->GetKnotPtr( newSeg.mStartPoint );
            Knot*   knot2   = newSeg.mSpline->GetKnotPtr( newSeg.mEndPoint );
            knot1->SetOutVector( outVec );
            knot2->SetInVector( inVec );
        }

        return newSeg;
*/
    }

    //operator=-----------------------------------------------------------------
    Spline::Segment& Spline::Segment::operator=( const Segment& src )
    {
        mSpline     = src.mSpline;
        mStartPoint = src.mStartPoint;
        mEndPoint   = src.mEndPoint;
        mSegmentType = src.mSegmentType;
        mLength     = src.mLength;
        return *this;
    }

    //lengthLinear--------------------------------------------------------------
    REAL Spline::Segment::lengthLinear( const Spline::Segment* segment, REAL delta /*= 0.001*/ )
    {
        // Get the start and end positions
        PointF startPos = segment->mSpline->GetKnot( segment->mStartPoint ).GetPosition();
        PointF endPos   = segment->mSpline->GetKnot( segment->mEndPoint ).GetPosition();

        return PointExtF::Distance( startPos, endPos );
    }

    //lengthQuadratic-----------------------------------------------------------
    REAL Spline::Segment::lengthQuadratic( const Spline::Segment* segment, REAL delta /*= 0.001*/ )
    {
        // Get the start position
        PointF startPos = segment->mSpline->GetKnot( segment->mStartPoint ).GetPosition();

        AggOO::INT  numSegments = 4;
        AggOO::REAL deltaLength = 1.0;
        AggOO::REAL prevLength = 0.0;
        AggOO::REAL ratio = delta;
        AggOO::REAL length = 0;

        while ( deltaLength > delta )
        {
            AggOO::REAL deltaValue = 1.0 / static_cast< AggOO::REAL >( numSegments );
            length = 0.0;
            AggOO::PointF prevPos = startPos;
            for ( int i = 1; i <= numSegments; i++ )
            {
                ratio = i * deltaValue;
                AggOO::PointF pos = mPositionFuncs[ segment->mSegmentType ]( segment, ratio );
                length += AggOO::PointExtF::Distance( pos, prevPos );
                prevPos = pos;
            }
            deltaLength = ( length - prevLength ) / length;
            prevLength  = length;
            numSegments *= 2;
        }

        return length;
    }

    //lengthCubic---------------------------------------------------------------
    REAL Spline::Segment::lengthCubic( const Spline::Segment* segment, REAL delta /*= 0.001*/ )
    {
        // Get the start position
        PointF startPos = segment->mSpline->GetKnot( segment->mStartPoint ).GetPosition();

        AggOO::INT  numSegments = 4;
        AggOO::REAL deltaLength = 1.0;
        AggOO::REAL prevLength = 0.0;
        AggOO::REAL ratio = delta;
        AggOO::REAL length = 0;

        while ( deltaLength > delta )
        {
            AggOO::REAL deltaValue = 1.0 / static_cast< AggOO::REAL >( numSegments );
            length = 0.0;
            AggOO::PointF prevPos = startPos;
            for ( int i = 1; i <= numSegments; i++ )
            {
                ratio = i * deltaValue;
                AggOO::PointF pos = mPositionFuncs[ segment->mSegmentType ]( segment, ratio );
                length += AggOO::PointExtF::Distance( pos, prevPos );
                prevPos = pos;
            }
            deltaLength = ( length - prevLength ) / length;
            prevLength  = length;
            numSegments *= 2;
        }

        return length;
    }

    //positionLinear------------------------------------------------------------
    PointF Spline::Segment::positionLinear( const Spline::Segment* segment, REAL ratio )
    {
        // Get the start position
        PointF startPos = segment->mSpline->GetKnot( segment->mStartPoint ).GetPosition();
        PointF endPos   = segment->mSpline->GetKnot( segment->mEndPoint ).GetPosition();

        AggOO::PointF pos = startPos * ( 1.0 - ratio ) + endPos * ratio;
        return pos;

        //return PointExtF::Lerp( ratio, startPos, endPos );
    }

    //positionQuadratic---------------------------------------------------------
    PointF Spline::Segment::positionQuadratic( const Spline::Segment* segment, REAL ratio )
    {
        // Get the start position
        PointF startPos = segment->mSpline->GetKnot( segment->mStartPoint ).GetPosition();
        PointF endPos   = segment->mSpline->GetKnot( segment->mEndPoint ).GetPosition();
        PointF ctrlPt   = startPos + segment->mSpline->GetKnot( segment->mStartPoint ).GetOutVector();

        REAL invRatio = 1.0 - ratio;
        PointF pos = startPos * invRatio * invRatio + ctrlPt * 2.0 * ratio * invRatio + endPos * ratio * ratio;
        return pos;
    }

    //positionCubic------------------------------------------------------------=
    PointF Spline::Segment::positionCubic( const Spline::Segment* segment, REAL ratio )
    {
        // Get the start position
        PointF startPos = segment->mSpline->GetKnot( segment->mStartPoint ).GetPosition();
        PointF endPos   = segment->mSpline->GetKnot( segment->mEndPoint ).GetPosition();
        PointF ctrlPt1  = startPos + segment->mSpline->GetKnot( segment->mStartPoint ).GetOutVector();
        PointF ctrlPt2  = endPos + segment->mSpline->GetKnot( segment->mEndPoint ).GetInVector();

        AggOO::REAL invRatio = 1.0 - ratio;
        AggOO::REAL invRatio2 = invRatio * invRatio;
        AggOO::REAL ratio2 = ratio * ratio;
        PointF pos = startPos * invRatio2 * invRatio + ctrlPt1 * 3.0 * ratio * invRatio2 + ctrlPt2 * 3.0 * ratio2 * invRatio + endPos * ratio2 * ratio;
        return pos;
    }

    //tangentLinear-------------------------------------------------------------
    PointF Spline::Segment::tangentLinear( const Spline::Segment* segment, REAL ratio )
    {
        return PointF( 1.0, 0.0 );
    }

    //tangentQuadratic----------------------------------------------------------
    PointF Spline::Segment::tangentQuadratic( const Spline::Segment* segment, REAL ratio )
    {
        return PointF( 1.0, 0.0 );
    }

    //tangentCubic--------------------------------------------------------------
    PointF Spline::Segment::tangentCubic( const Spline::Segment* segment, REAL ratio )
    {
        return PointF( 1.0, 0.0 );
    }

    //SegSegmentType------------------------------------------------------------
    void Spline::Segment::SetSegmentType( SegmentType type )
    {
        //mSegmentType = type;

        // If the new type is linear, the vectors of the surrounding nodes need
        // to be modified.
        if ( type == Linear )
        {
            Knot* startPos      = mSpline->GetKnotPtr( mStartPoint );
            Knot* endPos        = mSpline->GetKnotPtr( mEndPoint );

            REAL v1Length       = PointExtF::Length( startPos->GetOutVector() );
            REAL v2Length       = PointExtF::Length( endPos->GetInVector() );

            PointF v1           = PointExtF::SetLength( endPos->GetPosition() - startPos->GetPosition(), v1Length );
            PointF v2           = PointExtF::SetLength( -v1, v2Length );

            startPos->SetOutVector( v1 );
            endPos->SetInVector( v2 );
        }
    }

    //Position------------------------------------------------------------------
    PointF Spline::Segment::Position( REAL ratio ) const
    {
        return mPositionFuncs[ mSegmentType ]( this, ratio );
    }

    //Tangent-------------------------------------------------------------------
    PointF Spline::Segment::Tangent( REAL ratio ) const
    {
        return mTangentFuncs[ mSegmentType ]( this, ratio );
    }



    //------------------------------------------------------------------------------
    // Spline
    //------------------------------------------------------------------------------

    Spline::Spline(void)
        : mInterpolationDelta( 0.001 ),
          mIsClosed( false )
    {
    }

    Spline::~Spline(void)
    {
    }

    //MoveTo--------------------------------------------------------------------
    void Spline::MoveTo( const PointF& pos, bool rel /*= false*/ )
    {
        // If this is not the first point, use LineTo:
        if ( mKnots.size() > 0 )
            LineTo( pos, rel );
        else
        {
            Knot newKnot( pos );
            mKnots.push_back( newKnot );
        }
    }

    //LineTo--------------------------------------------------------------------
    void Spline::LineTo( const PointF& pos, bool rel /*= false*/ )
    {
        // If this is the first point, add a starting point at 0,0:
        if ( mKnots.size() == 0 )
            MoveTo( PointF( 0, 0 ) );

        // Store the point as an absolute position:
        PointF absPos = pos;
        PointF prevPos = mKnots.back().GetPosition();
        if ( rel )
            absPos += prevPos;

        // Calculate the in and out vectors.
        PointF ptVec  = pos - prevPos;
        PointF outVec = ptVec * 1.0 / 3.0;
        PointF inVec  = -outVec;

        // Make sure the previous knot is a corner and set its out vector
        mKnots.back().SetKnotType( Knot::Corner );
        mKnots.back().SetOutVector( outVec );

        // Create the new knot:
        Knot newKnot( pos, inVec, PointF( 0, 0 ) );

        // Add the point:
        mKnots.push_back( newKnot );

        // Add a new segment:
        SegmentSplineLength newSeg;
        newSeg.first = Segment( this, mKnots.size() - 2, mKnots.size() - 1 );
        newSeg.second = ( ( mSegments.size() > 0 ) ? mSegments.back().second : 0 ) + newSeg.first.Length();
        mSegments.push_back( newSeg );
    }

    //Curve3--------------------------------------------------------------------
    void Spline::Curve3( const PointF& pos, const PointF& ctrlPt, bool rel /*= false*/ )
    {
        // If this is the first point, add a starting point at 0,0:
        if ( mKnots.size() == 0 )
            MoveTo( PointF( 0, 0 ) );

        // Store the point as an absolute position:
        PointF absPos = pos;
        PointF prevPos = mKnots.back().GetPosition();
        PointF cp     = ctrlPt;
        if ( rel )
        {
            absPos += prevPos;
            cp     += prevPos;
        }

        // Set the out vector of the previous knot
        mKnots.back().SetOutVector( cp - prevPos );

        // Add the point:
        Knot newKnot( pos, cp - pos, PointF( 0, 0 ) );
        mKnots.push_back( newKnot );

        // Add a new segment:
        SegmentSplineLength newSeg;
        newSeg.first = Segment( this, mKnots.size() - 2, mKnots.size() - 1 );
        newSeg.second = ( ( mSegments.size() > 0 ) ? mSegments.back().second : 0 ) + newSeg.first.Length();
        mSegments.push_back( newSeg );
    }

    //Curve4--------------------------------------------------------------------
    void Spline::Curve4( const PointF& pos, const PointF& ctrlPt1, const PointF& ctrlPt2, bool rel /*= false*/ )
    {
        // If this is the first point, add a starting point at 0,0:
        if ( mKnots.size() == 0 )
            MoveTo( PointF( 0, 0 ) );

        // Store the point as an absolute position:
        PointF absPos = pos;
        PointF prevPos = mKnots.back().GetPosition();
        PointF cp1    = ctrlPt1;
        PointF cp2    = ctrlPt2;
        if ( rel )
        {
            absPos += prevPos;
            cp1    += prevPos;
            cp2    += prevPos;
        }

        // Set the out vector of the previous knot
        mKnots.back().SetOutVector( cp1 - prevPos );

        // Add the point:
        Knot newKnot( pos, cp2 - pos, PointF( 0, 0 ) );
        mKnots.push_back( newKnot );

        // Add a new segment:
        SegmentSplineLength newSeg;
        newSeg.first = Segment( this, mKnots.size() - 2, mKnots.size() - 1, Segment::Cubic );
        newSeg.second = ( ( mSegments.size() > 0 ) ? mSegments.back().second : 0 ) + newSeg.first.Length();
        mSegments.push_back( newSeg );
    }

    //mergeSegments-------------------------------------------------------------
    void Spline::mergeSegments( size_t segIndex1, size_t segIndex2 )
    {
        if ( segIndex1 < mSegments.size() && segIndex2 < mSegments.size() )
        {
            mSegments[ segIndex1 ].first.Merge( mSegments.at( segIndex2 ).first );

            // Remove any segments between these (including segIndex2)
            mSegments.erase( mSegments.begin() + segIndex1 + 1, mSegments.begin() + segIndex2 + 1 );

            // All orphaned knots need to be removed

            // The point indices need to be adjusted
            int decValue = mSegments[ segIndex1 ].first.mEndPoint - mSegments[ segIndex1 ].first.mStartPoint - 1;
            mSegments[ segIndex1 ].first.mEndPoint -= decValue;

            for ( int i = segIndex1 + 1; i < mSegments.size(); i++ )
            {
                mSegments[ i ].first.mStartPoint -= decValue;
                mSegments[ i ].first.mEndPoint   -= decValue;
            }
        }
    }

    //RemovePoint---------------------------------------------------------------
    size_t Spline::RemoveKnot( size_t index )
    {
        if ( index < mKnots.size() )
        {
        //    // Remove the point
        //    mPoints.erase( mPoints.begin() + index );

        //    if ( index > 0 && index < mPoints.size() )
        //        mPoints.at( index ).CalculateLength( mPoints.at( index - 1 ), mInterpolationDelta );

        //    // Update the distance of the remaining points
        //    for ( int i = index + 1; i < mPoints.size(); i++ )
        //    {
        //        if ( i > 0 )
        //            mPoints.at( i ).UpdateDistance( mPoints.at( i - 2 ) );
        //    }
        }
        return mKnots.size();
    }

    //Clear---------------------------------------------------------------------
    void Spline::Clear()
    {
        mKnots.clear();
        mSegments.clear();
        mIsClosed = false;
    }

    //GetKnot-------------------------------------------------------------------
    Knot Spline::GetKnot( size_t index ) const
    {
        return mKnots.at( index );
    }

    //GetKnotPtr----------------------------------------------------------------
    Knot* Spline::GetKnotPtr( size_t index )
    {
        if ( index < mKnots.size() )
            return &mKnots[ index ];
        return 0;
    }

    //Length--------------------------------------------------------------------
    AggOO::REAL Spline::Length() const
    {
        // If the spline is maintained properly, the distance to the final point
        // should be the length of the spline.
        if ( mSegments.size() == 0 )
            return 0;
        return mSegments.back().second;
    }

    //SegmentLength-------------------------------------------------------------
    AggOO::REAL Spline::SegmentLength( size_t segment ) const
    {
        if ( segment < mSegments.size() )
            return mSegments.at( segment ).first.Length();
        return 0.0;
    }

    //Position------------------------------------------------------------------
    AggOO::PointF Spline::Position( AggOO::REAL ratio ) const
    {
        // Calculate the distance to this position by multiplying the ratio by the
        // total spline length
        AggOO::REAL len = Length();
        AggOO::REAL dist = len * ratio;

        // Find the first point that has a greater distance than "dist":
        std::vector< SegmentSplineLength >::const_iterator iter = mSegments.begin();
        REAL prevDistance = 0;
        //std::vector< SegmentSplineLength >::const_iterator prevIter = iter;
        while ( iter != mSegments.end() && iter->second < dist )
        {
            prevDistance = iter->second;
            //prevIter = iter;
            iter++;
        }

        // Determine how far along this segment the position is:
        AggOO::REAL segDist = dist - prevDistance;

        // Ratio along the segment:
        AggOO::REAL segRatio = segDist / iter->first.Length();

        // Calculate the position:
        return iter->first.Position( segRatio );
        //return iter->mControlPointPtr->Position( *prevIter->mControlPointPtr.get(), segRatio );
    }

    //IsClosed------------------------------------------------------------------
    bool Spline::IsClosed() const
    {
        return mIsClosed;
        //if ( mKnots.size() < 2 || mKnots.at( 0 ).GetPosition().Equals( mKnots.back().GetPosition() ) )
        //    return true;
        //return false;
    }

    //Close---------------------------------------------------------------------
    size_t Spline::Close()
    {
        if ( IsClosed() || mKnots.size() < 1 )
            return mKnots.size();

        // If the last point matches the first, then we need to do some adjusting
        if ( mKnots.at( 0 ).PositionEquals( mKnots.back() ) )
        {
            // We can remove the last point, then just add a segment to the
            // first, but before doing that, recalculate the in and out vectors
            // of the first knot by averaging them with the last knot's.
            Knot* firstKnot = GetKnotPtr( 0 );
            Knot* lastKnot  = GetKnotPtr( GetKnotCount() - 1 );
            PointF newInVec = lastKnot->GetInVector();
            PointF newOutVec = firstKnot->GetOutVector();

            firstKnot->mInVector  = newInVec;
            firstKnot->mOutVector = newOutVec;

            // Remove the last knot
            mKnots.pop_back();

            // Adjust the last segment to close the spline
            mSegments.back().first.mEndPoint = 0;
        }

        // Otherwise, we need to add a new segment to connect the first and last points
        else
        {
            Knot* firstKnot = GetKnotPtr( 0 );
            Knot* lastKnot  = GetKnotPtr( GetKnotCount() - 1 );
            PointF firstPos = firstKnot->GetPosition();
            PointF lastPos  = lastKnot->GetPosition();

            // Calculate the in and out vectors.
            PointF ptVec  = firstPos - lastPos;
            PointF outVec = ptVec * 1.0 / 3.0;
            PointF inVec  = -outVec;
            firstKnot->mInVector = inVec;
            lastKnot->mOutVector = outVec;

            SegmentSplineLength newSeg;
            newSeg.first = Segment( this, mKnots.size() - 1, 0 );
            newSeg.second = mSegments.back().second + newSeg.first.Length();
            mSegments.push_back( newSeg );
        }

        mIsClosed = true;

        //if ( IsClosed() )
        //    return mPoints.size();

        //// Add a new point, using the type of the current last point in the
        //// spline, with the same position as the first point
        //return AddPoint( mPoints.back().mControlPointPtr->CreateContinuation( mPoints.at( 0 ).mControlPointPtr->mPosition ) );
        return 0;
    }

    //SetKnotType---------------------------------------------------------------
    void Spline::SetKnotType( int index, Knot::KnotType type )
    {
        if ( index < 0 )
        {
            std::vector< Knot >::iterator iter = mKnots.begin();
            while ( iter != mKnots.end() )
            {
                iter->SetKnotType( type );
                iter++;
            }
        }
        else if ( index < mKnots.size() )
        {
            mKnots.at( index ).SetKnotType( type );
        }
    }

    //SetSegmentType------------------------------------------------------------
    void Spline::SetSegmentType( int index, Segment::SegmentType type )
    {
        if ( index < 0 )
        {
            std::vector< SegmentSplineLength >::iterator iter = mSegments.begin();
            while ( iter != mSegments.end() )
            {
                iter->first.SetSegmentType( type );
                iter++;
            }
        }
        else if ( index < mSegments.size() )
        {
            mSegments.at( index ).first.SetSegmentType( type );
        }
    }

    std::ostream& Spline::ToStream( std::ostream& stream ) const
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;

        stream << "Spline\n";
        stream << "{\n";
        stream << "    Length = " << Length() << std::endl;
        stream << "    Knot Count = " << GetKnotCount() << std::endl;
        stream << "    {\n";
        for ( int i = 0; i < GetKnotCount(); i++ )
        {
            stream << "        Point " << i << ": " << GetKnot( i );
        }
        stream << "    }\n";
        stream << "}\n";

        return stream;
    }

} // namespace AggOO
