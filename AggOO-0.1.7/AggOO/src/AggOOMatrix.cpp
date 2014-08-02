
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
// $Id: AggOOMatrix.cpp,v 1.1 2006/11/01 22:41:13 dratek Exp $
//
// AggOOMatrix.h -- Defines an affine transformation matrix
//

#include "AggOOMatrix.h"

namespace AggOO
{
    Matrix::Matrix(void)
    {
        mMatrix.reset();
        mLastStatus = SetStatus( Ok );
    }

    Matrix::Matrix( REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy )
    {
        SetElements( m11, m12, m21, m22, dx, dy );
        mLastStatus = SetStatus( Ok );
    }

    Matrix::Matrix( const Rect& rect, const Point* dstplg )
    {
        Reset();
        mLastStatus = SetStatus( Ok );
    }

    Matrix::Matrix( const Rect& rect, const PointF* dstplg )
    {
        Reset();
        mLastStatus = SetStatus( Ok );
    }

    Matrix::~Matrix(void)
    {
    }

    //Clone---------------------------------------------------------------------
    Matrix* Matrix::Clone() const
    {
        // Get the elements of the matrix:
        double m[ 6 ];
        mMatrix.store_to( m );

        // Create and return the new matrix:
        Matrix* ret = new Matrix( m[ 0 ], m[ 1 ], m[ 2 ], m[ 3 ], m[ 4 ], m[ 5 ] );
        return ret;
    }

    //Equals--------------------------------------------------------------------
    BOOL Matrix::Equals( const Matrix* matrix ) const
    {
        return mMatrix.is_equal( matrix->mMatrix );
    }

    //GetElements---------------------------------------------------------------
    Status Matrix::GetElements( REAL* m ) const
    {
        // AGG uses double, AggOO uses REAL (which is actually float...)
        double temp[ 6 ];
        mMatrix.store_to( temp );

        m[ 0 ] = (REAL)temp[ 0 ];
        m[ 1 ] = (REAL)temp[ 1 ];
        m[ 2 ] = (REAL)temp[ 2 ];
        m[ 3 ] = (REAL)temp[ 3 ];
        m[ 4 ] = (REAL)temp[ 4 ];
        m[ 5 ] = (REAL)temp[ 5 ];

        return SetStatus( Ok );
    }

    //SetElements---------------------------------------------------------------
    Status Matrix::SetElements( REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy )
    {
        double m[] = { m11, m12, m21, m22, dx, dy };
        mMatrix.load_from( m );
        return SetStatus( Ok );
    }

    //operator=-----------------------------------------------------------------
    Matrix& Matrix::operator=( const Matrix& src )
    {
        REAL temp[ 6 ];
        if ( src.GetElements( temp ) == Ok )
            SetElements( temp[ 0 ], temp[ 1 ], temp[ 2 ], temp[ 3 ], temp[ 4 ], temp[ 5 ] );
        return *this;
    }

    //Invert--------------------------------------------------------------------
    Status Matrix::Invert(void)
    {
        mMatrix.invert();
        return SetStatus( Ok );
    }

    //IsIdentity----------------------------------------------------------------
    BOOL Matrix::IsIdentity(void) const
    {
        return mMatrix.is_identity();
    }

    //IsInvertible--------------------------------------------------------------
    BOOL Matrix::IsInvertible(void) const
    {
        // Calculate the determinant.  If this is close to 0, then there would
        // be a division-by-zero error, which means the matrix is not
        // invertible.
        double m[ 6 ];
        mMatrix.store_to( m );
        double det = m[ 0 ] * m[ 1 ] + m[ 2 ] * m[ 3 ] + m[ 4 ] * m[ 5 ];
        if ( fabs( det ) <= agg::affine_epsilon )
            return 0;
        return 1;
    }

    //Multiply------------------------------------------------------------------
    Status Matrix::Multiply( const Matrix* matrix, MatrixOrder order )
    {
        if ( order == MatrixOrderPrepend )
            mMatrix.premultiply( matrix->mMatrix );
        else
            mMatrix.multiply( matrix->mMatrix );
        return SetStatus( Ok );
    }

    //OffsetX-------------------------------------------------------------------
    REAL Matrix::OffsetX() const
    {
        return mMatrix.tx;
    }

    //OffsetY-------------------------------------------------------------------
    REAL Matrix::OffsetY() const
    {
        return mMatrix.tx;
    }

    //Reset---------------------------------------------------------------------
    Status Matrix::Reset(void)
    {
        mMatrix.reset();
        return SetStatus( Ok );
    }

    //Rotate--------------------------------------------------------------------
    Status Matrix::Rotate( REAL angle, MatrixOrder order )
    {
        if ( order == MatrixOrderPrepend )
            mMatrix.premultiply( agg::trans_affine_rotation( agg::deg2rad( angle ) ) );
        else
            mMatrix.multiply( agg::trans_affine_rotation( agg::deg2rad( angle ) ) );
        return SetStatus( Ok );
    }

    //RotateAt------------------------------------------------------------------
    Status Matrix::RotateAt( REAL angle, const PointF& center, MatrixOrder order )
    {
        // Translate to the position before rotation:
        Translate( center.X, center.Y, order );

        if ( order == MatrixOrderPrepend )
            mMatrix.premultiply( agg::trans_affine_rotation( agg::deg2rad( angle ) ) );
        else
            mMatrix.multiply( agg::trans_affine_rotation( agg::deg2rad( angle ) ) );
        return SetStatus( Ok );
    }

    //Scale---------------------------------------------------------------------
    Status Matrix::Scale( REAL scaleX, REAL scaleY, MatrixOrder order )
    {
        if ( order == MatrixOrderPrepend )
            mMatrix.premultiply( agg::trans_affine_scaling( scaleX, scaleY ) );
        else
            mMatrix.multiply( agg::trans_affine_scaling( scaleX, scaleY ) );
        return SetStatus( Ok );
    }

    //Shear---------------------------------------------------------------------
    Status Matrix::Shear( REAL shearX, REAL shearY, MatrixOrder order )
    {
        if ( order == MatrixOrderPrepend )
            mMatrix.premultiply( agg::trans_affine_skewing( shearX, shearY ) );
        else
            mMatrix.multiply( agg::trans_affine_skewing( shearX, shearY ) );
        return SetStatus( Ok );
    }

    //Translate-----------------------------------------------------------------
    Status Matrix::Translate( REAL offsetX, REAL offsetY, MatrixOrder order )
    {
        if ( order == MatrixOrderPrepend )
            mMatrix.premultiply( agg::trans_affine_translation( offsetX, offsetY ) );
        else
            mMatrix.multiply( agg::trans_affine_translation( offsetX, offsetY ) );
        return SetStatus( Ok );
    }

    //TransformPoints-----------------------------------------------------------
    Status Matrix::TransformPoints( Point* pts, INT count )
    {
        // Loop over the points and multiply each one by the matrix:
        for ( size_t i = 0; i < count; i++ )
        {
            double x = (double)pts[ i ].X;
            double y = (double)pts[ i ].Y;
            mMatrix.transform( &x, &y );
            pts[ i ].X = (INT)x;
            pts[ i ].Y = (INT)y;
        }
        return SetStatus( Ok );
    }
    Status Matrix::TransformPoints( PointF* pts, INT count )
    {
        // Loop over the points and multiply each one by the matrix:
        for ( size_t i = 0; i < count; i++ )
        {
            double x = (double)pts[ i ].X;
            double y = (double)pts[ i ].Y;
            mMatrix.transform( &x, &y );
            pts[ i ].X = (REAL)x;
            pts[ i ].Y = (REAL)y;
        }
        return SetStatus( Ok );
    }

    //TransformVectors----------------------------------------------------------
    Status Matrix::TransformVectors( Point* pts, INT count )
    {
        // Get the translation offset of the matrix.  After the point is
        // transformed, subtract this from the point.
        REAL dx = mMatrix.tx;
        REAL dy = mMatrix.ty;

        // Loop over the points and multiply each one by the matrix:
        for ( size_t i = 0; i < count; i++ )
        {
            double x = (double)pts[ i ].X;
            double y = (double)pts[ i ].Y;
            mMatrix.transform( &x, &y );
            pts[ i ].X = (INT)( x - dx );
            pts[ i ].Y = (INT)( y - dy );
        }
        return SetStatus( Ok );
    }
    Status Matrix::TransformVectors( PointF* pts, INT count )
    {
        // Get the translation offset of the matrix.  After the point is
        // transformed, subtract this from the point.
        REAL dx = mMatrix.tx;
        REAL dy = mMatrix.ty;

        // Loop over the points and multiply each one by the matrix:
        for ( size_t i = 0; i < count; i++ )
        {
            double x = (double)pts[ i ].X;
            double y = (double)pts[ i ].Y;
            mMatrix.transform( &x, &y );
            pts[ i ].X = (REAL)( x - dx );
            pts[ i ].Y = (REAL)( y - dy );
        }
        return SetStatus( Ok );
    }

} // namespace AggOO
