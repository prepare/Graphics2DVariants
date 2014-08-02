
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

/*! $Id: AggOOAbstractScanlineRenderer.h,v 1.2 2008/08/13 22:08:28 dratek Exp $
 *  @file   AggOOAbstractScanlineRenderer.h
 *  @author Chad M. Draper
 *  @date   February 8, 2008
 *  @brief  Defines the class AbstractScanlineRenderer, which is a base class
 *          for more specific (i.e. ScanlineRendererAASolid and
 *          ScanlineRendererBinSolid) scanline renderers.
 *  
 */

#ifndef AggOOAbstractScanlineRenderer_H_
#define AggOOAbstractScanlineRenderer_H_

#include "agg_basics.h"
#include "agg_color_gray.h"
#include "agg_color_rgba.h"
//#include "agg_pixfmt_gray.h"
//#include "agg_pixfmt_rgb.h"
//#include "agg_pixfmt_rgb_packed.h"
//#include "agg_pixfmt_rgba.h"
#include "agg_scanline_bin.h"
#include "agg_scanline_p.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"

namespace AggOO
{
    /** @class AbstractScanlineRenderer
        Base class for the polymorphic scanline renderer.  Typically, this
        should be created by a factory method in the base renderer.
    */
    class AbstractScanlineRenderer
    {
    public:
        AbstractScanlineRenderer() { }
        template < typename base_ren_type >
        void attach(base_ren_type& ren) { }
        
        //--------------------------------------------------------------------
        virtual void color( const agg::rgba& c ) = 0;
        template < typename color_type >
        const color_type& color() const = 0;
        //{
        //    color_type c;
        //    get_color( &c );
        //    return c;
        //    //return color_type::transparent();
        //}
        //virtual void get_color( agg::gray8*  c ) { }
        //virtual void get_color( agg::gray16* c ) { }
        //virtual void get_color( agg::rgba8*  c ) { }
        //virtual void get_color( agg::rgba16* c ) { }

        //--------------------------------------------------------------------
        virtual void prepare() = 0;

        //--------------------------------------------------------------------
        virtual void render(const agg::scanline_bin& sl)   { }
        virtual void render(const agg::scanline32_bin& sl) { }
        virtual void render(const agg::scanline_p8& sl)    { }
        virtual void render(const agg::scanline32_p8& sl)  { }
        virtual void render(const agg::scanline_u8& sl)    { }
        virtual void render(const agg::scanline32_u8& sl)  { }
    };

    /** @class ScanlineRendererAASolid
        Subclass of AbstractScanlineRenderer which provides functionality for
        the anti-aliased scanline renderer.  Requires the base renderer as a
        template parameter, and should typically be created by a factory
        method in the base renderer.
    */
    template < class BaseRenderer >
    class ScanlineRendererAASolid : public AbstractScanlineRenderer
    {
    private:
        agg::renderer_scanline_aa_solid< BaseRenderer > mRenderer;

    public:
        typedef BaseRenderer base_ren_type;
        typedef typename base_ren_type::color_type color_type;

        ScanlineRendererAASolid() : mRenderer() { }
        explicit ScanlineRendererAASolid( base_ren_type& ren ) : mRenderer( ren ) { }
        void attach(base_ren_type& ren)
        {
            mRenderer.attach( ren );
        }
        
        //--------------------------------------------------------------------
        void color( const agg::rgba& c ) { mRenderer.color( c ); }
        template < typename color_type >
        const color_type& color() const
        {
            return mRenderer.color();
        }
        //void get_color( agg::gray8* c )
        //{
        //    *c = mRenderer.color();
        //}

        //--------------------------------------------------------------------
        void prepare()
        {
            mRenderer.prepare();
        }

        //--------------------------------------------------------------------
        //virtual void render(const agg::scanline_bin& sl)   { mRenderer.render( sl ); }
        //virtual void render(const agg::scanline32_bin& sl) { mRenderer.render( sl ); }
        virtual void render(const agg::scanline_p8& sl)    { mRenderer.render( sl ); }
        virtual void render(const agg::scanline32_p8& sl)  { mRenderer.render( sl ); }
        virtual void render(const agg::scanline_u8& sl)    { mRenderer.render( sl ); }
        virtual void render(const agg::scanline32_u8& sl)  { mRenderer.render( sl ); }
        //template<class Scanline> void render(const Scanline& sl)
        //{
        //    mRenderer.render( sl );
        //}
    };

    /** @class ScanlineRendererBinSolid
        Subclass of AbstractScanlineRenderer which provides functionality for
        the aliased scanline renderer.  Requires the base renderer as a
        template parameter, and should typically be created by a factory
        method in the base renderer.
    */
    template < class BaseRenderer >
    class ScanlineRendererBinSolid : public AbstractScanlineRenderer
    {
    private:
        agg::renderer_scanline_bin_solid< BaseRenderer > mRenderer;

    public:
        typedef BaseRenderer base_ren_type;
        typedef typename base_ren_type::color_type color_type;

        ScanlineRendererBinSolid() : mRenderer() { }
        explicit ScanlineRendererBinSolid( base_ren_type& ren ) : mRenderer( ren ) { }
        void attach(base_ren_type& ren)
        {
            mRenderer.attach( ren );
        }
        
        //--------------------------------------------------------------------
        void color( const agg::rgba& c ) { mRenderer.color( c ); }
        template < typename color_type >
        const color_type& color() const
        {
            return mRenderer.color();
        }
        //void get_color( agg::gray8* c )
        //{
        //    *c = mRenderer.color();
        //}

        //--------------------------------------------------------------------
        void prepare()
        {
            mRenderer.prepare();
        }

        //--------------------------------------------------------------------
        virtual void render(const agg::scanline_bin& sl)   { mRenderer.render( sl ); }
        virtual void render(const agg::scanline32_bin& sl) { mRenderer.render( sl ); }
        virtual void render(const agg::scanline_p8& sl)    { mRenderer.render( sl ); }
        virtual void render(const agg::scanline32_p8& sl)  { mRenderer.render( sl ); }
        virtual void render(const agg::scanline_u8& sl)    { mRenderer.render( sl ); }
        virtual void render(const agg::scanline32_u8& sl)  { mRenderer.render( sl ); }
        //template<class Scanline> void render(const Scanline& sl)
        //{
        //    mRenderer.render( sl );
        //}
    };

} // namespace AggOO

#endif  // AggOOAbstractScanlineRenderer_H_
