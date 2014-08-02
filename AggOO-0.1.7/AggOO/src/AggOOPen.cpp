
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
// $Id: AggOOPen.cpp,v 1.2 2006/11/22 23:09:59 dratek Exp $
//
// AggOOPen.cpp
//

#include "AggOOPen.h"

namespace AggOO
{
    //Clone---------------------------------------------------------------------
    Pen* Pen::Clone() const
    {
        Pen* clone = new Pen( mBrush, mWidth );
        return clone;
    }

    //GetLineJoin---------------------------------------------------------------
    LineJoin Pen::GetLineJoin(void) const
    {
        return mLineJoin;
    }

    //SetLineJoin---------------------------------------------------------------
    Status Pen::SetLineJoin( LineJoin lineJoin )
    {
        mLineJoin = lineJoin;
        return SetStatus( Ok );
    }

    //GetMiterLimit-------------------------------------------------------------
    REAL Pen::GetMiterLimit(void) const
    {
        return mMiterLimit;
    }
    //SetMiterLimit-------------------------------------------------------------
    Status Pen::SetMiterLimit( REAL miterLimit )
    {
        mMiterLimit = miterLimit;
        return SetStatus( Ok );
    }

    //GetInternalMiterLimit-----------------------------------------------------
    REAL Pen::GetInternalMiterLimit(void) const
    {
        return mInternalMiterLimit;
    }
    //SetInternalMiterLimit-----------------------------------------------------
    Status Pen::SetInternalMiterLimit( REAL miterLimit )
    {
        mInternalMiterLimit = miterLimit;
        return SetStatus( Ok );
    }

    //GetStartCap---------------------------------------------------------------
    LineCap Pen::GetStartCap(void) const
    {
        return mStartCap;
    }

    //SetStartCap---------------------------------------------------------------
    Status Pen::SetStartCap( LineCap startCap )
    {
        mStartCap = startCap;
        return SetStatus( Ok );
    }

    //GetEndCap-----------------------------------------------------------------
    LineCap Pen::GetEndCap(void) const
    {
        return mEndCap;
    }

    //GetEndCap-----------------------------------------------------------------
    Status Pen::SetEndCap( LineCap endCap )
    {
        mEndCap = endCap;
        return SetStatus( Ok );
    }

    //GetDashCap----------------------------------------------------------------
    DashCap Pen::GetDashCap(void) const
    {
        return mDashCap;
    }

    //GetDashCap----------------------------------------------------------------
    Status Pen::SetDashCap( DashCap dashCap )
    {
        mDashCap = dashCap;
        return SetStatus( Ok );
    }

    //SetLineCap----------------------------------------------------------------
    Status Pen::SetLineCap( LineCap startCap, LineCap endCap, DashCap dashCap )
    {
        mStartCap = startCap;
        mEndCap   = endCap;
        mDashCap  = dashCap;
        return SetStatus( Ok );
    }

    //GetDashOffset-------------------------------------------------------------
    REAL Pen::GetDashOffset(void) const
    {
        return mDashOffset;
    }

    //GetDashOffset-------------------------------------------------------------
    Status Pen::SetDashOffset( REAL dashOffset )
    {
        mDashOffset = dashOffset;
        return SetStatus( Ok );
    }

    //GetDashStyle--------------------------------------------------------------
    DashStyle Pen::GetDashStyle(void) const
    {
        return mDashStyle;
    }

    //SetDashStyle--------------------------------------------------------------
    Status Pen::SetDashStyle( DashStyle dashStyle )
    {
        mDashStyle = dashStyle;
        return SetStatus( Ok );
    }

    //TranslateTransform--------------------------------------------------------
    Status Pen::TranslateTransform( REAL dx, REAL dy, MatrixOrder order )
    {
        return SetStatus( mMatrix.Translate( dx, dy, order ) );
    }

    //RotateTransform-----------------------------------------------------------
    Status Pen::RotateTransform( REAL angle, MatrixOrder order )
    {
        return SetStatus( mMatrix.Rotate( angle, order ) );
    }

    //ScaleTransform------------------------------------------------------------
    Status Pen::ScaleTransform( REAL sx, REAL sy, MatrixOrder order )
    {
        return SetStatus( mMatrix.Scale( sx, sy, order ) );
    }

    //MultiplyTransform---------------------------------------------------------
    Status Pen::MultiplyTransform( Matrix* matrix, MatrixOrder order )
    {
        return SetStatus( mMatrix.Multiply( matrix, order ) );
    }

    //GetTransform--------------------------------------------------------------
    Status Pen::GetTransform( Matrix* matrix ) const
    {
        *matrix = mMatrix;
        return SetStatus( Ok );
    }

    //SetTransform--------------------------------------------------------------
    Status Pen::SetTransform( const Matrix* matrix )
    {
        mMatrix = *matrix;
        return SetStatus( Ok );
    }

    //ResetTransform------------------------------------------------------------
    Status Pen::ResetTransform(void)
    {
        return SetStatus( mMatrix.Reset() );
    }

} // namespace AggOO
