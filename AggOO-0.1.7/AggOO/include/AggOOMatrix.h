
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
// $Id: AggOOMatrix.h,v 1.6 2007/08/10 18:38:42 dratek Exp $
//
// AggOOMatrix.h -- Defines an affine transformation matrix
//
// Author       : Chad M. Draper
// Date         : September 27, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-27      Initial implementation
//

#ifndef AggOOMatrix_H_
#define AggOOMatrix_H_

#include "AggOOEnums.h"
#include "AggOOTypes.h"
#include "AggOOBase.h"
#include "agg_trans_affine.h"

namespace AggOO
{

    /** @class Matrix
        @remarks
            Defines a 3x3 matrix used for affine transformations.  The Matrix
            actually only stores the first 6 items used in the transformation
            because all affine matrices have the same third row (0 0 1.)
    */
    class _AggOOExport Matrix : public Base
    {
    protected:
        mutable Status  mLastStatus;
        agg::trans_affine mMatrix;

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
        friend class GraphicsPath;
        friend class GraphicsRendererBase;
        friend class TextureBrush;
        friend class LinearGradientBrush;
        friend class PathGradientBrush;
        friend class Pen;
        friend class Region;

        /** Retrieve the last status message set by the brush
        */
        Status GetLastStatus() const    { return mLastStatus; }

    private:
        //! Assignment operator for copying the matrix
        Matrix& operator=( const Matrix& src );

    public:
        /** Constructor */
        Matrix(void);
        /** Constructor */
        Matrix( REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy );
        /** Constructor */
        Matrix( const Rect& rect, const Point* dstplg );
        /** Constructor */
        Matrix( const Rect& rect, const PointF* dstplg );
        /** Destructor */
        ~Matrix(void);

        /** Create a new clone of the matrix */
        Matrix* Clone() const;

        /** Determine if the elements of this matrix are the same as those in
            another matrix
        */
        BOOL Equals( const Matrix* matrix ) const;

        /** Get the elements of this matrix into a 6-element array */
        Status GetElements( REAL* m ) const;

        /** Set the elements of the matrix */
        Status SetElements( REAL m11, REAL m12, REAL m21, REAL m22, REAL dx, REAL dy );

        /** Convert this matrix into its inverse */
        Status Invert(void);

        /** Determine if this matrix is the identity matrix */
        BOOL IsIdentity(void) const;

        /** Determine if this matrix can be inverted */
        BOOL IsInvertible(void) const;

        /** Multiply this matrix by another matrix.  The product is stored in
            this matrix.
        */
        Status Multiply( const Matrix* matrix, MatrixOrder order = MatrixOrderPrepend );

        /** Get the horizontal offset of the matrix */
        REAL OffsetX() const;

        /** Get the vertical offset of the matrix */
        REAL OffsetY() const;

        /** Reset this matrix to the identity matrix */
        Status Reset(void);

        /** Rotate the matrix by a specified angle (in degrees) */
        Status Rotate( REAL angle, MatrixOrder order = MatrixOrderPrepend );

        /** Rotate the matrix by a specified angle (in degrees) about a
            specified point.
        */
        Status RotateAt( REAL angle, const PointF& center, MatrixOrder order = MatrixOrderPrepend );

        /** Scale the matrix */
        Status Scale( REAL scaleX, REAL scaleY, MatrixOrder order = MatrixOrderPrepend );

        /** Shear the matrix */
        Status Shear( REAL shearX, REAL shearY, MatrixOrder order = MatrixOrderPrepend );

        /** Translate the matrix */
        Status Translate( REAL offsetX, REAL offsetY, MatrixOrder order = MatrixOrderPrepend );

        /** Transform the points in an array by the matrix */
        Status TransformPoints( Point* pts, INT count );
        /** Transform the points in an array by the matrix */
        Status TransformPoints( PointF* pts, INT count );

        /** Multiply each vector in an array by the matrix.  The translation
            elements in the third row are ignored.
        */
        Status TransformVectors( Point* pts, INT count );
        /** Multiply each vector in an array by the matrix.  The translation
            elements in the third row are ignored.
        */
        Status TransformVectors( PointF* pts, INT count );

        /** Get the agg::trans_affine part of the matrix */
        agg::trans_affine GetAffine() const                 { return mMatrix; }
        /** Set the agg::trans_affine part of the matrix */
        Status SetAffine( const agg::trans_affine& mtx )
        {
            mMatrix = mtx;
            return SetStatus( Ok );
        }
    };

    // Write to a stream
    inline std::ostream& operator<<( std::ostream& stream, const Matrix& src )
    {
        std::ostream::sentry opfx(stream);
        if ( !opfx )
            return stream;
        REAL mat[ 6 ];
        src.GetElements( mat );
        return ( stream << "Matrix =\n" <<\
                           "    | " << mat[ 0 ] << "  " << mat[ 1 ] << " |\n" <<\
                           "    | " << mat[ 2 ] << "  " << mat[ 3 ] << " |\n" <<\
                           "    | " << mat[ 4 ] << "  " << mat[ 5 ] << " |\n" );

    }

} // namespace AggOO

#endif  // AggOOMatrix_H_
