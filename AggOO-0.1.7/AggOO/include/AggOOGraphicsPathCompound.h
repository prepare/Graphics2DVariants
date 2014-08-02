
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


/*! $Id: AggOOGraphicsPathCompound.h,v 1.2 2008/08/01 15:13:33 dratek Exp $
 *  @file   AggOOGraphicsPathCompound.h
 *  @author Chad M. Draper
 *  @date   January 25, 2008
 *  @brief  The GraphicsPathCompound class allows for rendering, naturally,
 *          compound paths.  These are paths which may intersect, share an
 *          edge, etc.
 *  
 */

#ifndef AggOOGraphicsPathCompound_H_
#define AggOOGraphicsPathCompound_H_

#include "AggOOBase.h"
#include "AggOOEnums.h"
#include "AggOOTypes.h"
#include "AggOOSharedPtr.h"

namespace AggOO
{
    class Brush;
    class Pen;
    class GraphicsPath;
    class StyleHandler;

    //=============================================render_scanlines_compound
    template<class Rasterizer, 
             class ScanlineAA, 
             class ScanlineBin, 
             class BaseRenderer, 
             class SpanAllocator,
             class StyleHandler>
    void RenderScanlinesCompound(Rasterizer& ras, 
                                   ScanlineAA& sl_aa,
                                   ScanlineBin& sl_bin,
                                   BaseRenderer& ren,
                                   SpanAllocator& alloc,
                                   StyleHandler& sh)
    {
        if(ras.rewind_scanlines())
        {
            int min_x = ras.min_x();
            int len = ras.max_x() - min_x + 2;
            sl_aa.reset(min_x, ras.max_x());
            sl_bin.reset(min_x, ras.max_x());

            typedef typename BaseRenderer::color_type color_type;
            color_type* color_span = alloc.allocate(len * 2);
            color_type* mix_buffer = color_span + len;
            unsigned num_spans;

            unsigned num_styles;
            unsigned style;
            bool     solid;
            while((num_styles = ras.sweep_styles()) > 0)
            {
                typename ScanlineAA::const_iterator span_aa;
                if(num_styles == 1)
                {
                    // Optimization for a single style. Happens often
                    //-------------------------
                    if(ras.sweep_scanline(sl_aa, 0))
                    {
                        style = ras.style(0);
                        if(sh.is_solid(style))
                        {
                            // Just solid fill
                            //-----------------------
                            render_scanline_aa_solid(sl_aa, ren, sh.color<color_type>(style));
                        }
                        else
                        {
                            // Arbitrary span generator
                            //-----------------------
                            span_aa   = sl_aa.begin();
                            num_spans = sl_aa.num_spans();
                            for(;;)
                            {
                                len = span_aa->len;
                                sh.generate_span(color_span, 
                                                 span_aa->x, 
                                                 sl_aa.y(), 
                                                 len, 
                                                 style);

                                ren.blend_color_hspan(span_aa->x, 
                                                      sl_aa.y(), 
                                                      span_aa->len,
                                                      color_span,
                                                      span_aa->covers);
                                if(--num_spans == 0) break;
                                ++span_aa;
                            }
                        }
                    }
                }
                else
                {
                    if(ras.sweep_scanline(sl_bin, -1))
                    {
                        // Clear the spans of the mix_buffer
                        //--------------------
                        typename ScanlineBin::const_iterator span_bin = sl_bin.begin();
                        num_spans = sl_bin.num_spans();
                        for(;;)
                        {
                            memset(mix_buffer + span_bin->x - min_x, 
                                   0, 
                                   span_bin->len * sizeof(color_type));

                            if(--num_spans == 0) break;
                            ++span_bin;
                        }

                        unsigned i;
                        for(i = 0; i < num_styles; i++)
                        {
                            style = ras.style(i);
                            solid = sh.is_solid(style);

                            if(ras.sweep_scanline(sl_aa, i))
                            {
                                color_type* colors;
                                color_type* cspan;
                                typename ScanlineAA::cover_type* covers;
                                span_aa   = sl_aa.begin();
                                num_spans = sl_aa.num_spans();
                                if(solid)
                                {
                                    // Just solid fill
                                    //-----------------------
                                    for(;;)
                                    {
                                        color_type c = sh.color<color_type>(style);
                                        len    = span_aa->len;
                                        colors = mix_buffer + span_aa->x - min_x;
                                        covers = span_aa->covers;
                                        do
                                        {
                                            if(*covers == cover_full) 
                                            {
                                                *colors = c;
                                            }
                                            else
                                            {
                                                colors->add(c, *covers);
                                            }
                                            ++colors;
                                            ++covers;
                                        }
                                        while(--len);
                                        if(--num_spans == 0) break;
                                        ++span_aa;
                                    }
                                }
                                else
                                {
                                    // Arbitrary span generator
                                    //-----------------------
                                    for(;;)
                                    {
                                        len = span_aa->len;
                                        colors = mix_buffer + span_aa->x - min_x;
                                        cspan  = color_span;
                                        sh.generate_span(cspan, 
                                                         span_aa->x, 
                                                         sl_aa.y(), 
                                                         len, 
                                                         style);
                                        covers = span_aa->covers;
                                        do
                                        {
                                            if(*covers == cover_full) 
                                            {
                                                *colors = *cspan;
                                            }
                                            else
                                            {
                                                colors->add(*cspan, *covers);
                                            }
                                            ++cspan;
                                            ++colors;
                                            ++covers;
                                        }
                                        while(--len);
                                        if(--num_spans == 0) break;
                                        ++span_aa;
                                    }
                                }
                            }
                        }

                        // Emit the blended result as a color hspan
                        //-------------------------
                        span_bin = sl_bin.begin();
                        num_spans = sl_bin.num_spans();
                        for(;;)
                        {
                            ren.blend_color_hspan(span_bin->x, 
                                                  sl_bin.y(), 
                                                  span_bin->len,
                                                  mix_buffer + span_bin->x - min_x,
                                                  0,
                                                  cover_full);
                            if(--num_spans == 0) break;
                            ++span_bin;
                        }
                    } // if(ras.sweep_scanline(sl_bin, -1))
                } // if(num_styles == 1) ... else
            } // while((num_styles = ras.sweep_styles()) > 0)
        } // if(ras.rewind_scanlines())
    }

    /** In order to work with the style handlers, the
        render_scanlines_compound_layered method needs to be modified to accept
        a template type for the StyleHandler::color method.
    */
    //RenderScanlinesCompoundLayered============================================
    template<class Rasterizer, 
             class ScanlineAA, 
             class BaseRenderer, 
             class SpanAllocator,
             class StyleHandler>
    void RenderScanlinesCompoundLayered(Rasterizer& ras, 
                                           ScanlineAA& sl_aa,
                                           BaseRenderer& ren,
                                           SpanAllocator& alloc,
                                           StyleHandler& sh)
    {
        if(ras.rewind_scanlines())
        {
            int min_x = ras.min_x();
            int len = ras.max_x() - min_x + 2;
            sl_aa.reset(min_x, ras.max_x());

            typedef typename BaseRenderer::color_type color_type;
            color_type* color_span   = alloc.allocate(len * 2);
            color_type* mix_buffer   = color_span + len;
            cover_type* cover_buffer = ras.allocate_cover_buffer(len);
            unsigned num_spans;

            unsigned num_styles;
            unsigned style;
            bool     solid;
            while((num_styles = ras.sweep_styles()) > 0)
            {
                typename ScanlineAA::const_iterator span_aa;
                if(num_styles == 1)
                {
                    // Optimization for a single style. Happens often
                    //-------------------------
                    if(ras.sweep_scanline(sl_aa, 0))
                    {
                        style = ras.style(0);
                        if(sh.is_solid(style))
                        {
                            // Just solid fill
                            //-----------------------
                            render_scanline_aa_solid(sl_aa, ren, sh.color<color_type>(style));
                        }
                        else
                        {
                            // Arbitrary span generator
                            //-----------------------
                            span_aa   = sl_aa.begin();
                            num_spans = sl_aa.num_spans();
                            for(;;)
                            {
                                len = span_aa->len;
                                sh.generate_span(color_span, 
                                                 span_aa->x, 
                                                 sl_aa.y(), 
                                                 len, 
                                                 style);

                                ren.blend_color_hspan(span_aa->x, 
                                                      sl_aa.y(), 
                                                      span_aa->len,
                                                      color_span,
                                                      span_aa->covers);
                                if(--num_spans == 0) break;
                                ++span_aa;
                            }
                        }
                    }
                }
                else
                {
                    int      sl_start = ras.scanline_start();
                    unsigned sl_len   = ras.scanline_length();

                    if(sl_len)
                    {
                        memset(mix_buffer + sl_start - min_x, 
                               0, 
                               sl_len * sizeof(color_type));

                        memset(cover_buffer + sl_start - min_x, 
                               0, 
                               sl_len * sizeof(cover_type));

                        int sl_y = 0x7FFFFFFF;
                        unsigned i;
                        for(i = 0; i < num_styles; i++)
                        {
                            style = ras.style(i);
                            solid = sh.is_solid(style);

                            if(ras.sweep_scanline(sl_aa, i))
                            {
                                unsigned    cover;
                                color_type* colors;
                                color_type* cspan;
                                cover_type* src_covers;
                                cover_type* dst_covers;
                                span_aa   = sl_aa.begin();
                                num_spans = sl_aa.num_spans();
                                sl_y      = sl_aa.y();
                                if(solid)
                                {
                                    // Just solid fill
                                    //-----------------------
                                    for(;;)
                                    {
                                        color_type c = sh.color<color_type>(style);
                                        len    = span_aa->len;
                                        colors = mix_buffer + span_aa->x - min_x;
                                        src_covers = span_aa->covers;
                                        dst_covers = cover_buffer + span_aa->x - min_x;
                                        do
                                        {
                                            cover = *src_covers;
                                            if(*dst_covers + cover > cover_full)
                                            {
                                                cover = cover_full - *dst_covers;
                                            }
                                            if(cover)
                                            {
                                                colors->add(c, cover);
                                                *dst_covers += cover;
                                            }
                                            ++colors;
                                            ++src_covers;
                                            ++dst_covers;
                                        }
                                        while(--len);
                                        if(--num_spans == 0) break;
                                        ++span_aa;
                                    }
                                }
                                else
                                {
                                    // Arbitrary span generator
                                    //-----------------------
                                    for(;;)
                                    {
                                        len = span_aa->len;
                                        colors = mix_buffer + span_aa->x - min_x;
                                        cspan  = color_span;
                                        sh.generate_span(cspan, 
                                                         span_aa->x, 
                                                         sl_aa.y(), 
                                                         len, 
                                                         style);
                                        src_covers = span_aa->covers;
                                        dst_covers = cover_buffer + span_aa->x - min_x;
                                        do
                                        {
                                            cover = *src_covers;
                                            if(*dst_covers + cover > cover_full)
                                            {
                                                cover = cover_full - *dst_covers;
                                            }
                                            if(cover)
                                            {
                                                colors->add(*cspan, cover);
                                                *dst_covers += cover;
                                            }
                                            ++cspan;
                                            ++colors;
                                            ++src_covers;
                                            ++dst_covers;
                                        }
                                        while(--len);
                                        if(--num_spans == 0) break;
                                        ++span_aa;
                                    }
                                }
                            }
                        }
                        ren.blend_color_hspan(sl_start, 
                                              sl_y, 
                                              sl_len,
                                              mix_buffer + sl_start - min_x,
                                              0,
                                              cover_full);
                    } //if(sl_len)
                } //if(num_styles == 1) ... else
            } //while((num_styles = ras.sweep_styles()) > 0)
        } //if(ras.rewind_scanlines())
    }

    /** @class GraphicsPathCompound
        The GraphicsPathCompound class allows users to create compound paths
        from GraphicsPath objects.  Rather than adding shapes to the compound
        path, the shape should be created in a GraphicsPath, and added to the
        compound path, along with the appropriate style (brush.)  The compound
        path can then be passed to the renderer.
    */
    class _AggOOExport GraphicsPathCompound : Base
    {
    public:
        friend class Graphics;

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

        typedef SharedPtr< GraphicsPath >       PathPtr;
        typedef SharedPtr< StyleHandler >  StylePtr;
        typedef std::pair< StylePtr, PathPtr >  CompoundItem;

        std::vector< CompoundItem >  mPaths;         /**< Array of paths in the compound path */

        /** Create a style handler from a given brush */
        AggOO::StyleHandler* createStyleHandler( const Brush* brush );
        //template < typename ColorT >
        //void createStyleHandler( const Brush* brush, StyleHandler* handler )
        //{
        //    handler = NULL;
        //    switch ( brush->GetType() )
        //    {
        //    case BrushTypeTextureFill:
        //        {
        //            handler = new SolidStyleHandler<agg::rgba8>( ColorExt::toRGBA8( Color::Red ) );
        //        }
        //        break;

        //    case BrushTypeLinearGradient:
        //        {
        //            handler = new SolidStyleHandler<agg::rgba8>( ColorExt::toRGBA8( Color::Green ) );
        //        }
        //        break;

        //    case BrushTypeSolidColor:
        //        {
        //            Color col;
        //            ((SolidBrush*)brush)->GetColor( &col );
        //            handler = new SolidStyleHandler<agg::rgba8>( ColorExt::toRGBA8( col ) );
        //        }
        //        break;

        //        // Need to add hatch and path gradient

        //    default:
        //        {
        //            // This is more of a fail-safe.  If something goes wrong, a solid
        //            // style handler will be created using a transparent color.  This
        //            // won't render anything, but it will prevent the application
        //            // from crashing.
        //            handler = new SolidStyleHandler< agg::rgba8 >( ColorExt::toRGBA8( Color::Transparent ) );
        //        }
        //        break;
        //    }
        //}


    public:
        GraphicsPathCompound(void);
        ~GraphicsPathCompound(void);

        /** Add a graphics path to the compound path */
        Status AddPath( const Pen* pen, const GraphicsPath* path );
        /** Add a graphics path to the compound path */
        Status AddPath( const Brush* brush, const GraphicsPath* path );

        /** Get the number of paths in the compound path */
        UINT GetPathCount() const;

        /** Get a style handler at a given index */
        Status GetStyleHandler( INT index, StyleHandler* &handler ) const;
        //template < typename ColorT >
        //Status GetStyleHandler( INT index, StyleHandler<ColorT>* &handler ) const
        //{
        //    if ( index < 0 || index >= mPaths.size() )
        //        return SetStatus( InvalidParameter );
        //    try
        //    {
        //        handler = mPaths.at( index ).first.getPointer();
        //        return SetStatus( Ok );
        //    }
        //    catch ( std::exception& e )
        //    {
        //        return SetStatus( GenericError );
        //    }

        //    return SetStatus( Ok );
        //}
        //agg::StyleHandler* GetStyleHandler( INT index ) const;

        /** Get a GraphicsPath at a given index */
        Status GetGraphicsPath( INT index, GraphicsPath* &path ) const;
        //GraphicsPath* GetGraphicsPath( INT index ) const;

    }; // class GraphicsPathCompound

} // namespace AggOO

#endif  // AggOOGraphicsPathCompound_H_
