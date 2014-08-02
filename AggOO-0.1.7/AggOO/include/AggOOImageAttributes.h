
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
// $Id: AggOOImageAttributes.h,v 1.2 2009/03/13 23:00:28 dratek Exp $
//
// AggOOImageAttributes.h -- Modifies pixel values when rendering an image.
//
// Author       : Chad M. Draper
// Date         : August 30, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2007-08-30      Initial implementation
//

#ifndef AggOOImageAttributes_H_
#define AggOOImageAttributes_H_

#include "AggOOBase.h"
#include "AggOOColorMatrix.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"

#include <map>

namespace AggOO
{
    class ImageAttributes : public Base
    {
    private:
        /** The ImageAttributes maintains five color adjustment categories:
            default, bitmap, brush, pen, and text.  This means a separate matrix
            and other adjustment values for each category, so use a class to
            represent the category.
        */
        struct AdjustmentCategory
        {
            bool    enabled;
            bool    specified;
            REAL    threshold;
            bool    enableThreshold;
            REAL    gamma;
            bool    enableGamma;
            ColorMatrix* colorMatrix;
            bool    enableColorMatrix;
            ColorMatrix* grayMatrix;
            bool    enableGrayMatrix;

            Color   lowKey, highKey;
            bool    enableColorKey;

            std::map< std::string, ColorMap > colorMap;
            typedef std::map< std::string, ColorMap >::const_iterator ColorMapIter_const;
            typedef std::map< std::string, ColorMap >::iterator ColorMapIter;
            bool    enableColorMap;

            AdjustmentCategory()
                : enabled( true ),
                  specified( false ),
                  threshold( 0.0 ),
                  enableThreshold( false ),
                  gamma( 0.0 ),
                  enableGamma( false ),
                  colorMatrix( NULL ),
                  enableColorMatrix( false ),
                  grayMatrix( NULL ),
                  enableGrayMatrix( false ),
                  enableColorKey( false ),
                  enableColorMap( false )
            {
            }

            // Create a copy of the AdjustmentCategory
            AdjustmentCategory& operator=( const AdjustmentCategory& src )
            {
                enabled         = src.enabled;
                specified       = src.specified;
                threshold       = src.threshold;
                enableThreshold = src.enableThreshold;
                gamma           = src.gamma;
                enableGamma     = src.enableGamma;
                colorMatrix     = src.colorMatrix;
                enableColorMatrix = src.enableColorMatrix;
                grayMatrix      = src.grayMatrix;
                enableGrayMatrix = src.enableGrayMatrix;
                lowKey          = src.lowKey;
                highKey         = src.highKey;
                enableColorKey  = src.enableColorKey;
                colorMap.clear();
                colorMap.insert( src.colorMap.begin(), src.colorMap.end() );
                enableColorMap  = src.enableColorMap;

                return *this;
            }

            ~AdjustmentCategory()
            {
                delete colorMatrix;
                delete grayMatrix;
            }

            void Clear()
            {
                // Disable all settings:
                enableThreshold = false;
                enableGamma     = false;
                enableColorMatrix = false;
                enableGrayMatrix = false;
                enableColorKey  = false;
                enableColorMap  = false;
            }

            // Set the color remap table
            void SetRemapTable( UINT mapSize, const ColorMap* map )
            {
                // Go through the source map.  Create an index string from the
                // 'old' color, and add to the color map.
                while ( --mapSize )
                {
                    std::string index = map[ mapSize ].oldColor.GetColorString();
                    colorMap[ index ] = map[ mapSize ];
                }
            }

            // Adjust a color using the adjustment settings
            void Adjust( unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a ) const
            {
                if ( specified && enabled )
                {
                    // Apply the color matrix:
                    if ( enableColorMatrix && colorMatrix )
                        ColorMatrixExt::Multiply( r, g, b, a, colorMatrix );

                    Color color( a, r, g, b );

                    // Check if this color is being remapped:
                    if ( enableColorMap )
                    {
                        // Attempt to find this color in the map
                        ColorMapIter_const iter = colorMap.find( color.GetColorString() );
                        if ( iter != colorMap.end() )
                            color = iter->second.newColor;
                    }

                    // If the color is in the color key range, then set the
                    // alpha to 0
                    if ( isColorKey( color ) )
                    {
                        color = Color( 0, color.GetR(), color.GetG(), color.GetB() );
                    }
                }
            }

        private:
            // Determine if a color lies within the color key range
            bool isColorKey( const Color& color ) const
            {
                BYTE r = color.GetR();
                BYTE g = color.GetG();
                BYTE b = color.GetB();
                BYTE a = color.GetA();
                if ( r >= lowKey.GetR() && r <= highKey.GetR() )
                {
                    if ( g >= lowKey.GetG() && g <= highKey.GetG() )
                        if ( b >= lowKey.GetB() && b <= highKey.GetB() )
                            return true;
                }
                return false;
            }
        };

    protected:
        AdjustmentCategory  mCategories[ ColorAdjustTypeCount ];
        WrapMode    mWrapMode;
        Color       mWrapColor;

        // Avoid public copying
        ImageAttributes( const ImageAttributes& attribs );
        ImageAttributes& operator=( const ImageAttributes& attribs );

        mutable Status  mLastStatus;

        /** Set the status of the pen
        */
        Status SetStatus( Status status ) const
        {
            if ( status != Ok )
                return ( mLastStatus = status );
            return status;
        }

    public:
        Status GetLastStatus() const
        {
            return mLastStatus;
        }

    public:
        ImageAttributes(void);
        ~ImageAttributes(void);

        /** Create a clone of this object */
        ImageAttributes* Clone() const;

        /** Reset the matrix to the identity
            @param  type        Specifies the category for which the matrix is
                                set to identity. (default=ColorAdjustTypeDefault)
        */
        Status  SetToIdentity( ColorAdjustType type = ColorAdjustTypeDefault );

        /** Reset all color and grayscale settings
            @param  type        Specifies the category for which the matrix is
                                set to identity. (default=ColorAdjustTypeDefault)
        */
        Status  Reset( ColorAdjustType type = ColorAdjustTypeDefault );

        /** Set the color matrix
            @param  colorMatrix     Pointer to the new matrix
            @param  mode            Specifies the type of image and color to
                                    apply the matrix to
            @param  type            Specifies the category to which to apply the
                                    matrix. (default=ColorAdjustTypeDefault)
        */
        Status  SetColorMatrix( const ColorMatrix* colorMatrix,
                                ColorMatrixFlags mode = ColorMatrixFlagsDefault,
                                ColorAdjustType type = ColorAdjustTypeDefault );

        /** Clear the color matrix
            @param  type        Specifies the category for which the matrix is
                                cleared. (default=ColorAdjustTypeDefault)
        */
        Status  ClearColorMatrix( ColorAdjustType type = ColorAdjustTypeDefault );

        /** Set the color matrix
            @param  colorMatrix     Pointer to the new color matrix
            @param  grayMatrix      Pointer to the new grayscale matrix
            @param  mode            Specifies the type of image and color to
                                    apply the matrix to
            @param  type            Specifies the category to which to apply the
                                    matrix. (default=ColorAdjustTypeDefault)
        */
        Status  SetColorMatrices( const ColorMatrix* colorMatrix,
                                const ColorMatrix* grayMatrix,
                                ColorMatrixFlags mode = ColorMatrixFlagsDefault,
                                ColorAdjustType type = ColorAdjustTypeDefault );

        /** Clear the color matrix
            @param  type        Specifies the category for which the matrix is
                                cleared. (default=ColorAdjustTypeDefault)
        */
        Status  ClearColorMatrices( ColorAdjustType type = ColorAdjustTypeDefault );

        /** Set the threshold for a specified category
            @param  threshold   Threshold value for the specified category
            @param  type        Category to which this threshold is applied.
                                (default=ColorAdjustTypeDefault)
        */
        Status  SetThreshold( REAL threshold, ColorAdjustType type = ColorAdjustTypeDefault )
        {
            mCategories[ type ].specified = true;
            mCategories[ type ].enableThreshold = true;
            mCategories[ type ].threshold = threshold;
        }

        /** Clear the threshold for a specified type
            @param  type        Category to which the threshold is cleared.
                                (default=ColorAdjustTypeDefault)
        */
        Status ClearThreshold( ColorAdjustType type = ColorAdjustTypeDefault )
        {
            mCategories[ type ].specified = true;
            mCategories[ type ].enableThreshold = false;
            //mCategories[ type ].threshold = 1.0;
        }

        /** Set the gamma for a specified category
            @param  gamma       Gamma value for the specified category
            @param  type        Category to which this threshold is applied.
                                (default=ColorAdjustTypeDefault)
        */
        Status SetGamma( REAL gamma, ColorAdjustType type = ColorAdjustTypeDefault )
        {
            mCategories[ type ].specified = true;
            mCategories[ type ].enableGamma = true;
            mCategories[ type ].gamma = gamma;
        }

        /** Clear the gamma for a specified type
            @param  type        Category to which the gamma is cleared.
                                (default=ColorAdjustTypeDefault)
        */
        Status ClearGamma( ColorAdjustType type = ColorAdjustTypeDefault )
        {
            mCategories[ type ].specified = true;
            mCategories[ type ].enableGamma = false;
            //mCategories[ type ].gamma = 0.0;
        }

        /** Disable color adjustment for a specified category
            @param  type        Category for which color adjustment is disabled
        */
        Status  SetNoOp( ColorAdjustType type = ColorAdjustTypeDefault )
        {
            mCategories[ type ].specified = true;
            mCategories[ type ].enabled = false;
        }

        /** Restore color adjustment for a specified category
            @param  type        Category for which color adjustment is restored
        */
        Status  ClearNoOp( ColorAdjustType type = ColorAdjustTypeDefault )
        {
            mCategories[ type ].specified = true;
            mCategories[ type ].enabled = true;
        }

        /** Set the range of colors to use as a color key.  If a color falls
            within this range, it is treated as transparent.
            @param  colorLow    Minimum color key value
            @param  colorHigh   Maximum color key value
            @param  type        Category for which color key is assigned
        */
        Status SetColorKey( const Color& colorLow, const Color& colorHigh, ColorAdjustType type = ColorAdjustTypeDefault )
        {
            mCategories[ type ].specified = true;
            mCategories[ type ].enableColorKey = true;
            mCategories[ type ].lowKey  = colorLow;
            mCategories[ type ].highKey = colorHigh;
        }

        /** Disable the color key for a specified type
            @param  type        Category for which color key is disabled
        */
        Status ClearColorKey( ColorAdjustType type = ColorAdjustTypeDefault )
        {
            mCategories[ type ].specified = true;
            mCategories[ type ].enableColorKey = false;
        }

        //Status SetOutputChannel( ColorChannelFlags channelFlags, ColorAdjustType type = ColorAdjustTypeDefault );
        //Status ClearOutputChannel( ColorAdjustType type = ColorAdjustTypeDefault );

        //Status SetOutputChannelColorProfile( const WCHAR *colorProfileFilename, ColorAdjustType type = ColorAdjustTypeDefault );
        //Status ClearOutputChannelColorProfile( ColorAdjustType type = ColorAdjustTypeDefault );

        //// The flags of the palette are ignored.
        //Status GetAdjustedPalette( ColorPalette* colorPalette, ColorAdjustType colorAdjustType ) const;

        /** Specify a color map used to replace one color with another.
            @param  mapSize     Number of colors in the map
            @param  map         Pairs of colors (old color and new color) which define the map
            @param  type        Category for which color map is defined
        */
        Status SetRemapTable( UINT mapSize, const ColorMap *map, ColorAdjustType type = ColorAdjustTypeDefault );

        /** Disable the color map
            @param  type        Category for which color map is disabled
        */
        Status ClearRemapTable( ColorAdjustType type = ColorAdjustTypeDefault )
        {
            mCategories[ type ].specified = true;
            mCategories[ type ].enableColorMap = false;
        }

        /** Specify a color map for the brush category
            @param  mapSize     Number of colors in the map
            @param  map         Pairs of colors (old color and new color) which define the map
        */
        Status SetBrushRemapTable( UINT mapSize, const ColorMap *map );

        /** Disable the color map for the brush category */
        Status ClearBrushRemapTable()
        {
            mCategories[ ColorAdjustTypeBrush ].specified = true;
            mCategories[ ColorAdjustTypeBrush ].enableColorMap = true;
        }

        /** Set the wrapping mode.
            @param  wrap        Wrap mode
            @param  color       Color used if the image is not as big as the
                                destination area (default is transparent)
            @param  clamp       Has no use in GDI+ 1.0, so it has no use here
                                (will GDI+ ever evolve? Who know.)
        */
        Status SetWrapMode( WrapMode wrap, const Color& color = Color::Transparent, BOOL clamp = FALSE )
        {
            mWrapMode   = wrap;
            mWrapColor  = color;
            return SetStatus( Ok );
        }



        /** Transform individual RGB components */
        void Transform( unsigned char& r, unsigned char& g, unsigned char& b ) const;
        /** Transform individual RGBA components */
        void Transform( unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a ) const;
    };

    /** Span converter to apply ImageAttributes to a rgb image */
    class span_conv_image_attrib_rgb
    {
    public:
        typedef agg::rgba8 color_type;

        span_conv_image_attrib_rgb( ImageAttributes* attribs )
            : mAttribs( attribs )
        {
        }

        void prepare()  { }

        /** Do the actual work.  Go over a span line, applying the image
            attributes to each pixel.
        */
        void generate( color_type* span, int x, int y, unsigned len ) const
        {
            if ( mAttribs )
            {
                do
                {
                    // Apply the attributes:
                    mAttribs->Transform( span->r, span->g, span->b, span->a );
                    span->premultiply();

                    ++span;
                } while ( --len );
            }
        }

    private:
        ImageAttributes* mAttribs;
    };

    /** Span converter to apply ImageAttributes to a rgba image */
    class span_conv_image_attrib_rgba
    {
    public:
        typedef agg::rgba8 color_type;

        span_conv_image_attrib_rgba( ImageAttributes* attribs )
            : mAttribs( attribs )
        {
        }

        void prepare()  { }

        /** Do the actual work.  Go over a span line, applying the image
            attributes to each pixel.
        */
        void generate( color_type* span, int x, int y, unsigned len ) const
        {
            if ( mAttribs )
            {
                do
                {
                    // Apply the attributes:
                    mAttribs->Transform( span->r, span->g, span->b, span->a );

                    ++span;
                } while ( --len );
            }
        }

    private:
        ImageAttributes* mAttribs;
    };
}

#endif  // AggOOImageAttributes_H_
