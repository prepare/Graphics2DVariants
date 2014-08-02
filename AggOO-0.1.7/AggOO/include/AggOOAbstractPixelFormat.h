
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

/*! $Id: AggOOAbstractPixelFormat.h,v 1.4 2009/06/19 23:12:35 dratek Exp $
 *  @file   AggOOAbstractPixelFormat.h
 *  @author Chad M. Draper
 *  @date   February 8, 2008
 *  @brief  Defines the AbstractPixelFormat, which allows for polymorphism
 *          of the AGG pixel formats.  Also defines the specialized format
 *          classes (SpecificPixelFormat, SpecificPixelFormatRGB, and
 *          SpecificPixelFormatRGBA) which take a template parameter to create
 *          the pixel format used for rendering.
 *  
 */

#ifndef AggOOAbstractPixelFormat_H_
#define AggOOAbstractPixelFormat_H_

#include "agg_basics.h"
#include "agg_color_gray.h"
#include "agg_color_rgba.h"
#include "agg_pixfmt_gray.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgb_packed.h"
#include "agg_pixfmt_rgba.h"

#include "AggOOEnums.h"
#include "AggOOAbstractRendererBase.h"
#include "AggOOBrush.h"
#include "AggOOImage.h"
#include "AggOOMatrix.h"
#include "AggOOStyleHandler.h"
#include "AggOOStyleHandlerGradient.h"
#include "AggOOStyleHandlerImage.h"
#include "AggOOPixelFormats.h"
#include "AggOOColorExt.h"
#include "AggOOPixelFormatConverter.h"

namespace AggOO
{
    /** @class  AbstractPixelFormat
        Base class for the polymorphic pixel formats.  There should be at least
        one subclass, which will take a template parameter for the AGG pixel
        format.  In some cases, there may be further subclassing to provide
        methods not common to all formats.

        @note
            Although the rendering buffer is technically a template paramter,
            all pixel formats use rendering_buffer.  Since this type is needed
            in the base class, it is defined as rendering_buffer.

        @note
            Many of the methods in this class are pure virtual.  In the event
            that a method requires a template type (such as color_type,) there
            will be prototypes for all types (there are only 4 color types in
            AGG -- gray8, gray16, rgba8, rgba16) and will have empty or default
            bodies.  The specific format will only need one override, as the
            template will handle the color type.
    */
    class AbstractPixelFormat
    {
    public:
        /** Empty constructor.  The derived classes will take a buffer to attach
            to the pixel format
        */
        AbstractPixelFormat()   { }

        virtual void attach_rendering_buffer( agg::rendering_buffer& buf ) = 0;

        /** Get the width of the attached buffer */
        AGG_INLINE virtual unsigned width()  const = 0;
        /** Get the height of the attached buffer */
        AGG_INLINE virtual unsigned height() const = 0;
        /** Get the stride of the attached buffer */
        AGG_INLINE virtual int      stride() const = 0;

        /** Get a pointer to a given row in the buffer */
        virtual int8u* row_ptr(int y) = 0;
        /** Get a const pointer to a given row in the buffer */
        const virtual int8u* row_ptr(int y) const = 0;
        /** Get the row data for a given row in the buffer */
        template < class RowT >
        RowT     row(int y)     const = 0;
        //virtual row_data     row(int y)     const = 0;

        /** Get a const pointer to the pixel buffer starting at a given position */
        const virtual int8u* pix_ptr(int x, int y) const = 0;

        /** Get a pointer to the pixel buffer starting at a given position */
        virtual int8u* pix_ptr(int x, int y) = 0;

        /** Create a pixel given an array of color information */
        //template < class color_type >
        //AGG_INLINE static void make_pix(int8u* p, const color_type& c) = 0;
        AGG_INLINE static void make_pix(int8u* p, const gray8& c)   { }
        AGG_INLINE static void make_pix(int8u* p, const gray16& c)  { }
        AGG_INLINE static void make_pix(int8u* p, const rgba8& c)   { }
        AGG_INLINE static void make_pix(int8u* p, const rgba16& c)  { }

        /** Get a pixel at a given position */
        //AGG_INLINE virtual color_type pixel(int x, int y) const  = 0;
        template < class ColorT >
        AGG_INLINE ColorT pixel(int x, int y) const  = 0;
        //AGG_INLINE virtual gray8 pixel(int x, int y) const  { }
        //AGG_INLINE virtual gray16 pixel(int x, int y) const { }
        //AGG_INLINE virtual rgba8 pixel(int x, int y) const  { }
        //AGG_INLINE virtual rgba16 pixel(int x, int y) const { }

        /** Set a given pixel in the buffer to a given color */
        //template < class color_type >
        //AGG_INLINE void copy_pixel(int x, int y, const color_type& c) = 0;
        AGG_INLINE virtual void copy_pixel(int x, int y, const gray8& c)    { }
        AGG_INLINE virtual void copy_pixel(int x, int y, const gray16& c)   { }
        AGG_INLINE virtual void copy_pixel(int x, int y, const rgba8& c)    { }
        AGG_INLINE virtual void copy_pixel(int x, int y, const rgba16& c)   { }

        /** Blend a color with a current pixel of the buffer */
        //template < class color_type >
        //AGG_INLINE void blend_pixel(int x, int y, const color_type& c, int8u cover) = 0;
        AGG_INLINE virtual void blend_pixel(int x, int y, const gray8& c, int8u cover)  { }
        AGG_INLINE virtual void blend_pixel(int x, int y, const gray16& c, int8u cover) { }
        AGG_INLINE virtual void blend_pixel(int x, int y, const rgba8& c, int8u cover)  { }
        AGG_INLINE virtual void blend_pixel(int x, int y, const rgba16& c, int8u cover) { }

        /** Fill a horizontal line with a given color */
        //template < class color_type >
        //AGG_INLINE void copy_hline(int x, int y, unsigned len, const color_type& c) = 0;
        AGG_INLINE virtual void copy_hline(int x, int y, unsigned len, const gray8& c)  { }
        AGG_INLINE virtual void copy_hline(int x, int y, unsigned len, const gray16& c) { }
        AGG_INLINE virtual void copy_hline(int x, int y, unsigned len, const rgba8& c)  { }
        AGG_INLINE virtual void copy_hline(int x, int y, unsigned len, const rgba16& c) { }

        /** Fill a vertical line with a given color */
        //template < class color_type >
        //AGG_INLINE void copy_vline(int x, int y, unsigned len, const color_type& c) = 0;
        AGG_INLINE virtual void copy_vline(int x, int y, unsigned len, const gray8& c)  { }
        AGG_INLINE virtual void copy_vline(int x, int y, unsigned len, const gray16& c) { }
        AGG_INLINE virtual void copy_vline(int x, int y, unsigned len, const rgba8& c)  { }
        AGG_INLINE virtual void copy_vline(int x, int y, unsigned len, const rgba16& c) { }

        /** Blend a color with a horizontal line */
        //template < class color_type >
        //void blend_hline(int x, int y, unsigned len, const color_type& c, int8u cover) = 0;
        virtual void blend_hline(int x, int y, unsigned len, const gray8& c, int8u cover)   { }
        virtual void blend_hline(int x, int y, unsigned len, const gray16& c, int8u cover)  { }
        virtual void blend_hline(int x, int y, unsigned len, const rgba8& c, int8u cover)   { }
        virtual void blend_hline(int x, int y, unsigned len, const rgba16& c, int8u cover)  { }

        /** Blend a color with a vertical line */
        //template < class color_type >
        //void blend_vline(int x, int y, unsigned len, const color_type& c, int8u cover) = 0;
        virtual void blend_vline(int x, int y, unsigned len, const gray8& c, int8u cover)   { }
        virtual void blend_vline(int x, int y, unsigned len, const gray16& c, int8u cover)  { }
        virtual void blend_vline(int x, int y, unsigned len, const rgba8& c, int8u cover)   { }
        virtual void blend_vline(int x, int y, unsigned len, const rgba16& c, int8u cover)  { }

        /** Blend a color with a horizontal span */
        //template < class color_type >
        //void blend_solid_hspan(int x, int y, unsigned len, const color_type& c, const int8u* covers) = 0;
        virtual void blend_solid_hspan(int x, int y, unsigned len, const gray8& c, const int8u* covers) { }
        virtual void blend_solid_hspan(int x, int y, unsigned len, const gray16& c, const int8u* covers) { }
        virtual void blend_solid_hspan(int x, int y, unsigned len, const rgba8& c, const int8u* covers) { }
        virtual void blend_solid_hspan(int x, int y, unsigned len, const rgba16& c, const int8u* covers) { }

        /** Blend a color with vertical span */
        //template < class color_type >
        //void blend_solid_vspan(int x, int y, unsigned len, const color_type& c, const int8u* covers) = 0;
        virtual void blend_solid_vspan(int x, int y, unsigned len, const gray8& c, const int8u* covers) { }
        virtual void blend_solid_vspan(int x, int y, unsigned len, const gray16& c, const int8u* covers) { }
        virtual void blend_solid_vspan(int x, int y, unsigned len, const rgba8& c, const int8u* covers) { }
        virtual void blend_solid_vspan(int x, int y, unsigned len, const rgba16& c, const int8u* covers) { }

        /** Copy a set of colors to a horizontal span */
        //template < class color_type >
        //void copy_color_hspan(int x, int y, unsigned len, const color_type* colors) = 0;
        virtual void copy_color_hspan(int x, int y, unsigned len, const gray8* colors)  { }
        virtual void copy_color_hspan(int x, int y, unsigned len, const gray16* colors) { }
        virtual void copy_color_hspan(int x, int y, unsigned len, const rgba8* colors)  { }
        virtual void copy_color_hspan(int x, int y, unsigned len, const rgba16* colors) { }

        /** Copy a set of colors to a vertical span */
        //template < class color_type >
        //void copy_color_vspan(int x, int y, unsigned len, const color_type* colors) = 0;
        virtual void copy_color_vspan(int x, int y, unsigned len, const gray8* colors)  { }
        virtual void copy_color_vspan(int x, int y, unsigned len, const gray16* colors) { }
        virtual void copy_color_vspan(int x, int y, unsigned len, const rgba8* colors)  { }
        virtual void copy_color_vspan(int x, int y, unsigned len, const rgba16* colors) { }

        /** Blend a set of colors with horizontal span */
        //template < class color_type >
        //void blend_color_hspan(int x, int y, unsigned len, const color_type* colors, const int8u* covers, int8u cover) = 0;
        virtual void blend_color_hspan(int x, int y, unsigned len, const gray8* colors, const int8u* covers, int8u cover)   { }
        virtual void blend_color_hspan(int x, int y, unsigned len, const gray16* colors, const int8u* covers, int8u cover)  { }
        virtual void blend_color_hspan(int x, int y, unsigned len, const rgba8* colors, const int8u* covers, int8u cover)   { }
        virtual void blend_color_hspan(int x, int y, unsigned len, const rgba16* colors, const int8u* covers, int8u cover)  { }

        /** Blend a set of colors with vertical span */
        //template < class color_type >
        //void blend_color_vspan(int x, int y, unsigned len, const color_type* colors, const int8u* covers, int8u cover) = 0;
        virtual void blend_color_vspan(int x, int y, unsigned len, const gray8* colors, const int8u* covers, int8u cover)   { }
        virtual void blend_color_vspan(int x, int y, unsigned len, const gray16* colors, const int8u* covers, int8u cover)  { }
        virtual void blend_color_vspan(int x, int y, unsigned len, const rgba8* colors, const int8u* covers, int8u cover)   { }
        virtual void blend_color_vspan(int x, int y, unsigned len, const rgba16* colors, const int8u* covers, int8u cover)  { }

        /** Apply a function to all pixels in the buffer */
        template<class Function> void for_each_pixel(Function f) = 0;

        /** The rgba formats allow premultiply.  Others don't use it. */
        virtual void premultiply()  { }

        /** The rgba formats allow demultiply.  Others don't use it. */
        virtual void demultiply()   { }

        /** Apply a gamma to all pixels in the buffer */
        template<class GammaLut> void apply_gamma_dir(const GammaLut& g) = 0;

        /** Apply a gamma to all pixels in the buffer */
        template<class GammaLut> void apply_gamma_inv(const GammaLut& g) = 0;

        /** Copy a region from another buffer */
        template<class RenBuf2>
        void copy_from(const RenBuf2& from, int xdst, int ydst, int xsrc, int ysrc, unsigned len) = 0;

        /** Some formats allow blending from a like pixel format */
        template<class SrcPixelFormatRenderer> 
        void blend_from(const SrcPixelFormatRenderer& from, 
                        int xdst, int ydst,
                        int xsrc, int ysrc,
                        unsigned len,
                        int8u cover)
        {
        }

        //--------------------------------------------------------------------
        //template<class SrcPixelFormatRenderer, class color_type>
        //    void blend_from_color(const SrcPixelFormatRenderer& from, const color_type& color, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover)    { }
        template<class SrcPixelFormatRenderer>
            void blend_from_color(const SrcPixelFormatRenderer& from, const gray8& color, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover)    { }
        template<class SrcPixelFormatRenderer>
            void blend_from_color(const SrcPixelFormatRenderer& from, const gray16& color, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover)   { }
        template<class SrcPixelFormatRenderer>
            void blend_from_color(const SrcPixelFormatRenderer& from, const rgba8& color, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover)    { }
        template<class SrcPixelFormatRenderer>
            void blend_from_color(const SrcPixelFormatRenderer& from, const rgba16& color, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover)   { }

        //--------------------------------------------------------------------
        //template<class SrcPixelFormatRenderer, class color_type>
        //    void blend_from_lut(const SrcPixelFormatRenderer& from, const color_type* color_lut, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover) { }
        template<class SrcPixelFormatRenderer>
            void blend_from_lut(const SrcPixelFormatRenderer& from, const gray8* color_lut, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover) { }
        template<class SrcPixelFormatRenderer>
            void blend_from_lut(const SrcPixelFormatRenderer& from, const gray16* color_lut, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover) { }
        template<class SrcPixelFormatRenderer>
            void blend_from_lut(const SrcPixelFormatRenderer& from, const rgba8* color_lut, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover) { }
        template<class SrcPixelFormatRenderer>
            void blend_from_lut(const SrcPixelFormatRenderer& from, const rgba16* color_lut, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover) { }

        /** Get the AggOO pixel format enumeration */
        PixelFormat GetPixelFormat() const      { return mAggOOPixelFormat; }

        //----------------------------------------------------------------------
        // Factory methods
        //----------------------------------------------------------------------

        /** Create the renderer base */
        virtual AbstractRendererBase* CreateRendererBase() = 0;

        /** Static method to create the actual pixel format, based on a pixel
            format enumeration.
        */
        static AbstractPixelFormat* CreatePixelFormat( agg::rendering_buffer& buf, PixelFormat format, ColorFormat colorFormat = ColorFormatPlain );

        /** Create a style handler from a given brush */
        virtual StyleHandler* CreateStyleHandler( const Brush* brush, const Matrix* matrix ) = 0;

    private:
        typedef AbstractPixelFormat* (*CreateFunc)( agg::rendering_buffer&, ColorFormat colorFormat );
        static const CreateFunc createPixelFormatArray[ PixelFormatMax ];

        static AbstractPixelFormat* create1bppIndexed( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create4bppIndexed( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create8bppIndexed( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create8bppGrayScale( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );

        static AbstractPixelFormat* create16bppGrayScale( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create16bppRGB555( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create16bppRGB565( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create16bppARGB1555( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );

        static AbstractPixelFormat* create24bppRGB( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );

        static AbstractPixelFormat* create32bppRGB( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create32bppARGB( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create32bppPARGB( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );

        static AbstractPixelFormat* create48bppRGB( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );

        static AbstractPixelFormat* create64bppARGB( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create64bppPARGB( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );

        static AbstractPixelFormat* create24bppBGR( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create30bppRGB( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create30bppBGR( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );

        static AbstractPixelFormat* create32bppBGR( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create32bppRGBA( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create32bppBGRA( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create32bppABGR( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );

        static AbstractPixelFormat* create48bppBGR( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create64bppRGBA( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create64bppABGR( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );
        static AbstractPixelFormat* create64bppBGRA( agg::rendering_buffer& buf, ColorFormat colorFormat = ColorFormatPlain );

    protected:
        PixelFormat mAggOOPixelFormat;      /**< Maintains the AggOO format used for creating the pixel format */
        ColorFormat mAggOOColorFormat;      /**< Maintains the AggOO color format */

    }; // class AbstractPixelFormat

    ////////////////////////////////////////////////////////////////////////////

    /** @class SpecificPixelFormat
        A derivation of AbstractPixelFormat which requires a template parameter
        for the pixel format.  This class can be used for most cases.
    */
    template < class PixFmt >
    class SpecificPixelFormat : public AbstractPixelFormat//< PixFmt::color_type >
    {
    public:
        typedef PixFmt                          pixel_type;
        typedef typename PixFmt::rbuf_type      rbuf_type;
        typedef typename PixFmt::color_type     color_type;
        typedef typename rbuf_type::row_data    row_data;

    public:
        SpecificPixelFormat()
        {
        }
        /** Constructor which takes a buffer to attach to the pixel format */
        explicit SpecificPixelFormat( rbuf_type& buf )
            : mPixel( buf )
        {
        }

        /** Attach a buffer to the pixel format */
        void attach(rbuf_type& rb)
        {
            mPixel.attach( rb );
        }
        /** Attach a rendering_buffer to the pixel format */
        void attach_rendering_buffer( agg::rendering_buffer& buf )
        {
            mPixel.attach( buf );
        }

        template<class PixFmt2>
        bool attach(PixFmt2& pixf, int x1, int y1, int x2, int y2)
        {
            mPixel.attach( pixf, x1, y1, x2, y2 );
        }

        /** Get the width of the attached buffer */
        AGG_INLINE unsigned width()  const  { return mPixel.width(); }
        /** Get the height of the attached buffer */
        AGG_INLINE unsigned height() const  { return mPixel.height(); }
        /** Get the stride of the attached buffer */
        AGG_INLINE int      stride() const  { return mPixel.stride(); }

        /** Get a pointer to a given row in the buffer */
        agg::int8u* row_ptr(int y)               { return mPixel.row_ptr( y ); }
        /** Get a const pointer to a given row in the buffer */
        const agg::int8u* row_ptr(int y) const   { return mPixel.row_ptr( y ); }
        /** Get the row data for a given row in the buffer */
        row_data   row(int y)     const
        {
            return mPixel.row( y );
        }

        /** Get a const pointer to the pixel buffer starting at a given position */
        const agg::int8u* pix_ptr(int x, int y) const
        {
            return mPixel.pix_ptr( x, y );
        }

        /** Get a pointer to the pixel buffer starting at a given position */
        int8u* pix_ptr(int x, int y)
        {
            return mPixel.pix_ptr( x, y );
        }

        /** Create a pixel given an array of color information */
        AGG_INLINE static void make_pix(agg::int8u* p, const color_type& c)  { mPixel.make_pix( p, c ); }

        ///** Get a pixel at a given position */
        AGG_INLINE color_type pixel(int x, int y) const
        {
            return mPixel.pixel( x, y );
        }

        /** Set a given pixel in the buffer to a given color */
        AGG_INLINE void copy_pixel(int x, int y, const color_type& c)   { mPixel.copy_pixel( x, y, c ); }

        /** Blend a color with a current pixel of the buffer */
        AGG_INLINE void blend_pixel(int x, int y, const color_type& c, int8u cover) { mPixel.blend_pixel( x, y, c, cover ); }

        /** Fill a horizontal line with a given color */
        AGG_INLINE void copy_hline(int x, int y, unsigned len, const color_type& c) { mPixel.copy_hline( x, y, len, c ); }

        /** Fill a vertical line with a given color */
        AGG_INLINE void copy_vline(int x, int y, unsigned len, const color_type& c) { mPixel.copy_vline( x, y, len, c ); }

        /** Blend a color with a horizontal line */
        void blend_hline(int x, int y, unsigned len, const color_type& c, int8u cover)  { mPixel.blend_hline( x, y, len, c, cover ); }

        /** Blend a color with a vertical line */
        void blend_vline(int x, int y, unsigned len, const color_type& c, int8u cover)  { mPixel.blend_vline( x, y, len, c, cover ); }

        /** Blend a color with a horizontal span */
        void blend_solid_hspan(int x, int y, unsigned len, const color_type& c, const int8u* covers)    { mPixel.blend_solid_hspan( x, y, len, c, covers ); }

        /** Blend a color with vertical span */
        void blend_solid_vspan(int x, int y, unsigned len, const color_type& c, const int8u* covers)    { mPixel.blend_solid_vspan( x, y, len, c, covers ); }

        /** Copy a set of colors to a horizontal span */
        void copy_color_hspan(int x, int y, unsigned len, const color_type* colors)     { mPixel.copy_color_hspan( x, y, len, colors ); }

        /** Copy a set of colors to a vertical span */
        void copy_color_vspan(int x, int y, unsigned len, const color_type* colors)     { mPixel.copy_color_vspan( x, y, len, colors ); }

        /** Blend a set of colors with horizontal span */
        void blend_color_hspan(int x, int y, unsigned len, const color_type* colors, const int8u* covers, int8u cover)  { mPixel.blend_color_hspan( x, y, len, colors, covers, cover ); }

        /** Blend a set of colors with vertical span */
        void blend_color_vspan(int x, int y, unsigned len, const color_type* colors, const int8u* covers, int8u cover)   { mPixel.blend_color_vspan( x, y, len, colors, covers, cover ); }

        /** Apply a function to all pixels in the buffer */
        template<class Function> void for_each_pixel(Function f)    { mPixel.for_each_pixel( f ); }

        /** Apply a gamma to all pixels in the buffer */
        template<class GammaLut> void apply_gamma_dir(const GammaLut& g)    { mPixel.apply_gamma_dir( g ); }

        /** Apply a gamma to all pixels in the buffer */
        template<class GammaLut> void apply_gamma_inv(const GammaLut& g)    { mPixel.apply_gamma_inv( g ); }

        /** Copy a region from another buffer */
        template<class RenBuf2>
        void copy_from(const RenBuf2& from, int xdst, int ydst, int xsrc, int ysrc, unsigned len)
        {
            mPixel.copy_from( from, xdst, ydst, xsrc, ysrc, len );
        }

        //--------------------------------------------------------------------
        template<class SrcPixelFormatRenderer>
            void blend_from_color(const SrcPixelFormatRenderer& from, const color_type& color, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover)
        {
            mPixel.blend_from_color( from, color, xdst, ydst, xsrc, ysrc, len, cover );
        }

        //--------------------------------------------------------------------
        template<class SrcPixelFormatRenderer>
            void blend_from_lut(const SrcPixelFormatRenderer& from, const color_type* color_lut, int xdst, int ydst, int xsrc, int ysrc, unsigned len, int8u cover)
        {
            mPixel.blend_from_lut( from, color_lut, xdst, ydst, xsrc, ysrc, len, cover );
        }

        /** Create the base renderer */
        virtual AbstractRendererBase* CreateRendererBase()
        {
            return new SpecificRenderer< SpecificPixelFormat< PixFmt > >( *this );
        }

        /** Create a style handler from a given brush */
        virtual StyleHandler* CreateStyleHandler( const Brush* brush, const Matrix* matrix )
        {
            AggOO::StyleHandler* handler = NULL;
            switch ( brush->GetType() )
            {
            case BrushTypeTextureFill:
                {
                    const TextureBrush* texBrush = static_cast< const TextureBrush* >( brush );
                    Image* image = texBrush->GetImage();
                    BitmapData* imageData = &image->mActiveBitmap;
                    Rect srcRect( 0, 0, image->GetWidth(), image->GetHeight() );

                    // Take a section from the image and convert it to the desired pixel
                    // format.  Create a temporary BitmapData to define the region to
                    // extract, and a BitmapData to receive the converted data.
                    BitmapData tempImageData( srcRect, imageData->Stride, imageData->PixelFormat, 0, imageData->Scan0 );
                    if ( tempImageData.X + tempImageData.Width > imageData->X + imageData->Width )
                        tempImageData.Width    = IMinClamp( imageData->Width - tempImageData.X, 0 );
                    if ( tempImageData.Y + tempImageData.Height > imageData->Y + imageData->Height )
                        tempImageData.Height   = IMinClamp( imageData->Height - tempImageData.Y, 0 );

                    // Convert the texture brush's image to the same format as
                    // the display
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
                        return NULL;

                    //memset( convImageData.Scan0, 255, convImageData.Stride * convImageData.Height );
                    BufferConverter::ConvertPixelFormat( &tempImageData, &convImageData );

                    // Create a rendering_buffer from the newly converted buffer
                    agg::rendering_buffer rbuf;
                    rbuf.attach( (BYTE*)convImageData.Scan0, convImageData.Width, convImageData.Height, convImageData.Stride );

                    // Create the appropriate pixel format:
                    AbstractPixelFormat* pixFormat = AbstractPixelFormat::CreatePixelFormat( rbuf, mAggOOPixelFormat, mAggOOColorFormat );

                    Color col( 255, 128, 255, 0 );
                    //handler = new SolidStyleHandler< PixFmt::color_type >( ColorExt::toRGBA( col ) );
                    //handler = new SolidStyleHandler( ColorExt::toRGBA( col ) );
                    const TextureBrush* textureBrush = static_cast< const TextureBrush* >( brush );
                    agg::trans_affine textureMtx( textureBrush->mMatrix.GetAffine() );
                    textureMtx.multiply( matrix->GetAffine() );
                    PixFmt imgPixf( rbuf );
                    //handler = new ImageStyleHandler< PixFmt >( imgPixf, textureMtx, ImageStyleHandler< PixFmt >::StyleRGBANN, ImageStyleHandler< PixFmt >::WrapTile );
                    handler = new SolidStyleHandler( ColorExt::toRGBA( Color::Yellow ) );

                    delete pixFormat;
                }
                break;

            case BrushTypeLinearGradient:
                {
                    //return brush->CreateStyleHandler( matrix );
                    //Color col( 255, 255, 0, 0 );
                    //handler = new SolidStyleHandler< PixFmt::color_type >( ColorExt::toRGBA( col ) );
                    const LinearGradientBrush* gradBrush = static_cast< const LinearGradientBrush* >( brush );
                    agg::trans_affine gradMtx( gradBrush->mMatrix.GetAffine() );
                    gradMtx.multiply( matrix->GetAffine() );
                    handler = new GradientStyleHandler< PixFmt::color_type >( &gradBrush->mColors.at( 0 ), gradBrush->mColors.size(), gradBrush->mGradientLength, gradMtx, gradBrush->mGradientStyle, gradBrush->mWrapMode, gradBrush->mGradientInterpolation == GradientInterpolationStep, 0.0, 0.0, gradBrush->data.spiralAngle );
                }
                break;

            case BrushTypeSolidColor:
                {
                    Color col;
                    ((SolidBrush*)brush)->GetColor( &col );
                    //handler = new SolidStyleHandler< PixFmt::color_type >( ColorExt::toRGBA( col ) );
                    handler = new SolidStyleHandler( ColorExt::toRGBA( col ) );
                }
                break;

                // Need to add hatch and path gradient
            }

            return handler;
        }

    protected:
        pixel_type  mPixel;

    }; // class SpecificPixelFormat

    ////////////////////////////////////////////////////////////////////////////

    /** @class SpecificPixelFormatGray
    */
    template < class PixFmt >
    class SpecificPixelFormatGray : public SpecificPixelFormat< PixFmt >
    {
    public:
        SpecificPixelFormatGray()
        {
        }
        /** Constructor which takes a buffer to attach to the pixel format */
        explicit SpecificPixelFormatGray( rbuf_type& buf )
            : SpecificPixelFormat<PixFmt>( buf )
        {
        }

        virtual AbstractRendererBase* CreateRendererBase()
        {
            return new SpecificRenderer< SpecificPixelFormatGray< PixFmt > >( *this );
        }
    };

    /** @class SpecificPixelFormatRGBA
        The pixfmt_rgbaNN types have some methods not in the other formats.
        This sub-class will allow for using those methods, such as comp_op, in
        the following manner:

            AbstractPixelFormat* pixFmtPtr;
            switch ( format )
            {
            case rgba32:
                {
                    SpecificPixelFormatRGBA< agg::pixfmt_rgba32 >* newPixFmt = new SpecificPixelFormatRGBA< agg::pixfmt_rgba32 >( buf, op );
                    newPixFmt->comp_op( new_op );
                    .
                    .
                    .
                    pixFmtPtr = newPixFmt;
                }
                break;
                .
                .
                .
            }

        Note that there needs to be a switch statement, or a factory of some
        sort to create the pixel format.
    */
    template < class PixFmt >
    class SpecificPixelFormatRGBA : public SpecificPixelFormat< PixFmt >
    {
    //public:
    //    typedef PixFmt              pixel_type;
    //    typedef PixFmt::rbuf_type   rbuf_type;
    //    typedef PixFmt::color_type  color_type;

    public:
        SpecificPixelFormatRGBA()
            : SpecificPixelFormat<PixFmt>()
        {
        }
        explicit SpecificPixelFormatRGBA( rbuf_type& buf, unsigned comp_op = 3 )
            : SpecificPixelFormat<PixFmt>( buf )
        {
            //mPixel.comp_op( comp_op );
        }

        ///** Set the composite operator */
        //void comp_op(unsigned op) { mPixel.comp_op( op ); }
        ///** Get the composite operator currently used by this format */
        //unsigned comp_op() const  { return mPixel.comp_op(); }

        /** The rgba formats allow premultiply.  Others don't use it. */
        void premultiply()  { mPixel.premultiply(); }

        /** The rgba formats allow demultiply.  Others don't use it. */
        void demultiply()   { mPixel.demultiply(); }

        /** The rgba formats allow blending from a like pixel format */
        template<class SrcPixelFormatRenderer> 
        void blend_from(const SrcPixelFormatRenderer& from, 
                        int xdst, int ydst,
                        int xsrc, int ysrc,
                        unsigned len,
                        agg::int8u cover)
        {
            mPixel.blend_from( from, xdst, ydst, xsrc, ysrc, len, cover );
        }

        virtual AbstractRendererBase* CreateRendererBase()
        {
            return new SpecificRenderer< SpecificPixelFormatRGBA< PixFmt > >( *this );
        }

    }; // class SpecificPixelFormatRGBA

    ////////////////////////////////////////////////////////////////////////////

    /** @class SpecificPixelFormatRGB
        The pixfmt_rgbNN types have some methods not in the other formats.

            AbstractPixelFormat* pixFmtPtr;
            switch ( format )
            {
            case rgb24:
                {
                    SpecificPixelFormatRGBA< agg::pixfmt_rgb24 >* newPixFmt = new SpecificPixelFormatRGBA< agg::pixfmt_rgb24 >( buf, op );
                    newPixFmt->comp_op( new_op );
                    .
                    .
                    .
                    pixFmtPtr = newPixFmt;
                }
                break;
                .
                .
                .
            }

        Note that there needs to be a switch statement, or a factory of some
        sort to create the pixel format.
    */
    template < class PixFmt >
    class SpecificPixelFormatRGB : public SpecificPixelFormat< PixFmt >
    {
    public:
        typedef PixFmt              pixel_type;
        typedef typename PixFmt::rbuf_type   rbuf_type;
        typedef typename PixFmt::color_type  color_type;
        typedef typename PixFmt::blender_type blender_type;

    public:
        SpecificPixelFormatRGB()
            : SpecificPixelFormat<PixFmt>()
        {
        }
        explicit SpecificPixelFormatRGB( rbuf_type& buf )
            : SpecificPixelFormat<PixFmt>( buf )
        {
        }

        blender_type& blender() { return mPixel.blender(); }

        /** The rgba formats allow blending from a like pixel format */
        template<class SrcPixelFormatRenderer> 
        void blend_from(const SrcPixelFormatRenderer& from, 
                        int xdst, int ydst,
                        int xsrc, int ysrc,
                        unsigned len,
                        agg::int8u cover)
        {
            mPixel.blend_from( from, xdst, ydst, xsrc, ysrc, len, cover );
        }

        virtual AbstractRendererBase* CreateRendererBase()
        {
            return new SpecificRenderer< SpecificPixelFormatRGB< PixFmt > >( *this );
        }

    }; // class SpecificPixelFormatRGB

} // namespace AggOO

#endif  // AggOOAbstractPixelFormat_H_
