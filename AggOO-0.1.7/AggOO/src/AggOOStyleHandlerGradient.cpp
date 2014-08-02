
/*! $Id: AggOOStyleHandlerGradient.cpp,v 1.2 2008/02/28 20:55:49 dratek Exp $
 *  @file   AggOOStyleHandlerGradient.cpp
 *  
 */

#include "AggOOStyleHandlerGradient.h"

namespace AggOO
{
    //GradientStyleHandler::GradientStyleHandler( const GradientColorItem* colors, int numColors, double gradLength, const agg::trans_affine& mtx, GradientStyle style, WrapMode wrapMode, bool useSolidInterp /*= false*/, double focalX /*= 0.0*/, double focalY /*= 0.0*/, double spiralAngle /*= agg::pi*/ )
    //    : StyleHandler( false ),
    //      mGradLength( gradLength ),
    //      mMatrix( mtx )
    //{
    //    mMatrix.invert();
    //    m_gamma_lut.gamma( 2.0 );
    //    build_gradient_lut( colors, numColors, useSolidInterp );

    //    double r = mGradLength;
    //    double fx = 0, fy = 0;

    //    GradFuncPtr funcPtr;// = agg::GradFuncPtr( NULL );
    //    switch ( style )
    //    {
    //    case GradientStyleCircle:
    //        funcPtr = GradFuncPtr( new GradientCircleFunc() );
    //        break;

    //    case GradientStyleRadial:
    //        funcPtr = GradFuncPtr( new GradientRadialFunc() );
    //        break;

    //    case GradientStyleRadialFocus:
    //        funcPtr = GradFuncPtr( new GradientRadialFocusFunc( r, fx, fy ) );
    //        break;

    //    case GradientStyleXY:
    //        funcPtr = GradFuncPtr( new GradientXYFunc() );
    //        break;

    //    case GradientStyleSqrtXY:
    //        funcPtr = GradFuncPtr( new GradientSqrtXYFunc() );
    //        break;

    //    case GradientStyleConic:
    //        funcPtr = GradFuncPtr( new GradientConicFunc() );
    //        break;

    //    case GradientStyleDiamond:
    //        funcPtr = GradFuncPtr( new GradientDiamondFunc() );
    //        break;

    //    case GradientStyleSpiral:
    //        funcPtr = GradFuncPtr( new GradientSpiral( spiralAngle ) );
    //        break;

    //    case GradientStyleLinear:
    //    default:
    //        funcPtr = GradFuncPtr( new GradientXFunc() );
    //        break;
    //    }


    //    GradAdapterPtr adapterPtr;// = agg::GradAdapterPtr( NULL );
    //    switch ( wrapMode )
    //    {
    //    case WrapModeTile:
    //        adapterPtr = GradAdapterPtr( new GradientRepeatAdapter( funcPtr ) );
    //        break;

    //    case WrapModeClamp:
    //        adapterPtr = GradAdapterPtr( new GradientNullAdapter( funcPtr ) );
    //        break;

    //    case WrapModeFlip:
    //    default:
    //        adapterPtr = GradAdapterPtr( new GradientReflectAdapter( funcPtr ) );
    //        break;
    //    }

    //    mGradAdapter = adapterPtr;

    //    mInterpolator.transformer( mMatrix );
    //    mSpanGen.interpolator( mInterpolator );
    //    mSpanGen.gradient_function( *( mGradAdapter.get() ) );
    //    mSpanGen.color_function( m_gradient_lut );
    //    mSpanGen.d1( 0 );
    //    mSpanGen.d2( mGradLength );
    //}

    //StyleHandler* GradientStyleHandler::Clone() const
    //{
    //    GradientStyleHandler* handler = new GradientStyleHandler();
    //    handler->mMatrix = mMatrix;

    //    handler->m_gradient_lut = m_gradient_lut;
    //    handler->mGradAdapter   = mGradAdapter;
    //    handler->mGradLength    = mGradLength;

    //    handler->mSpanGen = mSpanGen;

    //    return handler;
    //}

    //void GradientStyleHandler::generate_span( agg::rgba8* span, int x, int y, unsigned len )
    //{
    //    mSpanGen.generate( span, x, y, len );
    //}

    //void GradientStyleHandler::generate( agg::rgba8* span, int x, int y, unsigned len )
    //{
    //    mSpanGen.generate( span, x, y, len );
    //}

    //void GradientStyleHandler::build_gradient_lut( const GradientColorItem* colors, int numColors, bool useSolidInterp /*= false*/ )
    //{
    //    m_gradient_lut.remove_all();

    //    int i = 0;
    //    const GradientColorItem *curColor = &colors[ 0 ];
    //    while ( i < numColors )
    //    {
    //        m_gradient_lut.add_color( curColor->mPosition, agg::rgba8_gamma_dir( ColorExt::toRGBA( curColor->mColor ), m_gamma_lut));
    //        i++;
    //        curColor++;
    //    }

    //    m_gradient_lut.build_lut( useSolidInterp );
    //}

} // namespace AggOO
