
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
// $Id: AggOOBrush.cpp,v 1.10 2010/03/12 04:20:59 dratek Exp $
//
// AggOOBrush.cpp
//

#include "AggOOBrush.h"
#include "AggOOImage.h"
#include "AggOOMath.h"
#include "AggOOColorExt.h"
#include "AggOOStyleHandler.h"
#include "AggOOStyleHandlerGradient.h"
#include "AggOOStyleHandlerImage.h"

#include <algorithm>
#include <sstream>

namespace AggOO
{
    //--------------------------------------------------------------------------
    // class SolidBrush
    //--------------------------------------------------------------------------

    //Clone---------------------------------------------------------------------
    Brush* SolidBrush::Clone() const
    {
        Brush* newBrush = new SolidBrush( Color( mARGB ) );
        return newBrush;
    }

    ////ToString
    //STRING SolidBrush::ToString() const
    //{
    //    std::stringstream stream;
    //    Color color;
    //    GetColor( &color );
    //    stream << "SolidBrush: color = " << color;
    //    return stream.str();
    //}

    //CreateStyleHandler--------------------------------------------------------
    //StyleHandler* SolidBrush::CreateStyleHandler( const Matrix* gfxMtx ) const
    //template < typename PixFmt >
    //StyleHandlerPixFmt< PixFmt >* SolidBrush::CreateStyleHandler( const Matrix* gfxMtx ) const
    //{
    //    // Note: This only works when the color type is agg::rgba8, such as for
    //    // 24bpp or 32 bpp formats.  Otherwise, the AbstractPixelFormat has a
    //    // method for creating the style handler which should be used instead.
    //    Color col;
    //    GetColor( &col );
    //    SolidStyleHandler<PixFmt>* handler = new SolidStyleHandler< PixFmt >( ColorExt::toRGBA( col ) );
    //    return handler;
    //}

    //--------------------------------------------------------------------------
    // class TextureBrush
    //--------------------------------------------------------------------------
    TextureBrush::TextureBrush( Image* image, WrapMode wrapMode )
        : Brush( BrushTypeTextureFill, Ok ),
          mImage( image->Clone() ),
          mWrapMode( wrapMode )
    {
    }

    TextureBrush::~TextureBrush()
    {
        delete mImage;
    }

    //Clone---------------------------------------------------------------------
    Brush* TextureBrush::Clone() const
    {
        TextureBrush* ret = new TextureBrush( mImage, mWrapMode );
        ret->SetTransform( &mMatrix );
        return ret;
    }

    //GetTransform--------------------------------------------------------------
    Status TextureBrush::GetTransform( Matrix* matrix ) const
    {
        *matrix = mMatrix;
        return SetStatus( Ok );
    }

    //! Set the transformation matrix to a specified matrix
    Status TextureBrush::SetTransform( const Matrix* matrix )
    {
        mMatrix = *matrix;
        return SetStatus( Ok );
    }

    //ResetTransform------------------------------------------------------------
    Status TextureBrush::ResetTransform(void)
    {
        mMatrix.Reset();
        return SetStatus( Ok );
    }

    //MultiplyTransform---------------------------------------------------------
    Status TextureBrush::MultiplyTransform( Matrix* matrix, MatrixOrder order )
    {
        return SetStatus( mMatrix.Multiply( matrix, order ) );
    }

    //RotateTransform-----------------------------------------------------------
    Status TextureBrush::RotateTransform( REAL angle, MatrixOrder order )
    {
        return SetStatus( mMatrix.Rotate( angle, order ) );
    }

    //ScaleTransform------------------------------------------------------------
    Status TextureBrush::ScaleTransform( REAL sx, REAL sy, MatrixOrder order )
    {
        return SetStatus( mMatrix.Scale( sx, sy, order ) );
    }

    //TranslateTransform--------------------------------------------------------
    Status TextureBrush::TranslateTransform( REAL dx, REAL dy, MatrixOrder order )
    {
        return SetStatus( mMatrix.Translate( dx, dy, order ) );
    }

    //CreateStyleHandler--------------------------------------------------------
    //StyleHandler* TextureBrush::CreateStyleHandler( const Matrix* gfxMtx ) const
    //template < typename PixFmt >
    //StyleHandlerPixFmt< PixFmt >* TextureBrush::CreateStyleHandler( const Matrix* gfxMtx ) const
    //{
    //    //Color col( 255, 200, 120, 255 );
    //    //SolidStyleHandler* handler = new SolidStyleHandler( ColorExt::toRGBA8( col ) );
    //    //return handler;
    //    return NULL;
    //}

    //ToString
    STRING TextureBrush::ToString() const
    {
        std::stringstream stream;
        stream << "TextureBrush: wrap mode = " << mWrapMode << ", matrix = " << mMatrix;
        return stream.str();
    }

    //--------------------------------------------------------------------------
    // class LinearGradientBrush
    //--------------------------------------------------------------------------
    LinearGradientBrush::LinearGradientBrush( const PointF& point1, const PointF& point2, const Color& color1, const Color& color2 )
        : Brush( BrushTypeLinearGradient, Ok ),
          mGradientStyle( GradientStyleLinear ),
          mWrapMode( WrapModeClamp ),
          mGradientInterpolation( GradientInterpolationLinear )
    {
        LinearGradientMode mode = LinearGradientModeBackwardDiagonal;
        if ( point1.X == point2.X )
            mode = LinearGradientModeVertical;
        else if ( point1.Y == point2.Y )
            mode = LinearGradientModeHorizontal;
        else if ( ( point2.Y < point1.Y && point1.X < point2.X ) || ( point1.Y < point2.Y && point2.X < point1.X ) )
            mode = LinearGradientModeForwardDiagonal;

        createBrush( RectF( point1.X, point1.Y, point2.X - point1.X, point2.Y - point1.Y ), color1, color2, mode );
    }

    LinearGradientBrush::LinearGradientBrush( const Point& point1, const Point& point2, const Color& color1, const Color& color2 )
        : Brush( BrushTypeLinearGradient, Ok ),
          mGradientStyle( GradientStyleLinear ),
          mWrapMode( WrapModeClamp ),
          mGradientInterpolation( GradientInterpolationLinear )
    {
        LinearGradientMode mode = LinearGradientModeBackwardDiagonal;
        if ( point1.X == point2.X )
            mode = LinearGradientModeVertical;
        else if ( point1.Y == point2.Y )
            mode = LinearGradientModeHorizontal;
        else if ( ( point2.Y < point1.Y && point1.X < point2.X ) || ( point1.Y < point2.Y && point2.X < point1.X ) )
            mode = LinearGradientModeForwardDiagonal;

        createBrush( RectF( point1.X, point1.Y, point2.X - point1.X, point2.Y - point1.Y ), color1, color2, mode );
    }

    LinearGradientBrush::LinearGradientBrush( const RectF& rect, const Color& color1, const Color& color2, LinearGradientMode mode )
        : Brush( BrushTypeLinearGradient, Ok ),
          mGradientStyle( GradientStyleLinear ),
          mWrapMode( WrapModeClamp ),
          mGradientInterpolation( GradientInterpolationLinear )
    {
        createBrush( rect, color1, color2, mode );
    }

    LinearGradientBrush::LinearGradientBrush( const Rect& rect, const Color& color1, const Color& color2, LinearGradientMode mode )
        : Brush( BrushTypeLinearGradient, Ok ),
          mGradientStyle( GradientStyleLinear ),
          mWrapMode( WrapModeClamp ),
          mGradientInterpolation( GradientInterpolationLinear )
    {
        createBrush( rect, color1, color2, mode );
    }

    //LinearGradientBrush::LinearGradientBrush( const RectF& rect, const Color& color1, const Color& color2, REAL angle, BOOL isAngleScalable )
    //    : Brush( BrushTypeLinearGradient, Ok )
    //      mGradientStyle( GradientStyleLinear ),
    //      mWrapMode( WrapModeClamp ),
    //      mGradientInterpolation( GradientInterpolationLinear )
    //{
    //}

    //LinearGradientBrush::LinearGradientBrush( const Rect& rect, const Color& color1, const Color& color2, REAL angle, BOOL isAngleScalable )
    //    : Brush( BrushTypeLinearGradient, Ok )
    //      mGradientStyle( GradientStyleLinear ),
    //      mWrapMode( WrapModeClamp ),
    //      mGradientInterpolation( GradientInterpolationLinear )
    //{
    //}

    //createBrush---------------------------------------------------------------
    Status LinearGradientBrush::createBrush( const RectF& bounds, const Color& color1, const Color& color2, LinearGradientMode mode )
    {
        mGradientMode = mode;
        mBounds = bounds;

        REAL angle = 0.0;
        PointF startPos( bounds.X, bounds.Y ),
               endPos( bounds.GetRight(), bounds.GetBottom() );
        switch ( mode )
        {
        case LinearGradientModeVertical:
            mGradientLength = bounds.Height;
            angle = ( bounds.Height < 0.0 ) ? 270.0 : 90.0;
            break;

        case LinearGradientModeForwardDiagonal:
            // Need to change the start and end points
            startPos = PointF( bounds.X, bounds.GetBottom() );
            endPos   = PointF( bounds.GetRight(), bounds.Y );

        case LinearGradientModeBackwardDiagonal:
            // Calculate the length and the angle in one go:
            RectToPolar( &angle, &mGradientLength, endPos.X, endPos.Y, startPos.X, startPos.Y );

            // Convert the angle to degrees (kinda wasteful, since it will
            // eventually be converted back to radians...
            angle = agg::rad2deg( angle );
            break;

        case LinearGradientModeHorizontal:
        default:
            mGradientLength = bounds.Width;
            angle = ( bounds.Width < 0.0 ) ? 180.0 : 0.0;
            break;
        }

        mColors.resize( 2 );
        if ( mColors.size() )
        {
            mColors[ 0 ] = GradientColorItem( color1, 0.0 );
            mColors[ 1 ] = GradientColorItem( color2, 1.0 );
        }

        buildGradientLUT();

        // Set the initial matrix transformations for the brush to position and
        // rotate the gradient.  The user may modify this directly, using the
        // transformation methods, but this will setup the initial orientation.
        mMatrix.Reset();
        mMatrix.Translate( startPos.X, startPos.Y );
        mMatrix.Rotate( angle );

        return Ok;
    }

    //SetLinearColors-----------------------------------------------------------
    Status LinearGradientBrush::SetLinearColors( const Color& color1, const Color& color2 )
    {
        // This changes the first and last color.  If there are more colors in
        // the gradient, they are left alone.
        if ( mColors.size() < 1 )
            mColors.resize( 2 );
        if ( mColors.size() < 1 )
            return InsufficientBuffer;

        mColors[ 0 ].mColor = color1;
        mColors[ mColors.size() - 1 ].mColor = color2;

        return buildGradientLUT();
    }

    //GetLinearColors-----------------------------------------------------------
    Status LinearGradientBrush::GetLinearColors( Color* colors ) const
    {
        if ( !colors || mColors.size() < 1 )  return InvalidParameter;
        colors[ 0 ] = mColors[ 0 ].mColor;
        colors[ 1 ] = mColors[ mColors.size() - 1 ].mColor;

        return Ok;
    }

    //Clone---------------------------------------------------------------------
    Brush* LinearGradientBrush::Clone() const
    {
        if ( mColors.size() < 1 )
            return 0;
        LinearGradientBrush* ret = new LinearGradientBrush( mBounds, mColors[ 0 ].mColor, mColors[ mColors.size() - 1 ].mColor, mGradientMode );
        ret->mColors.assign( mColors.begin(), mColors.end() );
        ret->SetTransform( &mMatrix );
        ret->SetGradientStyle( mGradientStyle );
        ret->SetGradientInterpolationMode( mGradientInterpolation );
        ret->SetWrapMode( mWrapMode );
        return ret;
    }

    //GetInterpolationColorCount------------------------------------------------
    INT LinearGradientBrush::GetInterpolationColorCount(void) const
    {
        return mColors.size();
    }

    //GetInterpolationColors----------------------------------------------------
    Status LinearGradientBrush::GetInterpolationColors( Color* presetColors, REAL* blendPositions, INT count ) const
    {
        if ( !presetColors || !blendPositions )
            return InvalidParameter;

        int maxColors = min( count, (INT)mColors.size() );
        for ( int i = 0; i < maxColors; i++ )
        {
            presetColors[ i ]   = mColors[ i ].mColor;
            blendPositions[ i ] = mColors[ i ].mPosition;
        }

        return Ok;
    }

    //SetInterpolationColors----------------------------------------------------
    Status LinearGradientBrush::SetInterpolationColors( const Color* presetColors, const REAL* blendPositions, INT count )
    {
        if ( !presetColors || !blendPositions )
            return InvalidParameter;

        mColors.resize( count );
        int maxColors = count;
        for ( int i = 0; i < maxColors; i++ )
        {
            mColors[ i ] = GradientColorItem( presetColors[ i ], blendPositions[ i ] );
        }

        return buildGradientLUT();
    }

    //buildGradientLUT----------------------------------------------------------
    Status LinearGradientBrush::buildGradientLUT()
    {
        // Clear the look-up table:
        mGradientLUT.remove_all();

        //mGammaLUT.gamma( 0.5 );
        mGammaLUT.gamma( 0.5 );

        // Add each color and its position
        for ( mColorIter = mColors.begin(); mColorIter != mColors.end(); mColorIter++ )
        {
            mGradientLUT.add_color( mColorIter->mPosition, agg::rgba8_gamma_dir( ColorExt::toRGBA8( mColorIter->mColor ), mGammaLUT ) );
//            mGradientLUT.add_color( mColorIter->mPosition, agg::rgba8( ColorExt::convertColor( mColorIter->mColor ) ) );
        }

        // Generate the look-up table:
        mGradientLUT.build_lut( mGradientInterpolation == GradientInterpolationStep );

        return Ok;
    }



    //GetTransform--------------------------------------------------------------
    Status LinearGradientBrush::GetTransform( Matrix* matrix ) const
    {
        *matrix = mMatrix;
        return SetStatus( Ok );
    }

    //! Set the transformation matrix to a specified matrix
    Status LinearGradientBrush::SetTransform( const Matrix* matrix )
    {
        mMatrix = *matrix;
        return SetStatus( Ok );
    }

    //ResetTransform------------------------------------------------------------
    Status LinearGradientBrush::ResetTransform(void)
    {
        return SetStatus( mMatrix.Reset() );
    }

    //MultiplyTransform---------------------------------------------------------
    Status LinearGradientBrush::MultiplyTransform( Matrix* matrix, MatrixOrder order )
    {
        return SetStatus( mMatrix.Multiply( matrix, order ) );
    }

    //RotateTransform-----------------------------------------------------------
    Status LinearGradientBrush::RotateTransform( REAL angle, MatrixOrder order )
    {
        return SetStatus( mMatrix.Rotate( angle, order ) );
    }

    //ScaleTransform------------------------------------------------------------
    Status LinearGradientBrush::ScaleTransform( REAL sx, REAL sy, MatrixOrder order )
    {
        return SetStatus( mMatrix.Scale( sx, sy, order ) );
    }

    //TranslateTransform--------------------------------------------------------
    Status LinearGradientBrush::TranslateTransform( REAL dx, REAL dy, MatrixOrder order )
    {
        return SetStatus( mMatrix.Translate( dx, dy, order ) );
    }

    //CreateStyleHandler--------------------------------------------------------
    //StyleHandler* LinearGradientBrush::CreateStyleHandler( const Matrix* gfxMtx ) const
    //template < typename PixFmt >
    //StyleHandlerPixFmt< PixFmt >* LinearGradientBrush::CreateStyleHandler( const Matrix* gfxMtx ) const
    //{
    //    //agg::trans_affine gradMtx( mMatrix.GetAffine() );
    //    //gradMtx.multiply( gfxMtx->GetAffine() );
    //    //GradientStyleHandler* handler = new GradientStyleHandler( &mColors.at( 0 ), mColors.size(), mGradientLength, gradMtx, mGradientStyle, mWrapMode, mGradientInterpolation == GradientInterpolationStep, 0.0, 0.0, data.spiralAngle );
    //    //return handler;
    //    return NULL;
    //}

    //ToString
    STRING LinearGradientBrush::ToString() const
    {
        std::stringstream stream;
        stream << "LinearGradientBrush: wrap mode = " << mWrapMode << std::endl;
        stream << ", bounds = " << mBounds << std::endl;
        stream << ", length = " << mGradientLength << std::endl;
        stream << ", mode = " << mGradientMode << std::endl;
        stream << ", style = " << mGradientStyle << std::endl;
        stream << ", interolation = " << mGradientInterpolation << std::endl;
        stream << ", matrix = " << mMatrix << std::endl;

        std::vector< GradientColorItem >::const_iterator colorIter;
        for ( colorIter = mColors.begin(); colorIter != mColors.end(); colorIter++ )
        {
            stream << "color = " << colorIter->mColor << ", position = " << colorIter->mPosition << std::endl;
        }
        return stream.str();
    }

    //--------------------------------------------------------------------------
    // class LinearGradientBrushExt
    //--------------------------------------------------------------------------

    //Clone---------------------------------------------------------------------
    Brush* LinearGradientBrushExt::Clone() const
    {
        if ( mColors.size() < 1 )
            return 0;
        LinearGradientBrushExt* ret = new LinearGradientBrushExt( mBounds, mColors[ 0 ].mColor, mColors[ mColors.size() - 1 ].mColor, mGradientMode );
        ret->SetPointerEpsilon( mPointerEpsilon );
        ret->mColors.assign( mColors.begin(), mColors.end() );
        ret->SetTransform( &mMatrix );
        ret->SetGradientStyle( mGradientStyle );
        ret->SetGradientInterpolationMode( mGradientInterpolation );
        ret->SetWrapMode( mWrapMode );
        return ret;
    }

    //// Compare color positions
    //bool ColorItemCompare( const LinearGradientBrush::ColorItem& color1, const LinearGradientBrush::ColorItem& color2 )
    //{
    //    return color1.mPosition < color2.mPosition;
    //}

    Status LinearGradientBrushExt::OffsetGradient( REAL offset )
    {
        if ( !mColors.size() )
            return SetStatus( InvalidParameter );

        // This is basically going to rebuild the gradient look-up table, but
        // it will offset the positions first.  It is important to note that
        // the positions need to wrap around the range [0..1] based on the wrap
        // mode.

        // Clear the look-up table:
        mGradientLUT.remove_all();

        //mGammaLUT.gamma( 0.5 );
        mGammaLUT.gamma( 1.0 );

        // Create a temporary array of the offset colors:
        std::vector< GradientColorItem > tempColors;

        // For simplicity, slide the colors in the positive direction.  For a
        // negative offset, the offset becomes offset + 1.0.  This will make
        // interpolating around the boundary easier.
        while ( offset < 0.0 )
            offset += 1.0;

        // Copy the original colors to the temp array, offsetting their
        // positions and making sure the position stays in the range of [0..1].
        // Do the last color outside the loop, as its position will need to be
        // offset by the brush epsilon.
        for ( int i = 0; i < mColors.size() - 1; i++ )
        {
            //GradientColorItem& item = mColors.at( i );
            GradientColorItem item = mColors[ i ];
            item.mPosition = ModRangeInclusive( item.mPosition + offset, 0.0, 1.0 );

            tempColors.push_back( item );
        }

        // Update the last color:
        //GradientColorItem& item = mColors.back();
		GradientColorItem item = mColors.back();
        REAL lastPos = ModRangeInclusive( item.mPosition + offset, 0.0, 1.0 );
        if ( tempColors.size() && ( lastPos >= tempColors.at( 0 ).mPosition ) )
            lastPos -= mPointerEpsilon;
        item.mPosition = lastPos;
        tempColors.push_back( item );

        std::sort( tempColors.begin(), tempColors.end() );

        // Take the first and last colors in the array and find the split color
        // that will be put at positions 0.0 and 1.0.
        GradientColorItem& firstColor = tempColors.front();
        GradientColorItem& lastColor  = tempColors.back();

        REAL ratio = firstColor.mPosition / ( firstColor.mPosition - ( lastColor.mPosition - 1.0 )  );
        Color splitColor = ColorExt::Lerp( ratio, firstColor.mColor, lastColor.mColor );
        GradientColorItem split1( splitColor, 0.0 ), split2( splitColor, 1.0 );
        tempColors.push_back( split1 );
        tempColors.push_back( split2 );

        std::sort( tempColors.begin(), tempColors.end() );

		mColors.assign( tempColors.begin(), tempColors.end() );
/*
        // Add the colors to the look-up table:
        for ( mColorIter = tempColors.begin(); mColorIter != tempColors.end(); mColorIter++ )
        {
            // Calculate the position:
            REAL pos = mColorIter->mPosition;
            mGradientLUT.add_color( pos, agg::rgba8_gamma_dir( ColorExt::toRGBA8( mColorIter->mColor ), mGammaLUT ) );
        }
		//mGradientLUT.add_color( 0.0, agg::rgba8_gamma_dir( ColorExt::toRGBA8( Color::Yellow ), mGammaLUT ) );
		//mGradientLUT.add_color( 1.0, agg::rgba8_gamma_dir( ColorExt::toRGBA8( Color::Cyan ), mGammaLUT ) );

        // Generate the look-up table:
        mGradientLUT.build_lut( mGradientInterpolation == GradientInterpolationStep );
*/

        return Ok;
    }

    //ToString
    STRING LinearGradientBrushExt::ToString() const
    {
        STRING baseString = LinearGradientBrush::ToString();
        std::stringstream stream;
        stream << "epsilon = " << mPointerEpsilon << std::endl;
        baseString += stream.str();

        return baseString;
    }

} // namespace AggOO
