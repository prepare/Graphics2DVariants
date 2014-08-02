
/*! $Id: AggOOStyleHandlerImage.h,v 1.4 2009/06/19 23:12:35 dratek Exp $
 *  @file   AggOOStyleHandlerImage.h
 *  @author Chad M. Draper
 *  @date   January 23, 2008
 *  
 *  Defines a style handler for images (textures.)
 *
 */

#ifndef AGGOO_STYLE_HANDLER_IMAGE_H_
#define AGGOO_STYLE_HANDLER_IMAGE_H_

#include "AggOOStyleHandler.h"
#include "AggOOSharedPtr.h"

#include "agg_span_image_filter_gray.h"
#include "agg_span_image_filter_rgb.h"
#include "agg_span_image_filter_rgba.h"
#include "agg_span_interpolator_linear.h"
#include "agg_image_accessors.h"

#include <string>

namespace AggOO
{
    /** @class ImageAccessor
        There are 5 different wrapping modes for images:
            - Tile          Repeats the image in the x and y directions
            - FlipX         Alternates flipping the image horizontally, repeats the image vertically
            - FlipY         Alternates flipping the image vertically, repeats the image horizontally
            - FlipXY        Alternates flipping the image both vertically and horizontally
            - Clamp         Outside of the image area, the nearest edge pixels of the image are used

        The ImageAccessor class allows a polymorphic way of using these wrap
        modes by subclassing ImageAccessor with the specific modes.
    */
    template < class PixFmt >
    class ImageAccessor
    {
    protected:
        std::string mAccessorName;

    public:
        typedef typename PixFmt::color_type color_type;
        typedef typename PixFmt::order_type order_type;
        typedef typename PixFmt::value_type value_type;
        //enum pix_width_e { pix_width = pixfmt_type::pix_width };

    public:
        ImageAccessor()
        {
        }

        virtual void attach(const PixFmt& pixf) = 0;

        AGG_INLINE virtual const agg::int8u* span(int x, int y, unsigned) = 0;

        AGG_INLINE virtual const agg::int8u* next_x() = 0;

        AGG_INLINE virtual const agg::int8u* next_y() = 0;
    };

    /** @class ImageTileAccessor
        Repeats the image horizontally and vertically.
    */
    template < class PixFmt >
    class ImageTileAccessor : public ImageAccessor<PixFmt>
    {
    private:
        typedef agg::image_accessor_wrap< PixFmt, agg::wrap_mode_repeat, agg::wrap_mode_repeat > accessor_type;
        accessor_type mAccessor;

    public:
        ImageTileAccessor()
            : ImageAccessor<PixFmt>()
        {
            mAccessorName = "ImageTileAccessor";
        }

        explicit ImageTileAccessor( const PixFmt& pixf )
            : ImageAccessor<PixFmt>()
        {
            mAccessorName = "ImageTileAccessor";
            attach( pixf );
        }

        void attach(const PixFmt& pixf)
        {
            mAccessor.attach( pixf );
        }

        AGG_INLINE const agg::int8u* span(int x, int y, unsigned d)
        {
            return mAccessor.span( x, y, d );
        }

        AGG_INLINE const agg::int8u* next_x()
        {
            return mAccessor.next_x();
        }

        AGG_INLINE const agg::int8u* next_y()
        {
            return mAccessor.next_y();
        }
    };

    /** @class ImageFlipXAccessor
        Alternate flipping the image horizontally, repeat image vertically.
    */
    template < class PixFmt >
    class ImageFlipXAccessor : public ImageAccessor<PixFmt>
    {
    private:
        typedef agg::image_accessor_wrap< PixFmt, agg::wrap_mode_reflect, agg::wrap_mode_repeat > accessor_type;
        accessor_type mAccessor;

    public:
        ImageFlipXAccessor()
            : ImageAccessor<PixFmt>()
        {
            mAccessorName = "ImageFlipXAccessor";
        }

        explicit ImageFlipXAccessor( const PixFmt& pixf )
            : ImageAccessor<PixFmt>()
        {
            mAccessorName = "ImageFlipXAccessor";
            attach( pixf );
        }

        void attach(const PixFmt& pixf)
        {
            mAccessor.attach( pixf );
        }

        AGG_INLINE const agg::int8u* span(int x, int y, unsigned d)
        {
            return mAccessor.span( x, y, d );
        }

        AGG_INLINE const agg::int8u* next_x()
        {
            return mAccessor.next_x();
        }

        AGG_INLINE const agg::int8u* next_y()
        {
            return mAccessor.next_y();
        }
    };

    /** @class ImageFlipYAccessor
        Alternate flipping the image vertically, repeat image horizontally.
    */
    template < class PixFmt >
    class ImageFlipYAccessor : public ImageAccessor<PixFmt>
    {
    private:
        typedef agg::image_accessor_wrap< PixFmt, agg::wrap_mode_repeat, agg::wrap_mode_reflect > accessor_type;
        accessor_type mAccessor;

    public:
        ImageFlipYAccessor()
            : ImageAccessor<PixFmt>()
        {
            mAccessorName = "ImageFlipYAccessor";
        }

        explicit ImageFlipYAccessor( const PixFmt& pixf )
            : ImageAccessor<PixFmt>()
        {
            mAccessorName = "ImageFlipYAccessor";
            attach( pixf );
        }

        void attach(const PixFmt& pixf)
        {
            mAccessor.attach( pixf );
        }

        AGG_INLINE const agg::int8u* span(int x, int y, unsigned d)
        {
            return mAccessor.span( x, y, d );
        }

        AGG_INLINE const agg::int8u* next_x()
        {
            return mAccessor.next_x();
        }

        AGG_INLINE const agg::int8u* next_y()
        {
            return mAccessor.next_y();
        }
    };

    /** @class ImageFlipXYAccessor
        Alternate flipping the image horizontally and vertically.
    */
    template < class PixFmt >
    class ImageFlipXYAccessor : public ImageAccessor<PixFmt>
    {
    private:
        typedef agg::image_accessor_wrap< PixFmt, agg::wrap_mode_reflect, agg::wrap_mode_reflect > accessor_type;
        accessor_type mAccessor;

    public:
        ImageFlipXYAccessor()
            : ImageAccessor<PixFmt>()
        {
            mAccessorName = "ImageFlipXYAccessor";
        }

        explicit ImageFlipXYAccessor( const PixFmt& pixf )
            : ImageAccessor<PixFmt>()
        {
            mAccessorName = "ImageFlipXYAccessor";
            attach( pixf );
        }

        void attach(const PixFmt& pixf)
        {
            mAccessor.attach( pixf );
        }

        AGG_INLINE const agg::int8u* span(int x, int y, unsigned d)
        {
            return mAccessor.span( x, y, d );
        }

        AGG_INLINE const agg::int8u* next_x()
        {
            return mAccessor.next_x();
        }

        AGG_INLINE const agg::int8u* next_y()
        {
            return mAccessor.next_y();
        }
    };

    /** @class ImageClampAccessor
        Image is clamped.  Pixels outside the image area will use the nearest
        edge pixel of the image.
    */
    template < class PixFmt >
    class ImageClampAccessor : public ImageAccessor<PixFmt>
    {
    private:
        typedef agg::image_accessor_clip< PixFmt > accessor_type;
        accessor_type mAccessor;

    public:
        ImageClampAccessor()
            : ImageAccessor<PixFmt>()
        {
            mAccessorName = "ImageClampAccessor";
        }

        explicit ImageClampAccessor( const PixFmt& pixf, const color_type& bgColor )
            : ImageAccessor<PixFmt>()
        {
            mAccessorName = "ImageClampAccessor";
            attach( pixf );
        }

        void attach(const PixFmt& pixf)
        {
            mAccessor.attach( pixf );
        }

        AGG_INLINE const agg::int8u* span(int x, int y, unsigned d)
        {
            return mAccessor.span( x, y, d );
        }

        AGG_INLINE const agg::int8u* next_x()
        {
            return mAccessor.next_x();
        }

        AGG_INLINE const agg::int8u* next_y()
        {
            return mAccessor.next_y();
        }
    };

    //typedef boost::shared_ptr< ImageAccessor<PixFmt> >       ImageAccessorPtr;

    ////////////////////////////////////////////////////////////////////////////

    /** @class ImageFilter
        Base class for the image filter wrappers.  The subclasses will wrap an
        agg::span_image_filter_*, which is used for the scanline generation.
    */
    template < class PixFmt >
    class ImageFilter
    {
    protected:
        typedef AggOO::SharedPtr< ImageAccessor<PixFmt> >  ImageAccessorPtr;
        ImageAccessorPtr    mImageAccessor;
        std::string         mFilterName;
        agg::trans_affine   mMatrix;

    public:
        typedef agg::span_interpolator_linear<> interpolator_type_linear;
        typedef typename PixFmt::color_type  color_type;

    public:
        ImageFilter( ImageAccessorPtr imgAccessor, const agg::trans_affine& mtx )
            : mImageAccessor( imgAccessor ),
              mMatrix( mtx )
        {
            mMatrix.invert();
        }

        virtual void generate(color_type* span, int x, int y, unsigned len) = 0;
    };

    ///** @class ImageFilterGray
    //*/
    //template < class PixFmt >
    //class ImageFilterGray : public ImageFilter<PixFmt>
    //{
    //private:
    //    const image_filter_lut* mFilter;

    //public:
    //    ImageFilterGray( ImageAccessorPtr imgAccessor, const image_filter_lut& filter )
    //        : ImageFilter<PixFmt>( imgAccessor ),
    //          mFilter( &filter )
    //    {
    //        mFilterName = "ImageFilterGray";
    //    }

    //    void generate(color_type* span, int x, int y, unsigned len)
    //    {
    //        interpolator_type_linear interp;
    //        agg::span_image_filter_gray< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp, *mFilter );
    //        filter.generate( span, x, y, len );
    //    }
    //};

    ///** @class ImageFilterGray2x2
    //*/
    //template < class PixFmt >
    //class ImageFilterGray2x2 : public ImageFilter<PixFmt>
    //{
    //private:
    //    const image_filter_lut* mFilter;

    //public:
    //    ImageFilterGray2x2( ImageAccessorPtr imgAccessor, const image_filter_lut& filter )
    //        : ImageFilter<PixFmt>( imgAccessor ),
    //          mFilter( &filter )
    //    {
    //        mFilterName = "ImageFilterGray2x2";
    //    }

    //    void generate(color_type* span, int x, int y, unsigned len)
    //    {
    //        interpolator_type_linear interp;
    //        agg::span_image_filter_gray_2x2< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp, *mFilter );
    //        filter.generate( span, x, y, len );
    //    }
    //};

    /////** @class ImageFilterGrayAffine
    ////*/
    ////template < class PixFmt >
    ////class ImageFilterGrayAffine : public ImageFilter<PixFmt>
    ////{
    ////private:
    ////    const image_filter_lut* mFilter;

    ////public:
    ////    ImageFilterGrayAffine( ImageAccessorPtr imgAccessor, const image_filter_lut& filter )
    ////        : ImageFilter<PixFmt>( imgAccessor ),
    ////          mFilter( &filter )
    ////    {
    ////        mFilterName = "ImageFilterGrayAffine";
    ////    }

    ////    void generate(color_type* span, int x, int y, unsigned len)
    ////    {
    ////        interpolator_type_linear interp;
    ////        agg::span_image_filter_gray_affine< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp, *mFilter );
    ////        filter.generate( span, x, y, len );
    ////    }
    ////};

    ///** @class ImageFilterGrayBilinear
    //*/
    //template < class PixFmt >
    //class ImageFilterGrayBilinear : public ImageFilter<PixFmt>
    //{
    //public:
    //    ImageFilterGrayBilinear( ImageAccessorPtr imgAccessor )
    //        : ImageFilter<PixFmt>( imgAccessor )
    //    {
    //        mFilterName = "ImageFilterGrayBilinear";
    //    }

    //    void generate(color_type* span, int x, int y, unsigned len)
    //    {
    //        interpolator_type_linear interp;
    //        agg::span_image_filter_gray_bilinear< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp );
    //        filter.generate( span, x, y, len );
    //    }
    //};

    ///** @class ImageFilterGrayBilinearClip
    //*/
    //template < class PixFmt >
    //class ImageFilterGrayBilinearClip : public ImageFilter<PixFmt>
    //{
    //private:
    //    color_type  mBackColor;

    //public:
    //    ImageFilterGrayBilinearClip( ImageAccessorPtr imgAccessor, const color_type& backColor )
    //        : ImageFilter<PixFmt>( imgAccessor ),
    //          mBackColor( backColor )
    //    {
    //        mFilterName = "ImageFilterGrayBilinearClip";
    //    }

    //    void generate(color_type* span, int x, int y, unsigned len)
    //    {
    //        interpolator_type_linear interp;
    //        agg::span_image_filter_gray_bilinear_clip< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), mBackColor, interp );
    //        filter.generate( span, x, y, len );
    //    }
    //};

    ///** @class ImageFilterGrayNN
    //*/
    //template < class PixFmt >
    //class ImageFilterGrayNN : public ImageFilter<PixFmt>
    //{
    //public:
    //    ImageFilterGrayNN( ImageAccessorPtr imgAccessor )
    //        : ImageFilter<PixFmt>( imgAccessor )
    //    {
    //        mFilterName = "ImageFilterGrayNN";
    //    }

    //    void generate(color_type* span, int x, int y, unsigned len)
    //    {
    //        interpolator_type_linear interp;
    //        agg::span_image_filter_gray_nn< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp );
    //        filter.generate( span, x, y, len );
    //    }
    //};

    ////////////////////////////////////////////////////////////////////////////

    /** @class ImageFilterRGB
    */
    template < class PixFmt >
    class ImageFilterRGB : public ImageFilter<PixFmt>
    {
    private:
        const agg::image_filter_lut* mFilter;

    public:
        ImageFilterRGB( ImageAccessorPtr imgAccessor, const agg::trans_affine& mtx, const agg::image_filter_lut& filter )
            : ImageFilter<PixFmt>( imgAccessor, mtx ),
              mFilter( &filter )
        {
            mFilterName = "ImageFilterRGB";
        }

        void generate(color_type* span, int x, int y, unsigned len)
        {
            interpolator_type_linear interp( mMatrix );
            agg::span_image_filter_rgb< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp, *mFilter );
            filter.generate( span, x, y, len );
        }
    };

    /** @class ImageFilterRGB2x2
    */
    template < class PixFmt >
    class ImageFilterRGB2x2 : public ImageFilter<PixFmt>
    {
    private:
        const agg::image_filter_lut* mFilter;

    public:
        ImageFilterRGB2x2( ImageAccessorPtr imgAccessor, const agg::trans_affine& mtx, const agg::image_filter_lut& filter )
            : ImageFilter<PixFmt>( imgAccessor, mtx ),
              mFilter( &filter )
        {
            mFilterName = "ImageFilterRGB2x2";
        }

        void generate(color_type* span, int x, int y, unsigned len)
        {
            interpolator_type_linear interp( mMatrix );
            agg::span_image_filter_rgb_2x2< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp, *mFilter );
            filter.generate( span, x, y, len );
        }
    };

    ///** @class ImageFilterRGBAffine
    //*/
    //template < class PixFmt >
    //class ImageFilterRGBAffine : public ImageFilter<PixFmt>
    //{
    //private:
    //    const image_filter_lut* mFilter;

    //public:
    //    ImageFilterRGBAffine( ImageAccessorPtr imgAccessor, const image_filter_lut& filter )
    //        : ImageFilter<PixFmt>( imgAccessor ),
    //          mFilter( &filter )
    //    {
    //        mFilterName = "ImageFilterRGBAffine";
    //    }

    //    void generate(color_type* span, int x, int y, unsigned len)
    //    {
    //        interpolator_type_linear interp;
    //        agg::span_image_filter_rgb_affine< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp, *mFilter );
    //        filter.generate( span, x, y, len );
    //    }
    //};

    /** @class ImageFilterRGBBilinear
    */
    template < class PixFmt >
    class ImageFilterRGBBilinear : public ImageFilter<PixFmt>
    {
    public:
        ImageFilterRGBBilinear( ImageAccessorPtr imgAccessor, const agg::trans_affine& mtx )
            : ImageFilter<PixFmt>( imgAccessor, mtx )
        {
            mFilterName = "ImageFilterRGB2x2";
        }

        void generate(color_type* span, int x, int y, unsigned len)
        {
            interpolator_type_linear interp( mMatrix );
            agg::span_image_filter_rgb_bilinear< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp );
            filter.generate( span, x, y, len );
        }
    };

    ///** @class ImageFilterRGBBilinearClip
    //*/
    //template < class PixFmt >
    //class ImageFilterRGBBilinearClip : public ImageFilter<PixFmt>
    //{
    //private:
    //    color_type  mBackColor;

    //public:
    //    ImageFilterRGBBilinearClip( ImageAccessorPtr imgAccessor, const color_type& backColor )
    //        : ImageFilter<PixFmt>( imgAccessor ),
    //          mBackColor( backColor )
    //    {
    //        mFilterName = "ImageFilterRGBBilinearClip";
    //    }

    //    void generate(color_type* span, int x, int y, unsigned len)
    //    {
    //        interpolator_type_linear interp;
    //        agg::span_image_filter_rgb_bilinear_clip< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), mBackColor, interp );
    //        filter.generate( span, x, y, len );
    //    }
    //};

    /** @class ImageFilterRGBNN
    */
    template < class PixFmt >
    class ImageFilterRGBNN : public ImageFilter<PixFmt>
    {
    public:
        ImageFilterRGBNN( ImageAccessorPtr imgAccessor, const agg::trans_affine& mtx )
            : ImageFilter<PixFmt>( imgAccessor, mtx )
        {
            mFilterName = "ImageFilterRGBNN";
        }

        void generate(color_type* span, int x, int y, unsigned len)
        {
            interpolator_type_linear interp( mMatrix );
            agg::span_image_filter_rgb_nn< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp );
            filter.generate( span, x, y, len );
        }
    };

    ////////////////////////////////////////////////////////////////////////////

    /** @class ImageFilterRGBA
    */
    template < class PixFmt >
    class ImageFilterRGBA : public ImageFilter<PixFmt>
    {
    private:
        const agg::image_filter_lut* mFilter;

    public:
        ImageFilterRGBA( ImageAccessorPtr imgAccessor, const agg::trans_affine& mtx, const agg::image_filter_lut& filter )
            : ImageFilter<PixFmt>( imgAccessor, mtx ),
              mFilter( &filter )
        {
            mFilterName = "ImageFilterRGBA";
        }

        void generate(color_type* span, int x, int y, unsigned len)
        {
            interpolator_type_linear interp( mMatrix );
            agg::span_image_filter_rgba< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp, *mFilter );
            filter.generate( span, x, y, len );
        }
    };

    /** @class ImageFilterRGBA2x2
    */
    template < class PixFmt >
    class ImageFilterRGBA2x2 : public ImageFilter<PixFmt>
    {
    private:
        const agg::image_filter_lut* mFilter;

    public:
        ImageFilterRGBA2x2( ImageAccessorPtr imgAccessor, const agg::trans_affine& mtx, const agg::image_filter_lut& filter )
            : ImageFilter<PixFmt>( imgAccessor, mtx ),
              mFilter( &filter )
        {
            mFilterName = "ImageFilterRGBA2x2";
        }

        void generate(color_type* span, int x, int y, unsigned len)
        {
            interpolator_type_linear interp( mMatrix );
            agg::span_image_filter_rgba_2x2< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp, *mFilter );
            filter.generate( span, x, y, len );
        }
    };

    ///** @class ImageFilterRGBAAffine
    //*/
    //template < class PixFmt >
    //class ImageFilterRGBAAffine : public ImageFilter<PixFmt>
    //{
    //private:
    //    const image_filter_lut* mFilter;

    //public:
    //    ImageFilterRGBAAffine( ImageAccessorPtr imgAccessor, const image_filter_lut& filter )
    //        : ImageFilter<PixFmt>( imgAccessor ),
    //          mFilter( &filter )
    //    {
    //        mFilterName = "ImageFilterRGBAAffine";
    //    }

    //    void generate(color_type* span, int x, int y, unsigned len)
    //    {
    //        interpolator_type_linear interp;
    //        agg::span_image_filter_rgba_affine< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp, *mFilter );
    //        filter.generate( span, x, y, len );
    //    }
    //};

    /** @class ImageFilterRGBABilinear
    */
    template < class PixFmt >
    class ImageFilterRGBABilinear : public ImageFilter<PixFmt>
    {
    public:
        ImageFilterRGBABilinear( ImageAccessorPtr imgAccessor, const agg::trans_affine& mtx )
            : ImageFilter<PixFmt>( imgAccessor, mtx )
        {
            mFilterName = "ImageFilterRGBABilinear";
        }

        void generate(color_type* span, int x, int y, unsigned len)
        {
            interpolator_type_linear interp( mMatrix );
            agg::span_image_filter_rgba_bilinear< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp );
            filter.generate( span, x, y, len );
        }
    };

    ///** @class ImageFilterRGBABilinearClip
    //*/
    //template < class PixFmt >
    //class ImageFilterRGBABilinearClip : public ImageFilter<PixFmt>
    //{
    //private:
    //    color_type  mBackColor;

    //public:
    //    ImageFilterRGBABilinearClip( ImageAccessorPtr imgAccessor, const color_type& backColor )
    //        : ImageFilter<PixFmt>( imgAccessor ),
    //          mBackColor( backColor )
    //    {
    //        mFilterName = "ImageFilterRGBABilinearClip";
    //    }

    //    void generate(color_type* span, int x, int y, unsigned len)
    //    {
    //        interpolator_type_linear interp;
    //        agg::span_image_filter_rgba_bilinear_clip< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), mBackColor, interp );
    //        filter.generate( span, x, y, len );
    //    }
    //};

    /** @class ImageFilterRGBANN
    */
    template < class PixFmt >
    class ImageFilterRGBANN : public ImageFilter<PixFmt>
    {
    public:
        ImageFilterRGBANN( ImageAccessorPtr imgAccessor, const agg::trans_affine& mtx )
            : ImageFilter<PixFmt>( imgAccessor, mtx )
        {
            mFilterName = "ImageFilterRGBANN";
        }

        void generate(color_type* span, int x, int y, unsigned len)
        {
            interpolator_type_linear interp( mMatrix );
            agg::span_image_filter_rgba_nn< ImageAccessor<PixFmt>, interpolator_type_linear >  filter( *(mImageAccessor.get()), interp );
            filter.generate( span, x, y, len );
        }
    };

    ////////////////////////////////////////////////////////////////////////////

    /** @class ImageStyleHandler
        StyleHandler implementation for images, used as textures.
    */
    template < class PixFmt >
    class ImageStyleHandler : public StyleHandler
    {
    public:
        typedef agg::span_interpolator_linear<> interpolator_type;
        typedef AggOO::SharedPtr< ImageAccessor<PixFmt> >   ImageAccessorPtr;
        typedef AggOO::SharedPtr< ImageFilter<PixFmt> >     ImageFilterPtr;
        typedef typename PixFmt::color_type color_type;

        enum WrapMode   { WrapTile, WrapClamp, WrapFlipX, WrapFlipY, WrapFlipXY };
        enum FilterStyle {  StyleRGB, StyleRGB2x2, StyleRGBBilinear, StyleRGBBilinearClip, StyleRGBNN,
                            StyleRGBA, StyleRGBA2x2, StyleRGBABilinear, StyleRGBABilinearClip, StyleRGBANN };
        //enum FilterStyle {  StyleGray, StyleGray2x2, StyleGrayBilinear, StyleGrayBilinearClip, StyleGrayNN,
        //                    StyleRGB, StyleRGB2x2, StyleRGBBilinear, StyleRGBBilinearClip, StyleRGBNN,
        //                    StyleRGBA, StyleRGBA2x2, StyleRGBABilinear, StyleRGBABilinearClip, StyleRGBANN };

        ImageFilterPtr      mImageFilter;
        color_type          mTransparent;
        //Color   mTransparent;

        /** Create the style handler */
        void createHandler( const PixFmt& pixf, const agg::trans_affine& mtx, FilterStyle filterStyle, WrapMode wrapMode, const color_type& transColor = agg::rgba8( 0, 0, 0, 0 ) )
        {
            ImageAccessorPtr    accessorPtr;
            switch ( wrapMode )
            {
            case WrapTile:
                accessorPtr = ImageAccessorPtr( new ImageTileAccessor< PixFmt >( pixf ) );
                break;

            case WrapFlipX:
                accessorPtr = ImageAccessorPtr( new ImageFlipXAccessor< PixFmt >( pixf ) );
                break;

            case WrapFlipY:
                accessorPtr = ImageAccessorPtr( new ImageFlipYAccessor< PixFmt >( pixf ) );
                break;

            case WrapFlipXY:
                accessorPtr = ImageAccessorPtr( new ImageFlipXYAccessor< PixFmt >( pixf ) );
                break;

            default:
            case WrapClamp:
                accessorPtr = ImageAccessorPtr( new ImageClampAccessor< PixFmt >( pixf, mTransparent ) );
                break;
            }

            ImageFilterPtr filterPtr;
            switch ( filterStyle )
            {
            //case StyleGray:
            //    //filterPtr = ImageFilterPtr( new ImageFilterGray< PixFmt >( accessorPtr
            //    break;

            //case StyleGray2x2:
            //    break;

            //case StyleGrayNN:
            //    filterPtr = ImageFilterPtr( new ImageFilterGrayNN< PixFmt >( accessorPtr ) );
            //    break;

            //case StyleGrayBilinear:
            //    filterPtr = ImageFilterPtr( new ImageFilterGrayBilinear< PixFmt >( accessorPtr ) );
            //    break;

            //case StyleGrayBilinearClip:
            //    filterPtr = ImageFilterPtr( new ImageFilterGrayBilinearClip< PixFmt >( accessorPtr, mTransparent ) );
            //    break;

            case StyleRGB:
                break;

            case StyleRGB2x2:
                break;

            case StyleRGBNN:
                //filterPtr = ImageFilterPtr( new ImageFilterRGBNN< PixFmt >( accessorPtr, mtx ) );
                //filterPtr = ImageFilterPtr( new ImageFilterRGBNN< agg::pixfmt_rgb24 >( accessorPtr, mtx ) );
                break;

            case StyleRGBBilinear:
                //filterPtr = ImageFilterPtr( new ImageFilterRGBBilinear< PixFmt >( accessorPtr, mtx ) );
                //filterPtr = ImageFilterPtr( new ImageFilterRGBBilinear< agg::pixfmt_rgb24 >( accessorPtr, mtx ) );
                break;

            //case StyleRGBBilinearClip:
            //    filterPtr = ImageFilterPtr( new ImageFilterRGBBilinearClip< PixFmt >( accessorPtr, mTransparent ) );
            //    break;

            case StyleRGBA:
                break;

            case StyleRGBA2x2:
                break;

            case StyleRGBANN:
                filterPtr = ImageFilterPtr( new ImageFilterRGBANN< PixFmt >( accessorPtr, mtx ) );
                //filterPtr = ImageFilterPtr( new ImageFilterRGBANN< agg::pixfmt_rgba32 >( accessorPtr, mtx ) );
                break;

            case StyleRGBABilinear:
                //filterPtr = ImageFilterPtr( new ImageFilterRGBABilinear< PixFmt >( accessorPtr, mtx ) );
                //filterPtr = ImageFilterPtr( new ImageFilterRGBABilinear< agg::pixfmt_rgba32 >( accessorPtr, mtx ) );
                break;

            //case StyleRGBABilinearClip:
            //    filterPtr = ImageFilterPtr( new ImageFilterRGBABilinearClip< PixFmt >( accessorPtr, mTransparent ) );
            //    break;
            }

            mImageFilter = filterPtr;
        }

    public:
        ImageStyleHandler() : StyleHandler( false ) { }
        ImageStyleHandler( const PixFmt& pixf, const agg::trans_affine& mtx, FilterStyle filterStyle, WrapMode wrapMode, const color_type& transColor = agg::rgba8( 0, 0, 0, 0 ) )
            : StyleHandler( false ),
              mTransparent( transColor )
        {
            createHandler( pixf, mtx, filterStyle, wrapMode, transColor );
        }

        virtual StyleHandler* Clone() const
        {
            ImageStyleHandler* handler = new ImageStyleHandler();
            handler->mTransparent = mTransparent;
            handler->mImageFilter = mImageFilter;
            return handler;
        }

        void generate_span( color_type* span, int x, int y, unsigned len )
        {
            mImageFilter->generate( span, x, y, len );
        }
    };

} // namespace AggOO

#endif  // AGGOO_STYLE_HANDLER_IMAGE_H_
