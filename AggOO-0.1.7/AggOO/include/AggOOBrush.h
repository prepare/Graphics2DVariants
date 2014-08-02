
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
// $Id: AggOOBrush.h,v 1.9 2010/03/12 04:20:57 dratek Exp $
//
// AggOOBrush.h -- Brush classes used by AggOO.
//
// Author       : Chad M. Draper
// Date         : September 8, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-08      Initial implementation
//

#ifndef AggOOBrush_H_
#define AggOOBrush_H_

#include "AggOOEnums.h"
#include "AggOOBase.h"
#include "AggOOTypes.h"
#include "AggOOPixelFormats.h"
#include "AggOOPrereqs.h"
#include "AggOOColor.h"
#include "AggOOColorExt.h"
#include "AggOOMatrix.h"

#include <vector>
//#include <agg_gradient_lut.h>
#include <agg_gamma_lut.h>
//#include <agg_math.h>
#include "AggOOGradientLUT.h"

namespace AggOO
{
    class StyleHandler;
    class GradientColorItem;

    //==========================================================================
    /**
        @class Brush

        Brush is the base class for all AggOO brushes (SolidBrush,
        TextureBrush, etc.)
    */
    class _AggOOExport Brush : public Base
    {
    private:
        BrushType   mType;

    protected:
        mutable Status  mLastStatus;

    public:
        friend class Pen;
        friend class Graphics;

        //! Prevent default creation
    protected:
        Brush()
        {
            mType = static_cast< BrushType >( -1 );
            mLastStatus = SetStatus( NotImplemented );
        }
        Brush( BrushType type, Status status = NotImplemented ) : mType( type )
        {
            mLastStatus = SetStatus( status );
        }

        VOID SetType( BrushType type )  { mType = type; }

        /** Set the status of the brush
        */
        Status SetStatus( Status status ) const
        {
            if ( status != Ok )
                return ( mLastStatus = status );
            return status;
        }

        //! Prevent copying
    private:
        Brush( const Brush& src );
        Brush& operator=( const Brush& src );

    public:
        /** Destructor */
        ~Brush(void)
        {
        }

        /** GDI+ seems to store the brush type in the base class.  Since they
            work with this in DLL code, I'm not sure how they do things like
            clone it or retrieve its type.  AggOO will use abstraction, since
            the base class shouldn't be called directly anyway, and otherwise,
            it will appear to be the same.  The brush type, however, can be set
            rather easily, as was done in AggPlus.
        */
        virtual Brush* Clone() const = 0;

        /** Retrieve the brush type
        */
        BrushType GetType() const   { return mType; }

        /** Retrieve the last status message set by the brush
        */
        Status GetLastStatus() const    { return mLastStatus; }

        /** Create a style handler pointer for this brush */
        //virtual StyleHandler* CreateStyleHandler( const Matrix* gfxMtx ) const = 0;

        /** Create a string representation of the brush */
        virtual STRING ToString() const { return ""; }

        // Write to a stream
        inline friend std::ostream& operator<<( std::ostream& stream, const Brush& src )
        {
            std::ostream::sentry opfx(stream);
            if ( !opfx )
                return stream;
            return ( stream << src.ToString() );
        }

    }; // class Brush


    //==========================================================================

    /** @class SolidBrush

        A solid colored brush for filling shapes.
    */
    class _AggOOExport SolidBrush : public Brush
    {
    public:
        friend class Pen;

    private:
        ARGB    mARGB;

    public:
        /** Constructor */
        SolidBrush( const Color& color ) : Brush( BrushTypeSolidColor, Ok ), mARGB( color.GetValue() )
        {
        }

        /** Get the current brush color
        */
        Status GetColor( Color* color ) const
        {
            if ( !color )
                return SetStatus( InvalidParameter );

            color->SetValue( mARGB );
            return mLastStatus;
        }

        /** Set the brush color
        */
        Status SetColor( const Color& color )
        {
            mARGB = color.GetValue();
            return mLastStatus;
        }

        /** Create a clone of this brush
        */
        virtual Brush* Clone() const;

        /** Create a style handler pointer for this brush */
        //StyleHandler* CreateStyleHandler( const Matrix* gfxMtx ) const;

        /** Create a string representation of the brush */
        virtual STRING ToString() const
        {
            std::stringstream stream;
            Color color;
            GetColor( &color );
            stream << "SolidBrush: color = " << color;
            return stream.str();
        }

        /** Create a string representation of the brush */
        //virtual STRING ToString() const;
        //{
        //    STRING result = "";
        //    return result;
        //}

    protected:
        SolidBrush()    { }

        //! Prevent copying
    //private:
        SolidBrush( const SolidBrush& src ) : Brush( BrushTypeSolidColor, Ok ), mARGB( Color::Transparent ) { }
        SolidBrush& operator=( const SolidBrush& src )
        {
            Color color;
            src.GetColor( &color );
            SetColor( color );
            return *this;
        }

    }; // class SolidBrush


    //==========================================================================

    /** @class TextureBrush

        A brush which uses an image texture
    */
    class _AggOOExport TextureBrush : public Brush
    {
    public:
        friend class Pen;
        friend class AbstractPixelFormat;
        template< class PixFmt > friend class SpecificPixelFormat;
        template< class Pixfmt > friend class GraphicsRenderer;

    private:
        Image*      mImage;
        WrapMode    mWrapMode;
        Matrix      mMatrix;
        TextureBrush( const TextureBrush& src );
        TextureBrush& operator=( const TextureBrush& src );

    protected:
        // prevent default construction
        TextureBrush() : Brush( BrushTypeTextureFill, Ok ), mImage( 0 ), mWrapMode( WrapModeTile )
        {
        }

    public:
        /** Constructor */
        TextureBrush( Image* image, WrapMode wrapMode = WrapModeTile );
        /** Destructor */
        ~TextureBrush();

        /** Create a new clone of this brush */
        virtual Brush* Clone() const;

        /** Set the wrapping mode for out-of-bounds regions */
        void SetWrapMode( WrapMode wrapMode )   { mWrapMode = wrapMode; }
        /** Get the wrapping mode */
        WrapMode GetWrapMode() const            { return mWrapMode; }

        /** Get a pointer to the image used to define the brush */
        Image* GetImage(void) const             { return mImage; }

        /** Get the transformation matrix used by the brush */
        Status GetTransform( Matrix* matrix ) const;
        /** Set the transformation matrix to a specified matrix */
        Status SetTransform( const Matrix* matrix );

        /** Restore the brush's transformation matrix to the identity matrix */
        Status ResetTransform(void);

        /** Update the brush's transformation matrix with the product of another
            matrix and itself
        */
        Status MultiplyTransform( Matrix* matrix, MatrixOrder order = MatrixOrderPrepend );

        /** Rotate the brush's transformation matrix */
        Status RotateTransform( REAL angle, MatrixOrder order = MatrixOrderPrepend );

        /** Scale the brush's transformation matrix */
        Status ScaleTransform( REAL sx, REAL sy, MatrixOrder order = MatrixOrderPrepend );

        /** Translate the brush's transformation matrix */
        Status TranslateTransform( REAL dx, REAL dy, MatrixOrder order = MatrixOrderPrepend );

        /** Create a style handler pointer for this brush */
        //StyleHandler* CreateStyleHandler( const Matrix* gfxMtx ) const;

        /** Create a string representation of the brush */
        virtual STRING ToString() const;

    }; // class TextureBrush


    //==========================================================================

    /** @class LinearGradientBrush

        A brush which uses a gradient to fill a path
    */
    class _AggOOExport LinearGradientBrush : public Brush
    {
    public:
        friend class Pen;
        friend class Graphics;
        friend class AbstractPixelFormat;
        template< class PixFmt > friend class SpecificPixelFormat;
        template< class PixFmt > friend class GraphicsRenderer;

    protected:
        //struct ColorItem
        //{
        //    Color   mColor;
        //    REAL    mPosition;

        //    ColorItem()
        //    {
        //        mPosition = 0.0;
        //    }
        //    ColorItem( Color color, REAL pos )
        //    {
        //        mColor = color;
        //        mPosition = pos;
        //    }

        //    bool operator<( const ColorItem& item ) const
        //    {
        //        return mPosition < item.mPosition;
        //    }
        //};

        WrapMode    mWrapMode;
        Matrix      mMatrix;
        std::vector< GradientColorItem >            mColors;
        std::vector< GradientColorItem >::iterator  mColorIter;
        RectF       mBounds;
        REAL        mGradientLength;
        LinearGradientMode mGradientMode;
        GradientStyle mGradientStyle;
        GradientInterpolation mGradientInterpolation;

        // Gamma look-up table
        typedef agg::gamma_lut< agg::int8u, agg::int8u > GammaLutType;
        GammaLutType    mGammaLUT;

        // I'm not sure of a nice and clean way to switch between linear and
        // step interpolation for the gradient, so there will be two gradient
        // look-up tables.  These will be kept in synch, since they are modified
        // together.  Based on the selected gradient type, use the appropriate
        // table.
//        typedef agg::gradient_lut< agg::color_interpolator< agg::rgba8 >, 1024 > GradientLutType;
//        typedef agg::gradient_lut< agg::color_interpolator_step< agg::rgba8 >, 1024 > GradientLutType;
        typedef gradient_lut< agg::rgba8, 1024 > GradientLutType;
        GradientLutType mGradientLUT;

        // Create the brush
        Status createBrush( const RectF& bounds, const Color& color1, const Color& color2, LinearGradientMode mode );

        // Build the gradient look-up table
        Status buildGradientLUT();

		LinearGradientBrush( const LinearGradientBrush& src ) { }
		LinearGradientBrush& operator=( const LinearGradientBrush& src ) { return *this; }

    protected:
        LinearGradientBrush()
        {
        }

        //// Convert an AggOO::Color value to color_type
        //agg::rgba8 convertColor( const Color& color ) const
        //{
        //    return agg::rgba8( color.GetR(), color.GetG(), color.GetB(), color.GetA() );
        //}

    public:
        /** Constructor */
        LinearGradientBrush( const PointF& point1, const PointF& point2, const Color& color1, const Color& color2 );
        /** Constructor */
        LinearGradientBrush( const Point& point1, const Point& point2, const Color& color1, const Color& color2 );
        /** Constructor */
        LinearGradientBrush( const RectF& rect, const Color& color1, const Color& color2, LinearGradientMode mode );
        /** Constructor */
        LinearGradientBrush( const Rect& rect, const Color& color1, const Color& color2, LinearGradientMode mode );

        /** Create a new clone of this brush */
        virtual Brush* Clone() const;

        /** Set the start and end colors */
        Status SetLinearColors( const Color& color1, const Color& color2 );
        /** Get the start and end colors */
        Status GetLinearColors( Color* colors ) const;

        /** Get the wrap mode */
        WrapMode GetWrapMode() const    { return mWrapMode; }
        /** Set the wrap mode */
        Status SetWrapMode( WrapMode mode )
        {
            mWrapMode = mode;
            return Ok;
        }

        /** Get the gradient style (i.e. linear, radial, etc.) */
        GradientStyle GetGradientStyle() const
        {
            return mGradientStyle;
        }
        /** Set the gradient style (i.e. linear, radial, etc.) */
        Status SetGradientStyle( GradientStyle style )
        {
            mGradientStyle = style;
            return Ok;
        }

        /** Get the gradient interpolation mode */
        GradientInterpolation GetGradientInterpolationMode() const
        {
            return mGradientInterpolation;
        }
        /** Set the gradient interpolation mode */
        Status SetGradientInterpolationMode( GradientInterpolation mode )
        {
            mGradientInterpolation = mode;

            // It is necessary to rebuild the LUT:
            buildGradientLUT();

            return Ok;
        }

        /** Get the number of gradient colors */
        INT GetInterpolationColorCount(void) const;
        /** Get the interpolation colors and their positions */
        Status GetInterpolationColors( Color* presetColors, REAL* blendPositions, INT count ) const;
        /** Set the interpolation colors and their positions */
        Status SetInterpolationColors( const Color* presetColors, const REAL* blendPositions, INT count );


        /** Get the transformation matrix used by the brush */
        Status GetTransform( Matrix* matrix ) const;
        /** Set the transformation matrix to a specified matrix */
        Status SetTransform( const Matrix* matrix );

        /** Restore the brush's transformation matrix to the identity matrix */
        Status ResetTransform(void);

        /** Update the brush's transformation matrix with the product of another
            matrix and itself
        */
        Status MultiplyTransform( Matrix* matrix, MatrixOrder order = MatrixOrderPrepend );

        /** Rotate the brush's transformation matrix */
        Status RotateTransform( REAL angle, MatrixOrder order = MatrixOrderPrepend );

        /** Scale the brush's transformation matrix */
        Status ScaleTransform( REAL sx, REAL sy, MatrixOrder order = MatrixOrderPrepend );

        /** Translate the brush's transformation matrix */
        Status TranslateTransform( REAL dx, REAL dy, MatrixOrder order = MatrixOrderPrepend );

        /** Get the bounding rectangle for the brush */
        inline Status GetRectangle( RectF* rect ) const
        {
            if ( !rect )
                return InvalidParameter;
            *rect = mBounds;
            return Ok;
        }
        /** Get the bounding rectangle for the brush */
        inline Status GetRectangle( Rect* rect ) const
        {
            if ( !rect )
                return InvalidParameter;
            *rect = mBounds;
            return Ok;
        }

        /** Data for specific brush held in a union */
        union
        {
            REAL    spiralAngle;
        } data;

        /** @remarks
            // TODO:
            LinearGradientBrush( const RectF& rect, const Color& color1, const Color& color2, REAL angle, BOOL isAngleScalable = FALSE );
            LinearGradientBrush( const Rect& rect, const Color& color1, const Color& color2, REAL angle, BOOL isAngleScalable = FALSE );

            Status GetBlend( REAL* blendFactors, REAL* blendPositions, INT count ) const;
            Status SetBlend( const REAL* blendFactors, const REAL* blendPositions, INT count );
            INT GetBlendCount(void) const;
            Status SetBlendBellShape( REAL focus, REAL scale );
            Status SetBlendTriangularShape( REAL focus, REAL scale );

            BOOL GetGammaCorrection(void) const;
            Status SetGammaCorrection( BOOL useGammaCorrection );
        */

        /** Create a style handler pointer for this brush */
        //StyleHandler* CreateStyleHandler( const Matrix* gfxMtx ) const;

        /** Create a string representation of the brush */
        virtual STRING ToString() const;

    }; // class LinearGradientBrush

    /** @class LinearGradientBrushExt
        Adds extra functionality to the gradient brush
    */
    class _AggOOExport LinearGradientBrushExt : public LinearGradientBrush
    {
    private:
        REAL    mPointerEpsilon;

    protected:
		LinearGradientBrushExt( const LinearGradientBrush& src ) { }
		LinearGradientBrushExt& operator=( const LinearGradientBrushExt& src ) { return *this; }
        LinearGradientBrushExt() : LinearGradientBrush()
        {
        }

    public:
        /** Constructor */
        LinearGradientBrushExt( const PointF& point1, const PointF& point2, const Color& color1, const Color& color2 )
            : LinearGradientBrush( point1, point2, color1, color2 ), mPointerEpsilon( 0.001 )
        {
        }
        /** Constructor */
        LinearGradientBrushExt( const Point& point1, const Point& point2, const Color& color1, const Color& color2 )
            : LinearGradientBrush( point1, point2, color1, color2 ), mPointerEpsilon( 0.001 )
        {
        }
        /** Constructor */
        LinearGradientBrushExt( const RectF& rect, const Color& color1, const Color& color2, LinearGradientMode mode )
            : LinearGradientBrush( rect, color1, color2, mode ), mPointerEpsilon( 0.001 )
        {
        }
        /** Constructor */
        LinearGradientBrushExt( const Rect& rect, const Color& color1, const Color& color2, LinearGradientMode mode )
            : LinearGradientBrush( rect, color1, color2, mode ), mPointerEpsilon( 0.001 )
        {
        }

    public:

        /** Create a new clone of this brush */
        virtual Brush* Clone() const;

        //! Allow the user to offset the gradient.  This is useful when the
        //! gradient colors need to animate, but the brush location doesn't.
        Status OffsetGradient( REAL offset );

        //! Set the pointer epsilon value to prevent pointer positions from
        //! stepping on each other
        Status SetPointerEpsilon( REAL epsilon )
        {
            mPointerEpsilon = epsilon;
            return SetStatus( Ok );
        }
        /** Get the pointer epsilon value */
        REAL GetPointerEpsilon() const  { return mPointerEpsilon; }

        /** @remarks
            // TODO:
            LinearGradientBrush( const RectF& rect, const Color& color1, const Color& color2, REAL angle, BOOL isAngleScalable = FALSE );
            LinearGradientBrush( const Rect& rect, const Color& color1, const Color& color2, REAL angle, BOOL isAngleScalable = FALSE );
        */

        /** Create a string representation of the brush */
        virtual STRING ToString() const;
    };

} // namespace AggOO

#endif  // AggOOBrush_H_
