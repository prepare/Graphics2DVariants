
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
are(met)

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

/*! $Id: AggOOAbstractRendererBase.h,v 1.2 2008/08/13 22:08:28 dratek Exp $
 *  @file   AggOOAbstractRendererBase.h
 *  @author Chad M. Draper
 *  @date   February 8, 2008
 *  @brief  Defines the AbstractRendererBase class, which allows the
 *          renderer_base to be polymorphic.  Also defines the template
 *          class SpecificRendererBase, which takes a pixel format template
 *          parameter, and subclasses the abstract class.
 *  
 */

#ifndef AggOOAbstractRendererBase_H_
#define AggOOAbstractRendererBase_H_

#include "agg_basics.h"
#include "agg_color_gray.h"
#include "agg_color_rgba.h"
#include "agg_pixfmt_gray.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgb_packed.h"
#include "agg_pixfmt_rgba.h"
#include "agg_scanline_bin.h"
#include "agg_scanline_p.h"
#include "agg_scanline_u.h"
#include "agg_renderer_base.h"
#include "agg_rasterizer_scanline_aa.h"

#include "AggOOAbstractScanlineRenderer.h"
#include "AggOOStyleHandler.h"

namespace AggOO
{
    using namespace agg;

    class AbstractRendererBase
    {
    public:
        typedef agg::rgba8 color_type;

    public:
        AbstractRendererBase() { }

        template < class PixFmt >
        void attach( PixFmt& ren ) { }

        template < class PixFmt >
        const PixFmt& ren() const { }
        template < class PixFmt >
        PixFmt& ren() { }

        virtual unsigned width() const = 0;
        virtual unsigned height() const = 0;

        virtual bool clip_box( int x1, int y1, int x2, int y2 ) = 0;
        virtual void reset_clipping( bool visibility ) = 0;
        virtual void clip_box_naked( int x1, int y1, int x2, int y2 ) = 0;
        virtual bool inbox( int x, int y ) const = 0;
        virtual const rect_i& clip_box() const  = 0;

        virtual int xmin() const = 0;
        virtual int ymin() const = 0;
        virtual int xmax() const = 0;
        virtual int ymax() const = 0;

        virtual const rect_i& bounding_clip_box() const = 0;
        virtual       int     bounding_xmin()     const = 0;
        virtual       int     bounding_ymin()     const = 0;
        virtual       int     bounding_xmax()     const = 0;
        virtual       int     bounding_ymax()     const = 0;

        virtual void clear( const agg::rgba&   c ) = 0;
        //virtual void clear( const agg::gray8&  c ) { }
        //virtual void clear( const agg::gray16& c ) { }
        //virtual void clear( const agg::rgba8&  c ) { }
        //virtual void clear( const agg::rgba16& c ) { }

        virtual void copy_pixel( int x, int y, const agg::gray8&  c ) { }
        virtual void copy_pixel( int x, int y, const agg::gray16& c ) { }
        virtual void copy_pixel( int x, int y, const agg::rgba8&  c ) { }
        virtual void copy_pixel( int x, int y, const agg::rgba16& c ) { }

        virtual void blend_pixel(int x, int y, const agg::gray8&  c, cover_type cover) { }
        virtual void blend_pixel(int x, int y, const agg::gray16& c, cover_type cover) { }
        virtual void blend_pixel(int x, int y, const agg::rgba8&  c, cover_type cover) { }
        virtual void blend_pixel(int x, int y, const agg::rgba16& c, cover_type cover) { }

        template < typename color_type >
        color_type  pixel(int x, int y) const { return color_type::transparent(); }

        virtual void copy_hline(int x1, int y, int x2, const agg::gray8&  c) { }
        virtual void copy_hline(int x1, int y, int x2, const agg::gray16& c) { }
        virtual void copy_hline(int x1, int y, int x2, const agg::rgba8&  c) { }
        virtual void copy_hline(int x1, int y, int x2, const agg::rgba16& c) { }

        virtual void copy_vline(int x, int y1, int y2, const agg::gray8&  c) { }
        virtual void copy_vline(int x, int y1, int y2, const agg::gray16& c) { }
        virtual void copy_vline(int x, int y1, int y2, const agg::rgba8&  c) { }
        virtual void copy_vline(int x, int y1, int y2, const agg::rgba16& c) { }

        virtual void blend_hline(int x1, int y, int x2, const agg::gray8&  c, cover_type cover) { }
        virtual void blend_hline(int x1, int y, int x2, const agg::gray16& c, cover_type cover) { }
        virtual void blend_hline(int x1, int y, int x2, const agg::rgba8&  c, cover_type cover) { }
        virtual void blend_hline(int x1, int y, int x2, const agg::rgba16& c, cover_type cover) { }

        virtual void blend_vline(int x, int y1, int y2, const agg::gray8&  c, cover_type cover) { }
        virtual void blend_vline(int x, int y1, int y2, const agg::gray16& c, cover_type cover) { }
        virtual void blend_vline(int x, int y1, int y2, const agg::rgba8&  c, cover_type cover) { }
        virtual void blend_vline(int x, int y1, int y2, const agg::rgba16& c, cover_type cover) { }

        virtual void copy_bar(int x1, int y1, int x2, int y2, const agg::gray8&  c) { }
        virtual void copy_bar(int x1, int y1, int x2, int y2, const agg::gray16& c) { }
        virtual void copy_bar(int x1, int y1, int x2, int y2, const agg::rgba8&  c) { }
        virtual void copy_bar(int x1, int y1, int x2, int y2, const agg::rgba16& c) { }

        virtual void blend_bar(int x1, int y1, int x2, int y2, const agg::gray8&  c, cover_type cover) { }
        virtual void blend_bar(int x1, int y1, int x2, int y2, const agg::gray16& c, cover_type cover) { }
        virtual void blend_bar(int x1, int y1, int x2, int y2, const agg::rgba8&  c, cover_type cover) { }
        virtual void blend_bar(int x1, int y1, int x2, int y2, const agg::rgba16& c, cover_type cover) { }

        virtual void blend_solid_hspan(int x, int y, int len, const agg::gray8&  c, const cover_type* covers) { }
        virtual void blend_solid_hspan(int x, int y, int len, const agg::gray16& c, const cover_type* covers) { }
        virtual void blend_solid_hspan(int x, int y, int len, const agg::rgba8&  c, const cover_type* covers) { }
        virtual void blend_solid_hspan(int x, int y, int len, const agg::rgba16& c, const cover_type* covers) { }

        virtual void blend_solid_vspan(int x, int y, int len, const agg::gray8&  c, const cover_type* covers) { }
        virtual void blend_solid_vspan(int x, int y, int len, const agg::gray16& c, const cover_type* covers) { }
        virtual void blend_solid_vspan(int x, int y, int len, const agg::rgba8&  c, const cover_type* covers) { }
        virtual void blend_solid_vspan(int x, int y, int len, const agg::rgba16& c, const cover_type* covers) { }

        virtual void copy_color_hspan(int x, int y, int len, const agg::gray8 * colors) { }
        virtual void copy_color_hspan(int x, int y, int len, const agg::gray16* colors) { }
        virtual void copy_color_hspan(int x, int y, int len, const agg::rgba8 * colors) { }
        virtual void copy_color_hspan(int x, int y, int len, const agg::rgba16* colors) { }

        virtual void copy_color_vspan(int x, int y, int len, const agg::gray8 * colors) { }
        virtual void copy_color_vspan(int x, int y, int len, const agg::gray16* colors) { }
        virtual void copy_color_vspan(int x, int y, int len, const agg::rgba8 * colors) { }
        virtual void copy_color_vspan(int x, int y, int len, const agg::rgba16* colors) { }

        virtual void blend_color_hspan(int x, int y, int len, const agg::gray8 * colors, const cover_type* covers, cover_type cover = agg::cover_full) { }
        virtual void blend_color_hspan(int x, int y, int len, const agg::gray16* colors, const cover_type* covers, cover_type cover = agg::cover_full) { }
        virtual void blend_color_hspan(int x, int y, int len, const agg::rgba8 * colors, const cover_type* covers, cover_type cover = agg::cover_full) { }
        virtual void blend_color_hspan(int x, int y, int len, const agg::rgba16* colors, const cover_type* covers, cover_type cover = agg::cover_full) { }

        virtual void blend_color_vspan(int x, int y, int len, const agg::gray8 * colors, const cover_type* covers, cover_type cover = agg::cover_full) { }
        virtual void blend_color_vspan(int x, int y, int len, const agg::gray16* colors, const cover_type* covers, cover_type cover = agg::cover_full) { }
        virtual void blend_color_vspan(int x, int y, int len, const agg::rgba8 * colors, const cover_type* covers, cover_type cover = agg::cover_full) { }
        virtual void blend_color_vspan(int x, int y, int len, const agg::rgba16* colors, const cover_type* covers, cover_type cover = agg::cover_full) { }

        //--------------------------------------------------------------------
        virtual rect_i clip_rect_area(rect_i& dst, rect_i& src, int wsrc, int hsrc) const = 0;

        //--------------------------------------------------------------------
        template<class RenBuf>
        void copy_from(const RenBuf& src, 
                       const rect_i* rect_src_ptr = 0, 
                       int dx = 0, 
                       int dy = 0) { }

        //--------------------------------------------------------------------
        template<class SrcPixelFormatRenderer>
        void blend_from(const SrcPixelFormatRenderer& src, 
                        const rect_i* rect_src_ptr = 0, 
                        int dx = 0, 
                        int dy = 0,
                        cover_type cover = agg::cover_full) { }

        //--------------------------------------------------------------------
        template<class SrcPixelFormatRenderer>
        void blend_from_color(const SrcPixelFormatRenderer& src, const agg::gray8 & color, const rect_i* rect_src_ptr = 0, int dx = 0, int dy = 0, cover_type cover = agg::cover_full) { }
        template<class SrcPixelFormatRenderer>
        void blend_from_color(const SrcPixelFormatRenderer& src, const agg::gray16& color, const rect_i* rect_src_ptr = 0, int dx = 0, int dy = 0, cover_type cover = agg::cover_full) { }
        template<class SrcPixelFormatRenderer>
        void blend_from_color(const SrcPixelFormatRenderer& src, const agg::rgba8 & color, const rect_i* rect_src_ptr = 0, int dx = 0, int dy = 0, cover_type cover = agg::cover_full) { }
        template<class SrcPixelFormatRenderer>
        void blend_from_color(const SrcPixelFormatRenderer& src, const agg::rgba16& color, const rect_i* rect_src_ptr = 0, int dx = 0, int dy = 0, cover_type cover = agg::cover_full) { }

        //--------------------------------------------------------------------
        template<class SrcPixelFormatRenderer>
        void blend_from_lut(const SrcPixelFormatRenderer& src, const agg::gray8*  color_lut, const rect_i* rect_src_ptr = 0, int dx = 0, int dy = 0, cover_type cover = agg::cover_full) { }
        template<class SrcPixelFormatRenderer, typename color_type>
        void blend_from_lut(const SrcPixelFormatRenderer& src, const agg::gray16* color_lut, const rect_i* rect_src_ptr = 0, int dx = 0, int dy = 0, cover_type cover = agg::cover_full) { }
        template<class SrcPixelFormatRenderer, typename color_type>
        void blend_from_lut(const SrcPixelFormatRenderer& src, const agg::rgba8*  color_lut, const rect_i* rect_src_ptr = 0, int dx = 0, int dy = 0, cover_type cover = agg::cover_full) { }
        template<class SrcPixelFormatRenderer, typename color_type>
        void blend_from_lut(const SrcPixelFormatRenderer& src, const agg::rgba16* color_lut, const rect_i* rect_src_ptr = 0, int dx = 0, int dy = 0, cover_type cover = agg::cover_full) { }

        //----------------------------------------------------------------------
        // Factories for creating the scanline renderer associated with this
        // base renderer
        //----------------------------------------------------------------------

        virtual AbstractScanlineRenderer* CreateScanlineRendererAASolid() = 0;
        virtual AbstractScanlineRenderer* CreateScanlineRendererBinSolid() = 0;

        //----------------------------------------------------------------------
        // Some of the render methods require information from the specific
        // renderer, and will not work with the abstract renderer.  Provide
        // methods which will end up using the necessary renderer, rather than
        // a pointer to the base.
        //----------------------------------------------------------------------
        virtual void RenderScanlinesAA( agg::rasterizer_scanline_aa<>& ras,
                                        agg::scanline_u8& sl,
                                        StyleHandler& sg ) = 0;
        virtual void RenderScanlinesBin( agg::rasterizer_scanline_aa<>& ras,
                                        agg::scanline_u8& sl,
                                        StyleHandler& sg ) = 0;
    };

    template < class PixFmt >
    class SpecificRenderer : public AbstractRendererBase
    {
    private:
        renderer_base< PixFmt > mRenderer;

    public:
        typedef PixFmt pixfmt_type;
        typedef typename pixfmt_type::color_type color_type;
        typedef typename pixfmt_type::row_data row_data;

        //--------------------------------------------------------------------
        SpecificRenderer() : AbstractRendererBase() {}
        SpecificRenderer(pixfmt_type& ren) :
            mRenderer(ren)
        {}
        void attach(pixfmt_type& ren)
        {
            mRenderer.attach( ren );
        }

        //--------------------------------------------------------------------
        const pixfmt_type& ren() const { return mRenderer.ren();  }
        pixfmt_type& ren() { return mRenderer.ren();  }
          
        //--------------------------------------------------------------------
        unsigned width()  const { return mRenderer.width();  }
        unsigned height() const { return mRenderer.height(); }

        //--------------------------------------------------------------------
        bool clip_box(int x1, int y1, int x2, int y2)
        {
            return mRenderer.clip_box( x1, y1, x2, y2 );
        }

        //--------------------------------------------------------------------
        void reset_clipping(bool visibility)
        {
            mRenderer.reset_clipping( visibility );
        }

        //--------------------------------------------------------------------
        void clip_box_naked(int x1, int y1, int x2, int y2)
        {
            mRenderer.clip_box_naked( x1, y1, x2, y2 );
        }

        //--------------------------------------------------------------------
        bool inbox(int x, int y) const
        {
            return mRenderer.inbox( x, y );
        }

        //--------------------------------------------------------------------
        const rect_i& clip_box() const { return mRenderer.clip_box();    }
        int           xmin()     const { return mRenderer.xmin(); }
        int           ymin()     const { return mRenderer.ymin(); }
        int           xmax()     const { return mRenderer.xmax(); }
        int           ymax()     const { return mRenderer.ymax(); }

        //--------------------------------------------------------------------
        const rect_i& bounding_clip_box() const { return mRenderer.bounding_clip_box();    }
        int           bounding_xmin()     const { return mRenderer.bounding_xmin(); }
        int           bounding_ymin()     const { return mRenderer.bounding_ymin(); }
        int           bounding_xmax()     const { return mRenderer.bounding_xmax(); }
        int           bounding_ymax()     const { return mRenderer.bounding_ymax(); }

        //--------------------------------------------------------------------
        void clear(const agg::rgba& c)
        {
            mRenderer.clear( c );
        }
          

        //--------------------------------------------------------------------
        void copy_pixel(int x, int y, const color_type& c)
        {
            mRenderer.copy_pixel( x, y, c );
        }

        //--------------------------------------------------------------------
        void blend_pixel(int x, int y, const color_type& c, cover_type cover)
        {
            mRenderer.blend_pixel( x, y, c, cover );
        }

        //--------------------------------------------------------------------
        color_type pixel(int x, int y) const
        {
            return mRenderer.pixel( x, y );
        }

        //--------------------------------------------------------------------
        void copy_hline(int x1, int y, int x2, const color_type& c)
        {
            mRenderer.copy_hline( x1, y, x2, c );
        }

        //--------------------------------------------------------------------
        void copy_vline(int x, int y1, int y2, const color_type& c)
        {
            mRenderer.copy_vline( x, y1, y2, c );
        }

        //--------------------------------------------------------------------
        void blend_hline(int x1, int y, int x2, 
                         const color_type& c, cover_type cover)
        {
            mRenderer.blend_hline( x1, y, x2, c, cover );
        }

        //--------------------------------------------------------------------
        void blend_vline(int x, int y1, int y2, 
                         const color_type& c, cover_type cover)
        {
            mRenderer.blend_vline( x, y1, y2, c, cover );
        }


        //--------------------------------------------------------------------
        void copy_bar(int x1, int y1, int x2, int y2, const color_type& c)
        {
            mRenderer.copy_bar( x1, y1, x2, y2, c );
        }

        //--------------------------------------------------------------------
        void blend_bar(int x1, int y1, int x2, int y2, 
                       const color_type& c, cover_type cover)
        {
            mRenderer.blend_bar( x1, y1, x2, y2, c, cover );
        }

        //--------------------------------------------------------------------
        void blend_solid_hspan(int x, int y, int len, 
                               const color_type& c, 
                               const cover_type* covers)
        {
            mRenderer.blend_solid_hspan( x, y, len, c, covers );
        }

        //--------------------------------------------------------------------
        void blend_solid_vspan(int x, int y, int len, 
                               const color_type& c, 
                               const cover_type* covers)
        {
            mRenderer.blend_solid_hspan( x, y, len, c, covers );
        }


        //--------------------------------------------------------------------
        void copy_color_hspan(int x, int y, int len, const color_type* colors)
        {
            mRenderer.copy_color_hspan( x, y, len, colors );
        }


        //--------------------------------------------------------------------
        void copy_color_vspan(int x, int y, int len, const color_type* colors)
        {
            mRenderer.copy_color_vspan( x, y, len, colors );
        }


        //--------------------------------------------------------------------
        void blend_color_hspan(int x, int y, int len, 
                               const color_type* colors, 
                               const cover_type* covers,
                               cover_type cover = agg::cover_full)
        {
            mRenderer.blend_color_hspan( x, y, len, colors, covers, cover );
        }

        //--------------------------------------------------------------------
        void blend_color_vspan(int x, int y, int len, 
                               const color_type* colors, 
                               const cover_type* covers,
                               cover_type cover = agg::cover_full)
        {
            mRenderer.blend_color_vspan( x, y, len, colors, covers, cover );
        }

        //--------------------------------------------------------------------
        rect_i clip_rect_area(rect_i& dst, rect_i& src, int wsrc, int hsrc) const
        {
            return mRenderer.clip_rect_area( dst, src, wsrc, hsrc );
        }

        //--------------------------------------------------------------------
        template<class RenBuf>
        void copy_from(const RenBuf& src, 
                       const rect_i* rect_src_ptr = 0, 
                       int dx = 0, 
                       int dy = 0)
        {
            mRenderer.copy_from( src, rect_src_ptr, dx, dy );
        }

        //--------------------------------------------------------------------
        template<class SrcPixelFormatRenderer>
        void blend_from(const SrcPixelFormatRenderer& src, 
                        const rect_i* rect_src_ptr = 0, 
                        int dx = 0, 
                        int dy = 0,
                        cover_type cover = agg::cover_full)
        {
            mRenderer.blend_from( src, rect_src_ptr, dx, dy, cover );
        }

        //--------------------------------------------------------------------
        template<class SrcPixelFormatRenderer>
        void blend_from_color(const SrcPixelFormatRenderer& src, 
                              const color_type& color,
                              const rect_i* rect_src_ptr = 0, 
                              int dx = 0, 
                              int dy = 0,
                              cover_type cover = agg::cover_full)
        {
            mRenderer.blend_from_color( src, color, rect_src_ptr, dx, dy, cover );
        }

        //--------------------------------------------------------------------
        template<class SrcPixelFormatRenderer>
        void blend_from_lut(const SrcPixelFormatRenderer& src, 
                            const color_type* color_lut,
                            const rect_i* rect_src_ptr = 0, 
                            int dx = 0, 
                            int dy = 0,
                            cover_type cover = agg::cover_full)
        {
            mRenderer.blend_from_lut( src, color_lut, rect_src_ptr, dx, dy, cover );
        }

        AbstractScanlineRenderer* CreateScanlineRendererAASolid()
        {
            //return NULL;
            return new ScanlineRendererAASolid< SpecificRenderer< PixFmt > >( *this );
        }

        AbstractScanlineRenderer* CreateScanlineRendererBinSolid()
        {
            //return NULL;
            return new ScanlineRendererBinSolid< SpecificRenderer< PixFmt > >( *this );
        }

        void RenderScanlinesAA( agg::rasterizer_scanline_aa<>& ras,
                                agg::scanline_u8& sl,
                                StyleHandler& sg )
        {
            agg::span_allocator< color_type > sa;
            agg::render_scanlines_aa( ras, sl, *this, sa, sg );
        }

        void RenderScanlinesBin( agg::rasterizer_scanline_aa<>& ras,
                                agg::scanline_u8& sl,
                                StyleHandler& sg )
        {
            agg::span_allocator< color_type > sa;
            agg::render_scanlines_bin( ras, sl, *this, sa, sg );
        }
    };

} // namespace AggOO

#endif  // AggOOAbstractRendererBase_H_
