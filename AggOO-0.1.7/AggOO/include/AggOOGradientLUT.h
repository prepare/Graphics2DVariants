
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
// $Id: AggOOGradientLUT.h,v 1.2 2007/04/19 11:49:57 dratek Exp $
//
// AggOOGradientLUT.h -- A gradient look-up table capable of linear and step
//                       interpolation.
//
// Author       : Chad M. Draper
// Date         : October 19, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-10-19      Initial implementation
//

#ifndef AggOOGradientLUT_H_
#define AggOOGradientLUT_H_

#include <agg_gradient_lut.h>
#include "AggOOEnums.h"

namespace AggOO
{
/*
    //======================================================color_interpolator
    template<class ColorT> struct color_interpolator
    {
    public:
        typedef ColorT color_type;

        color_interpolator(const color_type& c1, 
                           const color_type& c2, 
                           unsigned len) :
            m_c1(c1),
            m_c2(c2),
            m_len(len),
            m_count(0)
        {}

        void operator ++ ()
        {
            ++m_count;
        }

        color_type color() const
        {
            return m_c1.gradient(m_c2, double(m_count) / m_len);
        }

    private:
        color_type m_c1;
        color_type m_c2;
        unsigned   m_len;
        unsigned   m_count;
    };

    //========================================================================
    // Fast specialization for rgba8
    template<> struct color_interpolator<rgba8>
    {
    public:
        typedef rgba8 color_type;

        color_interpolator(const color_type& c1, 
                           const color_type& c2, 
                           unsigned len) :
            r(c1.r, c2.r, len),
            g(c1.g, c2.g, len),
            b(c1.b, c2.b, len),
            a(c1.a, c2.a, len)
        {}

        void operator ++ ()
        {
            ++r; ++g; ++b; ++a;
        }

        color_type color() const
        {
            return color_type(r.y(), g.y(), b.y(), a.y());
        }

    private:
        agg::dda_line_interpolator<14> r, g, b, a;
    };

    //========================================================================
    // Fast specialization for gray8
    template<> struct color_interpolator<gray8>
    {
    public:
        typedef gray8 color_type;

        color_interpolator(const color_type& c1, 
                           const color_type& c2, 
                           unsigned len) :
            v(c1.v, c2.v, len),
            a(c1.a, c2.a, len)
        {}

        void operator ++ ()
        {
            ++v; ++a;
        }

        color_type color() const
        {
            return color_type(v.y(), a.y());
        }

    private:
        agg::dda_line_interpolator<14> v,a;
    };
*/

    //======================================================color_interpolator_step
    /** @class color_interpolator_step

        A specialization of agg::color_interpolator which allows for step-
        interpolation of gradients.
    */
    template<class ColorT> struct color_interpolator_step
    {
    public:
        typedef ColorT color_type;

        color_interpolator_step(const color_type& c1, 
                           const color_type& c2, 
                           unsigned len) :
            m_c1(c1),
            m_c2(c1),
            m_len(len),
            m_count(0)
        {}

        void operator ++ ()
        {
            ++m_count;
        }

        color_type color() const
        {
            return m_c1;
//            return m_c1.gradient(m_c2, double(m_count) / m_len);
        }

    private:
        color_type m_c1;
        color_type m_c2;
        unsigned   m_len;
        unsigned   m_count;
    };

    //========================================================================
    // Fast specialization for rgba8
    template<> struct color_interpolator_step<agg::rgba8>
    {
    public:
        typedef agg::rgba8 color_type;

        color_interpolator_step(const color_type& c1, 
                           const color_type& c2, 
                           unsigned len) :
            r(c1.r, c1.r, len),
            g(c1.g, c1.g, len),
            b(c1.b, c1.b, len),
            a(c1.a, c1.a, len)
        {}

        void operator ++ ()
        {
            ++r; ++g; ++b; ++a;
        }

        color_type color() const
        {
            return color_type(r.y(), g.y(), b.y(), a.y());
        }

    private:
        agg::dda_line_interpolator<14> r, g, b, a;
    };

    //========================================================================
    // Fast specialization for gray8
    template<> struct color_interpolator_step<agg::gray8>
    {
    public:
        typedef agg::gray8 color_type;

        color_interpolator_step(const color_type& c1, 
                           const color_type& c2, 
                           unsigned len) :
            v(c1.v, c1.v, len),
            a(c1.a, c1.a, len)
        {}

        void operator ++ ()
        {
            ++v; ++a;
        }

        color_type color() const
        {
            return color_type(v.y(), a.y());
        }

    private:
        agg::dda_line_interpolator<14> v,a;
    };

    //============================================================gradient_lut
    /** @class gradient_lut

        A class for generating a gradient look-up table.  Similar to the class
        in AGG, but allows the interpolation to be specified (linear or step.)
    */
    template<class ColorT, 
             unsigned ColorLutSize=256> class gradient_lut
    {
    public:
//        typedef ColorInterpolator interpolator_type;
        typedef typename ColorT color_type;
        enum { color_lut_size = ColorLutSize };

        //--------------------------------------------------------------------
        gradient_lut() : m_color_lut(color_lut_size), mUseStepInterpolation( false ) {}

        /**
        // Build Gradient Lut
        // First, call remove_all(), then add_color() at least twice, 
        // then build_lut(). Argument "offset" in add_color must be 
        // in range [0...1] and defines a color stop as it is described 
        // in SVG specification, section Gradients and Patterns. 
        // The simplest linear gradient is:
        //    gradient_lut.add_color(0.0, start_color);
        //    gradient_lut.add_color(1.0, end_color);
        //--------------------------------------------------------------------
        */
        void remove_all();
        void add_color(double offset, const color_type& color);
        void build_lut( bool useStepInterp = false )
        {
            mUseStepInterpolation = useStepInterp;
            if ( useStepInterp )
                buildLut< color_interpolator_step< color_type > >();
            else
                buildLut< agg::color_interpolator< color_type > >();
        }

    private:
        template< class InterpT >
        void buildLut()
        {
            typedef InterpT interpolator_type;

            agg::quick_sort(m_color_profile, offset_less);
            m_color_profile.cut_at(agg::remove_duplicates(m_color_profile, offset_equal));

            if(m_color_profile.size() >= 2)
            {
                unsigned i;
                unsigned start = agg::uround(m_color_profile[0].offset * color_lut_size);
                unsigned end;
                color_type c = m_color_profile[0].color;
                for(i = 0; i < start; i++) 
                {
                    m_color_lut[i] = c;
                }
                for(i = 1; i < m_color_profile.size(); i++)
                {
                    end  = agg::uround(m_color_profile[i].offset * color_lut_size);
                    interpolator_type ci(m_color_profile[i-1].color, 
                                        m_color_profile[i  ].color, 
                                        end - start + 1);
                    while(start < end)
                    {
                        m_color_lut[start] = ci.color();
                        ++ci;
                        ++start;
                    }
                }
                c = m_color_profile.last().color;
                for(; end < m_color_lut.size(); end++)
                {
                    m_color_lut[end] = c;
                }
            }
        }

    public:
        // Size-index Interface. This class can be used directly as the 
        // ColorF in span_gradient. All it needs is two access methods 
        // size() and operator [].
        //--------------------------------------------------------------------
        static unsigned size() 
        { 
            return color_lut_size; 
        }
        const color_type& operator [] (unsigned i) const 
        { 
            return m_color_lut[i]; 
        }

        //!---------------------------------------------------------------------
        //! Set whether to use step interpolation for the gradient
        void StepInterpolation( bool useStepInterp )    { mUseStepInterpolation = useStepInterp; }
        bool StepInterpolation() const                  { return mUseStepInterpolation; }

    private:
        //--------------------------------------------------------------------
        struct color_point
        {
            double     offset;
            color_type color;

            color_point() {}
            color_point(double off, const color_type& c) : 
                offset(off), color(c)
            {
                if(offset < 0.0) offset = 0.0;
                if(offset > 1.0) offset = 1.0;
            }
        };
        typedef agg::pod_bvector<color_point, 4> color_profile_type;
        typedef agg::pod_array<color_type>       color_lut_type;

        static bool offset_less(const color_point& a, const color_point& b)
        {
            return a.offset < b.offset;
        }
        static bool offset_equal(const color_point& a, const color_point& b)
        {
            return a.offset == b.offset;
        }

        //--------------------------------------------------------------------
        color_profile_type  m_color_profile;
        color_lut_type      m_color_lut;
        bool                mUseStepInterpolation;
    };



    //------------------------------------------------------------------------
    template<class T, unsigned S>
    void gradient_lut<T,S>::remove_all()
    { 
        m_color_profile.remove_all(); 
    }

    //------------------------------------------------------------------------
    template<class T, unsigned S>
    void gradient_lut<T,S>::add_color(double offset, const color_type& color)
    {
        m_color_profile.add(color_point(offset, color));
    }

} // namespace AggOO

#endif  // AggOOGradientLUT_H_
