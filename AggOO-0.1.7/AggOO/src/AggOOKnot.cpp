
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

/*! $Id: AggOOKnot.cpp,v 1.1 2008/04/02 18:27:17 dratek Exp $
 *  @file   AggOOKnot.cpp
 *  
 */

#include "AggOOKnot.h"

namespace AggOO
{
    Knot::Knot(void)
        : mKnotType( Knot::Corner )
    {
    }

    Knot::Knot( Knot::KnotType type )
        : mKnotType( type )
    {
    }

    Knot::Knot( const PointF& pos )
        : mKnotType( Knot::Corner ),
          mPosition( pos )
    {
    }

    Knot::Knot( const PointF& pos, const PointF& inVec, const PointF& outVec )
        : mKnotType( Knot::Corner ),
          mPosition( pos ),
          mInVector( inVec ),
          mOutVector( outVec )
    {
    }
    
    Knot::Knot( Knot::KnotType type, const PointF& pos, const PointF& inVec, const PointF& outVec )
        : mKnotType( type ),
          mPosition( pos ),
          mInVector( inVec ),
          mOutVector( outVec )
    {
    }

    Knot::~Knot(void)
    {
    }

    //SetKnotType---------------------------------------------------------------
    void Knot::SetKnotType( Knot::KnotType type )
    {
        KnotType oldType = mKnotType;
        mKnotType = type;

        // If the old type and new type are not the same, there may be
        // additional steps necessary, unless the new type is "corner",
        // which requires no additional steps at this time.
        if ( mKnotType != Corner && mKnotType != oldType )
        {
            // Average the vectors
            PointF vecAvg = ( mOutVector - mInVector ) * 0.5;

            // If the new type is "curve", then the magnitudes do not need to
            // change (just the vector directions)
            if ( mKnotType == Curve )
            {
                mInVector  = -( PointExtF::SetLength( vecAvg, PointExtF::Length( mInVector ) ) );
                mOutVector =  ( PointExtF::SetLength( vecAvg, PointExtF::Length( mOutVector ) ) );
            }
            else    // Symmetric
            {
                REAL length = ( PointExtF::Length( mInVector ) + PointExtF::Length( mOutVector ) ) * 0.5;
                vecAvg     = PointExtF::SetLength( vecAvg, length );
                mInVector  = -vecAvg;
                mOutVector =  vecAvg;
            }
        }
    }

    //SetPosition---------------------------------------------------------------
    void Knot::SetPosition( const PointF& pos )
    {
        mPosition = pos;
    }

    //SetInVector---------------------------------------------------------------
    void Knot::SetInVector( const PointF& vec )
    {
        mInVector = vec;

        // If the node type is curve, then then out vector needs to be adjusted
        // so that its direction is a reflection of the new in vector.
        if ( mKnotType == Curve )
        {
            mOutVector = -( PointExtF::SetLength( vec, PointExtF::Length( mOutVector ) ) );
        }

        // If the node type is "symmetric", then the out vector needs to be a
        // reflection of the in vector
        else if ( mKnotType == Symmetric )
        {
            mOutVector = -vec;
        }
    }

    //SetOutVector--------------------------------------------------------------
    void Knot::SetOutVector( const PointF& vec )
    {
        mOutVector = vec;

        // If the node type is curve, then then out vector needs to be adjusted
        // so that its direction is a reflection of the new in vector.
        if ( mKnotType == Curve )
        {
            mInVector = -( PointExtF::SetLength( vec, PointExtF::Length( mInVector ) ) );
        }

        // If the node type is "symmetric", then the out vector needs to be a
        // reflection of the in vector
        else if ( mKnotType == Symmetric )
        {
            mInVector = -vec;
        }
    }

    //Equals--------------------------------------------------------------------
    bool Knot::Equals( const Knot& other ) const
    {
        return mPosition.Equals( other.GetPosition() ) && mInVector.Equals( other.GetInVector() ) && mOutVector.Equals( other.GetOutVector() ) && ( mKnotType == other.GetKnotType() );
    }

    //PositionEquals------------------------------------------------------------
    bool Knot::PositionEquals( const Knot& other ) const
    {
        return mPosition.Equals( other.GetPosition() );
    }

} // namespace AggOO
