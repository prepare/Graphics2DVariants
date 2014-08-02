
/*! $Id: AggOOStyleHandlerGradient.h,v 1.2 2008/02/28 20:55:48 dratek Exp $
 *  @file   AggOOStyleHandlerGradient.h
 *  @author Chad M. Draper
 *  @date   January 16, 2008
 *  
 *  Defines a style handler for gradients.
 *
 */

#ifndef AGGOO_STYLE_HANDLER_GRADIENT_H_
#define AGGOO_STYLE_HANDLER_GRADIENT_H_

#include "AggOOStyleHandler.h"
#include "AggOOSharedPtr.h"

#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
#include "agg_trans_affine.h"
//#include "agg_gradient_lut.h"
#include "agg_gamma_lut.h"
//#include "agg_span_interpolator_linear.h"

#include "AggOOGradientLUT.h"
#include "AggOOColorExt.h"
#include "AggOOEnums.h"

#include <string>

namespace AggOO
{
    /** @class gradient_spiral
        Calculates values for a spiral gradient
    */
    class gradient_spiral
    {
    private:
        double mSpiralAngle;

    public:
        gradient_spiral() : mSpiralAngle( 1 )   { }
        gradient_spiral( double angle ) : mSpiralAngle( angle )
        {
        }

        int calculate(int x, int y, int d) const
        {
			double dist = sqrt( static_cast<double>( x * x + y * y ) );
            double ang  = atan2( double( y ), double( x ) );
            double distRatio = dist / double( d );
            double curAngle = distRatio * agg::pi;

            double two_pi = agg::pi * 2.0;
            double low = 0.0, high = two_pi;
            double range = high - low;
            double val = ang + 10.0 * two_pi + -mSpiralAngle * curAngle;
            val -= low;
            if ( val < 0 )
            {
                val = fmod( val, range );
                val = fmod( val + range, range );
            }
            else
                val = fmod( val, range );
            val += low;

            //ang = ModRange( ang + 10.0 * two_pi + -mSpiralAngle * curAngle, 0.0, two_pi );
            ang = val;

            double angRatio = ang / two_pi;

            return int( angRatio * d );
        }
    };

    ////////////////////////////////////////////////////////////////////////////

    /** @class GradientFunc
        Base class for the gradient function
    */
    class GradientFunc
    {
    protected:
        std::string mFunctionName;

    public:
        GradientFunc()  { }

        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            return 0;
        }
    };

    /** @class GradientCircleFunc
        Calculates a circle gradient
    */
    class GradientCircleFunc : public GradientFunc
    {
    public:
        GradientCircleFunc() : GradientFunc()   { mFunctionName = "Circle"; }
        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            agg::gradient_circle func;
            return func.calculate( x, y, d );
        }
    };

    /** @class GradientRadialFunc
        Calculates a radial gradient
    */
    class GradientRadialFunc : public GradientFunc
    {
    public:
        GradientRadialFunc() : GradientFunc()   { mFunctionName = "Radial"; }
        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            agg::gradient_radial func;
            return func.calculate( x, y, d );
        }
    };

    /** @class GradientRadialFocusFunc
        Calculates a radial gradient with a focal point
        @param r    Radius of the focal area
        @param fx   X-position of the focal point
        @param fy   Y-position of the focal point
    */
    class GradientRadialFocusFunc : public GradientFunc
    {
    private:
        double mR, mFX, mFY;

    public:
        GradientRadialFocusFunc()
            : GradientFunc(),
              mR( 100 * agg::gradient_subpixel_scale ), mFX( 0 ), mFY( 0 )
        {
            mFunctionName = "Radial Focus";
        }
        GradientRadialFocusFunc( double r, double fx, double fy )
            : GradientFunc(),
              mR( r ), mFX( fx ), mFY( fy )
        {
            mFunctionName = "Radial Focus";
        }
        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            agg::gradient_radial_focus func;
            return func.calculate( x, y, d );
        }
    };

    /** @class GradientXFunc
        Calculates a linear gradient in the x direction
    */
    class GradientXFunc : public GradientFunc
    {
    public:
        GradientXFunc() : GradientFunc()   { mFunctionName = "X"; }
        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            agg::gradient_x func;
            return func.calculate( x, y, d );
        }
    };

    /** @class GradientYFunc
        Calculates a linear gradient in the y direction
    */
    class GradientYFunc : public GradientFunc
    {
    public:
        GradientYFunc() : GradientFunc()   { mFunctionName = "Y"; }
        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            agg::gradient_y func;
            return func.calculate( x, y, d );
        }
    };

    /** @class GradientXYFunc
        Calculates an xy gradient
    */
    class GradientXYFunc : public GradientFunc
    {
    public:
        GradientXYFunc() : GradientFunc()   { mFunctionName = "XY"; }
        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            agg::gradient_xy func;
            return func.calculate( x, y, d );
        }
    };

    /** @class GradientXYFunc
        Calculates a sqrt( xy ) gradient
    */
    class GradientSqrtXYFunc : public GradientFunc
    {
    public:
        GradientSqrtXYFunc() : GradientFunc()   { mFunctionName = "SqrtXY"; }
        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            agg::gradient_sqrt_xy func;
            return func.calculate( x, y, d );
        }
    };

    /** @class GradientXYFunc
        Calculates a diamond gradient
    */
    class GradientDiamondFunc : public GradientFunc
    {
    public:
        GradientDiamondFunc() : GradientFunc()   { mFunctionName = "Diamond"; }
        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            agg::gradient_diamond func;
            return func.calculate( x, y, d );
        }
    };

    /** @class GradientXYFunc
        Calculates a conic gradient
    */
    class GradientConicFunc : public GradientFunc
    {
    public:
        GradientConicFunc() : GradientFunc()   { mFunctionName = "Conic"; }

        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            agg::gradient_conic func;
            return func.calculate( x, y, d );
        }
    };

    /** @class GradientSpiral
        Calculates a spiral gradient
    */
    class GradientSpiral : public GradientFunc
    {
    private:
        double  mAngle;

    public:
        GradientSpiral()
            : GradientFunc(), mAngle( agg::pi )
        {
            mFunctionName = "Spiral";
        }
        GradientSpiral( double angle )
            : GradientFunc(), mAngle( angle )
        {
            mFunctionName = "Spiral";
        }

        AGG_INLINE virtual int calculate(int x, int y, int d)
        {
            gradient_spiral func( mAngle );
            return func.calculate( x, y, d );
        }
    };

    typedef AggOO::SharedPtr< GradientFunc >       GradFuncPtr;

    ////////////////////////////////////////////////////////////////////////////

    /** @class GradientAdapter
        Base class for the gradient adapter

        @remark     The gradient styles were basically wrappers for the AGG
                    versions.  The adapters are going to be reimplementations.
                    The reason is that the adapters require the gradient
                    function as a template parameter, but because we are doing
                    this to avoid templates, that is a bit tricky...  Instead,
                    since there are only 2 adapters, the code will be copied,
                    and they will take a pointer to the function, of type
                    GradientFunc*.
    */
    class GradientAdapter
    {
    protected:
        GradFuncPtr mFunction;
        std::string mAdapterName;

    public:
        GradientAdapter( GradFuncPtr func )
            : mFunction( func )
        {
        }

        AGG_INLINE virtual int calculate(int x, int y, int d) const = 0;
    };

    /** @class GradientNullAdapter
        Sometimes, there is no adapter applied to the gradient function (i.e.
        the clamp method.)  This allows passing the gradient function on to
        the span_generator by just applying an empty GradientAdapter class
        to the function.
    */
    class GradientNullAdapter : public GradientAdapter
    {
    public:
        GradientNullAdapter( GradFuncPtr func )
            : GradientAdapter( func )
        {
            mAdapterName = "NULL";
        }

        AGG_INLINE int calculate(int x, int y, int d) const
        {
            int ret = mFunction->calculate(x, y, d);
            return ret;
        }
    };

    /** @class GradientRepeatAdapter
        Tiles the gradient out of bounds
    */
    class GradientRepeatAdapter : public GradientAdapter
    {
    public:
        GradientRepeatAdapter( GradFuncPtr func )
            : GradientAdapter( func )
        {
            mAdapterName = "Repeat";
        }

        AGG_INLINE int calculate(int x, int y, int d) const
        {
            if ( d == 0 ) return 0;
            int ret = mFunction->calculate(x, y, d) % d;
            if(ret < 0) ret += d;
            return ret;
        }
    };

    /** @class GradientRepeatAdapter
        Mirrors the gradient out of bounds
    */
    class GradientReflectAdapter : public GradientAdapter
    {
    public:
        GradientReflectAdapter( GradFuncPtr func )
            : GradientAdapter( func )
        {
            mAdapterName = "Reflect";
        }

        AGG_INLINE int calculate(int x, int y, int d) const
        {
            int d2 = d << 1;
            if ( d2 == 0 ) return 0;
            int ret = mFunction->calculate(x, y, d) % d2;
            if(ret <  0) ret += d2;
            if(ret >= d) ret  = d2 - ret;
            return ret;
        }
    };
    typedef AggOO::SharedPtr< GradientAdapter >    GradAdapterPtr;

    ////////////////////////////////////////////////////////////////////////////

    /** @class GradientStyleHandler

    */
    template < typename ColorT >
    class GradientStyleHandler : public StyleHandler
    {
    public:
        typedef ColorT color_type;
        typedef agg::gamma_lut< agg::int8u, agg::int8u > gamma_lut_type;
        //typedef agg::gradient_radial_focus gradient_func_type;
        //typedef agg::gradient_radial gradient_func_type;
        //typedef agg::gradient_lut< agg::color_interpolator< color_type >, 1024 > color_func_type;
        typedef AggOO::gradient_lut< color_type, 1024 > color_func_type;
        typedef agg::span_interpolator_linear<> interpolator_type;
        //typedef agg::span_allocator< agg::rgba8 > span_allocator_type;

        //typedef agg::gradient_reflect_adaptor<gradient_func_type> gradient_adaptor_type;
        //typedef agg::span_gradient<color_type, 
        //                           interpolator_type, 
        //                           gradient_adaptor_type, 
        //                           color_func_type> span_gradient_type;

        typedef agg::span_gradient<color_type, 
                                interpolator_type, 
                                GradientAdapter, 
                                color_func_type> span_gradient_type;

        //enum WrapMode   { WrapTile, WrapClamp, WrapFlip };
        //enum GradStyle  { StyleCircle, StyleRadial, StyleRadialFocus, StyleXY, StyleSqrtXY, StyleConic, StyleDiamond, StyleSpiral, StyleLinear };

    private:
        color_func_type     m_gradient_lut;
        gamma_lut_type      m_gamma_lut;
        agg::trans_affine   mMatrix;
        double              mGradLength;

        //agg::GradFuncPtr    mGradientFuncPtr;
        GradAdapterPtr mGradAdapter;
        //GradientAdapterT    mGradAdapter;

        void build_gradient_lut( const GradientColorItem* colors, int numColors, bool useSolidInterp = false )
        {
            m_gradient_lut.remove_all();

            int i = 0;
            const GradientColorItem *curColor = &colors[ 0 ];
            while ( i < numColors )
            {
                m_gradient_lut.add_color( curColor->mPosition, agg::rgba8_gamma_dir( ColorExt::toRGBA( curColor->mColor ), m_gamma_lut));
                i++;
                curColor++;
            }

            m_gradient_lut.build_lut( useSolidInterp );
        }

    public:
        GradientStyleHandler() : StyleHandler( false )  { }
        GradientStyleHandler( const GradientColorItem* colors, int numColors, double gradLength, const agg::trans_affine& mtx, GradientStyle style, WrapMode wrapMode, bool useSolidInterp = false, double focalX = 0.0, double focalY = 0.0, double spiralAngle = agg::pi )
            : StyleHandler( false ),
            mGradLength( gradLength ),
            mMatrix( mtx )
        {
            mMatrix.invert();
            m_gamma_lut.gamma( 2.0 );
            build_gradient_lut( colors, numColors, useSolidInterp );

            double r = mGradLength;
            double fx = 0, fy = 0;

            GradFuncPtr funcPtr;// = agg::GradFuncPtr( NULL );
            switch ( style )
            {
            case GradientStyleCircle:
                funcPtr = GradFuncPtr( new GradientCircleFunc() );
                break;

            case GradientStyleRadial:
                funcPtr = GradFuncPtr( new GradientRadialFunc() );
                break;

            case GradientStyleRadialFocus:
                funcPtr = GradFuncPtr( new GradientRadialFocusFunc( r, fx, fy ) );
                break;

            case GradientStyleXY:
                funcPtr = GradFuncPtr( new GradientXYFunc() );
                break;

            case GradientStyleSqrtXY:
                funcPtr = GradFuncPtr( new GradientSqrtXYFunc() );
                break;

            case GradientStyleConic:
                funcPtr = GradFuncPtr( new GradientConicFunc() );
                break;

            case GradientStyleDiamond:
                funcPtr = GradFuncPtr( new GradientDiamondFunc() );
                break;

            case GradientStyleSpiral:
                funcPtr = GradFuncPtr( new GradientSpiral( spiralAngle ) );
                break;

            case GradientStyleLinear:
            default:
                funcPtr = GradFuncPtr( new GradientXFunc() );
                break;
            }


            GradAdapterPtr adapterPtr;// = agg::GradAdapterPtr( NULL );
            switch ( wrapMode )
            {
            case WrapModeTile:
                adapterPtr = GradAdapterPtr( new GradientRepeatAdapter( funcPtr ) );
                break;

            case WrapModeClamp:
                adapterPtr = GradAdapterPtr( new GradientNullAdapter( funcPtr ) );
                break;

            case WrapModeFlip:
            default:
                adapterPtr = GradAdapterPtr( new GradientReflectAdapter( funcPtr ) );
                break;
            }

            mGradAdapter = adapterPtr;

            mInterpolator.transformer( mMatrix );
            mSpanGen.interpolator( mInterpolator );
            mSpanGen.gradient_function( *( mGradAdapter.get() ) );
            mSpanGen.color_function( m_gradient_lut );
            mSpanGen.d1( 0 );
            mSpanGen.d2( mGradLength );
            int d2 = mSpanGen.d2();
            int i = 45;
        }

        virtual StyleHandler* Clone() const
        {
            GradientStyleHandler* handler = new GradientStyleHandler();
            handler->mMatrix = mMatrix;

            handler->m_gradient_lut = m_gradient_lut;
            handler->mGradAdapter   = mGradAdapter;
            handler->mGradLength    = mGradLength;

            handler->mSpanGen = mSpanGen;

            return handler;
        }

        void generate_span( ColorT* span, int x, int y, unsigned len )
        {
            mSpanGen.generate( span, x, y, len );
        }
        void generate( ColorT* span, int x, int y, unsigned len )
        {
            mSpanGen.generate( span, x, y, len );
        }

    private:
        interpolator_type   mInterpolator;
        span_gradient_type  mSpanGen;
    //    agg::rgba8  mColor;
    };

} // namespace AggOO

#endif  // AGGOO_STYLE_HANDLER_GRADIENT_H_
