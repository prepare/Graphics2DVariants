
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
// $Id: AggOOImageAttributes.cpp,v 1.1 2007/09/06 21:50:48 dratek Exp $
//
// AggOOImageAttributes.h -- Modifies pixel values when rendering an image.
//

#include "AggOOImageAttributes.h"

namespace AggOO
{
    ImageAttributes::ImageAttributes(void)
    {
        SetStatus( Ok );

        // All color adjustment categories are unspecified initially, except the
        // default.
        mCategories[ ColorAdjustTypeDefault ].specified = true;
    }

    ImageAttributes::ImageAttributes( const ImageAttributes& attribs )
    {
        *this = attribs;
    }

    ImageAttributes& ImageAttributes::operator=( const ImageAttributes& attribs )
    {
        for ( size_t i = 0; i < ColorAdjustTypeCount; i++ )
            mCategories[ i ] = attribs.mCategories[ i ];
        mWrapMode   = attribs.mWrapMode;
        mWrapColor  = attribs.mWrapColor;
        return *this;
    }

    ImageAttributes::~ImageAttributes(void)
    {
    }

    //Clone---------------------------------------------------------------------
    ImageAttributes* ImageAttributes::Clone() const
    {
        return new ImageAttributes( *this );
    }

    //SetToIdentity-------------------------------------------------------------
    Status  ImageAttributes::SetToIdentity( ColorAdjustType type )
    {
        mCategories[ type ].specified = true;
        mCategories[ type ].enableColorMap = true;
        mCategories[ type ].colorMatrix = ColorMatrix::IDENTITY.Clone();
        return SetStatus( Ok );
    }

    //Reset---------------------------------------------------------------------
    Status  ImageAttributes::Reset( ColorAdjustType type )
    {
        mCategories[ type ].specified = true;
        mCategories[ type ].Clear();
        return SetStatus( Ok );
    }

    //SetColorMatrix------------------------------------------------------------
    Status  ImageAttributes::SetColorMatrix( const ColorMatrix* colorMatrix,
                            ColorMatrixFlags mode,
                            ColorAdjustType type )
    {
        mCategories[ type ].specified = true;
        mCategories[ type ].enableColorMatrix = true;
        mCategories[ type ].colorMatrix = colorMatrix->Clone();
        return SetStatus( Ok );
    }

    //ClearColorMatrix----------------------------------------------------------
    Status  ImageAttributes::ClearColorMatrix( ColorAdjustType type )
    {
        mCategories[ type ].specified = true;
        mCategories[ type ].enableColorMatrix = false;
        return SetStatus( Ok );
    }

    //SetColorMatrices----------------------------------------------------------
    Status  ImageAttributes::SetColorMatrices( const ColorMatrix* colorMatrix,
                            const ColorMatrix* grayMatrix,
                            ColorMatrixFlags mode,
                            ColorAdjustType type )
    {
        mCategories[ type ].specified = true;
        mCategories[ type ].colorMatrix = colorMatrix->Clone();
        mCategories[ type ].grayMatrix  = grayMatrix->Clone();
        mCategories[ type ].enableColorMatrix = true;
        mCategories[ type ].enableGrayMatrix  = true;
        return SetStatus( Ok );
    }

    Status  ImageAttributes::ClearColorMatrices( ColorAdjustType type )
    {
        mCategories[ type ].specified = true;
        mCategories[ type ].enableColorMatrix = false;
        mCategories[ type ].enableGrayMatrix = false;
        return SetStatus( Ok );
    }

    //SetRemapTable-------------------------------------------------------------
    Status ImageAttributes::SetRemapTable( UINT mapSize, const ColorMap *map, ColorAdjustType type )
    {
        mCategories[ type ].specified = true;
        mCategories[ type ].enableColorMap = true;
        mCategories[ type ].SetRemapTable( mapSize, map );
        return SetStatus( Ok );
    }

    Status ImageAttributes::SetBrushRemapTable( UINT mapSize, const ColorMap *map )
    {
        mCategories[ ColorAdjustTypeBrush ].specified = true;
        mCategories[ ColorAdjustTypeBrush ].enableColorMap = true;
        mCategories[ ColorAdjustTypeBrush ].SetRemapTable( mapSize, map );
        return SetStatus( Ok );
    }



    //Transform-----------------------------------------------------------------
    void ImageAttributes::Transform( unsigned char& r, unsigned char& g, unsigned char& b ) const
    {
        unsigned char a;
        mCategories[ ColorAdjustTypeDefault ].Adjust( r, g, b, a );
        //r = 100;
        //g = 200;
        //b = 50;
    }

    //Transform-----------------------------------------------------------------
    void ImageAttributes::Transform( unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a ) const
    {
        mCategories[ ColorAdjustTypeDefault ].Adjust( r, g, b, a );
        //r = 50;
        //g = 100;
        //b = 150;
        //a = 200;
    }

}
