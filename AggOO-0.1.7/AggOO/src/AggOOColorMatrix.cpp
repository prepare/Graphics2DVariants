
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
// $Id: AggOOColorMatrix.cpp,v 1.2 2007/09/06 21:50:47 dratek Exp $
//
// AggOOColorMatrix.cpp
//

#include "AggOOColorMatrix.h"
#include "AggOOMath.h"

namespace AggOO
{
    const ColorMatrix ColorMatrix::IDENTITY( 1, 0, 0, 0, 0,\
                                             0, 1, 0, 0, 0,\
                                             0, 0, 1, 0, 0,\
                                             0, 0, 0, 1, 0,\
                                             0, 0, 0, 0, 1 );

    const ColorMatrix ColorMatrix::ZERO(     0, 0, 0, 0, 0,\
                                             0, 0, 0, 0, 0,\
                                             0, 0, 0, 0, 0,\
                                             0, 0, 0, 0, 0,\
                                             0, 0, 0, 0, 0 );

    ColorMatrixExt ColorMatrixExt::preHue    = ColorMatrix::IDENTITY;
    ColorMatrixExt ColorMatrixExt::postHue   = ColorMatrix::IDENTITY;
    bool        ColorMatrixExt::mInitialized = false;

    // The luminance color weights are close to the values of the NTSC color
    // weights, but these values are preferable.
    const REAL lumR = 0.3086f;
    const REAL lumG = 0.6094f;
    const REAL lumB = 0.0820f;

    // Grayscale color matrix is modified from the GDI+ FAQ (submitted by
    // Gilles Khouzam) to use the NTSC color values.  The version in the FAQ
    // used 0.3, 0.59, and 0.11, so it was close...
    const ColorMatrix ColorMatrix::GRAYSCALE( lumR, lumR, lumR, 0.0, 0.0,\
                                              lumG, lumG, lumG, 0.0, 0.0,\
                                              lumB, lumB, lumB, 0.0, 0.0,\
                                              0.0, 0.0, 0.0, 1.0, 0.0,
                                              0.0, 0.0, 0.0, 0.0, 1.0 );

    const ColorMatrix ColorMatrix::WHITE(   1.0, 1.0, 1.0, 0.0, 0.0,\
                                            1.0, 1.0, 1.0, 0.0, 0.0,\
                                            1.0, 1.0, 1.0, 0.0, 0.0,\
                                            0.0, 0.0, 0.0, 1.0, 0.0,\
                                            0.0, 0.0, 0.0, 0.0, 1.0 );

    const ColorMatrix ColorMatrix::HALF_WHITE( 0.5, 0.5, 0.5, 0.0, 0.0,\
                                            0.5, 0.5, 0.5, 0.0, 0.0,\
                                            0.5, 0.5, 0.5, 0.0, 0.0,\
                                            0.0, 0.0, 0.0, 1.0, 0.0,\
                                            0.0, 0.0, 0.0, 0.0, 1.0 );

    // From http://blogs.techrepublic.com.com/howdoi/?p=120, using values
    // recommended by Microsoft.
    const ColorMatrix ColorMatrix::SEPIA(   0.393, 0.349, 0.272, 0.0, 0.0,\
                                            0.769, 0.686, 0.534, 0.0, 0.0,\
                                            0.189, 0.168, 0.131, 0.0, 0.0,\
                                            0.000, 0.000, 0.000, 1.0, 0.0,\
                                            0.000, 0.000, 0.000, 0.0, 1.0 );

    //initHue-------------------------------------------------------------------
    void ColorMatrixExt::initHue()
    {
        // [CMD 2006-02-22]
        // The green rotation is the angle where the sin is 1 / sqrt( 3 )
        // and the cosine is sqrt( 2 / 3 ).  According to the documentation
        // for QColorMatrix, this is 39.182655, but I came up with
        // 35.26439.  Considering that Sjaak Priester explains that 35.0
        // gave better results than 39.182655, I wonder where the higher
        // value came from...  If you want to use 39.182655, simply
        // change the value of greenRotation below.
        const REAL greenRotation( Deg2Rad( 35.26439 ) );

        if ( !mInitialized )
        {
            mInitialized = true;

            // Rotating the hue requires several matrix multiplications.  To
            // speed things up a bit, use two static matrices (pre and post
            // rotation.)

            // Prepare the preHue matrix
            // Rotate the gray vector in the red plane:
            preHue.RotateRed( Deg2Rad( 45.0 ) );

            // Rotate again in the green plane so it coinsides with the
            // blue axis:
            preHue.RotateGreen( -greenRotation, MatrixOrderPrepend );

            // Shear the blue plane, in order to keep the color luminance
            // constant:
            REAL lum[ 4 ] = { lumR, lumG, lumB, 1.0 };

            // Transform by the luminance vector:
            preHue.transformVector( lum );

            // Calculate the red and green factors:
            REAL shearRed   = lum[ 0 ] / lum[ 2 ];
            REAL shearGreen = lum[ 1 ] / lum[ 2 ];

            // Shear the blue plane:
            preHue.ShearBlue( shearRed, shearGreen, MatrixOrderPrepend );

            // Prepare the postHue matrix, which is actually the inverse of the
            // preHue matrix
            postHue.ShearBlue( -shearRed, -shearGreen );
            postHue.RotateGreen( greenRotation, MatrixOrderPrepend );
            postHue.RotateRed( Deg2Rad( -45.0 ), MatrixOrderPrepend );
        }
    }

    //Scale---------------------------------------------------------------------
    Status ColorMatrixExt::Scale( REAL scaleR, REAL scaleG, REAL scaleB, REAL scaleA, MatrixOrder order )
    {
        ColorMatrix scaleMat = ColorMatrix::IDENTITY;
        scaleMat[ 0 ][ 0 ] = scaleR;
        scaleMat[ 1 ][ 1 ] = scaleG;
        scaleMat[ 2 ][ 2 ] = scaleB;
        scaleMat[ 3 ][ 3 ] = scaleA;

        return MergeMatrix( scaleMat, order );
    }

    //Translate-----------------------------------------------------------------
    Status ColorMatrixExt::Translate( REAL transR, REAL transG, REAL transB, REAL transA, MatrixOrder order )
    {
        ColorMatrix transMat = ColorMatrix::IDENTITY;
        transMat[ 4 ][ 0 ] = transR;
        transMat[ 4 ][ 1 ] = transG;
        transMat[ 4 ][ 2 ] = transB;
        transMat[ 4 ][ 3 ] = transA;

        return MergeMatrix( transMat, order );
    }

    //rotateColor---------------------------------------------------------------
    Status ColorMatrixExt::rotateColor( REAL phi, int x, int y, MatrixOrder order )
    {
        ColorMatrix m = ColorMatrix::IDENTITY;
        m.m[ x ][ x ] = m[ y ][ y ] = cos( phi );

        REAL sinVal = sin( phi );
        m.m[ y ][ x ] = sinVal;
        m.m[ x ][ y ] = -sinVal;
        MergeMatrix( m, order );

        return Ok;
    }

    //shearColor----------------------------------------------------------------
    Status ColorMatrixExt::shearColor( int x, int y1, REAL col1, int y2, REAL col2, MatrixOrder order )
    {
        ColorMatrix m = ColorMatrix::IDENTITY;

        m.m[ y1 ][ x ] = col1;
        m.m[ y2 ][ x ] = col2;

        MergeMatrix( m, order );

        return Ok;
    }

    //SetSaturation-------------------------------------------------------------
    Status ColorMatrixExt::SetSaturation( REAL sat, MatrixOrder order )
    {
        // If the saturation is 1.0, then this matrix remains unchanged.
        // If the saturation is 0.0, each color is scaled by its luminance
        REAL satComplement  = 1.0 - sat;
        REAL satComplementR = lumR * satComplement;
        REAL satComplementG = lumG * satComplement;
        REAL satComplementB = lumB * satComplement;

        // Create the matrix:
        ColorMatrix m(
            satComplementR + sat, satComplementR, satComplementR, 0.0, 0.0,
            satComplementG, satComplementG + sat, satComplementG, 0.0, 0.0,
            satComplementB, satComplementB, satComplementB + sat, 0.0, 0.0,
            0.0, 0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 1.0 );

        MergeMatrix( m, order );
        return Ok;
    }

    //RotateHue-----------------------------------------------------------------
    Status ColorMatrixExt::RotateHue( REAL phi )
    {
        // Make sure the hue matrices have been initialized:
        initHue();
        assert( mInitialized );

        // Rotate the gray vector to the blue axis:
        MergeMatrix( preHue, MatrixOrderPrepend );

        // Rotate around the blue axis:
        RotateBlue( phi, MatrixOrderPrepend );

        MergeMatrix( postHue, MatrixOrderPrepend );
        return Ok;
    }

    //SetTint-------------------------------------------------------------------
    Status ColorMatrixExt::SetTint( REAL phi, REAL amount )
    {
        // Make sure the hue matrices have been initialized:
        initHue();
        assert( mInitialized );

        // Rotate the hue:
        RotateHue( phi );

        // Scale the blue:
        Scale( 1.0, 1.0, 1.0 + amount, 1.0, MatrixOrderPrepend );

        // Rotate the hue back:
        RotateHue( -phi );
        return Ok;
    }

} // namespace AggOO
