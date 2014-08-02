
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
// $Id: AggOOColorMatrix.h,v 1.4 2008/08/01 15:13:33 dratek Exp $
//
// AggOOColorMatrix.h -- Color transformation matrix.
//
// Author       : Chad M. Draper
// Date         : July 25, 2007
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2007-07-25      Initial implementation
//

#ifndef AGGOOCOLORMATRIX_H_
#define AGGOOCOLORMATRIX_H_

#include <assert.h>
#include <iostream>
#include "AggOOEnums.h"
#include "AggOOTypes.h"
#include "AggOOColor.h"
#include "AggOOMath.h"

namespace AggOO
{

    //----------------------------------------------------------------------------
    // Color Matrix flags
    //----------------------------------------------------------------------------

    enum ColorMatrixFlags
    {
        ColorMatrixFlagsDefault   = 0,
        ColorMatrixFlagsSkipGrays = 1,
        ColorMatrixFlagsAltGray   = 2
    };

    //----------------------------------------------------------------------------
    // Color Adjust Type
    //----------------------------------------------------------------------------

    enum ColorAdjustType
    {
        ColorAdjustTypeDefault,
        ColorAdjustTypeBitmap,
        ColorAdjustTypeBrush,
        ColorAdjustTypePen,
        ColorAdjustTypeText,
        ColorAdjustTypeCount,
        ColorAdjustTypeAny      // Reserved
    };

    //----------------------------------------------------------------------------
    // Color Map
    //----------------------------------------------------------------------------

    struct ColorMap
    {
        Color oldColor;
        Color newColor;
    };

    /** @class ColorMatrix

        The ColorMatrix is a 5x5 matrix that manipulates the value of a Color.
        Because AggOO is aimed at being mostly compatible with GDI+, and the
        GDI+ ColorMatrix is little more than a 5x5 matrix with no methods, this
        class will also be fairly bare.  Use ColorMatrixExt for more useful
        methods to adjust the matrix and manipulate a Color object.
    */
    class _AggOOExport ColorMatrix
    {
    public:
        //REAL m[ 5 ][ 5 ];
        //REAL m[ 25 ];
        union 
        {
            REAL    m[ 5 ][ 5 ];
            REAL    _m[ 25 ];
        };

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
        ColorMatrix(void)   { *this = IDENTITY; }
        ColorMatrix( const REAL src[ 5 ][ 5 ] )
        {
            memcpy( m, src, sizeof( REAL ) * 25 );
        }
        ColorMatrix( const REAL src[ 25 ] )
        {
            memcpy( _m, src, sizeof( REAL ) * 25 );
        }
        ColorMatrix( const ColorMatrix& src )
        {
            memcpy( m, src.m, sizeof( REAL ) * 25 );
        }
        ColorMatrix(    REAL m00, REAL m01, REAL m02, REAL m03, REAL m04,\
                        REAL m10, REAL m11, REAL m12, REAL m13, REAL m14,\
                        REAL m20, REAL m21, REAL m22, REAL m23, REAL m24,\
                        REAL m30, REAL m31, REAL m32, REAL m33, REAL m34,\
                        REAL m40, REAL m41, REAL m42, REAL m43, REAL m44 )
        {
            m[ 0 ][ 0 ] = m00; m[ 0 ][ 1 ] = m01; m[ 0 ][ 2 ] = m02; m[ 0 ][ 3 ] = m03; m[ 0 ][ 4 ] = m04;
            m[ 1 ][ 0 ] = m10; m[ 1 ][ 1 ] = m11; m[ 1 ][ 2 ] = m12; m[ 1 ][ 3 ] = m13; m[ 1 ][ 4 ] = m14;
            m[ 2 ][ 0 ] = m20; m[ 2 ][ 1 ] = m21; m[ 2 ][ 2 ] = m22; m[ 2 ][ 3 ] = m23; m[ 2 ][ 4 ] = m24;
            m[ 3 ][ 0 ] = m30; m[ 3 ][ 1 ] = m31; m[ 3 ][ 2 ] = m32; m[ 3 ][ 3 ] = m33; m[ 3 ][ 4 ] = m34;
            m[ 4 ][ 0 ] = m40; m[ 4 ][ 1 ] = m41; m[ 4 ][ 2 ] = m42; m[ 4 ][ 3 ] = m43; m[ 4 ][ 4 ] = m44;
        }
        ~ColorMatrix(void)  { }

        /** Identity matrix */
        static const ColorMatrix IDENTITY;      /**< Identity matrix */
        static const ColorMatrix ZERO;          /**< Matrix filled with 0's */
        static const ColorMatrix GRAYSCALE;     /**< Converts an color to grayscale, using NTSC values. */
        static const ColorMatrix WHITE;         /**< Pushes all colors to white. */
        static const ColorMatrix HALF_WHITE;    /**< Makes all colors 50% white. */
        static const ColorMatrix SEPIA;         /**< EXPERIMENTAL!!! Convert an image to sepia tone. */

        /** Overload the [] operator for access to a row.  This will enable
            access to the elements by using [r][c].
        */
        inline REAL* operator[]( size_t idx )
        {
            assert( idx < 5 );
            return m[ idx ];
        }

        inline const REAL* const operator[]( size_t idx ) const
        {
            assert( idx < 5 );
            return m[ idx ];
        }

        /** Assignment operator */
        inline ColorMatrix& operator=( const ColorMatrix& src )
        {
            memcpy( m, src.m, sizeof( REAL ) * 25 );
            return *this;
        }

        /** Equality operator */
        inline bool operator==( const ColorMatrix& src )
        {
            return !( memcmp( m, src.m, sizeof( REAL ) * 25 ) );
        }
        /** Inequality operator */
        inline bool operator!=( const ColorMatrix& src )
        {
            return memcmp( m, src.m, sizeof( REAL ) * 25 );
        }

        /** Create a clone of the matrix */
        ColorMatrix* Clone() const
        {
            return new ColorMatrix( _m );
        }

        /** Output the matrix to a stream */
        inline /*_AggOOExport*/ friend std::ostream& operator<<( std::ostream& stream, const ColorMatrix& mat )
        {
            std::ostream::sentry opfx(stream);
            if ( !opfx )
                return stream;
            stream << "ColorMatrix =\n";
            stream << "    | " << mat._m[  0 ] << "  " << mat._m[  1 ] << "  " << mat._m[  2 ] << "  " << mat._m[  3 ] << "  " << mat._m[  4 ] << " |\n";
            stream << "    | " << mat._m[  5 ] << "  " << mat._m[  6 ] << "  " << mat._m[  7 ] << "  " << mat._m[  8 ] << "  " << mat._m[  9 ] << " |\n";
            stream << "    | " << mat._m[ 10 ] << "  " << mat._m[ 11 ] << "  " << mat._m[ 12 ] << "  " << mat._m[ 13 ] << "  " << mat._m[ 14 ] << " |\n";
            stream << "    | " << mat._m[ 15 ] << "  " << mat._m[ 16 ] << "  " << mat._m[ 17 ] << "  " << mat._m[ 18 ] << "  " << mat._m[ 19 ] << " |\n";
            stream << "    | " << mat._m[ 20 ] << "  " << mat._m[ 21 ] << "  " << mat._m[ 22 ] << "  " << mat._m[ 23 ] << "  " << mat._m[ 24 ] << " |\n";
            return stream;
        }

    }; // class ColorMatrix

    /** @class ColorMatrixExt
        Provides an extended color matrix, offering methods to handle most
        common tasks.
    */
    class _AggOOExport ColorMatrixExt : public ColorMatrix
    {
    public:
        ColorMatrixExt()    { *this = IDENTITY; }
        ColorMatrixExt( const REAL src[ 5 ][ 5 ] )
            : ColorMatrix( src )
        {
//            memcpy( m, src, sizeof( REAL ) * 25 );
        }
        ColorMatrixExt( const REAL src[ 25 ] )
            : ColorMatrix( src )
        {
//            memcpy( m, src, sizeof( REAL ) * 25 );
        }
        ColorMatrixExt( const ColorMatrix& src )
            : ColorMatrix( src )
        {
//            memcpy( m, src.m, sizeof( REAL ) * 25 );
        }
        ColorMatrixExt( REAL m00, REAL m01, REAL m02, REAL m03, REAL m04,\
                        REAL m10, REAL m11, REAL m12, REAL m13, REAL m14,\
                        REAL m20, REAL m21, REAL m22, REAL m23, REAL m24,\
                        REAL m30, REAL m31, REAL m32, REAL m33, REAL m34,\
                        REAL m40, REAL m41, REAL m42, REAL m43, REAL m44 )
            : ColorMatrix( m00, m01, m02, m03, m04,\
                           m10, m11, m12, m13, m14,\
                           m20, m21, m22, m23, m24,\
                           m30, m31, m32, m33, m34,\
                           m40, m41, m42, m43, m44 )
        {
            //m[ 0 ][ 0 ] = m00; m[ 0 ][ 1 ] = m01; m[ 0 ][ 2 ] = m02; m[ 0 ][ 3 ] = m03; m[ 0 ][ 4 ] = m04;
            //m[ 1 ][ 0 ] = m10; m[ 1 ][ 1 ] = m01; m[ 1 ][ 2 ] = m12; m[ 1 ][ 3 ] = m13; m[ 1 ][ 4 ] = m14;
            //m[ 2 ][ 0 ] = m20; m[ 2 ][ 1 ] = m01; m[ 2 ][ 2 ] = m22; m[ 2 ][ 3 ] = m23; m[ 2 ][ 4 ] = m24;
            //m[ 3 ][ 0 ] = m30; m[ 3 ][ 1 ] = m01; m[ 3 ][ 2 ] = m32; m[ 3 ][ 3 ] = m33; m[ 3 ][ 4 ] = m34;
            //m[ 4 ][ 0 ] = m40; m[ 4 ][ 1 ] = m01; m[ 4 ][ 2 ] = m42; m[ 4 ][ 3 ] = m43; m[ 4 ][ 4 ] = m44;
        }
        ~ColorMatrixExt()   { }

        /** Multiply this matrix with another matrix. */
        ColorMatrix Multiply( const ColorMatrix& mat, MatrixOrder order = MatrixOrderPrepend ) const
        {
            ColorMatrix tempMat;
            ColorMatrix mat1, mat2;
            if ( order == MatrixOrderPrepend )
            {
                mat1 = *this;
                mat2 = mat;
            }
            else
            {
                mat1 = mat;
                mat2 = *this;
            }
            tempMat.m[ 0 ][ 0 ] = mat1.m[ 0 ][ 0 ] * mat2.m[ 0 ][ 0 ] + mat1.m[ 0 ][ 1 ] * mat2.m[ 1 ][ 0 ] + mat1.m[ 0 ][ 2 ] * mat2.m[ 2 ][ 0 ] + mat1.m[ 0 ][ 3 ] * mat2.m[ 3 ][ 0 ] + mat1.m[ 0 ][ 4 ] * mat2.m[ 4 ][ 0 ];
            tempMat.m[ 0 ][ 1 ] = mat1.m[ 0 ][ 0 ] * mat2.m[ 0 ][ 1 ] + mat1.m[ 0 ][ 1 ] * mat2.m[ 1 ][ 1 ] + mat1.m[ 0 ][ 2 ] * mat2.m[ 2 ][ 1 ] + mat1.m[ 0 ][ 3 ] * mat2.m[ 3 ][ 1 ] + mat1.m[ 0 ][ 4 ] * mat2.m[ 4 ][ 1 ];
            tempMat.m[ 0 ][ 2 ] = mat1.m[ 0 ][ 0 ] * mat2.m[ 0 ][ 2 ] + mat1.m[ 0 ][ 1 ] * mat2.m[ 1 ][ 2 ] + mat1.m[ 0 ][ 2 ] * mat2.m[ 2 ][ 2 ] + mat1.m[ 0 ][ 3 ] * mat2.m[ 3 ][ 2 ] + mat1.m[ 0 ][ 4 ] * mat2.m[ 4 ][ 2 ];
            tempMat.m[ 0 ][ 3 ] = mat1.m[ 0 ][ 0 ] * mat2.m[ 0 ][ 3 ] + mat1.m[ 0 ][ 1 ] * mat2.m[ 1 ][ 3 ] + mat1.m[ 0 ][ 2 ] * mat2.m[ 2 ][ 3 ] + mat1.m[ 0 ][ 3 ] * mat2.m[ 3 ][ 3 ] + mat1.m[ 0 ][ 4 ] * mat2.m[ 4 ][ 3 ];
            tempMat.m[ 0 ][ 4 ] = mat1.m[ 0 ][ 0 ] * mat2.m[ 0 ][ 4 ] + mat1.m[ 0 ][ 1 ] * mat2.m[ 1 ][ 4 ] + mat1.m[ 0 ][ 2 ] * mat2.m[ 2 ][ 4 ] + mat1.m[ 0 ][ 3 ] * mat2.m[ 3 ][ 4 ] + mat1.m[ 0 ][ 4 ] * mat2.m[ 4 ][ 4 ];

            tempMat.m[ 1 ][ 0 ] = mat1.m[ 1 ][ 0 ] * mat2.m[ 0 ][ 0 ] + mat1.m[ 1 ][ 1 ] * mat2.m[ 1 ][ 0 ] + mat1.m[ 1 ][ 2 ] * mat2.m[ 2 ][ 0 ] + mat1.m[ 1 ][ 3 ] * mat2.m[ 3 ][ 0 ] + mat1.m[ 1 ][ 4 ] * mat2.m[ 4 ][ 0 ];
            tempMat.m[ 1 ][ 1 ] = mat1.m[ 1 ][ 0 ] * mat2.m[ 0 ][ 1 ] + mat1.m[ 1 ][ 1 ] * mat2.m[ 1 ][ 1 ] + mat1.m[ 1 ][ 2 ] * mat2.m[ 2 ][ 1 ] + mat1.m[ 1 ][ 3 ] * mat2.m[ 3 ][ 1 ] + mat1.m[ 1 ][ 4 ] * mat2.m[ 4 ][ 1 ];
            tempMat.m[ 1 ][ 2 ] = mat1.m[ 1 ][ 0 ] * mat2.m[ 0 ][ 2 ] + mat1.m[ 1 ][ 1 ] * mat2.m[ 1 ][ 2 ] + mat1.m[ 1 ][ 2 ] * mat2.m[ 2 ][ 2 ] + mat1.m[ 1 ][ 3 ] * mat2.m[ 3 ][ 2 ] + mat1.m[ 1 ][ 4 ] * mat2.m[ 4 ][ 2 ];
            tempMat.m[ 1 ][ 3 ] = mat1.m[ 1 ][ 0 ] * mat2.m[ 0 ][ 3 ] + mat1.m[ 1 ][ 1 ] * mat2.m[ 1 ][ 3 ] + mat1.m[ 1 ][ 2 ] * mat2.m[ 2 ][ 3 ] + mat1.m[ 1 ][ 3 ] * mat2.m[ 3 ][ 3 ] + mat1.m[ 1 ][ 4 ] * mat2.m[ 4 ][ 3 ];
            tempMat.m[ 1 ][ 4 ] = mat1.m[ 1 ][ 0 ] * mat2.m[ 0 ][ 4 ] + mat1.m[ 1 ][ 1 ] * mat2.m[ 1 ][ 4 ] + mat1.m[ 1 ][ 2 ] * mat2.m[ 2 ][ 4 ] + mat1.m[ 1 ][ 3 ] * mat2.m[ 3 ][ 4 ] + mat1.m[ 1 ][ 4 ] * mat2.m[ 4 ][ 4 ];

            tempMat.m[ 2 ][ 0 ] = mat1.m[ 2 ][ 0 ] * mat2.m[ 0 ][ 0 ] + mat1.m[ 2 ][ 1 ] * mat2.m[ 1 ][ 0 ] + mat1.m[ 2 ][ 2 ] * mat2.m[ 2 ][ 0 ] + mat1.m[ 2 ][ 3 ] * mat2.m[ 3 ][ 0 ] + mat1.m[ 2 ][ 4 ] * mat2.m[ 4 ][ 0 ];
            tempMat.m[ 2 ][ 1 ] = mat1.m[ 2 ][ 0 ] * mat2.m[ 0 ][ 1 ] + mat1.m[ 2 ][ 1 ] * mat2.m[ 1 ][ 1 ] + mat1.m[ 2 ][ 2 ] * mat2.m[ 2 ][ 1 ] + mat1.m[ 2 ][ 3 ] * mat2.m[ 3 ][ 1 ] + mat1.m[ 2 ][ 4 ] * mat2.m[ 4 ][ 1 ];
            tempMat.m[ 2 ][ 2 ] = mat1.m[ 2 ][ 0 ] * mat2.m[ 0 ][ 2 ] + mat1.m[ 2 ][ 1 ] * mat2.m[ 1 ][ 2 ] + mat1.m[ 2 ][ 2 ] * mat2.m[ 2 ][ 2 ] + mat1.m[ 2 ][ 3 ] * mat2.m[ 3 ][ 2 ] + mat1.m[ 2 ][ 4 ] * mat2.m[ 4 ][ 2 ];
            tempMat.m[ 2 ][ 3 ] = mat1.m[ 2 ][ 0 ] * mat2.m[ 0 ][ 3 ] + mat1.m[ 2 ][ 1 ] * mat2.m[ 1 ][ 3 ] + mat1.m[ 2 ][ 2 ] * mat2.m[ 2 ][ 3 ] + mat1.m[ 2 ][ 3 ] * mat2.m[ 3 ][ 3 ] + mat1.m[ 2 ][ 4 ] * mat2.m[ 4 ][ 3 ];
            tempMat.m[ 2 ][ 4 ] = mat1.m[ 2 ][ 0 ] * mat2.m[ 0 ][ 4 ] + mat1.m[ 2 ][ 1 ] * mat2.m[ 1 ][ 4 ] + mat1.m[ 2 ][ 2 ] * mat2.m[ 2 ][ 4 ] + mat1.m[ 2 ][ 3 ] * mat2.m[ 3 ][ 4 ] + mat1.m[ 2 ][ 4 ] * mat2.m[ 4 ][ 4 ];

            tempMat.m[ 3 ][ 0 ] = mat1.m[ 3 ][ 0 ] * mat2.m[ 0 ][ 0 ] + mat1.m[ 3 ][ 1 ] * mat2.m[ 1 ][ 0 ] + mat1.m[ 3 ][ 2 ] * mat2.m[ 2 ][ 0 ] + mat1.m[ 3 ][ 3 ] * mat2.m[ 3 ][ 0 ] + mat1.m[ 3 ][ 4 ] * mat2.m[ 4 ][ 0 ];
            tempMat.m[ 3 ][ 1 ] = mat1.m[ 3 ][ 0 ] * mat2.m[ 0 ][ 1 ] + mat1.m[ 3 ][ 1 ] * mat2.m[ 1 ][ 1 ] + mat1.m[ 3 ][ 2 ] * mat2.m[ 2 ][ 1 ] + mat1.m[ 3 ][ 3 ] * mat2.m[ 3 ][ 1 ] + mat1.m[ 3 ][ 4 ] * mat2.m[ 4 ][ 1 ];
            tempMat.m[ 3 ][ 2 ] = mat1.m[ 3 ][ 0 ] * mat2.m[ 0 ][ 2 ] + mat1.m[ 3 ][ 1 ] * mat2.m[ 1 ][ 2 ] + mat1.m[ 3 ][ 2 ] * mat2.m[ 2 ][ 2 ] + mat1.m[ 3 ][ 3 ] * mat2.m[ 3 ][ 2 ] + mat1.m[ 3 ][ 4 ] * mat2.m[ 4 ][ 2 ];
            tempMat.m[ 3 ][ 3 ] = mat1.m[ 3 ][ 0 ] * mat2.m[ 0 ][ 3 ] + mat1.m[ 3 ][ 1 ] * mat2.m[ 1 ][ 3 ] + mat1.m[ 3 ][ 2 ] * mat2.m[ 2 ][ 3 ] + mat1.m[ 3 ][ 3 ] * mat2.m[ 3 ][ 3 ] + mat1.m[ 3 ][ 4 ] * mat2.m[ 4 ][ 3 ];
            tempMat.m[ 3 ][ 4 ] = mat1.m[ 3 ][ 0 ] * mat2.m[ 0 ][ 4 ] + mat1.m[ 3 ][ 1 ] * mat2.m[ 1 ][ 4 ] + mat1.m[ 3 ][ 2 ] * mat2.m[ 2 ][ 4 ] + mat1.m[ 3 ][ 3 ] * mat2.m[ 3 ][ 4 ] + mat1.m[ 3 ][ 4 ] * mat2.m[ 4 ][ 4 ];

            tempMat.m[ 4 ][ 0 ] = mat1.m[ 4 ][ 0 ] * mat2.m[ 0 ][ 0 ] + mat1.m[ 4 ][ 1 ] * mat2.m[ 1 ][ 0 ] + mat1.m[ 4 ][ 2 ] * mat2.m[ 2 ][ 0 ] + mat1.m[ 4 ][ 3 ] * mat2.m[ 3 ][ 0 ] + mat1.m[ 4 ][ 4 ] * mat2.m[ 4 ][ 0 ];
            tempMat.m[ 4 ][ 1 ] = mat1.m[ 4 ][ 0 ] * mat2.m[ 0 ][ 1 ] + mat1.m[ 4 ][ 1 ] * mat2.m[ 1 ][ 1 ] + mat1.m[ 4 ][ 2 ] * mat2.m[ 2 ][ 1 ] + mat1.m[ 4 ][ 3 ] * mat2.m[ 3 ][ 1 ] + mat1.m[ 4 ][ 4 ] * mat2.m[ 4 ][ 1 ];
            tempMat.m[ 4 ][ 2 ] = mat1.m[ 4 ][ 0 ] * mat2.m[ 0 ][ 2 ] + mat1.m[ 4 ][ 1 ] * mat2.m[ 1 ][ 2 ] + mat1.m[ 4 ][ 2 ] * mat2.m[ 2 ][ 2 ] + mat1.m[ 4 ][ 3 ] * mat2.m[ 3 ][ 2 ] + mat1.m[ 4 ][ 4 ] * mat2.m[ 4 ][ 2 ];
            tempMat.m[ 4 ][ 3 ] = mat1.m[ 4 ][ 0 ] * mat2.m[ 0 ][ 3 ] + mat1.m[ 4 ][ 1 ] * mat2.m[ 1 ][ 3 ] + mat1.m[ 4 ][ 2 ] * mat2.m[ 2 ][ 3 ] + mat1.m[ 4 ][ 3 ] * mat2.m[ 3 ][ 3 ] + mat1.m[ 4 ][ 4 ] * mat2.m[ 4 ][ 3 ];
            tempMat.m[ 4 ][ 4 ] = mat1.m[ 4 ][ 0 ] * mat2.m[ 0 ][ 4 ] + mat1.m[ 4 ][ 1 ] * mat2.m[ 1 ][ 4 ] + mat1.m[ 4 ][ 2 ] * mat2.m[ 2 ][ 4 ] + mat1.m[ 4 ][ 3 ] * mat2.m[ 3 ][ 4 ] + mat1.m[ 4 ][ 4 ] * mat2.m[ 4 ][ 4 ];

            return tempMat;
        }

        ColorMatrix operator*( const ColorMatrix& mat ) const
        {
            return Multiply( mat );
        }

        /** Merge a matrix with this matrix */
        Status MergeMatrix( const ColorMatrix& mat, MatrixOrder order = MatrixOrderPrepend )
        {
            *this = Multiply( mat, order );
            return SetStatus( Ok );
        }

        /** Add a matrix to this matrix */
        ColorMatrix operator+( const ColorMatrix& mat ) const
        {
            ColorMatrix tempMat;
            for ( size_t i = 0; i < 25; i++ )
                tempMat._m[ i ] = _m[ i ] + mat._m[ i ];
            return tempMat;
        }

        /** Subtract a matrix from this matrix */
        ColorMatrix operator-( const ColorMatrix& mat ) const
        {
            ColorMatrix tempMat;
            for ( size_t i = 0; i < 25; i++ )
                tempMat._m[ i ] = _m[ i ] - mat._m[ i ];
            return tempMat;
        }

        /** Multiply the matrix by a scalar */
        ColorMatrix operator*( REAL scalar ) const
        {
            ColorMatrix tempMat;
            for ( size_t i = 0; i < 25; i++ )
                tempMat._m[ i ] = _m[ i ] * scalar;
            return tempMat;
        }

        /** Transform a 1x4 vector by the matrix */
        inline void transformVector( REAL* v ) const
        {
            if ( v )
            {
                REAL temp[ 4 ];
                temp[ 0 ] = v[ 0 ] * m[ 0 ][ 0 ] + v[ 1 ] * m[ 1 ][ 0 ] + v[ 2 ] * m[ 2 ][ 0 ] + v[ 3 ] * m[ 3 ][ 0 ] + m[ 4 ][ 0 ];
                temp[ 1 ] = v[ 0 ] * m[ 0 ][ 1 ] + v[ 1 ] * m[ 1 ][ 1 ] + v[ 2 ] * m[ 2 ][ 1 ] + v[ 3 ] * m[ 3 ][ 1 ] + m[ 4 ][ 1 ];
                temp[ 2 ] = v[ 0 ] * m[ 0 ][ 2 ] + v[ 1 ] * m[ 1 ][ 2 ] + v[ 2 ] * m[ 2 ][ 2 ] + v[ 3 ] * m[ 3 ][ 2 ] + m[ 4 ][ 2 ];
                temp[ 3 ] = v[ 0 ] * m[ 0 ][ 3 ] + v[ 1 ] * m[ 1 ][ 3 ] + v[ 2 ] * m[ 2 ][ 3 ] + v[ 3 ] * m[ 3 ][ 3 ] + m[ 4 ][ 3 ];

                memcpy( v, temp, sizeof( REAL ) * 4 );
            }
        }

        /** Multiply a Color object by this matrix */
        Status Multiply( Color& col ) const
        {
            // Get the R,G,B,A components of the color:
            int    R = col.GetR();
            int    G = col.GetG();
            int    B = col.GetB();
            int    A = col.GetA();

            int R2 = AggOO::IClamp( R * m[ 0 ][ 0 ] + G * m[ 1 ][ 0 ] + B * m[ 2 ][ 0 ] + A * m[ 3 ][ 0 ] + m[ 4 ][ 0 ], 0, 255 );
            int G2 = AggOO::IClamp( R * m[ 0 ][ 1 ] + G * m[ 1 ][ 1 ] + B * m[ 2 ][ 1 ] + A * m[ 3 ][ 1 ] + m[ 4 ][ 1 ], 0, 255 );
            int B2 = AggOO::IClamp( R * m[ 0 ][ 2 ] + G * m[ 1 ][ 2 ] + B * m[ 2 ][ 2 ] + A * m[ 3 ][ 2 ] + m[ 4 ][ 2 ], 0, 255 );
            int A2 = AggOO::IClamp( R * m[ 0 ][ 3 ] + G * m[ 1 ][ 3 ] + B * m[ 2 ][ 3 ] + A * m[ 3 ][ 3 ] + m[ 4 ][ 3 ], 0, 255 );

            col = Color( A2, R2, G2, B2 );

            return SetStatus( Ok );
        }

        static Status Multiply( Color& col, const ColorMatrix* mtx )
        {
            // Get the R,G,B,A components of the color:
            int    R = col.GetR();
            int    G = col.GetG();
            int    B = col.GetB();
            int    A = col.GetA();

            int R2 = AggOO::IClamp( R * mtx->m[ 0 ][ 0 ] + G * mtx->m[ 1 ][ 0 ] + B * mtx->m[ 2 ][ 0 ] + A * mtx->m[ 3 ][ 0 ] + mtx->m[ 4 ][ 0 ], 0, 255 );
            int G2 = AggOO::IClamp( R * mtx->m[ 0 ][ 1 ] + G * mtx->m[ 1 ][ 1 ] + B * mtx->m[ 2 ][ 1 ] + A * mtx->m[ 3 ][ 1 ] + mtx->m[ 4 ][ 1 ], 0, 255 );
            int B2 = AggOO::IClamp( R * mtx->m[ 0 ][ 2 ] + G * mtx->m[ 1 ][ 2 ] + B * mtx->m[ 2 ][ 2 ] + A * mtx->m[ 3 ][ 2 ] + mtx->m[ 4 ][ 2 ], 0, 255 );
            int A2 = AggOO::IClamp( R * mtx->m[ 0 ][ 3 ] + G * mtx->m[ 1 ][ 3 ] + B * mtx->m[ 2 ][ 3 ] + A * mtx->m[ 3 ][ 3 ] + mtx->m[ 4 ][ 3 ], 0, 255 );

            col = Color( A2, R2, G2, B2 );

            return Ok;
        }

        /** Multiply a Color object by this matrix */
        Color operator*( const Color& col ) const
        {
            Color c = col;
            Multiply( c );
            return c;
        }

        /** Multiply individual color components by this matrix */
        Status Multiply( BYTE& r, BYTE& g, BYTE& b, BYTE& a ) const
        {
            // Get the R,G,B,A components of the color:
            int    R = r;
            int    G = g;
            int    B = b;
            int    A = a;

            r = AggOO::IClamp( R * m[ 0 ][ 0 ] + G * m[ 1 ][ 0 ] + B * m[ 2 ][ 0 ] + A * m[ 3 ][ 0 ] + m[ 4 ][ 0 ], 0, 255 );
            g = AggOO::IClamp( R * m[ 0 ][ 1 ] + G * m[ 1 ][ 1 ] + B * m[ 2 ][ 1 ] + A * m[ 3 ][ 1 ] + m[ 4 ][ 1 ], 0, 255 );
            b = AggOO::IClamp( R * m[ 0 ][ 2 ] + G * m[ 1 ][ 2 ] + B * m[ 2 ][ 2 ] + A * m[ 3 ][ 2 ] + m[ 4 ][ 2 ], 0, 255 );
            a = AggOO::IClamp( R * m[ 0 ][ 3 ] + G * m[ 1 ][ 3 ] + B * m[ 2 ][ 3 ] + A * m[ 3 ][ 3 ] + m[ 4 ][ 3 ], 0, 255 );

            return SetStatus( Ok );
        }

        static Status Multiply( BYTE& r, BYTE& g, BYTE& b, BYTE& a, const ColorMatrix* mtx )
        {
            // Get the R,G,B,A components of the color:
            int    R = r;
            int    G = g;
            int    B = b;
            int    A = a;

            r = AggOO::IClamp( R * mtx->m[ 0 ][ 0 ] + G * mtx->m[ 1 ][ 0 ] + B * mtx->m[ 2 ][ 0 ] + A * mtx->m[ 3 ][ 0 ] + mtx->m[ 4 ][ 0 ], 0, 255 );
            g = AggOO::IClamp( R * mtx->m[ 0 ][ 1 ] + G * mtx->m[ 1 ][ 1 ] + B * mtx->m[ 2 ][ 1 ] + A * mtx->m[ 3 ][ 1 ] + mtx->m[ 4 ][ 1 ], 0, 255 );
            b = AggOO::IClamp( R * mtx->m[ 0 ][ 2 ] + G * mtx->m[ 1 ][ 2 ] + B * mtx->m[ 2 ][ 2 ] + A * mtx->m[ 3 ][ 2 ] + mtx->m[ 4 ][ 2 ], 0, 255 );
            a = AggOO::IClamp( R * mtx->m[ 0 ][ 3 ] + G * mtx->m[ 1 ][ 3 ] + B * mtx->m[ 2 ][ 3 ] + A * mtx->m[ 3 ][ 3 ] + mtx->m[ 4 ][ 3 ], 0, 255 );

            return Ok;
        }

        ////////////////////////////////////////////////////////////////////////

        /** Scale the color components of the matrix */
        Status Scale( REAL scaleR, REAL scaleG, REAL scaleB, REAL scaleA = 1.0, MatrixOrder order = MatrixOrderPrepend );
        inline Status ScaleColors( REAL scaleVal, MatrixOrder order = MatrixOrderPrepend )
        {
            return Scale( scaleVal, scaleVal, scaleVal, 1.0, order );
        }
        inline Status ScaleOpacity( REAL scaleVal, MatrixOrder order = MatrixOrderPrepend )
        {
            return Scale( 1.0, 1.0, 1.0, scaleVal, order );
        }

        /** Translate the color components of the matrix */
        Status Translate( REAL transR, REAL transG, REAL transB, REAL transA = 0.0, MatrixOrder order = MatrixOrderPrepend );
        inline Status TranslateColors( REAL transVal, MatrixOrder order = MatrixOrderPrepend )
        {
            return Translate( transVal, transVal, transVal, 0.0, order );
        }
        inline Status TranslateOpacity( REAL transVal, MatrixOrder order = MatrixOrderPrepend )
        {
            return Translate( 1.0, 1.0, 1.0, transVal, order );
        }

    protected:
        /** Rotate the matrix about a color axis.
            @note   The color of the axis is unchanged, while the others are
                    rotated in color space.
        */
        Status rotateColor( REAL phi, int x, int y, MatrixOrder order = MatrixOrderPrepend );

    public:
        inline Status RotateRed( REAL phi, MatrixOrder order = MatrixOrderPrepend )
        {
            return rotateColor( phi, 2, 1, order );
        }
        inline Status RotateGreen( REAL phi, MatrixOrder order = MatrixOrderPrepend )
        {
            return rotateColor( phi, 0, 2, order );
        }
        inline Status RotateBlue( REAL phi, MatrixOrder order = MatrixOrderPrepend )
        {
            return rotateColor( phi, 1, 0, order );
        }

    protected:
        /** Shear the matrix in one of the color planes */
        Status shearColor( int x, int y1, REAL col1, int y2, REAL col2, MatrixOrder order = MatrixOrderPrepend );

    public:
        inline Status ShearRed( REAL g, REAL b, MatrixOrder order = MatrixOrderPrepend )
        {
            return shearColor( 0, 1, g, 2, b, order );
        }
        inline Status ShearGreen( REAL r, REAL b, MatrixOrder order = MatrixOrderPrepend )
        {
            return shearColor( 1, 0, r, 2, b, order );
        }
        inline Status ShearBlue( REAL r, REAL g, MatrixOrder order = MatrixOrderPrepend )
        {
            return shearColor( 2, 0, r, 1, g, order );
        }

        /** Set the saturation of the matrix
            @remark     Saturation of 0.0 yields black & white, 1.0 is neutral.
        */
        Status SetSaturation( REAL sat, MatrixOrder order = MatrixOrderPrepend );

        /** Rotate the hue about the gray axis
            @remark     Luminance is fixed, other colors are rotated.
        */
        Status RotateHue( REAL phi );

        /** Scale the tint
            @param  phi         [-180..180, 0 = blue]
            @param  amount      [-1.0..1.0, 0 = neutral]
        */
        Status SetTint( REAL phi, REAL amount );

    private:
        // Matrices for adjusting the hue
        static ColorMatrixExt preHue;
        static ColorMatrixExt postHue;
        static bool mInitialized;
        void initHue();
    };

    /** Multiply a Color object by a ColorMatrix */
    inline Color operator*( const Color& col, const ColorMatrix& mat )
    {
        // Get the R,G,B,A components of the color:
        int    R = col.GetR();
        int    G = col.GetG();
        int    B = col.GetB();
        int    A = col.GetA();

        int R2 = AggOO::IClamp( R * mat[ 0 ][ 0 ] + G * mat[ 1 ][ 0 ] + B * mat[ 2 ][ 0 ] + A * mat[ 3 ][ 0 ] + mat[ 4 ][ 0 ], 0, 255 );
        int G2 = AggOO::IClamp( R * mat[ 0 ][ 1 ] + G * mat[ 1 ][ 1 ] + B * mat[ 2 ][ 1 ] + A * mat[ 3 ][ 1 ] + mat[ 4 ][ 1 ], 0, 255 );
        int B2 = AggOO::IClamp( R * mat[ 0 ][ 2 ] + G * mat[ 1 ][ 2 ] + B * mat[ 2 ][ 2 ] + A * mat[ 3 ][ 2 ] + mat[ 4 ][ 2 ], 0, 255 );
        int A2 = AggOO::IClamp( R * mat[ 0 ][ 3 ] + G * mat[ 1 ][ 3 ] + B * mat[ 2 ][ 3 ] + A * mat[ 3 ][ 3 ] + mat[ 4 ][ 3 ], 0, 255 );

        return Color( A2, R2, G2, B2 );
    }

    // NOTE: in order to use the color matrix, there will need to be something like a
    //      span_generator implemented to multiply the colors appropriatly.  I'm not yet
    //      sure how to pull this off, as it will also be necessary to maintain the current
    //      span generators.

} // namespace AggOO
#endif  // AGGOOCOLORMATRIX_H_
