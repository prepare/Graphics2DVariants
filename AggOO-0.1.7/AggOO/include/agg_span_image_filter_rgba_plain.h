
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
// $Id: agg_span_image_filter_rgba_plain.h,v 1.2 2007/08/10 18:38:42 dratek Exp $
//
// agg_span_image_filter_rgba_plain.h -- Image filtering for rgba_plain colors.
//
// Author       : Chad M. Draper
// Date         : November 14, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-11-14      Initial implementation
//

#ifndef agg_span_image_filter_rgba_plain_H_
#define agg_span_image_filter_rgba_plain_H_


#include "agg_basics.h"
#include "agg_color_rgba.h"
#include "agg_span_image_filter.h"

namespace agg
{
    /**
        The image filters provided with Anti-Grain Geometry work only with pre-
        multiplied alpha (for performance reasons.)  However, there are times
        where using pre-multiplied alpha is inappropriate (namely, when
        rendering to transparent layers in a third-party application.)  For this
        reason, AggOO provides custom image filters to handle plain alpha, but
        will adhere to the AGG conventions (including keeping the classes in the
        'agg' namespace.
    */

    int clampi( int val, int min, int max )
    {
        int ret = val;
        if ( max < min )
        {
            if ( ret < max )
                ret = max;
            else if ( ret > min )
                ret = min;
        }
        else
        {
            if ( ret < min )
                ret = min;
            else if ( ret > max )
                ret = max;
        }
        return ret;
    }

    float clampf( float val, float min, float max )
    {
        float ret = val;
        if ( max < min )
        {
            if ( ret < max )
                ret = max;
            else if ( ret > min )
                ret = min;
        }
        else
        {
            if ( ret < min )
                ret = min;
            else if ( ret > max )
                ret = max;
        }
        return ret;
    }

    int roundi( float val )     { return (int)( val + 0.5f ); }
    int roundi( double val )    { return (int)( val + 0.5 ); }
    float roundf( float val )   { return (float)( int( val + 0.5f ) ); }

    int lerpi( float ratio, int min, int max )
    {
        int diff = ( max - min );
        return int( ratio * diff + min );
    }
    int lerpf( float ratio, float min, float max )
    {
        float diff = ( max - min );
        return ratio * diff + min;
    }

    /** @class span_image_filter_rgba_plain_bilinear

        Perform bilinear filtering
    */
    template<class Source, class Interpolator> 
    class span_image_filter_rgba_plain_bilinear : 
    public span_image_filter<Source, Interpolator>
    {
    public:
        typedef Source source_type;
        typedef typename source_type::color_type color_type;
        typedef typename source_type::order_type order_type;
        typedef Interpolator interpolator_type;
        typedef span_image_filter<source_type, interpolator_type> base_type;
        typedef typename color_type::value_type value_type;
        typedef typename color_type::calc_type calc_type;
        enum base_scale_e
        {
            base_shift = color_type::base_shift,
            base_mask  = color_type::base_mask
        };

        //--------------------------------------------------------------------
        span_image_filter_rgba_plain_bilinear() {}
        span_image_filter_rgba_plain_bilinear(source_type& src, 
                                        interpolator_type& inter) :
            base_type(src, inter, 0) 
        {}


        //--------------------------------------------------------------------
        void generate(color_type* span, int x, int y, unsigned len)
        {
            base_type::interpolator().begin(x + base_type::filter_dx_dbl(), 
                                            y + base_type::filter_dy_dbl(), len);

            calc_type fg[4];
            const value_type *fg_ptr;

            do
            {
                int x_hr;
                int y_hr;

                base_type::interpolator().coordinates(&x_hr, &y_hr);

                x_hr -= base_type::filter_dx_int();
                y_hr -= base_type::filter_dy_int();

                int x_lr = x_hr >> image_subpixel_shift;
                int y_lr = y_hr >> image_subpixel_shift;

                unsigned weight;

                fg[0] = 
                fg[1] = 
                fg[2] = 
                fg[3] = image_subpixel_scale * image_subpixel_scale / 2;

                x_hr &= image_subpixel_mask;
                y_hr &= image_subpixel_mask;

                fg_ptr = (const value_type*)base_type::source().span(x_lr, y_lr, 2);
                weight = (image_subpixel_scale - x_hr) * 
                         (image_subpixel_scale - y_hr);
                fg[0] += weight * *fg_ptr++;
                fg[1] += weight * *fg_ptr++;
                fg[2] += weight * *fg_ptr++;
                fg[3] += weight * *fg_ptr;

                fg_ptr = (const value_type*)base_type::source().next_x();
                weight = x_hr * (image_subpixel_scale - y_hr);
                fg[0] += weight * *fg_ptr++;
                fg[1] += weight * *fg_ptr++;
                fg[2] += weight * *fg_ptr++;
                fg[3] += weight * *fg_ptr;

                fg_ptr = (const value_type*)base_type::source().next_y();
                weight = (image_subpixel_scale - x_hr) * y_hr;
                fg[0] += weight * *fg_ptr++;
                fg[1] += weight * *fg_ptr++;
                fg[2] += weight * *fg_ptr++;
                fg[3] += weight * *fg_ptr;

                fg_ptr = (const value_type*)base_type::source().next_x();
                weight = x_hr * y_hr;
                fg[0] += weight * *fg_ptr++;
                fg[1] += weight * *fg_ptr++;
                fg[2] += weight * *fg_ptr++;
                fg[3] += weight * *fg_ptr;

                span->r = value_type(fg[order_type::R] >> (image_subpixel_shift * 2));
                span->g = value_type(fg[order_type::G] >> (image_subpixel_shift * 2));
                span->b = value_type(fg[order_type::B] >> (image_subpixel_shift * 2));
                span->a = value_type(fg[order_type::A] >> (image_subpixel_shift * 2));

                ++span;
                ++base_type::interpolator();

            } while(--len);
        }
    };


    //====================================span_image_filter_rgba_plain_bilinear_clip
    template<class Source, class Interpolator> 
    class span_image_filter_rgba_plain_bilinear_clip : 
    public span_image_filter<Source, Interpolator>
    {
    public:
        typedef Source source_type;
        typedef typename source_type::color_type color_type;
        typedef typename source_type::order_type order_type;
        typedef Interpolator interpolator_type;
        typedef span_image_filter<source_type, interpolator_type> base_type;
        typedef typename color_type::value_type value_type;
        typedef typename color_type::calc_type calc_type;
        enum base_scale_e
        {
            base_shift = color_type::base_shift,
            base_mask  = color_type::base_mask
        };

        //--------------------------------------------------------------------
        span_image_filter_rgba_plain_bilinear_clip() {}
        span_image_filter_rgba_plain_bilinear_clip(source_type& src, 
                                             const color_type& back_color,
                                             interpolator_type& inter) :
            base_type(src, inter, 0),
            m_back_color(back_color)
        {}
        const color_type& background_color() const { return m_back_color; }
        void background_color(const color_type& v)   { m_back_color = v; }


        //--------------------------------------------------------------------
        void generate(color_type* span, int x, int y, unsigned len)
        {
            base_type::interpolator().begin(x + base_type::filter_dx_dbl(), 
                                            y + base_type::filter_dy_dbl(), len);

            calc_type fg[4];
            value_type back_r = m_back_color.r;
            value_type back_g = m_back_color.g;
            value_type back_b = m_back_color.b;
            value_type back_a = m_back_color.a;

            const value_type *fg_ptr;
            int maxx = base_type::source().width() - 1;
            int maxy = base_type::source().height() - 1;

            do
            {
                int x_hr;
                int y_hr;

                base_type::interpolator().coordinates(&x_hr, &y_hr);

                x_hr -= base_type::filter_dx_int();
                y_hr -= base_type::filter_dy_int();

                int x_lr = x_hr >> image_subpixel_shift;
                int y_lr = y_hr >> image_subpixel_shift;

                unsigned weight;
                double weight_dbl, x_weight_dbl, y_weight_dbl;
//                float weight;

                x_weight_dbl = ( x_hr / 256.0 ) - x_lr;
                y_weight_dbl = ( y_hr / 256.0 ) - y_lr;

                if(x_lr >= 0    && y_lr >= 0 &&
                   x_lr <  maxx && y_lr <  maxy) 
                {
                    fg[order_type::R] = 
                    fg[order_type::G] = 
                    fg[order_type::B] = 
                    fg[order_type::A] = image_subpixel_scale * image_subpixel_scale / 2;

                    calc_type baseVal = image_subpixel_scale * image_subpixel_scale / 2;

//std::cout << "x = " << x << ", y = " << y << ", x_hr = " << x_hr << ", y_hr = " << y_hr << ", x_lr = " << x_lr << ", y_lr = " << y_lr << ", x_weight_dbl = " << x_weight_dbl << ", y_weight_dbl = " << y_weight_dbl;

                    x_hr &= image_subpixel_mask;
                    y_hr &= image_subpixel_mask;

                    fg_ptr = (const value_type*)
                        base_type::source().row_ptr(y_lr) + (x_lr << 2);

                    // Top color:
                    calc_type tl[ 4 ], tr[ 4 ];
/*
                    weight = (image_subpixel_scale - x_hr) * (image_subpixel_scale - y_hr);
                    tl[ order_type::R ] = ( baseVal + weight * fg_ptr[ order_type::R ] ) >> ( image_subpixel_shift * 2 );
                    tl[ order_type::G ] = ( baseVal + weight * fg_ptr[ order_type::G ] ) >> ( image_subpixel_shift * 2 );
                    tl[ order_type::B ] = ( baseVal + weight * fg_ptr[ order_type::B ] ) >> ( image_subpixel_shift * 2 );
                    tl[ order_type::A ] = ( baseVal + weight * fg_ptr[ order_type::A ] ) >> ( image_subpixel_shift * 2 );
*/
                    weight_dbl = ( 1.0 - x_weight_dbl ) * ( 1.0 - y_weight_dbl );
/*
                    tl[ order_type::R ] = ( weight_dbl * fg_ptr[ order_type::R ] );
                    tl[ order_type::G ] = ( weight_dbl * fg_ptr[ order_type::G ] );
                    tl[ order_type::B ] = ( weight_dbl * fg_ptr[ order_type::B ] );
                    tl[ order_type::A ] = ( weight_dbl * fg_ptr[ order_type::A ] );
*/
//std::cout << ", weight1 = " << weight_dbl;
//weight_dbl = ( 1.0 - x_weight_dbl );
                    tl[ order_type::R ] = weight_dbl * fg_ptr[ order_type::R ];
                    tl[ order_type::G ] = weight_dbl * fg_ptr[ order_type::G ];
                    tl[ order_type::B ] = weight_dbl * fg_ptr[ order_type::B ];
                    tl[ order_type::A ] = weight_dbl * fg_ptr[ order_type::A ];
                    fg_ptr += 4;

/*
                    weight = x_hr * (image_subpixel_scale - y_hr);
                    tr[ order_type::R ] = ( baseVal + weight * fg_ptr[ order_type::R ] ) >> ( image_subpixel_shift * 2 );
                    tr[ order_type::G ] = ( baseVal + weight * fg_ptr[ order_type::G ] ) >> ( image_subpixel_shift * 2 );
                    tr[ order_type::B ] = ( baseVal + weight * fg_ptr[ order_type::B ] ) >> ( image_subpixel_shift * 2 );
                    tr[ order_type::A ] = ( baseVal + weight * fg_ptr[ order_type::A ] ) >> ( image_subpixel_shift * 2 );
*/
                    weight_dbl = x_weight_dbl * ( 1.0 - y_weight_dbl );
/*
                    tr[ order_type::R ] = ( weight_dbl * fg_ptr[ order_type::R ] );
                    tr[ order_type::G ] = ( weight_dbl * fg_ptr[ order_type::G ] );
                    tr[ order_type::B ] = ( weight_dbl * fg_ptr[ order_type::B ] );
                    tr[ order_type::A ] = ( weight_dbl * fg_ptr[ order_type::A ] );
*/
//std::cout << ", weight2 = " << weight_dbl;
//weight_dbl = x_weight_dbl;
                    tr[ order_type::R ] = weight_dbl * fg_ptr[ order_type::R ];
                    tr[ order_type::G ] = weight_dbl * fg_ptr[ order_type::G ];
                    tr[ order_type::B ] = weight_dbl * fg_ptr[ order_type::B ];
                    tr[ order_type::A ] = weight_dbl * fg_ptr[ order_type::A ];
                    fg_ptr += 4;

                    // Blend the top-left and top-right colors:
//                    alpha_blend( tl, tr, tl );
                    tl[ order_type::R ] = tl[ order_type::R ] + tr[ order_type::R ];
                    tl[ order_type::G ] = tl[ order_type::G ] + tr[ order_type::G ];
                    tl[ order_type::B ] = tl[ order_type::B ] + tr[ order_type::B ];
                    tl[ order_type::A ] = tl[ order_type::A ] + tr[ order_type::A ];

                    fg_ptr += 4;

                    ++y_lr;
                    fg_ptr = (const value_type*)
                        base_type::source().row_ptr(y_lr) + (x_lr << 2);

                    // Bottom color:
                    calc_type bl[ 4 ], br[ 4 ];
/*
                    weight = (image_subpixel_scale - x_hr) * y_hr;
                    bl[ order_type::R ] = ( baseVal + weight * fg_ptr[ order_type::R ] ) >> ( image_subpixel_shift * 2 );
                    bl[ order_type::G ] = ( baseVal + weight * fg_ptr[ order_type::G ] ) >> ( image_subpixel_shift * 2 );
                    bl[ order_type::B ] = ( baseVal + weight * fg_ptr[ order_type::B ] ) >> ( image_subpixel_shift * 2 );
                    bl[ order_type::A ] = ( baseVal + weight * fg_ptr[ order_type::A ] ) >> ( image_subpixel_shift * 2 );
*/
                    weight_dbl = ( 1.0 - x_weight_dbl ) * y_weight_dbl;
/*
                    bl[ order_type::R ] = ( weight_dbl * fg_ptr[ order_type::R ] );
                    bl[ order_type::G ] = ( weight_dbl * fg_ptr[ order_type::G ] );
                    bl[ order_type::B ] = ( weight_dbl * fg_ptr[ order_type::B ] );
                    bl[ order_type::A ] = ( weight_dbl * fg_ptr[ order_type::A ] );
*/
//std::cout << ", weight3 = " << weight_dbl;
//weight_dbl = ( 1.0 - x_weight_dbl );
                    bl[ order_type::R ] = weight_dbl * fg_ptr[ order_type::R ];
                    bl[ order_type::G ] = weight_dbl * fg_ptr[ order_type::G ];
                    bl[ order_type::B ] = weight_dbl * fg_ptr[ order_type::B ];
                    bl[ order_type::A ] = weight_dbl * fg_ptr[ order_type::A ];
                    fg_ptr += 4;

/*
                    weight = x_hr * y_hr;
                    br[ order_type::R ] = ( baseVal + weight * fg_ptr[ order_type::R ] ) >> ( image_subpixel_shift * 2 );
                    br[ order_type::G ] = ( baseVal + weight * fg_ptr[ order_type::G ] ) >> ( image_subpixel_shift * 2 );
                    br[ order_type::B ] = ( baseVal + weight * fg_ptr[ order_type::B ] ) >> ( image_subpixel_shift * 2 );
                    br[ order_type::A ] = ( baseVal + weight * fg_ptr[ order_type::A ] ) >> ( image_subpixel_shift * 2 );
*/
                    weight_dbl = x_weight_dbl * y_weight_dbl;
/*
                    br[ order_type::R ] = ( weight_dbl * fg_ptr[ order_type::R ] );
                    br[ order_type::G ] = ( weight_dbl * fg_ptr[ order_type::G ] );
                    br[ order_type::B ] = ( weight_dbl * fg_ptr[ order_type::B ] );
                    br[ order_type::A ] = ( weight_dbl * fg_ptr[ order_type::A ] );
*/
//std::cout << ", weight4 = " << weight_dbl << std::endl;
//weight_dbl = 1.0 - x_weight_dbl;
                    br[ order_type::R ] = weight_dbl * fg_ptr[ order_type::R ];
                    br[ order_type::G ] = weight_dbl * fg_ptr[ order_type::G ];
                    br[ order_type::B ] = weight_dbl * fg_ptr[ order_type::B ];
                    br[ order_type::A ] = weight_dbl * fg_ptr[ order_type::A ];
                    fg_ptr += 4;

                    // Blend the bottom-left and bottom-right colors:
//                    alpha_blend( bl, br, bl );
                    bl[ order_type::R ] = bl[ order_type::R ] + br[ order_type::R ];
                    bl[ order_type::G ] = bl[ order_type::G ] + br[ order_type::G ];
                    bl[ order_type::B ] = bl[ order_type::B ] + br[ order_type::B ];
                    bl[ order_type::A ] = bl[ order_type::A ] + br[ order_type::A ];

                    fg[ order_type::R ] = tl[ order_type::R ] + bl[ order_type::R ];
                    fg[ order_type::G ] = tl[ order_type::G ] + bl[ order_type::G ];
                    fg[ order_type::B ] = tl[ order_type::B ] + bl[ order_type::B ];
                    fg[ order_type::A ] = tl[ order_type::A ] + bl[ order_type::A ];

//                    tl[ order_type::A ] = roundi( ( 1.0 - y_weight_dbl ) * tl[ order_type::A ] );
//                    bl[ order_type::A ] = roundi( y_weight_dbl * bl[ order_type::A ] );

                    // Blend the top and bottom colors:
                    alpha_blend( tl, bl, fg );
//memcpy( fg, tl, sizeof( calc_type ) * 4 );
                }
                else
                {
                    // Handle the border cases:
                    if(x_lr < -1   || y_lr < -1 ||
                       x_lr > maxx || y_lr > maxy)
                    {
                        fg[order_type::R] = back_r;
                        fg[order_type::G] = back_g;
                        fg[order_type::B] = back_b;
                        fg[order_type::A] = back_a;
                    }
                    else
                    {
/*
                        fg[0] = 
                        fg[1] = 
                        fg[2] = 
                        fg[3] = image_subpixel_scale * image_subpixel_scale / 2;

                        x_hr &= image_subpixel_mask;
                        y_hr &= image_subpixel_mask;


                        // Top color:
                        calc_type tl[ 4 ], tr[ 4 ];

                        weight = (image_subpixel_scale - x_hr) * 
                                 (image_subpixel_scale - y_hr);
                        if(x_lr >= 0    && y_lr >= 0 &&
                           x_lr <= maxx && y_lr <= maxy)
                        {
                            fg_ptr = (const value_type*)
                                base_type::source().row_ptr(y_lr) + (x_lr << 2);

                            tl[order_type::R] = ( fg_ptr[order_type::R] * weight ) >> image_subpixel_shift * 2;
                            tl[order_type::G] = ( fg_ptr[order_type::G] * weight ) >> image_subpixel_shift * 2;
                            tl[order_type::B] = ( fg_ptr[order_type::B] * weight ) >> image_subpixel_shift * 2;
                            tl[order_type::A] = ( fg_ptr[order_type::A] * weight ) >> image_subpixel_shift * 2;
                            fg_ptr += 4;
                        }
                        else
                        {
                            tl[order_type::R] = ( back_r * weight ) >> image_subpixel_shift * 2;
                            tl[order_type::G] = ( back_g * weight ) >> image_subpixel_shift * 2;
                            tl[order_type::B] = ( back_b * weight ) >> image_subpixel_shift * 2;
                            tl[order_type::A] = ( back_a * weight ) >> image_subpixel_shift * 2;
                        }


                        x_lr++;

                        weight = x_hr * (image_subpixel_scale - y_hr);
                        if(x_lr >= 0    && y_lr >= 0 &&
                           x_lr <= maxx && y_lr <= maxy)
                        {
                            fg_ptr = (const value_type*)
                                base_type::source().row_ptr(y_lr) + (x_lr << 2);

                            tr[order_type::R] = ( weight * fg_ptr[order_type::R] ) >> image_subpixel_shift * 2;
                            tr[order_type::G] = ( weight * fg_ptr[order_type::G] ) >> image_subpixel_shift * 2;
                            tr[order_type::B] = ( weight * fg_ptr[order_type::B] ) >> image_subpixel_shift * 2;
                            tr[order_type::A] = ( weight * fg_ptr[order_type::A] ) >> image_subpixel_shift * 2;
                        }
                        else
                        {
                            tr[order_type::R] = ( back_r * weight ) >> image_subpixel_shift * 2;
                            tr[order_type::G] = ( back_g * weight ) >> image_subpixel_shift * 2;
                            tr[order_type::B] = ( back_b * weight ) >> image_subpixel_shift * 2;
                            tr[order_type::A] = ( back_a * weight ) >> image_subpixel_shift * 2;
                        }

                        // Blend the top-left and top-right colors:
                        alpha_blend( tl, tr, tl );

                        x_lr--;
                        y_lr++;

                        // Bottom color:
                        calc_type bl[ 4 ], br[ 4 ];

                        weight = (image_subpixel_scale - x_hr) * y_hr;
                        if(x_lr >= 0    && y_lr >= 0 &&
                           x_lr <= maxx && y_lr <= maxy)
                        {
                            fg_ptr = (const value_type*)
                                base_type::source().row_ptr(y_lr) + (x_lr << 2);

                            bl[order_type::R] = ( weight * fg_ptr[order_type::R] ) >> image_subpixel_shift * 2;
                            bl[order_type::G] = ( weight * fg_ptr[order_type::G] ) >> image_subpixel_shift * 2;
                            bl[order_type::B] = ( weight * fg_ptr[order_type::B] ) >> image_subpixel_shift * 2;
                            bl[order_type::A] = ( weight * fg_ptr[order_type::A] ) >> image_subpixel_shift * 2;
                        }
                        else
                        {
                            bl[order_type::R] = ( back_r * weight ) >> image_subpixel_shift * 2;
                            bl[order_type::G] = ( back_g * weight ) >> image_subpixel_shift * 2;
                            bl[order_type::B] = ( back_b * weight ) >> image_subpixel_shift * 2;
                            bl[order_type::A] = ( back_a * weight ) >> image_subpixel_shift * 2;
                        }

                        x_lr++;

                        weight = x_hr * y_hr;
                        if(x_lr >= 0    && y_lr >= 0 &&
                           x_lr <= maxx && y_lr <= maxy)
                        {
                            fg_ptr = (const value_type*)
                                base_type::source().row_ptr(y_lr) + (x_lr << 2);

                            br[order_type::R] = ( weight * fg_ptr[order_type::R] ) >> image_subpixel_shift * 2;
                            br[order_type::G] = ( weight * fg_ptr[order_type::G] ) >> image_subpixel_shift * 2;
                            br[order_type::B] = ( weight * fg_ptr[order_type::B] ) >> image_subpixel_shift * 2;
                            br[order_type::A] = ( weight * fg_ptr[order_type::A] ) >> image_subpixel_shift * 2;
                        }
                        else
                        {
                            br[order_type::R] = ( back_r * weight ) >> image_subpixel_shift * 2;
                            br[order_type::G] = ( back_g * weight ) >> image_subpixel_shift * 2;
                            br[order_type::B] = ( back_b * weight ) >> image_subpixel_shift * 2;
                            br[order_type::A] = ( back_a * weight ) >> image_subpixel_shift * 2;
                        }

                        // Blend the bottom-left and bottom-right colors:
                        alpha_blend( bl, br, bl );

                        // Blend the top and bottom colors:
                        alpha_blend( tl, bl, fg );
*/
                    }
                }

                span->r = (value_type)fg[order_type::R];
                span->g = (value_type)fg[order_type::G];
                span->b = (value_type)fg[order_type::B];
                span->a = (value_type)fg[order_type::A];
                ++span;
                ++base_type::interpolator();

            } while(--len);

//            base_type::interpolator().begin(x + base_type::filter_dx_dbl(), 
//                                            y + base_type::filter_dy_dbl(), len);
//
//            calc_type fg[4];
//            const value_type *fg_ptr;
//
//            do
//            {
//                int x_hr;
//                int y_hr;
//
//                base_type::interpolator().coordinates(&x_hr, &y_hr);
//
//                x_hr -= base_type::filter_dx_int();
//                y_hr -= base_type::filter_dy_int();
//
//                int x_lr = x_hr >> image_subpixel_shift;
//                int y_lr = y_hr >> image_subpixel_shift;
//
//                unsigned weight;
//
//                fg[0] = 
//                fg[1] = 
//                fg[2] = 
//                fg[3] = image_subpixel_scale * image_subpixel_scale / 2;
//
//                x_hr &= image_subpixel_mask;
//                y_hr &= image_subpixel_mask;
//
///*
//                fg_ptr = (const value_type*)base_type::source().span(x_lr, y_lr, 2);
//                weight = (image_subpixel_scale - x_hr) * 
//                         (image_subpixel_scale - y_hr);
//                fg[0] += weight * *fg_ptr++;
//                fg[1] += weight * *fg_ptr++;
//                fg[2] += weight * *fg_ptr++;
//                fg[3] += weight * *fg_ptr;
//
//                fg_ptr = (const value_type*)base_type::source().next_x();
//                weight = x_hr * (image_subpixel_scale - y_hr);
//                fg[0] += weight * *fg_ptr++;
//                fg[1] += weight * *fg_ptr++;
//                fg[2] += weight * *fg_ptr++;
//                fg[3] += weight * *fg_ptr;
//
//                fg_ptr = (const value_type*)base_type::source().next_y();
//                weight = (image_subpixel_scale - x_hr) * y_hr;
//                fg[0] += weight * *fg_ptr++;
//                fg[1] += weight * *fg_ptr++;
//                fg[2] += weight * *fg_ptr++;
//                fg[3] += weight * *fg_ptr;
//
//                fg_ptr = (const value_type*)base_type::source().next_x();
//                weight = x_hr * y_hr;
//                fg[0] += weight * *fg_ptr++;
//                fg[1] += weight * *fg_ptr++;
//                fg[2] += weight * *fg_ptr++;
//                fg[3] += weight * *fg_ptr;
//*/
//
//                span->r = value_type(fg[order_type::R] >> (image_subpixel_shift * 2));
//                span->g = value_type(fg[order_type::G] >> (image_subpixel_shift * 2));
//                span->b = value_type(fg[order_type::B] >> (image_subpixel_shift * 2));
//                span->a = value_type(fg[order_type::A] >> (image_subpixel_shift * 2));
//
//                ++span;
//                ++base_type::interpolator();
//
//            } while(--len);
        }

    protected:
        /** Blend two color arrays by alpha and store the result in a third
            array
        */
        void alpha_blend( calc_type color1[4], calc_type color2[4], calc_type result[4] )
        {
            // Use the blend method that PhotoShop uses:
            //      A_result = 1.0 - ( 1.0 - A_front ) * ( 1.0 - A_back )
            //      A_ratio  = A_front / A_result
            //      C_result = ( 1.0 - ( A_ratio ) ) * C_back +
            //              A_ratio * ( ( 1.0 - A_back ) * C_front + A_back * BlendFunc )
            // where BlendFunc defines a function that blends between colors
            // as if the alphas were 100%.  For normal blends, this is the
            // source color.  When using more advanced blending, this will be
            // a computed value.
            //
            // For a simple alpha blend, the value of BlendFunc is the same as
            // the foreground color.  Therefore, the forumla can be simplified
            // to:
            //      C_result = ( 1.0 - ( A_ratio ) ) * C_back + A_ratio * C_front

            // If the first color has alpha=0, return the second color:
            if ( color1[ order_type::A ] == 0 )
            {
                result[ order_type::R ] = color2[ order_type::R ];
                result[ order_type::G ] = color2[ order_type::G ];
                result[ order_type::B ] = color2[ order_type::B ];
                result[ order_type::A ] = color2[ order_type::A ];

                return;
            }

            // If the first color has alpha=255, return the first color:
            if ( color1[ order_type::A ] == 255 )
            {
                result[ order_type::R ] = color1[ order_type::R ];
                result[ order_type::G ] = color1[ order_type::G ];
                result[ order_type::B ] = color1[ order_type::B ];
                result[ order_type::A ] = color1[ order_type::A ];

                return;
            }

            // Calculate the alpha ratios:
            double alphaRatio1 = color1[ order_type::A ] / 255.0;
            double alphaRatio2 = color2[ order_type::A ] / 255.0;

            // Calculate the resultant alpha:
            double resultAlpha = 1.0 - ( 1.0 - alphaRatio2 ) * ( 1.0 - alphaRatio1 );
            result[ order_type::A ] = clampi( resultAlpha * 255, 0, 255 );
//std::cout << "alpha1 = " << color1[ order_type::A ] << ", ratio = " << alphaRatio1 << ", alpha2 = " << color2[ order_type::A ] << ", ratio " << alphaRatio2 << std::endl;

            // Calculate the resultant RGB values:
            resultAlpha = alphaRatio1 / resultAlpha;
            result[ order_type::R ] =  clampi( alpha_blend( color1[ order_type::R ], color2[ order_type::R ], resultAlpha ), 0, 255 );
            result[ order_type::G ] =  clampi( alpha_blend( color1[ order_type::G ], color2[ order_type::G ], resultAlpha ), 0, 255 );
            result[ order_type::B ] =  clampi( alpha_blend( color1[ order_type::B ], color2[ order_type::B ], resultAlpha ), 0, 255 );
        }

        /** Blend two color components based on an alpha ratio */
        calc_type alpha_blend( value_type c1, value_type c2, double alpha )
        {
            return value_type( alpha * c1 + ( 1.0 - alpha ) * c2 );
        }

    private:
        color_type m_back_color;
    };

} // namespace agg


#endif  // agg_span_image_filter_rgba_plain_H_
