
/*! $Id: AggOOStyleHandler.h,v 1.5 2009/01/15 15:44:49 dratek Exp $
 *  @file   AggOOStyleHandler.h
 *  @author Chad M. Draper
 *  @date   January 16, 2008
 *
 *  Contains the definition of the style handler base class, used for compound
 *  rendering, as well as the solid style handler, which is commonly used.
 *  
 */

#ifndef AGGOO_STYLE_HANDLER_H_
#define AGGOO_STYLE_HANDLER_H_

#include "agg_basics.h"
#include "agg_pixfmt_rgba.h"
#include "agg_pixfmt_gray.h"
#include "agg_span_allocator.h"
#include "agg_span_solid.h"
#include "agg_color_rgba.h"

#include "AggOOSharedPtr.h"
#include <vector>

namespace AggOO
{
    /** @class StyleHandler
        Base class for style handlers.  All custom styles should derive from
        here and overload the appropriate method (either color() or
        generate_span(...), depending on how the style is to be rendered.
    */
    //template < typename ColorT >
    class StyleHandler
    {
    public:
        /** Constructor
            @param  solid       Indicates if the style is a solid color
        */
        StyleHandler( bool solid )
            : mIsSolid( solid )
        {
        }

        virtual StyleHandler* Clone() const = 0;

        bool    is_solid() const    { return mIsSolid; }

        /** Get the single color used by this style (solid color) */
        template < typename ColorT >
        const ColorT& color() const
        {
            ColorT col;
            GetColor( col );
            return col;
        }

        virtual void GetColor( agg::rgba8& col ) const { }
        virtual void GetColor( agg::rgba16& col ) const { }
        virtual void GetColor( agg::gray8& col ) const { }
        virtual void GetColor( agg::gray16& col ) const { }

        //virtual const agg::rgba& color() const { return agg::rgba( 0, 0, 0, 0 ); }
        //virtual const agg::rgba& color() const
        //{
        //    return mTransparent;
        //}
        //template < typename ColorT >
        //void color( ColorT& col ) const { return agg::rgba( 0, 0, 0, 0 ); }

        virtual void prepare() { }

        /** Generate a span line
            @param  span    [out] Row of generated pixels
            @param  x       [in]  Horizontal start of the span line
            @param  y       [in]  Vertical start of the span line
            @param  len     [in]  Length of the span
        */
        virtual void generate_span( agg::gray8* span, int x, int y, unsigned len )  { }
        virtual void generate_span( agg::gray16* span, int x, int y, unsigned len ) { }
        virtual void generate_span( agg::rgba8* span, int x, int y, unsigned len )  { }
        virtual void generate_span( agg::rgba16* span, int x, int y, unsigned len ) { }

        /** Generate a span line
            @param  span    [out] Row of generated pixels
            @param  x       [in]  Horizontal start of the span line
            @param  y       [in]  Vertical start of the span line
            @param  len     [in]  Length of the span
        */
        virtual void generate( agg::gray8* span, int x, int y, unsigned len )  { }
        virtual void generate( agg::gray16* span, int x, int y, unsigned len ) { }
        virtual void generate( agg::rgba8* span, int x, int y, unsigned len )  { }
        virtual void generate( agg::rgba16* span, int x, int y, unsigned len ) { }

    protected:
        bool                mIsSolid;       /**< Indicates if the style is a solid color.  This is an optimization,
                                                 and provides little other use (generate_span could do the same
                                                 thing, but would be overkill.)
                                            */
    };

    /** @class StyleCollection
        Essentially holds multiple StyleHandlers.  The compound renderers use
        multiple style handlers, and this is just an interface to allowing them.
        By using the AddStyle method, pointers to differing styles can be added.
    */
    //template < typename ColorT >
    class StyleCollection
    {
    public:
        StyleCollection() { }

        /** Add a pointer to a new style handler
            @param  style   Pointer to add to the list
            @return true if the pointer was added, false otherwise.
        */
        bool AddStyle( StyleHandler* style )
        {
            unsigned origCount = mStyles.size();
            mStyles.push_back( StylePtr( style->Clone() ) );
            return ( origCount != mStyles.size() );
        }

        /** Find out if a style is solid
            @param  style   Index of the style to check
            @return true if the style is solid, false otherwise.
        */
        bool is_solid( unsigned style ) const
        {
            if ( style < count() )
                return mStyles.at( style )->is_solid();
            return true;
        }

        /** Get the number of styles in the collection */
        unsigned count() const                  { return mStyles.size(); }

        /** Get the color of a given style
            @param  style   Index of the style to use
            @return Color of the style.
        */
        template < typename ColorT >
        const ColorT& color( unsigned style ) const
        {
            if ( style < count() )
                return mStyles.at( style )->color<ColorT>();
            return agg::rgba( 0, 0, 0, 0 );
        }

        /** Generate the span line with a given style
            @param  span    [out] Row of generated pixels
            @param  x       [in]  Horizontal start of the span line
            @param  y       [in]  Vertical start of the span line
            @param  len     [in]  Length of the span
            @param  style   [in]  Index of the style to use
        */
        template < typename ColorT >
        void generate_span( ColorT* span, int x, int y, unsigned len, unsigned style )
        {
            if ( style < count() )
                mStyles.at( style )->generate_span( span, x, y, len );
        }

    private:
        typedef AggOO::SharedPtr< StyleHandler > StylePtr;
        std::vector< StylePtr > mStyles;        /**< List of style handlers in the collection */
    };

    /** @class SolidStyleHandler
        Style handler for solid colors.  This is common.
    */
    //template < typename ColorT >
    class SolidStyleHandler : public StyleHandler
    {
    public:
        typedef agg::rgba8 ColorT;

        SolidStyleHandler( const ColorT& col, const ColorT& transColor = agg::rgba( 0, 0, 0, 0 ) )
            : StyleHandler( true ),
              mTransparent( transColor ),
              mColor( col )
        {
            mSpanGen.color( mColor );
        }

        ////template < typename ColorU >
        //const ColorT& color() const
        //{
        //    return mColor;
        //}

        virtual void GetColor( ColorT& col ) const
        {
            col = mColor;
        }

        virtual StyleHandler* Clone() const
        {
            return new SolidStyleHandler( mColor, mTransparent );
        }

        /** Generate a span line
            @param  span    [out] Row of generated pixels
            @param  x       [in]  Horizontal start of the span line
            @param  y       [in]  Vertical start of the span line
            @param  len     [in]  Length of the span
        */
        virtual void generate( ColorT* span, int x, int y, unsigned len )
        {
            mSpanGen.generate( span, x, y, len );
        }

    private:
        ColorT  mColor;
        ColorT  mTransparent;
        agg::span_solid< ColorT >   mSpanGen;
    };

} // namespace AggOO

#endif  // AGGOO_STYLE_HANDLER_H_
