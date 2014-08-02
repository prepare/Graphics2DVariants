
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
// $Id: AggOOGraphics.cpp,v 1.23 2010/03/12 04:20:59 dratek Exp $
//
// AggOOGraphics.cpp

#include "AggOOGraphics.h"
#include "AggOOGraphicsExt.h"
#include "AggOOBrush.h"
#include "AggOOPen.h"
#include "AggOOImage.h"
#include "AggOOGraphicsPath.h"
#include "AggOOGraphicsPathCompound.h"
//#include "AggOOSVGPath.h"
#include "AggOOPixelFormatConverter.h"
#include "AggOOColorExt.h"
#include "AggOOImageAttributes.h"
#include "AggOOSharedPtr.h"
#include "AggOOAbstractPixelFormat.h"
#include "AggOOAbstractRendererBase.h"
#include "AggOOAbstractScanlineRenderer.h"

#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_renderer_base.h"
#include "agg_renderer_scanline.h"
#include "agg_scanline_p.h"
#include "agg_path_storage.h"
#include "agg_path_storage_integer.h"
#include "agg_conv_stroke.h"
#include "agg_conv_dash.h"
#include "agg_arc.h"
#include "agg_bezier_arc.h"
#include "agg_conv_curve.h"
#include "agg_span_allocator.h"
#include "agg_span_interpolator_linear.h"
#include "agg_span_image_filter_rgb.h"
#include "agg_span_image_filter_rgba.h"
#include "agg_span_image_filter_rgba_plain.h"
#include "agg_span_converter.h"
#include "agg_span_solid.h"
#include "agg_span_pattern_rgba.h"
#include "agg_image_accessors.h"
#include "agg_span_gradient.h"

#include "AggOOStyleHandler.h"

#include <fstream>

namespace AggOO
{
    typedef agg::path_base< agg::vertex_block_storage<int> > int_path_storage;

    //==========================================================================
    // GraphicsRendererBase
    //==========================================================================

    //getBitmapData-------------------------------------------------------------
    BitmapData* GraphicsRendererBase::getBitmapData( const Image* image ) const
    {
        return (BitmapData*)&image->mActiveBitmap;
    }

    //Save----------------------------------------------------------------------
    GraphicsState GraphicsRendererBase::Save()
    {
        // Create the data block to save
        GraphicsStateData data;
        data.matrix             = mMatrix;
//        data.fillMode           = mFillMode;
        data.compositeMode      = mCompositeMode;
        data.compositeQuality   = mCompositeQuality;
        data.interpolationMode  = mInterpolationMode;
        data.pageScale          = mPageScale;
        data.pageUnit           = mPageUnit;
        data.pixelOffsetMode    = mPixelOffsetMode;
        data.renderingOrigin    = mRenderingOrigin;
        data.smoothingMode      = mSmoothingMode;
        data.gammaThreshold     = mGammaThreshold;

        GraphicsState index = (GraphicsState)mGraphicsStates.size();
        mGraphicsStates.push( data );

        return index;
    }

    //Restore-------------------------------------------------------------------
    Status GraphicsRendererBase::Restore( GraphicsState state )
    {
        // Make sure the state is valid:
        if ( state >= mGraphicsStates.size() )
            return InvalidParameter;

        // All states above the restored state are discarded
        while ( mGraphicsStates.size() > state + 1 )
            mGraphicsStates.pop();

        // Restore the top state of the stack:
        GraphicsStateData* topState = &mGraphicsStates.top();
        mMatrix             = topState->matrix;
//        mFillMode           = topState->fillMode;
        mCompositeMode      = topState->compositeMode;
        mCompositeQuality   = topState->compositeQuality;
        mInterpolationMode  = topState->interpolationMode;
        mPageScale          = topState->pageScale;
        mPageUnit           = topState->pageUnit;
        mPixelOffsetMode    = topState->pixelOffsetMode;
        mRenderingOrigin    = topState->renderingOrigin;
        mSmoothingMode      = topState->smoothingMode;
        mGammaThreshold     = topState->gammaThreshold;

        // Remove the state:
        mGraphicsStates.pop();

        return Ok;
    }


    //==========================================================================
    // GraphicsRenderer
    //
    // This is a templatized class, which requires the pixel format in order to
    // create the desired renderer.
    //==========================================================================

    template< class PixFmt >
    class GraphicsRenderer : public GraphicsRendererBase
    {
    private:
        PixFmt      mPixFmt;

        typedef typename PixFmt::color_type         color_type;
        typedef agg::renderer_base< PixFmt >        base_renderer_type;
        typedef agg::span_allocator< agg::rgba8 >   span_alloc_type;
        typedef agg::span_interpolator_linear<>     interpolator_type_linear;

        base_renderer_type    mRendererBase;
        agg::renderer_scanline_aa_solid< base_renderer_type > mRendererAA;
        agg::renderer_scanline_bin_solid< base_renderer_type > mRendererBin;

        // Render the path using a specified brush.  This method will be used
        // for both the draw and fill routines.  Prior to calling this method,
        // the rasterizer needs to be set to either stroke or fill the path.
        Status renderPath( const Brush* brush, const GraphicsPath* path );
        Status renderPathSolidColor( const Brush* brush, const GraphicsPath* path );
        Status renderPathTextureFill( const Brush* brush, const GraphicsPath* path );

        // Render an image into a specified rect region
        Status renderImage( const BitmapData* imageData, const RectF& destRect, const RectF& srcRect, const Matrix* matrix = 0, Unit srcUnit = UnitPixel, ImageAttributes* imageAttributes = NULL, WrapMode wrapMode = WrapModeClamp );

        template< class ImgPixFmt, class ImageAccessor, class SpanGen, class SpanConv >
        Status renderImageClamp( ImgPixFmt imgPixf, interpolator_type_linear interp, Unit srcUnit = UnitPixel, ImageAttributes* attribs = NULL )
        {
            span_alloc_type sa;
            ImageAccessor imgSrc( imgPixf, agg::rgba( 0, 0, 0, 0 ) );
            SpanGen sg( imgPixf, agg::rgba8( 0, 0, 0, 0 ), interp );


            switch ( mSmoothingMode )
            {
            case SmoothingModeHighQuality:
            case SmoothingModeAntiAlias:
                mRasterizer.gamma( agg::gamma_none() );
//                mRasterizer.gamma( agg::gamma_multiply( 2 ) );
                if ( attribs )
                {
                    SpanConv spanAttrib( attribs );
                    agg::span_converter< SpanGen, SpanConv > sc( sg, spanAttrib );
                    agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sc );
                }
                else
                    agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sg );
                break;

            default:
                mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
//                agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sg );
                if ( attribs )
                {
                    SpanConv spanAttrib( attribs );
                    agg::span_converter< SpanGen, SpanConv > sc( sg, spanAttrib );
                    agg::render_scanlines_bin( mRasterizer, mScanline, mRendererBase, sa, sc );
                }
                else
                    agg::render_scanlines_bin( mRasterizer, mScanline, mRendererBase, sa, sg );
                break;
            }

            return Ok;
        }

        template< class ImgPixFmt, class ImageAccessor, class SpanGen, class SpanConv >
        Status renderImageClamp2( ImgPixFmt imgPixf, interpolator_type_linear interp, Unit srcUnit = UnitPixel, ImageAttributes* attribs = NULL )
        {
            span_alloc_type sa;
//            ImageAccessor imgSrc( imgPixf, agg::rgba( 0, 0, 0, 0 ) );
            ImageAccessor imgSrc( imgPixf );
            SpanGen sg( imgSrc, interp );
//            SpanGen sg( imgSrc, 0.4, 0.76 );

            switch ( mSmoothingMode )
            {
            case SmoothingModeHighQuality:
            case SmoothingModeAntiAlias:
                mRasterizer.gamma( agg::gamma_none() );
//                mRasterizer.gamma( agg::gamma_multiply( 2 ) );
                if ( attribs )
                {
                    SpanConv spanAttrib( attribs );
                    agg::span_converter< SpanGen, SpanConv > sc( sg, spanAttrib );
                    agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sc );
                }
                else
                    agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sg );
                break;

            default:
                mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
//                agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sg );
                if ( attribs )
                {
                    SpanConv spanAttrib( attribs );
                    agg::span_converter< SpanGen, SpanConv > sc( sg, spanAttrib );
                    agg::render_scanlines_bin( mRasterizer, mScanline, mRendererBase, sa, sc );
                }
                else
                    agg::render_scanlines_bin( mRasterizer, mScanline, mRendererBase, sa, sg );
                break;
            }

            return Ok;
        }

        template< class ImgPixFmt, class ImageAccessor, class SpanGen, class SpanConv >
        Status renderImageWrap( ImgPixFmt imgPixf, interpolator_type_linear interp, Unit srcUnit = UnitPixel, ImageAttributes* attribs = NULL )
        {
            span_alloc_type sa;
            ImageAccessor imgSrc( imgPixf );
            SpanGen sg( imgSrc, interp );

//            agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sg );

            switch ( mSmoothingMode )
            {
            case SmoothingModeHighQuality:
            case SmoothingModeAntiAlias:
                mRasterizer.gamma( agg::gamma_none() );
//                mRasterizer.gamma( agg::gamma_multiply( 2 ) );
                if ( attribs )
                {
                    SpanConv spanAttrib( attribs );
                    agg::span_converter< SpanGen, SpanConv > sc( sg, spanAttrib );
                    agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sc );
                }
                else
                    agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sg );
                break;

            default:
                mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
//                agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sg );
                if ( attribs )
                {
                    SpanConv spanAttrib( attribs );
                    agg::span_converter< SpanGen, SpanConv > sc( sg, spanAttrib );
                    agg::render_scanlines_bin( mRasterizer, mScanline, mRendererBase, sa, sc );
                }
                else
                    agg::render_scanlines_bin( mRasterizer, mScanline, mRendererBase, sa, sg );
                break;
            }

            return Ok;
        }

        ////////////////////////////////////////////////////////////////////////
        // Class for handling spiral gradients
        class gradient_spiral
        {
        private:
            REAL mSpiralAngle;

        public:
            gradient_spiral() : mSpiralAngle( 1 )   { }
            gradient_spiral( REAL angle ) : mSpiralAngle( angle )
            {
            }

            int calculate(int x, int y, int d) const
            {
				REAL dist = sqrt( static_cast<double>( x * x + y * y ) );
//                REAL dist = agg::fast_sqrt( x * x + y * y );
                REAL ang  = atan2( double( y ), double( x ) );
                REAL distRatio = dist / REAL( d );
                REAL curAngle = distRatio * pi;

//                ang = ModRange( ang + 10.0 * two_pi + mSpiralAngle * curAngle, 0.0, two_pi );
                ang = ModRange( ang + 10.0 * two_pi + -mSpiralAngle * curAngle, 0.0, two_pi );

                REAL angRatio = ang / two_pi;

                return int( angRatio * d );
            }
        };
        ////////////////////////////////////////////////////////////////////////

        // Render the path using a linear gradient brush
        Status renderPathLinearGradient( const Brush* brush, const GraphicsPath* path )
        {
            const LinearGradientBrush* gradBrush = static_cast< const LinearGradientBrush* >( brush );

            // Style of gradient
            switch ( gradBrush->GetGradientStyle() )
            {
            case GradientStyleRadial:
                {
                    agg::gradient_radial gradFunc;
                    return renderGradient< agg::gradient_radial >( gradBrush, gradFunc );
                }

            case GradientStyleXY:
                {
                    agg::gradient_xy gradFunc;
                    return renderGradient< agg::gradient_xy >( gradBrush, gradFunc );
                }

            case GradientStyleSqrtXY:
                {
                    agg::gradient_sqrt_xy gradFunc;
                    return renderGradient< agg::gradient_sqrt_xy >( gradBrush, gradFunc );
                }

            case GradientStyleConic:
                {
                    agg::gradient_conic gradFunc;
                    return renderGradient< agg::gradient_conic >( gradBrush, gradFunc );
                }

            case GradientStyleDiamond:
                {
                    agg::gradient_diamond gradFunc;
                    return renderGradient< agg::gradient_diamond >( gradBrush, gradFunc );
                }

            case GradientStyleSpiral:
                {
                    gradient_spiral gradFunc( gradBrush->data.spiralAngle );
                    return renderGradient< gradient_spiral >( gradBrush, gradFunc );
                }

            case GradientStyleLinear:
            default:
                {
                    agg::gradient_x gradFunc;
                    return renderGradient< agg::gradient_x >( gradBrush, gradFunc );
                }
            }
            return GenericError;
        }

        template< class gradient_func_type >
        Status renderGradient( const LinearGradientBrush* brush, const gradient_func_type& gradFunc )
        {
//            gradient_func_type gradFunc;

            // Wrapping style.  Any of the 'flip' modes will use the gradient
            // reflection, while 'tile' will repeat and 'clamp' will clamp.
            switch ( brush->GetWrapMode() )
            {
            case WrapModeTile:
                {
                    // Repeat
                    typedef agg::gradient_repeat_adaptor< gradient_func_type > gradient_adapter_type;
                    gradient_adapter_type adapter( gradFunc );
                    return renderGradient2< gradient_adapter_type >( brush, adapter );
                }

            case WrapModeClamp:
                {
                    return renderGradient2< gradient_func_type >( brush, gradFunc );
                }

            default:    // any of the flips
                {
                    // Reflect
                    typedef agg::gradient_reflect_adaptor< gradient_func_type > gradient_adapter_type;
                    gradient_adapter_type adapter( gradFunc );
                    return renderGradient2< gradient_adapter_type >( brush, adapter );
                }
            }

            return GenericError;
        }

        template< class gradient_adapter_type >
        Status renderGradient2( const LinearGradientBrush* brush, const gradient_adapter_type& adapter )
        {
            if ( !brush )
                return InvalidParameter;

            typedef agg::span_gradient< color_type, interpolator_type_linear, gradient_adapter_type, LinearGradientBrush::GradientLutType > gradient_span_type;

            agg::trans_affine gradMtx( brush->mMatrix.GetAffine() );
            gradMtx.multiply( mMatrix.GetAffine() );
            gradMtx.invert();
            interpolator_type_linear interp( gradMtx );
            span_alloc_type     span_alloc;
            gradient_span_type span_gradient( interp, adapter, brush->mGradientLUT, 0, brush->mGradientLength );

            typedef agg::renderer_scanline_aa< base_renderer_type, span_alloc_type, gradient_span_type > renderer_gradient_type;

            renderer_gradient_type ren( mRendererBase, span_alloc, span_gradient );
//            agg::render_scanlines( mRasterizer, mScanline, ren );

            switch ( mSmoothingMode )
            {
            case SmoothingModeHighQuality:
            case SmoothingModeAntiAlias:
                mRasterizer.gamma( agg::gamma_none() );
//                mRasterizer.gamma( agg::gamma_multiply( 2 ) );
                agg::render_scanlines( mRasterizer, mScanline, ren );
                break;

            default:
//                mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
                agg::render_scanlines( mRasterizer, mScanline, ren );
                break;
            }

            return Ok;
        }

    public:
        GraphicsRenderer( agg::rendering_buffer& buf, PixelFormat aggooFormat, FillMode fillMode = FillModeAlternate )
            : mPixFmt( buf ),
              mRendererBase( mPixFmt ),
              mRendererAA( mRendererBase ),
              mRendererBin( mRendererBase )
        {
            //mFillMode = fillMode;
            mAggOOPixelFormat   = aggooFormat;
            mCompositeMode      = CompositingModeSourceOver;
            mCompositeQuality   = CompositingQualityDefault;
            mInterpolationMode  = InterpolationModeDefault;
            mPageScale          = 1.0;
            mPageUnit           = UnitPixel;
            mPixelOffsetMode    = PixelOffsetModeDefault;
            mRenderingOrigin    = Point( 0, 0 );
            mSmoothingMode      = SmoothingModeDefault;
            mGammaThreshold     = 0.5;
//mPixFmt.comp_op( agg::comp_op_src );

            mRasterizer.gamma( agg::gamma_none() );
            //mCompoundRasterizer.gamma( agg::gamma_none() );
//            mRasterizer.gamma( agg::gamma_multiply( 2 ) );
//            mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
            mRasterizer.clip_box( 0, 0, mRendererBase.width(), mRendererBase.height() );
            //mCompoundRasterizer.clip_box( 0, 0, mRendererBase.width(), mRendererBase.height() );
        }

        //----------------------------------------------------------------------

        Status Clear( const Color& color )
        {
            mRendererBase.clear( ColorExt::toRGBA8( color ) );
            return Ok;
        }

        //DrawPath--------------------------------------------------------------
        Status DrawPath( const Pen* pen, const GraphicsPath* path )
        {
            // TODO: Need to implement additional pen attributes
            //      Not implemented:
            //          custom caps
            //          custom dash styles

            if ( !pen || !path )
                return InvalidParameter;

            mScanline.reset_spans();
            mRasterizer.reset();

            // Convert values such as the line caps and dash style to values
            // compatable with AGG:
            agg::line_join_e lineJoin = convertLineJoin( pen->GetLineJoin() );
            agg::inner_join_e innerJoin = convertInnerJoin( pen->GetLineJoin() );
            //NOTE: AGG only uses one cap.  Start and end will both use this (for now.)
            agg::line_cap_e  startCap = convertLineCap( pen->GetStartCap() );


            // Use the product of the pen and the graphics matrices:
            Matrix penMtx, mtx = mMatrix;
            pen->GetTransform( &penMtx );
            mtx.Multiply( &penMtx );
            agg::path_storage path2( *((GraphicsPath*)path)->getPathStorage() );
            agg::trans_affine aggMtx = mtx.GetAffine();

            typedef agg::conv_curve< agg::path_storage > conv_curve_type;
            conv_curve_type conv_curve_path( path2 );

            // It may be necessary to offset the particle by ( 0.5, 0.5 ) if
            // anti-aliasing is disabled.

            // In GDI+, the pen width is scaled according to the matrix.  I
            // haven't been able to duplicate this properly, so it will render
            // a line of the exact width of the pen, regardless of scale.
            REAL width = pen->GetWidth();
            if ( pen->GetDashStyle() == DashStyleSolid )
            {
                typedef agg::conv_stroke< conv_curve_type > path_conv_stroke_type;
                typedef agg::conv_transform< path_conv_stroke_type > path_conv_trans_type;
                path_conv_stroke_type stroke( conv_curve_path );
                path_conv_trans_type trans( stroke, aggMtx );

                stroke.line_join( lineJoin );
                stroke.inner_join( innerJoin );
                stroke.line_cap( startCap );
                stroke.miter_limit( pen->GetMiterLimit() );
                stroke.inner_miter_limit( pen->GetInternalMiterLimit() );
//                stroke.width( pen->GetWidth() );
                stroke.width( width );

                stroke.approximation_scale( aggMtx.scale() );
                conv_curve_path.approximation_scale( aggMtx.scale() );

//-                mRasterizer.add_path( stroke );
                mRasterizer.add_path( trans );
            }
            else
            {
                typedef agg::conv_dash< conv_curve_type > path_conv_dash_type;
                typedef agg::conv_stroke< path_conv_dash_type > path_conv_stroke_type;

                path_conv_dash_type dash_line( conv_curve_path );
                path_conv_stroke_type stroke( dash_line );

                typedef agg::conv_transform< path_conv_stroke_type > path_conv_trans_type;
                path_conv_trans_type trans( stroke, aggMtx );

                stroke.line_join( lineJoin );
                stroke.inner_join( innerJoin );
                stroke.line_cap( startCap );
                stroke.miter_limit( pen->GetMiterLimit() );
                stroke.inner_miter_limit( pen->GetInternalMiterLimit() );

//                REAL width = pen->GetWidth();
                stroke.width( width );

                // Set the dash style:
                switch ( pen->GetDashStyle() )
                {
                case DashStyleDot:
                    dash_line.add_dash( width, width );
                    break;

                case DashStyleDashDot:
                    dash_line.add_dash( 3.0 * width, width );
                    dash_line.add_dash( width, width );
                    break;

                case DashStyleDashDotDot:
                    dash_line.add_dash( 3.0 * width, width );
                    dash_line.add_dash( width, width );
                    dash_line.add_dash( width, width );
                    break;

                //case DashStyleCustom:
                //    break;

                default:    // DashStyleDash
                    dash_line.add_dash( 3.0 * width, width );
                    break;
                }

                stroke.approximation_scale( aggMtx.scale() );

//-                mRasterizer.add_path( stroke );
                mRasterizer.add_path( trans );
            }

            return renderPath( pen->GetBrush(), path );

        }

        //FillPath--------------------------------------------------------------
        Status FillPath( const Brush* brush, const GraphicsPath* path )
        {
            if ( !brush || !path )
                return InvalidParameter;

            typedef agg::conv_curve< agg::path_storage > conv_curve_type;
            typedef agg::conv_transform< conv_curve_type > path_conv_trans_type;

            agg::trans_affine aggMtx = mMatrix.GetAffine();
            agg::path_storage path2( *((GraphicsPath*)path)->getPathStorage() );
            conv_curve_type conv_curve_path( path2 );
            path_conv_trans_type trans( conv_curve_path, aggMtx );
            conv_curve_path.approximation_scale( aggMtx.scale() );

            mScanline.reset_spans();
            mRasterizer.reset();
            mRasterizer.add_path( trans );

            mRasterizer.filling_rule( ( path->GetFillMode() == FillModeAlternate ) ? agg::fill_even_odd : agg::fill_non_zero );
            Status status = renderPath( brush, path );

            // Reset the fulling rule (unfortunately, there is no method to
            // fetch the mode from the rasterizer, so we "just know" what the
            // default value is.
            mRasterizer.filling_rule( agg::fill_non_zero );

            return status;
        }

        //RenderCompoundPath----------------------------------------------------
        Status RenderCompoundPath( const GraphicsPathCompound* path )
        {
            if ( !path )
                return InvalidParameter;

            GraphicsPathCompound* compound = const_cast< GraphicsPathCompound* >( path );

            // If there is just one path, don't use the compound renderer (it
            // won't work as expected.)  Instead, just pass it off to a simple
            // renderer...
            bool singlePath = false;
            if ( path->GetPathCount() == 1 )
            {
                //singlePath = true;
                // Add a transparent rect.  It won't show anything,
                // but it should allow the scene to render.  This isn't ideal,
                // but, hey! :-)
                GraphicsPath* pathStorage = NULL;
                if ( compound->GetGraphicsPath( 0, pathStorage ) != Ok )
                    return InvalidParameter;
                SolidBrush tempBr( Color::Blue );
                GraphicsPath tempPath;
                tempPath.AddRectangle( RectF( 0, 0, 10, 10 ) );
                compound->AddPath( &tempBr, pathStorage );
            }

            //else
            {
                typedef agg::conv_curve< agg::path_storage > conv_curve_type;
                typedef agg::conv_transform< conv_curve_type > path_conv_trans_type;

                agg::trans_affine aggMtx = mMatrix.GetAffine();
                mScanline.reset_spans();

                // Create a compound rasterizer:
                agg::rasterizer_compound_aa< agg::rasterizer_sl_clip_dbl > rasterizer;
                rasterizer.clip_box( 0, 0, mRendererBase.width(), mRendererBase.height() );
                StyleCollection styleCollection;

                // Go through the compound path, and add each style to a style
                // collection, and add the paths (with the corresponding style
                // index) to the rasterizer.
                for ( int i = 0; i < compound->GetPathCount(); i++ )
                {
                    StyleHandler* styleHandler = NULL;
                    GraphicsPath* pathStorage = NULL;
                    //agg::StyleHandler* styleHandler = path->GetStyleHandler( i );
                    //GraphicsPath* pathStorage = path->GetGraphicsPath( i );

                    if ( compound->GetStyleHandler( i, styleHandler ) != Ok || compound->GetGraphicsPath( i, pathStorage ) != Ok )
                        return InvalidParameter;

                    agg::path_storage path2( *pathStorage->getPathStorage() );

                    conv_curve_type conv_curve_path( path2 );
                    path_conv_trans_type trans( conv_curve_path, aggMtx );
                    conv_curve_path.approximation_scale( aggMtx.scale() );

                    styleCollection.AddStyle( styleHandler );
                    rasterizer.styles( i, -1 );
                    rasterizer.add_path( trans );

                    //SolidBrush br( ColorExt::RandomRGB() );
                    ////renderPath( &br, pathStorage );
                    //FillPath( &br, pathStorage );
                }

                rasterizer.filling_rule( agg::fill_even_odd );
                //rasterizer.filling_rule( ( path->GetFillMode() == FillModeAlternate ) ? agg::fill_even_odd : agg::fill_non_zero );

                span_alloc_type sa;
                if ( singlePath )
                    RenderScanlinesCompound( rasterizer, mScanline, mScanline, mRendererBase, sa, styleCollection);
                else
                    //RenderScanlinesCompound( rasterizer, mScanline, mScanline, mRendererBase, sa, styleCollection);
                    RenderScanlinesCompoundLayered( rasterizer, mScanline, mRendererBase, sa, styleCollection);
            }

            return Ok;
        }

        ////RenderSVGPath---------------------------------------------------------
        //Status RenderSVGPath( svg::SVGPath* path, const Matrix* mtx, const Rect& clipBox, REAL opacity )
        //{
        //    agg::rect_i cb;
        //    cb.x1 = clipBox.X;
        //    cb.y1 = clipBox.Y;
        //    cb.x2 = clipBox.GetRight();
        //    cb.y2 = clipBox.GetBottom();
        //    path->getAggPath()->render( mRasterizer, mScanline, mRendererAA, mtx->GetAffine(), cb, opacity );
        //    return Ok;
        //}

        //DrawImage-------------------------------------------------------------
        Status DrawImage( Image* image, REAL x, REAL y, REAL width, REAL height )
        {
            if ( !image || !width || !height )
                return InvalidParameter;

            BitmapData* imageData = getBitmapData( image );
            agg::path_storage path;
            path.move_to( x, y );
            path.line_to( x + width, y );
            path.line_to( x + width, y + height );
            path.line_to( x, y + height );

            // Transform the path:
            agg::trans_affine pathMat = mMatrix.GetAffine();
            agg::conv_transform< agg::path_storage > transPath( path, pathMat );

            mScanline.reset_spans();
            mRasterizer.reset();
            mRasterizer.add_path( transPath );

            // Create a matrix for the image
            Matrix imgMat;
            imgMat.Reset();
            imgMat.Translate( x, y );
            imgMat.Scale( width / REAL( image->GetWidth() ), height / REAL( image->GetHeight() ) );

            RectF srcRect( imageData->X, imageData->Y, imageData->Width, imageData->Height );
            //srcRect.Intersect( RectF( 0, 0, imageData->Width, imageData->Height ) );
            RectF destRect( x, y, width, height );
            //destRect.Intersect( RectF( 0, 0, imageData->Width, imageData->Height ) );

            Status status = renderImage( imageData, destRect, srcRect, &imgMat );
            return status;
        }

        //DrawImage-------------------------------------------------------------
        Status DrawImage( Image* image, const RectF& destRect, REAL srcX, REAL srcY, REAL srcW, REAL srcH, Unit srcUnit, ImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData )
        {
            Status status = Ok;
            if ( !image || !destRect.Width || !destRect.Height )
                return InvalidParameter;

            // If the source width or height is <= 0, then use the actual image size:
            if ( srcW <= 0 )    srcW = image->GetWidth();
            if ( srcH <= 0 )    srcH = image->GetHeight();

            BitmapData* imageData = getBitmapData( image );
            agg::path_storage path;
            path.move_to( destRect.X, destRect.Y );
            path.line_to( destRect.X + destRect.Width, destRect.Y );
            path.line_to( destRect.X + destRect.Width, destRect.Y + destRect.Height );
            path.line_to( destRect.X, destRect.Y + destRect.Height );

            // Transform the path:
            agg::trans_affine pathMat = mMatrix.GetAffine();
            agg::conv_transform< agg::path_storage > transPath( path, pathMat );

            mScanline.reset_spans();
            mRasterizer.reset();
            mRasterizer.add_path( transPath );

            // Create a matrix for the image
            Matrix imgMat;
            imgMat.Reset();
            imgMat.Translate( destRect.X, destRect.Y );
            imgMat.Scale( destRect.Width / srcW, destRect.Height / srcH );

            RectF srcRect( srcX, srcY, srcW, srcH );
            //srcRect.Intersect( RectF( 0, 0, imageData->Width, imageData->Height ) );
            RectF destRect2( destRect );
            //destRect2.Intersect( RectF( 0, 0, imageData->Width, imageData->Height ) );

            status = renderImage( imageData, destRect2, srcRect, &imgMat, srcUnit, imageAttributes );
            return status;
        }

    };

    //renderPath----------------------------------------------------------------
    template< class PixFmt >
    inline Status GraphicsRenderer< PixFmt >::renderPath( const Brush* brush, const GraphicsPath* path )
    {
/*
        typedef SharedPtr< StyleHandler > StylePtr;
        //StylePtr handler = StylePtr( brush->CreateStyleHandler( &mMatrix ) );
        StylePtr handler = StylePtr( pixFmtPtr->CreateStyleHandler( brush, &mMatrix ) );

        span_alloc_type sa;
        switch ( mSmoothingMode )
        {
        case SmoothingModeHighQuality:
        case SmoothingModeAntiAlias:
//            mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
            mRasterizer.gamma( agg::gamma_none() );
//            mRasterizer.gamma( agg::gamma_multiply( 2 ) );
            agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, *(handler.get()) );
//            agg::render_scanlines( mRasterizer, mScanline, ren );
            break;

        default:
            mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
            agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, *(handler.get()) );
//            agg::render_scanlines_bin( mRasterizer, mScanline, mRendererBase, sa, sg );
//            agg::render_scanlines( mRasterizer, mScanline, ren );
            break;
        }

        return Ok;
*/

        switch ( brush->GetType() )
        {
        case BrushTypeSolidColor:
            return renderPathSolidColor( brush, path );

        case BrushTypeTextureFill:
            return renderPathTextureFill( brush, path );

        case BrushTypeLinearGradient:
            return renderPathLinearGradient( brush, path );
        }

        return NotImplemented;
    }

    //renderPathSolidColor------------------------------------------------------
    template< class PixFmt >
    inline Status GraphicsRenderer< PixFmt >::renderPathSolidColor( const Brush* brush, const GraphicsPath* path )
    {
        // Brush color:
        Color color;
        ((SolidBrush*)brush)->GetColor( &color );

//        typedef agg::renderer_scanline_aa_solid< base_renderer_type > solid_renderer_type;
//        solid_renderer_type ren( mRendererBase );
//        ren.color( convertColor( color ) );
//        agg::render_scanlines( mRasterizer, mScanline, ren );

        agg::span_solid< color_type > sg;
        sg.color( ColorExt::toRGBA8( color ) );
        span_alloc_type sa;
        switch ( mSmoothingMode )
        {
        case SmoothingModeHighQuality:
        case SmoothingModeAntiAlias:
//            mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
            mRasterizer.gamma( agg::gamma_none() );
//            mRasterizer.gamma( agg::gamma_multiply( 2 ) );
            agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sg );
//            agg::render_scanlines( mRasterizer, mScanline, ren );
            break;

        default:
            mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
            agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, sa, sg );
//            agg::render_scanlines_bin( mRasterizer, mScanline, mRendererBase, sa, sg );
//            agg::render_scanlines( mRasterizer, mScanline, ren );
            break;
        }

        return Ok;
    }

    //renderPathTextureFill-----------------------------------------------------
    template< class PixFmt >
    inline Status GraphicsRenderer< PixFmt >::renderPathTextureFill( const Brush* brush, const GraphicsPath* path )
    {
        // Fill the path based on the clamping method:
        TextureBrush* texBrush = (TextureBrush*)( brush );
        Image* texImage = texBrush->GetImage();
        if ( !texImage )    return InvalidParameter;
        BitmapData* imageData = getBitmapData( texImage );

/*
            int x = 20, y = 30;
            int width = imageData->Width, height = imageData->Height;
            agg::path_storage p;
            p.move_to( x, y );
            p.line_to( x + width * 4, y );
            p.line_to( x + width * 4, y + height * 3 );
            p.line_to( x, y + height * 3 );

            // Transform the path:
            agg::trans_affine pathMat = mMatrix.GetAffine();
            agg::conv_transform< agg::path_storage > transPath( p, pathMat );

            mScanline.reset_spans();
            mRasterizer.reset();
            mRasterizer.add_path( transPath );

            // Create a matrix for the image
            Matrix imgMat;
            imgMat.Reset();
            imgMat.Translate( x, y );
            imgMat.Scale( width / REAL( texImage->GetWidth() ), height / REAL( texImage->GetHeight() ) );
*/

        return renderImage( imageData, RectF( 0, 0, imageData->Width, imageData->Height ), RectF( imageData->X, imageData->Y, imageData->Width, imageData->Height ), &texBrush->mMatrix, UnitPixel, NULL, texBrush->GetWrapMode() );
    }

    //renderImage---------------------------------------------------------------
    template< class PixFmt >
    inline Status GraphicsRenderer< PixFmt >::renderImage( const BitmapData* imageData, const RectF& destRect, const RectF& srcRect, const Matrix* matrix, Unit srcUnit, ImageAttributes* imageAttributes, WrapMode wrapMode )
    {
        if ( !imageData )
            return InvalidParameter;

        //srcRect.X       = IClamp( srcRect.X, 0, imageData->Width );
        //srcRect.Y       = IClamp( srcRect.Y, 0, imageData->Height );
        //srcRect.Width   = min( srcRect.Width, imageData->GetRight() - srcRect.X );
        //srcRect.Height  = min( srcRect.Height, imageData->GetBottom() - srcRect.Y );
        //destRect.X      = IClamp( destRect.X, 0, imageData->Width );
        //destRect.Y      = IClamp( destRect.Y, 0, imageData->Height );
        //destRect.Width  = min( destRect.Width, imageData->GetRight() - destRect.X );
        //destRect.Height = min( destRect.Height, imageData->GetBottom() - destRect.Y );

        // TODO: Indexed and extended colors are not supported yet
        if ( IsIndexedPixelFormat( imageData->PixelFormat ) || IsExtendedPixelFormat( imageData->PixelFormat ) )
            return NotImplemented;

        Status status = Ok;

        agg::trans_affine workMatrix;
        if ( matrix )
            workMatrix = matrix->GetAffine();
//        agg::trans_affine workMatrix( matrix->GetAffine() );
        workMatrix.multiply( mMatrix.GetAffine() );
        workMatrix.invert();
        interpolator_type_linear interp( workMatrix );

        // Take a section from the image and convert it to the desired pixel
        // format.  Create a temporary BitmapData to define the region to
        // extract, and a BitmapData to receive the converted data.
        BitmapData tempImageData( srcRect, imageData->Stride, imageData->PixelFormat, 0, imageData->Scan0 );
        if ( tempImageData.X + tempImageData.Width > imageData->X + imageData->Width )
            tempImageData.Width    = IMinClamp( imageData->Width - tempImageData.X, 0 );
        if ( tempImageData.Y + tempImageData.Height > imageData->Y + imageData->Height )
            tempImageData.Height   = IMinClamp( imageData->Height - tempImageData.Y, 0 );

        BitmapData convImageData;
        convImageData.X             = 0;
        convImageData.Y             = 0;
        convImageData.Width         = tempImageData.Width;
        convImageData.Height        = tempImageData.Height;
        //convImageData.Width         = destRect.Width;
        //convImageData.Height        = destRect.Height;
        convImageData.PixelFormat   = mAggOOPixelFormat;
        convImageData.Stride        = GetRowWidth( convImageData.Width, GetPixelFormatSize( mAggOOPixelFormat ) );
        convImageData.Flags        |= ImageFlagOwnScan0;
        convImageData.Scan0         = new BYTE [ convImageData.Stride * convImageData.Height ];

        // If either stride is 0, this is technically incorrect, but since it
        // really just means the image won't be rendered, return Ok to ignore it.
        if ( convImageData.Stride == 0 || tempImageData.Stride == 0 )
            return Ok;

        //memset( convImageData.Scan0, 255, convImageData.Stride * convImageData.Height );
        BufferConverter::ConvertPixelFormat( &tempImageData, &convImageData );

        //// If the pixel format of the image is not the same as the graphics
        //// class, then it should probably converted
        //BitmapData convImageData( Rect( srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height ),\
        //                          imageData->Stride, imageData->PixelFormat, 0, imageData->Scan0 );
        //if ( mAggOOPixelFormat != imageData->PixelFormat )
        //{
        //    convImageData.PixelFormat = mAggOOPixelFormat;
        //    convImageData.Stride = GetRowWidth( convImageData.Width, GetPixelFormatSize( mAggOOPixelFormat ) );
        //    convImageData.Scan0 = new BYTE [ convImageData.Stride * convImageData.Height ];
        //    convImageData.Flags |= ImageFlagOwnScan0;

        //    BufferConverter::ConvertPixelFormat( imageData, &convImageData );
        //}

        REAL gammaValue = 1.0;
        span_alloc_type sa;
        agg::rendering_buffer imgBuf( (BYTE*)convImageData.Scan0, convImageData.Width, convImageData.Height, convImageData.Stride );
        switch ( convImageData.PixelFormat )
        {
/*          // TODO: Implement additional color formats
        case PixelFormat1bppIndexed      :
            break;

        case PixelFormat4bppIndexed      :
            break;

        case PixelFormat8bppIndexed      :
            break;

        case PixelFormat8bppGrayScale    :
            break;

        case PixelFormat16bppGrayScale   :
            break;

        case PixelFormat16bppRGB555      :
            break;

        case PixelFormat16bppRGB565      :
            break;

        case PixelFormat16bppARGB1555    :
            break;

        case PixelFormat30bppRGB         :
            break;

        case PixelFormat30bppBGR         :
            break;

        case PixelFormat32bppRGB         :
            break;

        case PixelFormat32bppBGR         :
            break;

        case PixelFormat32bppPARGB       :
            break;

        case PixelFormat48bppBGR         :
            break;

        case PixelFormat48bppRGB         :
            break;

        case PixelFormat64bppARGB        :
            break;

        case PixelFormat64bppPARGB       :
            break;

        case PixelFormat64bppRGBA        :
            break;

        case PixelFormat64bppABGR        :
            break;

        case PixelFormat64bppBGRA        :
            break;
*/

        case PixelFormat24bppRGB         :
            {
                typedef agg::pixfmt_rgb24 srcPixFmt;
                typedef srcPixFmt::color_type color_type;
                typedef color_type::value_type value_type;
                typedef agg::gamma_lut<value_type, value_type, color_type::base_shift, color_type::base_shift> gamma_lut_type;
                gamma_lut_type gamma( gammaValue );
                srcPixFmt imgPixf( imgBuf );
                imgPixf.apply_gamma_dir( gamma );

                if ( wrapMode == WrapModeTile )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_repeat wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgb_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgb >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipX )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_repeat  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgb_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgb >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipY )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgb_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgb >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipXY )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgb_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgb >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else    // Clamp
                {
                    typedef agg::image_accessor_clip< srcPixFmt > img_src_type;
                    typedef agg::span_image_filter_rgb_bilinear_clip< srcPixFmt, interpolator_type_linear > span_gen_type;

                    status = renderImageClamp< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgb >( imgPixf, interp, srcUnit, imageAttributes );
                }
            }
            break;

        case PixelFormat24bppBGR         :
            {
                typedef agg::pixfmt_bgr24 srcPixFmt;
                typedef srcPixFmt::color_type color_type;
                typedef color_type::value_type value_type;
                typedef agg::gamma_lut<value_type, value_type, color_type::base_shift, color_type::base_shift> gamma_lut_type;
                gamma_lut_type gamma( gammaValue );
                srcPixFmt imgPixf( imgBuf );
                imgPixf.apply_gamma_dir( gamma );

                if ( wrapMode == WrapModeTile )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_repeat wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgb_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgb >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipX )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_repeat  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgb_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgb >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipY )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgb_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgb >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipXY )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgb_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgb >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else    // Clamp
                {
                    typedef agg::image_accessor_clip< srcPixFmt > img_src_type;
                    typedef agg::span_image_filter_rgb_bilinear_clip< srcPixFmt, interpolator_type_linear > span_gen_type;

                    status = renderImageClamp< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgb >( imgPixf, interp, srcUnit, imageAttributes );
                }
            }
            break;

        case PixelFormat32bppARGB        :
            {
                typedef agg::pixfmt_argb32 srcPixFmt;
                typedef srcPixFmt::color_type color_type;
                typedef color_type::value_type value_type;
                typedef agg::gamma_lut<value_type, value_type, color_type::base_shift, color_type::base_shift> gamma_lut_type;
                gamma_lut_type gamma( gammaValue );
                srcPixFmt imgPixf( imgBuf );
                imgPixf.apply_gamma_dir( gamma );

                if ( wrapMode == WrapModeTile )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_repeat wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
//                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;
                    typedef agg::image_accessor_clone<srcPixFmt> img_accessor_type;
                    typedef agg::span_image_filter_rgba_nn< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipX )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_repeat  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
//                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;
                    typedef agg::image_accessor_clone<srcPixFmt> img_accessor_type;
                    typedef agg::span_image_filter_rgba_nn< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipY )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
//                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;
                    typedef agg::image_accessor_clone<srcPixFmt> img_accessor_type;
                    typedef agg::span_image_filter_rgba_nn< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipXY )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
//                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;
                    typedef agg::span_image_filter_rgba_nn< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else    // Clamp
                {
                    typedef agg::image_accessor_clip< srcPixFmt > img_src_type;
//                    typedef agg::span_image_filter_rgba_bilinear_clip< srcPixFmt, interpolator_type_linear > span_gen_type;
//                    typedef agg::span_image_filter_rgba_plain_bilinear_clip< srcPixFmt, interpolator_type_linear > span_gen_type;
        typedef agg::image_accessor_clone<srcPixFmt> img_accessor_type;
                    typedef agg::span_image_filter_rgba_nn< img_accessor_type, interpolator_type_linear > span_gen_type;

//                    status = renderImageClamp< srcPixFmt, img_src_type, span_gen_type >( imgPixf, interp, srcUnit, imageAttributes );
                    status = renderImageClamp2< srcPixFmt, img_accessor_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
            }
            break;

        case PixelFormat32bppRGBA        :
            {
                typedef agg::pixfmt_rgba32 srcPixFmt;
                typedef srcPixFmt::color_type color_type;
                typedef color_type::value_type value_type;
                typedef agg::gamma_lut<value_type, value_type, color_type::base_shift, color_type::base_shift> gamma_lut_type;
                gamma_lut_type gamma( gammaValue );
                srcPixFmt imgPixf( imgBuf );
                imgPixf.apply_gamma_dir( gamma );

/*
                if ( wrapMode == WrapModeTile )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_repeat wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipX )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_repeat  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipY )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipXY )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else    // Clamp
                {
                    typedef agg::image_accessor_clip< srcPixFmt > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear_clip< srcPixFmt, interpolator_type_linear > span_gen_type;

                    status = renderImageClamp< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
*/
            }
            break;

        case PixelFormat32bppBGRA        :
            {
                typedef agg::pixfmt_bgra32 srcPixFmt;
                typedef srcPixFmt::color_type color_type;
                typedef color_type::value_type value_type;
                typedef agg::gamma_lut<value_type, value_type, color_type::base_shift, color_type::base_shift> gamma_lut_type;
                gamma_lut_type gamma( gammaValue );
                srcPixFmt imgPixf( imgBuf );
                imgPixf.apply_gamma_dir( gamma );

                if ( wrapMode == WrapModeTile )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_repeat wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipX )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_repeat  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipY )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipXY )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else    // Clamp
                {
                    typedef agg::image_accessor_clip< srcPixFmt > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear_clip< srcPixFmt, interpolator_type_linear > span_gen_type;

                    status = renderImageClamp< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
            }
            break;

        case PixelFormat32bppABGR        :
            {
                typedef agg::pixfmt_abgr32 srcPixFmt;
                typedef srcPixFmt::color_type color_type;
                typedef color_type::value_type value_type;
                typedef agg::gamma_lut<value_type, value_type, color_type::base_shift, color_type::base_shift> gamma_lut_type;
                gamma_lut_type gamma( gammaValue );
                srcPixFmt imgPixf( imgBuf );
                imgPixf.apply_gamma_dir( gamma );

                if ( wrapMode == WrapModeTile )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_repeat wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipX )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_repeat  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipY )
                {
                    typedef agg::wrap_mode_repeat wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else if ( wrapMode == WrapModeTileFlipXY )
                {
                    typedef agg::wrap_mode_reflect wrap_x_type;
                    typedef agg::wrap_mode_reflect  wrap_y_type;
                    typedef agg::image_accessor_wrap< srcPixFmt, wrap_x_type, wrap_y_type > img_src_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear< img_src_type, interpolator_type_linear > span_gen_type;

                    status = renderImageWrap< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
                else    // Clamp
                {
                    typedef agg::image_accessor_clip< srcPixFmt > img_src_type;
//                    typedef agg::span_image_filter_rgba_nn< srcPixFmt, interpolator_type_linear > span_gen_type;
                    typedef agg::span_image_filter_rgba_plain_bilinear_clip< srcPixFmt, interpolator_type_linear > span_gen_type;

                    status = renderImageClamp< srcPixFmt, img_src_type, span_gen_type, span_conv_image_attrib_rgba >( imgPixf, interp, srcUnit, imageAttributes );
                }
            }
            break;

        default:
            status = NotImplemented;
            break;
        }

        return status;
    }


    //==========================================================================
    // Graphics class
    //==========================================================================

    //Constructor---------------------------------------------------------------
    Graphics::Graphics( Image* image, ColorFormat colFmt )
        : mActiveBitmap( new BitmapData ),
          mRenderer( 0 ),
          mBusyCount( 0 )
    {
#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
        mHDC        = 0;
        //mHBitmap    = 0;
#endif

        // Create the graphics object:
        createGraphics( image->GetWidth(), image->GetHeight(), image->mActiveBitmap.Stride, image->mActiveBitmap.Scan0, image->GetPixelFormat(), colFmt );

        mLastStatus = SetStatus( Ok );
    }

    //FromImage-----------------------------------------------------------------
    Graphics* Graphics::FromImage( Image* image )
    {
        return new Graphics( image, ColorFormatDefault );
    }

#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )

    //createFromHDC-------------------------------------------------------------
    void Graphics::createFromHDC( HDC hdc, ColorFormat colFmt )
    {
        mHDC = hdc;

        // Get the clipping rectangle
        //RECT clipRect;
        ::GetClipBox( hdc, &mClipRectDC );

        // Get additional information about the device
        int bitsPerPixel = ::GetDeviceCaps( hdc, BITSPIXEL );
        PixelFormat format = PixelFormat24bppBGR;
        int stride = 3;
        switch ( bitsPerPixel )
        {
        case 16:
            format = PixelFormat16bppRGB555;
            stride = 2;
            break;

        case 24:
            format = PixelFormat24bppRGB;
            stride = 3;
            break;

        case 32:
            format = PixelFormat32bppRGB;
            stride = 4;
            break;

        default:
            {
                // not implemented:
                std::stringstream stream;
                stream << "Bit depth of " << bitsPerPixel << " not currently supported when creating from an HDC.";
                throw Exception( stream.str() );
            }
            break;
        };

        //mHBitmap = ::CreateCompatibleBitmap( hdc, clipRect.right - clipRect.left, clipRect.bottom - clipRect.top );
        //mOldBitmap = (HBITMAP)::SelectObject( hdc, mHBitmap );

        // Create the active bitmap from the HDC
        createGraphics( mClipRectDC.right - mClipRectDC.left,
                        mClipRectDC.bottom - mClipRectDC.top,
                        stride,
                        0,
                        format,
                        colFmt );

        mLastStatus = SetStatus( Ok );
    }

    //Constructor---------------------------------------------------------------
    Graphics::Graphics( HDC hdc, ColorFormat colFmt )
        : mActiveBitmap( new BitmapData ),
          mRenderer( 0 ),
          mBusyCount( 0 )
    {
        createFromHDC( hdc, colFmt );
    }

    //FromHDC-------------------------------------------------------------------
    Graphics* Graphics::FromHDC( HDC hdc, ColorFormat colFmt )
    {
        return new Graphics( hdc, colFmt );
    }

    //Constructor---------------------------------------------------------------
    Graphics::Graphics( HDC hdc, HANDLE hdevice, ColorFormat colFmt )
        : mActiveBitmap( new BitmapData ),
          mRenderer( 0 ),
          mBusyCount( 0 )
    {
		// This is actually more involved, but for now, we'll just create it
		// with the device context
		Graphics( hdc, colFmt );
    }

    //FromHDC-------------------------------------------------------------------
    Graphics* Graphics::FromHDC( HDC hdc, HANDLE hdevice, ColorFormat colFmt )
    {
        return new Graphics( hdc, hdevice );
    }

    //Constructor---------------------------------------------------------------
    Graphics::Graphics( HWND hwnd, bool icm, ColorFormat colFmt )
        : mActiveBitmap( new BitmapData ),
          mRenderer( 0 ),
          mBusyCount( 0 )
    {
    }

    //FromHWND------------------------------------------------------------------
    Graphics* Graphics::FromHWND( HWND hWnd, BOOL icm, ColorFormat colFmt )
    {
        return new Graphics( hWnd, icm, ColorFormatDefault );
    }

    //GetHDC--------------------------------------------------------------------
    HDC Graphics::GetHDC( void )
    {
        mBusyCount++;
        return mHDC;
    }

    //ReleaseHDC----------------------------------------------------------------
    VOID Graphics::ReleaseHDC( HDC hdc )
    {
        mBusyCount--;
        //if ( mHDC )
        //{
        //    mHBitmap = (HBITMAP)::SelectObject( mHDC, mOldBitmap );
        //    ::DeleteObject( mHBitmap );
        //    //::DeleteDC( mHDC );
        //}
    }

#endif  // AGGOO_PLATFORM_WIN32

    //Destructor----------------------------------------------------------------
    Graphics::~Graphics(void)
    {
#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
        // If the Graphics object was created with a device context handle, the
        // image needs to be displayed to the device context.
        if ( mHDC )
        {
            // Create a device independent bitmap:
            BITMAPINFO dib;
            memset( &dib, 0, sizeof( BITMAPINFO ) );
            dib.bmiHeader.biSize        = sizeof( BITMAPINFOHEADER );
            dib.bmiHeader.biWidth       = mActiveBitmap->Width;
            dib.bmiHeader.biHeight      = mActiveBitmap->Height;
            dib.bmiHeader.biPlanes      = 1;
            dib.bmiHeader.biBitCount    = GetPixelFormatSize( mActiveBitmap->PixelFormat );
            dib.bmiHeader.biCompression = BI_RGB;
            dib.bmiHeader.biSizeImage   = mActiveBitmap->Height * mActiveBitmap->Stride;

#ifndef NODCALPHA
            // Create the DIB section
            LPVOID pBits;
            HBITMAP tempBmp = CreateDIBSection( mHDC, &dib, DIB_RGB_COLORS, &pBits, 0, 0 );
            if ( !tempBmp )
            {
                // An error occurred creating the DIB section
                std::stringstream stream;
                stream << "Error creating DIB Section for HDC.";
                throw Exception( stream.str() );
            }
            else
            {
                memcpy( pBits, mActiveBitmap->Scan0, mActiveBitmap->Stride * mActiveBitmap->Height );
                HDC tempDC = CreateCompatibleDC( mHDC );
                if ( !tempDC )
                {
                    std::stringstream stream;
                    stream << "Error displaying image to HDC.";
                    throw Exception( stream.str() );
                }
                else
                {
                    HBITMAP bmpOld = (HBITMAP)::SelectObject( tempDC, tempBmp );
                    BLENDFUNCTION blendFunc;
                    blendFunc.BlendOp               = AC_SRC_OVER;
                    blendFunc.BlendFlags            = 0;
                    blendFunc.SourceConstantAlpha   = 255;
                    blendFunc.AlphaFormat           = AC_SRC_ALPHA;
                    AlphaBlend( mHDC,
                                mClipRectDC.left,
                                mClipRectDC.top,
                                mActiveBitmap->Width,
                                mActiveBitmap->Height,
                                tempDC,
                                0, 0, 
                                mActiveBitmap->Width,
                                mActiveBitmap->Height,
                                blendFunc );
                    ::SelectObject( tempDC, bmpOld );
                    DeleteDC( tempDC );
                }
                ::DeleteObject( tempBmp );
            }
#else
            SetDIBitsToDevice( mHDC,
                               mClipRectDC.left,
                               mClipRectDC.top,
                               mActiveBitmap->Width,
                               mActiveBitmap->Height,
                               0, 0, 0,
                               mActiveBitmap->Height,
                               mActiveBitmap->Scan0,
                               &dib,
                               DIB_RGB_COLORS );
#endif  // NODCALPHA
        }

#endif  // AGGOO_PLATFORM_WIN32

        if ( mActiveBitmap )
            delete mActiveBitmap;
        if ( mRenderer )
            delete mRenderer;
    }


    //--------------------------------------------------------------------------

    //createGraphics------------------------------------------------------------
    Status Graphics::createGraphics( INT width, INT height, INT rowWidth, VOID* scan0, PixelFormat pixelFormat, ColorFormat colorFormat )
    {
        if ( width < 0 || height < 0 )
            return SetStatus( InvalidParameter );

        //mFillMode = fillMode;
        mAggOOPixelFormat   = pixelFormat;
        mCompositeMode      = CompositingModeSourceOver;
        mCompositeQuality   = CompositingQualityDefault;
        mInterpolationMode  = InterpolationModeDefault;
        mPageScale          = 1.0;
        mPageUnit           = UnitPixel;
        mPixelOffsetMode    = PixelOffsetModeDefault;
        mRenderingOrigin    = Point( 0, 0 );
        mSmoothingMode      = SmoothingModeDefault;
        mGammaThreshold     = 0.5;

        mRasterizer.gamma( agg::gamma_none() );
        mRasterizer.clip_box( 0, 0, width, height );

        if ( mActiveBitmap->Scan0 && ( mActiveBitmap->Flags & ImageFlagOwnScan0 ) )
            delete mActiveBitmap->Scan0;

        // Create the rendering buffer:
        if ( scan0 )
        {
            // Use the existing buffer
            mActiveBitmap->Scan0 = (BYTE*)scan0;
            mActiveBitmap->Flags &= ~ImageFlagOwnScan0;
        }
        else
        {
            // Allocate the raw buffer:
            INT bufSize = abs( rowWidth ) * height;
            mActiveBitmap->Scan0 = new BYTE [ bufSize ];
            mActiveBitmap->Flags |= ImageFlagOwnScan0;
            if ( !mActiveBitmap->Scan0 )
            {
                return SetStatus( OutOfMemory );
            }

            // Clear the buffer:
            memset( mActiveBitmap->Scan0, 0, bufSize );
        }

        // Attach the buffer to the rendering_buffer:
        mRenderingBuffer.attach( (BYTE*)mActiveBitmap->Scan0, width, height, rowWidth );

        // Create the pixel format:
        mPixelFormat = PixFmtPtr( AbstractPixelFormat::CreatePixelFormat( mRenderingBuffer, pixelFormat, colorFormat ) );
        assert( mPixelFormat.get() != NULL );

        // Create the base renderer:
        mRendererBase = RendererPtr( mPixelFormat->CreateRendererBase() );
        assert( mRendererBase.get() != NULL );

        // Create the scanline renderer:
        mScanlineRendererAA  = ScanlineRendererPtr( mRendererBase->CreateScanlineRendererAASolid() );
        mScanlineRendererBin = ScanlineRendererPtr( mRendererBase->CreateScanlineRendererBinSolid() );
        assert( mScanlineRendererAA.get() != NULL );
        assert( mScanlineRendererBin.get() != NULL );

        // Default gamma setting:
//        mRasterizer.gamma( agg::gamma_none() );
        // Create the renderer based on the desired pixel format:
        delete mRenderer;
        switch ( pixelFormat )
        {
        // TODO:

        //case PixelFormat16bppRGB555:
        //    mRenderer = new GraphicsRenderer< agg:: >( mRenderingBuffer );
        //    break;

        //case PixelFormat16bppRGB565:
        //    mRenderer = new GraphicsRenderer< agg::pix_format_rgb565 >( mRenderingBuffer );
        //    break;

        // TODO: This needs to be faked somehow...

        //case PixelFormat16bppRGB1555:
        //    mRenderer = new GraphicsRenderer< agg::pix_format_rg >( mRenderingBuffer );
        //    break;

        case PixelFormat24bppRGB:
            {
            //if ( colorFormat == ColorFormatPremultiplied )
            //    mRenderer = new GraphicsRenderer< agg::pixfmt_rgb24_pre >( mRenderingBuffer, pixelFormat );
            ////else if ( colorFormat == ColorFormatPlain )
            ////    mRenderer = new GraphicsRenderer< agg::pixfmt_rgb24_gamma >( mRenderingBuffer );
            //else // ColorFormatDefault
            //    mRenderer = new GraphicsRenderer< agg::pixfmt_rgb24 >( mRenderingBuffer, pixelFormat );
                //typedef agg::gamma_lut<agg::int8u, agg::int8u, 8, 8> gamma_lut_type;
                //typedef agg::pixfmt_rgb24_gamma<gamma_lut_type> pixfmt;
                //mRenderer = new GraphicsRenderer< pixfmt >( mRenderingBuffer, pixelFormat );
                mRenderer = new GraphicsRenderer< agg::pixfmt_rgb24 >( mRenderingBuffer, pixelFormat );
            }
            break;

        case PixelFormat24bppBGR:
            {
            //if ( colorFormat == ColorFormatPremultiplied )
            //    mRenderer = new GraphicsRenderer< agg::pixfmt_bgr24_pre >( mRenderingBuffer, pixelFormat );
            ////else if ( colorFormat == ColorFormatPlain )
            ////    mRenderer = new GraphicsRenderer< agg::pixfmt_bgr24_gamma >( mRenderingBuffer );
            //else // ColorFormatDefault
            //    mRenderer = new GraphicsRenderer< agg::pixfmt_bgr24 >( mRenderingBuffer, pixelFormat );
                //typedef agg::gamma_lut<agg::int8u, agg::int8u, 8, 8> gamma_lut_type;
                //typedef agg::pixfmt_bgr24_gamma<gamma_lut_type> pixfmt;
                //mRenderer = new GraphicsRenderer< pixfmt >( mRenderingBuffer, pixelFormat );
                mRenderer = new GraphicsRenderer< agg::pixfmt_bgr24 >( mRenderingBuffer, pixelFormat );
            }
            break;

        case PixelFormat32bppARGB:
            {
                if ( colorFormat == ColorFormatPremultiplied )
                    mRenderer = new GraphicsRenderer< agg::pixfmt_argb32_pre >( mRenderingBuffer, pixelFormat );
                else if ( colorFormat == ColorFormatPlain )
                    mRenderer = new GraphicsRenderer< agg::pixfmt_argb32_plain >( mRenderingBuffer, pixelFormat );
                else // ColorFormatDefault
                    mRenderer = new GraphicsRenderer< agg::pixfmt_argb32 >( mRenderingBuffer, pixelFormat );
            }
            break;

        case PixelFormat32bppPARGB:
            // This is pre-multiplied, regardless of the color format
            mRenderer = new GraphicsRenderer< agg::pixfmt_argb32_pre >( mRenderingBuffer, pixelFormat );
            break;

        case PixelFormat32bppRGBA:
            if ( colorFormat == ColorFormatPremultiplied )
                mRenderer = new GraphicsRenderer< agg::pixfmt_rgba32_pre >( mRenderingBuffer, pixelFormat );
            else if ( colorFormat == ColorFormatPlain )
                mRenderer = new GraphicsRenderer< agg::pixfmt_rgba32_plain >( mRenderingBuffer, pixelFormat );
            else // ColorFormatDefault
                mRenderer = new GraphicsRenderer< agg::pixfmt_rgba32 >( mRenderingBuffer, pixelFormat );
            break;

        case PixelFormat32bppABGR:
            if ( colorFormat == ColorFormatPremultiplied )
                mRenderer = new GraphicsRenderer< agg::pixfmt_abgr32_pre >( mRenderingBuffer, pixelFormat );
            else if ( colorFormat == ColorFormatPlain )
                mRenderer = new GraphicsRenderer< agg::pixfmt_abgr32_plain >( mRenderingBuffer, pixelFormat );
            else // ColorFormatDefault
                mRenderer = new GraphicsRenderer< agg::pixfmt_abgr32 >( mRenderingBuffer, pixelFormat );
            break;

        case PixelFormat32bppBGRA:
            if ( colorFormat == ColorFormatPremultiplied )
                mRenderer = new GraphicsRenderer< agg::pixfmt_bgra32_pre >( mRenderingBuffer, pixelFormat );
            else if ( colorFormat == ColorFormatPlain )
                mRenderer = new GraphicsRenderer< agg::pixfmt_bgra32_plain >( mRenderingBuffer, pixelFormat );
            else // ColorFormatDefault
                mRenderer = new GraphicsRenderer< agg::pixfmt_bgra32 >( mRenderingBuffer, pixelFormat );
            break;

        // TODO: I'm not sure these will work right with the rgba8 type...

        //case PixelFormat30bppRGB:
        //    break;

        //case PixelFormat30bppBGR:
        //    break;

        //case PixelFormat32bppRGB:
        //    break;

        //case PixelFormat32bppBGR:
        //    break;

        //case PixelFormat48bppRGB:
        //    break;

        //case PixelFormat48bppBGR:
        //    break;

        //case PixelFormat64bppARGB:
        //    break;

        //case PixelFormat64bppPARGB:
        //    break;

        //case PixelFormat64bppRGBA:
        //    break;

        //case PixelFormat64bppABGR:
        //    break;

        //case PixelFormat64bppBGRA:
        //    break;

        default:
            return SetStatus( NotImplemented );
        }

        return SetStatus( Ok );
    }

    //--------------------------------------------------------------------------

    //Clear---------------------------------------------------------------------
    Status Graphics::Clear( const Color& color )
    {
        if ( mRendererBase.get() )  mRendererBase->clear( ColorExt::toRGBA( color ) );
        return SetStatus( NotImplemented );
    }

    //Save----------------------------------------------------------------------
    GraphicsState Graphics::Save(void)
    {
        //if ( mRenderer )
        //    return mRenderer->Save();
        //return (GraphicsState)-1;

        // Create the data block to save
        GraphicsStateData data;
        data.matrix             = mMatrix;
//        data.fillMode           = mFillMode;
        data.compositeMode      = mCompositeMode;
        data.compositeQuality   = mCompositeQuality;
        data.interpolationMode  = mInterpolationMode;
        data.pageScale          = mPageScale;
        data.pageUnit           = mPageUnit;
        data.pixelOffsetMode    = mPixelOffsetMode;
        data.renderingOrigin    = mRenderingOrigin;
        data.smoothingMode      = mSmoothingMode;
        data.gammaThreshold     = mGammaThreshold;

        GraphicsState index = (GraphicsState)mGraphicsStates.size();
        mGraphicsStates.push( data );

        return index;
    }

    //Restore-------------------------------------------------------------------
    Status Graphics::Restore( GraphicsState gstate )
    {
        //if ( mRenderer )
        //    return mRenderer->Restore( gstate );
        //return InsufficientBuffer;

        // Make sure the state is valid:
        if ( gstate >= mGraphicsStates.size() )
            return InvalidParameter;

        // All states above the restored state are discarded
        while ( mGraphicsStates.size() > gstate + 1 )
            mGraphicsStates.pop();

        // Restore the top state of the stack:
        GraphicsStateData* topState = &mGraphicsStates.top();
        mMatrix             = topState->matrix;
//        mFillMode           = topState->fillMode;
        mCompositeMode      = topState->compositeMode;
        mCompositeQuality   = topState->compositeQuality;
        mInterpolationMode  = topState->interpolationMode;
        mPageScale          = topState->pageScale;
        mPageUnit           = topState->pageUnit;
        mPixelOffsetMode    = topState->pixelOffsetMode;
        mRenderingOrigin    = topState->renderingOrigin;
        mSmoothingMode      = topState->smoothingMode;
        mGammaThreshold     = topState->gammaThreshold;

        // Remove the state:
        mGraphicsStates.pop();

        return Ok;
    }

    //DrawLine------------------------------------------------------------------
    Status Graphics::DrawLine( const Pen* pen, const Point& pt1, const Point& pt2 )
    {
        GraphicsPath path;
        path.AddLine( pt1.X, pt1.Y, pt2.X, pt2.Y );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawLine( const Pen* pen, const PointF& pt1, const PointF& pt2 )
    {
        GraphicsPath path;
        path.AddLine( pt1.X, pt1.Y, pt2.X, pt2.Y );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawLine( const Pen* pen, INT x1, INT y1, INT x2, INT y2 )
    {
        GraphicsPath path;
        path.AddLine( x1, y1, x2, y2 );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawLine( const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2 )
    {
        GraphicsPath path;
        path.AddLine( x1, y1, x2, y2 );
        return DrawPath( pen, &path );
    }

    //DrawLines-----------------------------------------------------------------
    Status Graphics::DrawLines( const Pen* pen, const Point* points, INT count )
    {
        GraphicsPath path;
        path.AddLines( points, count );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawLines( const Pen* pen, const PointF* points, INT count )
    {
        GraphicsPath path;
        path.AddLines( points, count );
        return DrawPath( pen, &path );
    }

    //DrawArc-------------------------------------------------------------------
    Status Graphics::DrawArc( const Pen* pen, const Rect& rect, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddArc( rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawArc( const Pen* pen, const RectF& rect, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddArc( rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawArc( const Pen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddArc( x, y, width, height, startAngle, sweepAngle );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawArc( const Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddArc( x, y, width, height, startAngle, sweepAngle );
        return DrawPath( pen, &path );
    }

    //DrawEllipse---------------------------------------------------------------
    Status Graphics::DrawEllipse( const Pen* pen, const Rect& rect )
    {
        GraphicsPath path;
        path.AddEllipse( rect.X, rect.Y, rect.Width, rect.Height );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawEllipse( const Pen* pen, const RectF& rect )
    {
        GraphicsPath path;
        path.AddEllipse( rect.X, rect.Y, rect.Width, rect.Height );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawEllipse( const Pen* pen, INT x, INT y, INT width, INT height )
    {
        GraphicsPath path;
        path.AddEllipse( x, y, width, height );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawEllipse( const Pen* pen, REAL x, REAL y, REAL width, REAL height )
    {
        GraphicsPath path;
        path.AddEllipse( x, y, width, height );
        return DrawPath( pen, &path );
    }

    //FillEllipse---------------------------------------------------------------
    Status Graphics::FillEllipse( const Brush* brush, const Rect& rect )
    {
        GraphicsPath path;
        path.AddEllipse( rect );
        return FillPath( brush, &path );
    }

    Status Graphics::FillEllipse( const Brush* brush, const RectF& rect )
    {
        GraphicsPath path;
        path.AddEllipse( rect );
        return FillPath( brush, &path );
    }

    Status Graphics::FillEllipse( const Brush* brush, INT x, INT y, INT width, INT height )
    {
        GraphicsPath path;
        path.AddEllipse( x, y, width, height );
        return FillPath( brush, &path );
    }

    Status Graphics::FillEllipse( const Brush* brush, REAL x, REAL y, REAL width, REAL height )
    {
        GraphicsPath path;
        path.AddEllipse( x, y, width, height );
        return FillPath( brush, &path );
    }

    Status Graphics::renderPath( const Brush* brush, const GraphicsPath* path )
    {
        if ( !brush || !path )
            return InvalidParameter;
        if ( !mPixelFormat.get() || !mRendererBase.get() || !mScanlineRendererAA.get() || !mScanlineRendererBin.get() )
            return InvalidParameter;

        typedef SharedPtr< StyleHandler > StylePtr;
        StylePtr handler = StylePtr( mPixelFormat->CreateStyleHandler( brush, &mMatrix ) );
        //StylePtr handler = StylePtr( brush->CreateStyleHandler( &mMatrix ) );

        switch ( mSmoothingMode )
        {
        case SmoothingModeHighQuality:
        case SmoothingModeAntiAlias:
            mRasterizer.gamma( agg::gamma_none() );
            mRendererBase->RenderScanlinesAA( mRasterizer, mScanline, *(handler.get()) );
            //agg::render_scanlines( mRasterizer, mScanline, *(mScanlineRendererAA.get()) );
            break;

        default:
            mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
            mRendererBase->RenderScanlinesBin( mRasterizer, mScanline, *(handler.get()) );
            //agg::render_scanlines( mRasterizer, mScanline, *(mScanlineRendererBin.get()) );
            break;
        }


        ////span_alloc_type sa;
        //switch ( mSmoothingMode )
        //{
        //case SmoothingModeHighQuality:
        //case SmoothingModeAntiAlias:
        //    mRasterizer.gamma( agg::gamma_none() );
        //    agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, *(mScanlineRendererAA.get()), *(handler.get()) );
        //    break;

        //default:
        //    mRasterizer.gamma( agg::gamma_threshold( mGammaThreshold ) );
        //    agg::render_scanlines_aa( mRasterizer, mScanline, mRendererBase, *(mScanlineRendererBin.get()), *(handler.get()) );
        //    break;
        //}

        return Ok;
    }

    //DrawPath------------------------------------------------------------------
    Status Graphics::DrawPath( const Pen* pen, const GraphicsPath* path )
    {
        //if ( !mRenderer )   return SetStatus( NotImplemented );
        //return SetStatus( mRenderer->DrawPath( pen, path ) );

        // TODO: Need to implement additional pen attributes
        //      Not implemented:
        //          custom caps
        //          custom dash styles

        if ( !pen || !path )
            return InvalidParameter;

        mScanline.reset_spans();
        mRasterizer.reset();

        // Convert values such as the line caps and dash style to values
        // compatable with AGG:
        agg::line_join_e lineJoin = convertLineJoin( pen->GetLineJoin() );
        agg::inner_join_e innerJoin = convertInnerJoin( pen->GetLineJoin() );
        //NOTE: AGG only uses one cap.  Start and end will both use this (for now.)
        agg::line_cap_e  startCap = convertLineCap( pen->GetStartCap() );


        // Use the product of the pen and the graphics matrices:
        Matrix penMtx, mtx = mMatrix;
        pen->GetTransform( &penMtx );
        mtx.Multiply( &penMtx );
        agg::path_storage path2( *((GraphicsPath*)path)->getPathStorage() );
        agg::trans_affine aggMtx = mtx.GetAffine();

        typedef agg::conv_curve< agg::path_storage > conv_curve_type;
        conv_curve_type conv_curve_path( path2 );

        // In GDI+, the pen width is scaled according to the matrix.  I
        // haven't been able to duplicate this properly, so it will render
        // a line of the exact width of the pen, regardless of scale.
        REAL width = pen->GetWidth();
        if ( pen->GetDashStyle() == DashStyleSolid )
        {
            typedef agg::conv_stroke< conv_curve_type > path_conv_stroke_type;
            typedef agg::conv_transform< path_conv_stroke_type > path_conv_trans_type;
            path_conv_stroke_type stroke( conv_curve_path );
            path_conv_trans_type trans( stroke, aggMtx );

            stroke.line_join( lineJoin );
            stroke.inner_join( innerJoin );
            stroke.line_cap( startCap );
            stroke.miter_limit( pen->GetMiterLimit() );
            stroke.inner_miter_limit( pen->GetInternalMiterLimit() );
            stroke.width( width );

            stroke.approximation_scale( aggMtx.scale() );
            conv_curve_path.approximation_scale( aggMtx.scale() );

            mRasterizer.add_path( trans );
        }
        else
        {
            typedef agg::conv_dash< conv_curve_type > path_conv_dash_type;
            typedef agg::conv_stroke< path_conv_dash_type > path_conv_stroke_type;

            path_conv_dash_type dash_line( conv_curve_path );
            path_conv_stroke_type stroke( dash_line );

            typedef agg::conv_transform< path_conv_stroke_type > path_conv_trans_type;
            path_conv_trans_type trans( stroke, aggMtx );

            stroke.line_join( lineJoin );
            stroke.inner_join( innerJoin );
            stroke.line_cap( startCap );
            stroke.miter_limit( pen->GetMiterLimit() );
            stroke.inner_miter_limit( pen->GetInternalMiterLimit() );

            stroke.width( width );

            // Set the dash style:
            switch ( pen->GetDashStyle() )
            {
            case DashStyleDot:
                dash_line.add_dash( width, width );
                break;

            case DashStyleDashDot:
                dash_line.add_dash( 3.0 * width, width );
                dash_line.add_dash( width, width );
                break;

            case DashStyleDashDotDot:
                dash_line.add_dash( 3.0 * width, width );
                dash_line.add_dash( width, width );
                dash_line.add_dash( width, width );
                break;

            //case DashStyleCustom:
            //    break;

            default:    // DashStyleDash
                dash_line.add_dash( 3.0 * width, width );
                break;
            }

            stroke.approximation_scale( aggMtx.scale() );

            mRasterizer.add_path( trans );
        }

        return renderPath( pen->GetBrush(), path );
    }

    //FillPath------------------------------------------------------------------
    Status Graphics::FillPath( const Brush* brush, const GraphicsPath* path )
    {
        //if ( !mRenderer )   return SetStatus( NotImplemented );
        //return SetStatus( mRenderer->FillPath( brush, path ) );
        if ( !brush || !path )
            return InvalidParameter;

        typedef agg::conv_curve< agg::path_storage > conv_curve_type;
        typedef agg::conv_transform< conv_curve_type > path_conv_trans_type;

        agg::trans_affine aggMtx = mMatrix.GetAffine();
        agg::path_storage path2( *((GraphicsPath*)path)->getPathStorage() );
        conv_curve_type conv_curve_path( path2 );
        path_conv_trans_type trans( conv_curve_path, aggMtx );
        conv_curve_path.approximation_scale( aggMtx.scale() );

        mScanline.reset_spans();
        mRasterizer.reset();
        mRasterizer.add_path( trans );

        mRasterizer.filling_rule( ( path->GetFillMode() == FillModeAlternate ) ? agg::fill_even_odd : agg::fill_non_zero );
        Status status = renderPath( brush, path );

        // Reset the fulling rule (unfortunately, there is no method to
        // fetch the mode from the rasterizer, so we "just know" what the
        // default value is.
        mRasterizer.filling_rule( agg::fill_non_zero );

        return SetStatus( status );

    }

    //RenderCompoundPath--------------------------------------------------------
    Status Graphics::RenderCompoundPath( const GraphicsPathCompound* path )
    {
        if ( !mRenderer )   return SetStatus( NotImplemented );
        mRenderer->mMatrix = mMatrix;
        return SetStatus( mRenderer->RenderCompoundPath( path ) );

/*
        if ( !path )
            return InvalidParameter;

        GraphicsPathCompound* compound = const_cast< GraphicsPathCompound* >( path );

        // If there is just one path, don't use the compound renderer (it
        // won't work as expected.)  Instead, just pass it off to a simple
        // renderer...
        bool singlePath = false;
        if ( path->GetPathCount() == 1 )
        {
            singlePath = true;
            // Add a transparent rect.  It won't show anything,
            // but it should allow the scene to render.  This isn't ideal,
            // but, hey! :-)
            GraphicsPath* pathStorage = NULL;
            if ( compound->GetGraphicsPath( 0, pathStorage ) != Ok )
                return InvalidParameter;
            SolidBrush tempBr( Color::Transparent );
            GraphicsPath tempPath;
            tempPath.AddRectangle( RectF( 0, 0, 1, 1 ) );
            //compound->AddPath( &tempBr, pathStorage );
        }

        //else
        {
            typedef agg::conv_curve< agg::path_storage > conv_curve_type;
            typedef agg::conv_transform< conv_curve_type > path_conv_trans_type;

            agg::trans_affine aggMtx = mMatrix.GetAffine();
            mScanline.reset_spans();

            // Create a compound rasterizer:
            agg::rasterizer_compound_aa< agg::rasterizer_sl_clip_dbl > rasterizer;
            rasterizer.clip_box( 0, 0, mRendererBase->width(), mRendererBase->height() );
            StyleCollection styleCollection;

            // Go through the compound path, and add each style to a style
            // collection, and add the paths (with the corresponding style
            // index) to the rasterizer.
            for ( int i = 0; i < compound->GetPathCount(); i++ )
            {
                StyleHandler* styleHandler = NULL;
                GraphicsPath* pathStorage = NULL;
                //agg::StyleHandler* styleHandler = path->GetStyleHandler( i );
                //GraphicsPath* pathStorage = path->GetGraphicsPath( i );

                if ( compound->GetStyleHandler( i, styleHandler ) != Ok || compound->GetGraphicsPath( i, pathStorage ) != Ok )
                    return InvalidParameter;

                agg::path_storage path2( *pathStorage->getPathStorage() );

                conv_curve_type conv_curve_path( path2 );
                path_conv_trans_type trans( conv_curve_path, aggMtx );
                conv_curve_path.approximation_scale( aggMtx.scale() );

                styleCollection.AddStyle( styleHandler );
                rasterizer.styles( i, -1 );
                rasterizer.add_path( trans );

                //SolidBrush br( ColorExt::RandomRGB() );
                ////renderPath( &br, pathStorage );
                //FillPath( &br, pathStorage );
            }

            rasterizer.filling_rule( agg::fill_even_odd );
            //rasterizer.filling_rule( ( path->GetFillMode() == FillModeAlternate ) ? agg::fill_even_odd : agg::fill_non_zero );

            typedef agg::span_allocator< agg::rgba8 >   span_alloc_type;
            span_alloc_type sa;
            if ( singlePath )
                RenderScanlinesCompound( rasterizer, mScanline, mScanline, *mRendererBase.get(), sa, styleCollection);
            else
                //RenderScanlinesCompound( rasterizer, mScanline, mScanline, mRendererBase, sa, styleCollection);
                RenderScanlinesCompoundLayered( rasterizer, mScanline, *mRendererBase.get(), sa, styleCollection);
        }

        return Ok;
*/
    }

    ////RenderSVGPath-------------------------------------------------------------
    //Status Graphics::RenderSVGPath( svg::SVGPath* path, const Matrix* mtx, const Rect& clipBox, REAL opacity )
    //{
    //    if ( !mRenderer )   return SetStatus( NotImplemented );
    //    return SetStatus( mRenderer->RenderSVGPath( path, mtx, clipBox, opacity ) );
    //}

    //DrawPolygon---------------------------------------------------------------
    Status Graphics::DrawPolygon( const Pen* pen, const Point* points, INT count )
    {
        GraphicsPath path;
        path.AddPolygon( points, count );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawPolygon( const Pen* pen, const PointF* points, INT count )
    {
        GraphicsPath path;
        path.AddPolygon( points, count );
        return DrawPath( pen, &path );
    }

    //FillPolygon---------------------------------------------------------------
    Status Graphics::FillPolygon( const Brush* brush, const Point* points, INT count )
    {
        GraphicsPath path;
        path.AddPolygon( points, count );
        return FillPath( brush, &path );
    }

    Status Graphics::FillPolygon( const Brush* brush, const PointF* points, INT count )
    {
        GraphicsPath path;
        path.AddPolygon( points, count );
        return FillPath( brush, &path );
    }

    Status Graphics::FillPolygon( const Brush* brush, const Point* points, INT count, FillMode fillMode )
    {
        GraphicsPath path;
        FillMode origFillMode = path.GetFillMode();
        path.SetFillMode( fillMode );
        path.AddPolygon( points, count );
        Status status = FillPath( brush, &path );
        return status;
    }

    Status Graphics::FillPolygon( const Brush* brush, const PointF* points, INT count, FillMode fillMode )
    {
        GraphicsPath path;
        FillMode origFillMode = path.GetFillMode();
        path.SetFillMode( fillMode );
        path.AddPolygon( points, count );
        Status status = FillPath( brush, &path );
        return status;
    }

    //DrawRectangle-------------------------------------------------------------
    Status Graphics::DrawRectangle( const Pen* pen, const Rect& rect )
    {
        GraphicsPath path;
        path.AddRectangle( rect );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawRectangle( const Pen* pen, const RectF& rect )
    {
        GraphicsPath path;
        path.AddRectangle( rect );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawRectangle( const Pen* pen, INT x, INT y, INT width, INT height )
    {
        GraphicsPath path;
        path.AddRectangle( Rect( x, y, width, height ) );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawRectangle( const Pen* pen, REAL x, REAL y, REAL width, REAL height )
    {
        GraphicsPath path;
        path.AddRectangle( RectF( x, y, width, height ) );
        return DrawPath( pen, &path );
    }

    //DrawRectangles------------------------------------------------------------
    Status Graphics::DrawRectangles( const Pen* pen, const Rect* rects, INT count )
    {
        GraphicsPath path;
        path.AddRectangles( rects, count );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawRectangles( const Pen* pen, const RectF* rects, INT count )
    {
        GraphicsPath path;
        path.AddRectangles( rects, count );
        return DrawPath( pen, &path );
    }

    //FillRectangle-------------------------------------------------------------
    Status Graphics::FillRectangle( const Brush* brush, const Rect& rect )
    {
        GraphicsPath path;
        path.AddRectangle( rect );
        return FillPath( brush, &path );
    }

    Status Graphics::FillRectangle( const Brush* brush, const RectF& rect )
    {
        GraphicsPath path;
        path.AddRectangle( rect );
        return FillPath( brush, &path );
    }

    Status Graphics::FillRectangle( const Brush* brush, INT x, INT y, INT width, INT height )
    {
        GraphicsPath path;
        path.AddRectangle( Rect( x, y, width, height ) );
        return FillPath( brush, &path );
    }

    Status Graphics::FillRectangle( const Brush* brush, REAL x, REAL y, REAL width, REAL height )
    {
        GraphicsPath path;
        path.AddRectangle( RectF( x, y, width, height ) );
        return FillPath( brush, &path );
    }

    //FillRectangles------------------------------------------------------------
    Status Graphics::FillRectangles( const Brush* brush, const Rect* rects, INT count )
    {
        GraphicsPath path;
        path.AddRectangles( rects, count );
        return FillPath( brush, &path );
    }

    Status Graphics::FillRectangles( const Brush* brush, const RectF* rects, INT count )
    {
        GraphicsPath path;
        path.AddRectangles( rects, count );
        return FillPath( brush, &path );
    }

    //DrawPie-------------------------------------------------------------------
    Status Graphics::DrawPie( const Pen* pen, const Rect& rect, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddPie( rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawPie( const Pen* pen, const RectF& rect, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddPie( rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawPie( const Pen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddPie( x, y, width, height, startAngle, sweepAngle );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawPie( const Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddPie( x, y, width, height, startAngle, sweepAngle );
        return DrawPath( pen, &path );
    }

    //DrawPie-------------------------------------------------------------------
    Status Graphics::FillPie( const Brush* brush, const Rect& rect, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddPie( rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle );
        return FillPath( brush, &path );
    }

    Status Graphics::FillPie( const Brush* brush, const RectF& rect, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddPie( rect.X, rect.Y, rect.Width, rect.Height, startAngle, sweepAngle );
        return FillPath( brush, &path );
    }

    Status Graphics::FillPie( const Brush* brush, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddPie( x, y, width, height, startAngle, sweepAngle );
        return FillPath( brush, &path );
    }

    Status Graphics::FillPie( const Brush* brush, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle )
    {
        GraphicsPath path;
        path.AddPie( x, y, width, height, startAngle, sweepAngle );
        return FillPath( brush, &path );
    }

    //DrawBezier----------------------------------------------------------------
    Status Graphics::DrawBezier( const Pen* pen, const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4 )
    {
        GraphicsPath path;
        path.AddBezier( pt1, pt2, pt3, pt4 );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawBezier( const Pen* pen, const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4 )
    {
        GraphicsPath path;
        path.AddBezier( pt1, pt2, pt3, pt4 );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawBezier( const Pen* pen, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4 )
    {
        GraphicsPath path;
        path.AddBezier( x1, y1, x2, y2, x3, y3, x4, y4 );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawBezier( const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4 )
    {
        GraphicsPath path;
        path.AddBezier( x1, y1, x2, y2, x3, y3, x4, y4 );
        return DrawPath( pen, &path );
    }

    //DrawBeziers---------------------------------------------------------------
    Status Graphics::DrawBeziers( const Pen* pen, const Point* points, INT count )
    {
        GraphicsPath path;
        path.AddBeziers( points, count );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawBeziers( const Pen* pen, const PointF* points, INT count )
    {
        GraphicsPath path;
        path.AddBeziers( points, count );
        return DrawPath( pen, &path );
    }

    //DrawClosedCurve-----------------------------------------------------------
    Status Graphics::DrawClosedCurve( const Pen* pen, const Point* points, INT count )
    {
        GraphicsPath path;
        path.AddClosedCurve( points, count );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawClosedCurve( const Pen* pen, const PointF* points, INT count )
    {
        GraphicsPath path;
        path.AddClosedCurve( points, count );
        return DrawPath( pen, &path );
    }

    //Status Graphics::DrawClosedCurve( const Pen* pen, const Point* points, INT count, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //Status Graphics::DrawClosedCurve( const Pen* pen, const PointF* points, INT count, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //FillClosedCurve-----------------------------------------------------------
    Status Graphics::FillClosedCurve( const Brush* brush, const Point* points, INT count )
    {
        GraphicsPath path;
        path.AddClosedCurve( points, count );
        return FillPath( brush, &path );
    }

    Status Graphics::FillClosedCurve( const Brush* brush, const PointF* points, INT count )
    {
        GraphicsPath path;
        path.AddClosedCurve( points, count );
        return FillPath( brush, &path );
    }

    //Status Graphics::FillClosedCurve( const Brush* brush, const Point* points, INT count, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //Status Graphics::FillClosedCurve( const Brush* brush, const PointF* points, INT count, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //DrawCurve-----------------------------------------------------------------
    Status Graphics::DrawCurve( const Pen* pen, const Point* points, INT count )
    {
        GraphicsPath path;
        path.AddCurve( points, count );
        return DrawPath( pen, &path );
    }

    Status Graphics::DrawCurve( const Pen* pen, const PointF* points, INT count )
    {
        GraphicsPath path;
        path.AddCurve( points, count );
        return DrawPath( pen, &path );
    }

    //Status Graphics::DrawCurve( const Pen* pen, const Point* points, INT count, INT offset, INT numberOfSegments, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //Status Graphics::DrawCurve( const Pen* pen, const PointF* points, INT count, INT offset, INT numberOfSegments, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //Status Graphics::DrawCurve( const Pen* pen, const Point* points, INT count, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //Status Graphics::DrawCurve( const Pen* pen, const PointF* points, INT count, REAL tension )
    //{
    //    return SetStatus( Ok );
    //}

    //DrawImage-----------------------------------------------------------------
    //Status Graphics::DrawImage( Image* image, const Point* destPoints, INT count );
    //Status Graphics::DrawImage( Image* image, const PointF* destPoints, INT count );
    //Status Graphics::DrawImage( Image* image, const Point* destPoints, INT count, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, ImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData );
    //Status Graphics::DrawImage( Image* image, const PointF* destPoints, INT count, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, ImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData );
    
    //Status Graphics::DrawImage( Image* image, const Rect& destRect, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, ImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData )
    //{
    //    Status status = Ok;
    //    if ( mRenderer )    status = mRenderer->DrawImage( image, destRect, srcx, srcy, srcwidth, srcheight, srcUnit, imageAttributes, callback, callbackData );
    //    return status;
    //}
    Status Graphics::DrawImage( Image* image, const Rect& destRect, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, ImageAttributes* imageAttributes )
    {
        Status status = Ok;
        if ( mRenderer )    status = mRenderer->DrawImage( image, destRect, srcx, srcy, srcwidth, srcheight, srcUnit, imageAttributes );
        return status;
    }

    //Status Graphics::DrawImage( Image* image, const RectF& destRect, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, ImageAttributes* imageAttributes, DrawImageAbort callback, VOID* callbackData )
    //{
    //    Status status = Ok;
    //    if ( mRenderer )    status = mRenderer->DrawImage( image, destRect, srcx, srcy, srcwidth, srcheight, srcUnit, imageAttributes, callback, callbackData );
    //    return status;
    //}
    Status Graphics::DrawImage( Image* image, const RectF& destRect, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, ImageAttributes* imageAttributes )
    {
        Status status = Ok;
        if ( mRenderer )    status = mRenderer->DrawImage( image, destRect, srcx, srcy, srcwidth, srcheight, srcUnit, imageAttributes );
        return status;
    }

    Status Graphics::DrawImage( Image* image, const Point& point )
    {
        return DrawImage( image, point.X, point.Y, image->GetWidth(), image->GetHeight() );
    }

    Status Graphics::DrawImage( Image* image, const PointF& point )
    {
        return DrawImage( image, point.X, point.Y, image->GetWidth(), image->GetHeight() );
    }

    Status Graphics::DrawImage( Image* image, const Rect& rect )
    {
        return DrawImage( image, rect.X, rect.Y, rect.Width, rect.Height );
    }

    Status Graphics::DrawImage( Image* image, const RectF& rect )
    {
        return DrawImage( image, rect.X, rect.Y, rect.Width, rect.Height );
    }

    Status Graphics::DrawImage( Image* image, INT x, INT y )
    {
        return DrawImage( image, x, y, image->GetWidth(), image->GetHeight() );
    }

    Status Graphics::DrawImage( Image* image, REAL x, REAL y )
    {
        return DrawImage( image, x, y, image->GetWidth(), image->GetHeight() );
    }

    // TODO: The Unit parameter is not yet implemented
    Status Graphics::DrawImage( Image* image, INT x, INT y, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit )
    {
        Status status = Ok;
        if ( mRenderer )    status = mRenderer->DrawImage( image, RectF( x, y, image->GetWidth(), image->GetHeight() ), srcx, srcy, srcwidth, srcheight, srcUnit );
        return status;
    }

    Status Graphics::DrawImage( Image* image, REAL x, REAL y, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit )
    {
        Status status = Ok;
        if ( mRenderer )    status = mRenderer->DrawImage( image, RectF( x, y, image->GetWidth(), image->GetHeight() ), srcx, srcy, srcwidth, srcheight, srcUnit );
        return status;
    }

    Status Graphics::DrawImage( Image* image, INT x, INT y, INT width, INT height )
    {
        Status status = Ok;
        if ( mRenderer )    status = mRenderer->DrawImage( image, x, y, width, height );
        return status;
    }

    Status Graphics::DrawImage( Image* image, REAL x, REAL y, REAL width, REAL height )
    {
        Status status = Ok;
        if ( mRenderer )    status = mRenderer->DrawImage( image, x, y, width, height );
        return status;
    }

    //TranslateTransform--------------------------------------------------------
    Status Graphics::TranslateTransform( REAL dx, REAL dy, MatrixOrder order )
    {
        mMatrix.Translate( dx, dy, order );
        return SetStatus( Ok );
        //Status status = Ok;
        //if ( mRenderer )    status = mRenderer->TranslateTransform( dx, dy, order );
        //return SetStatus( status );
    }

    //RotateTransform-----------------------------------------------------------
    Status Graphics::RotateTransform( REAL angle, MatrixOrder order )
    {
        mMatrix.Rotate( angle, order );
        return SetStatus( Ok );
        //Status status = Ok;
        //if ( mRenderer )    status = mRenderer->RotateTransform( angle, order );
        //return SetStatus( status );
    }

    //ScaleTransform------------------------------------------------------------
    Status Graphics::ScaleTransform( REAL sx, REAL sy, MatrixOrder order )
    {
        mMatrix.Scale( sx, sy, order );
        return SetStatus( Ok );
        //Status status = Ok;
        //if ( mRenderer )    status = mRenderer->ScaleTransform( sx, sy, order );
        //return SetStatus( status );
    }

    //MultiplyTransform---------------------------------------------------------
    Status Graphics::MultiplyTransform( Matrix* matrix, MatrixOrder order )
    {
        mMatrix.Multiply( matrix, order );
        return SetStatus( Ok );
        //Status status = Ok;
        //if ( mRenderer )    status = mRenderer->MultiplyTransform( matrix, order );
        //return SetStatus( status );
    }

    //GetTransform--------------------------------------------------------------
    Status Graphics::GetTransform( Matrix* matrix )
    {
        matrix->Reset();
        matrix->Multiply( &mMatrix );
        return SetStatus( Ok );
        //Status status = Ok;
        //if ( mRenderer )    status = mRenderer->GetTransform( matrix );
        //return SetStatus( status );
    }

    //SetTransform--------------------------------------------------------------
    Status Graphics::SetTransform( const Matrix* matrix )
    {
        mMatrix.Reset();
        mMatrix.Multiply( matrix );
        return SetStatus( Ok );
        //Status status = Ok;
        //if ( mRenderer )    status = mRenderer->SetTransform( matrix );
        //return SetStatus( status );
    }

    //ResetTransform------------------------------------------------------------
    Status Graphics::ResetTransform(void)
    {
        mMatrix.Reset();
        return SetStatus( Ok );
        //Status status = Ok;
        //if ( mRenderer )    status = mRenderer->ResetTransform();
        //return SetStatus( status );
    }

    //GetCompositingMode--------------------------------------------------------
    CompositingMode Graphics::GetCompositingMode(void)
    {
        return mCompositeMode;
        //return ( mRenderer ) ? mRenderer->GetCompositingMode() : CompositingModeSourceOver;
    }

    //SetCompositingMode--------------------------------------------------------
    Status Graphics::SetCompositingMode( CompositingMode compositingMode )
    {
        mCompositeMode = compositingMode;
        return SetStatus( Ok );
        //Status status = Ok;
        //if ( mRenderer )    status = mRenderer->SetCompositingMode( compositingMode );
        //return SetStatus( status );
    }

    //GetCompositingQuality-----------------------------------------------------
    CompositingQuality Graphics::GetCompositingQuality(void)
    {
        return mCompositeQuality;
        //return ( mRenderer ) ? mRenderer->GetCompositingQuality() : CompositingQualityInvalid;
    }

    //SetCompositingQuality-----------------------------------------------------
    Status Graphics::SetCompositingQuality( CompositingQuality compositingQuality )
    {
        mCompositeQuality = compositingQuality;
        return SetStatus( Ok );
        //Status status = Ok;
        //if ( mRenderer )    status = mRenderer->SetCompositingQuality( compositingQuality );
        //return SetStatus( status );
    }

    //GetSmoothingMode----------------------------------------------------------
    SmoothingMode Graphics::GetSmoothingMode(void) const
    {
        return mSmoothingMode;
        //return ( mRenderer ) ? mRenderer->GetSmoothingMode() : SmoothingModeInvalid;
    }

    //SetSmoothingMode----------------------------------------------------------
    Status Graphics::SetSmoothingMode( SmoothingMode smoothingMode )
    {
        mSmoothingMode = smoothingMode;
        return SetStatus( Ok );
        //Status status = Ok;
        //if ( mRenderer )    status = mRenderer->SetSmoothingMode( smoothingMode );
        //return SetStatus( status );
    }

    //SetGammaThreshold---------------------------------------------------------
    Status Graphics::SetGammaThreshold( REAL gamma )
    {
        mGammaThreshold = gamma;
        return SetStatus( Ok );
        //if ( mRenderer )    return SetStatus( mRenderer->SetGammaThreshold( gamma ) );
        //return SetStatus( InvalidParameter );
    }
    //GetGammaThreshold---------------------------------------------------------
    REAL Graphics::GetGammaThreshold() const
    {
        return mGammaThreshold;
        //if ( mRenderer )    return mRenderer->GetGammaThreshold();
        //return 0.5;
    }

} // namespace AggOO
