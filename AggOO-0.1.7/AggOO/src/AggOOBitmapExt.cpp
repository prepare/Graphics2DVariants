
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
// $Id: AggOOBitmapExt.cpp,v 1.4 2007/10/03 14:13:50 dratek Exp $
//
// AggOOBitmapExt.cpp
//

#include "AggOOBitmapExt.h"
#include "AggOOColorExt.h"
#include "AggOOGraphics.h"

//#include <cmd/LogFileManager.h>
//using cmd::LogFileManager;

namespace AggOO
{
    BitmapExt::BitmapExt( INT width, INT height, PixelFormat format )
        : Bitmap(width, height, format)
    {
    }
    BitmapExt::BitmapExt( INT width, INT height, INT stride, PixelFormat format, BYTE *scan0 )
        : Bitmap( width, height, stride, format, scan0 )
    {
    }
    BitmapExt::BitmapExt( INT width, INT height, Graphics *target )
        : Bitmap(width, height, target)
    {
    }

    //Clear---------------------------------------------------------------------
    Status BitmapExt::Clear( const Color& bgCol )
    {
        Graphics gfx( this );
        gfx.Clear( bgCol );
        return SetStatus( Ok );
    }

    //InvertRGB-----------------------------------------------------------------
    Status BitmapExt::InvertRGB()
    {
        Status statusCode = Ok;

        // Lock the bitmap for reading and writing:
        int     bmpWidth    = GetWidth();
        int     bmpHeight   = GetHeight();
        Rect    bmpRect( 0, 0, bmpWidth, bmpHeight );
        PixelFormat pixelFormat = GetPixelFormat();
        BitmapData bmpData;
        statusCode = LockBits( \
                        &bmpRect, \
                        ImageLockModeWrite | ImageLockModeRead, \
                        PixelFormat32bppARGB, \
                        &bmpData );
        if ( statusCode != Ok )
            return statusCode;

        // Go through the bitmap and invert the colors:
        BYTE *pixelData = (BYTE*)bmpData.Scan0;
        for ( int y = 0; y < bmpHeight; y++ )
        {
            BYTE *curRow = &pixelData[ y * bmpData.Stride ];

            for ( int x = 0; x < bmpWidth; x++  )
            {
                // Skip the alpha:
                curRow++;

                // Invert the RGB values (leave the alpha alone):
                *curRow = 255 - *curRow;
                curRow++;

                *curRow = 255 - *curRow;
                curRow++;

                *curRow = 255 - *curRow;
                curRow++;
            }
        }

        // Unlock the bits:
        statusCode = UnlockBits( &bmpData );

        return SetStatus( statusCode );
    }

    //InvertRGBA----------------------------------------------------------------
    Status BitmapExt::InvertRGBA()
    {
        Status statusCode = Ok;

        // Lock the bitmap for reading and writing:
        int     bmpWidth    = GetWidth();
        int     bmpHeight   = GetHeight();
        Rect    bmpRect( 0, 0, bmpWidth, bmpHeight );
        PixelFormat pixelFormat = GetPixelFormat();
        BitmapData bmpData;
        statusCode = LockBits( \
                        &bmpRect, \
                        ImageLockModeWrite | ImageLockModeRead, \
                        PixelFormat32bppARGB, \
                        &bmpData );
        if ( statusCode != Ok )
            return statusCode;

        // Go through the bitmap and invert the colors:
        BYTE *pixelData = (BYTE*)bmpData.Scan0;
        for ( int y = 0; y < bmpHeight; y++ )
        {
            BYTE *curRow = &pixelData[ y * bmpData.Stride ];

            for ( int x = 0; x < bmpWidth; x++  )
            {
                // Invert the RGB values (leave the alpha alone):
                *curRow = 255 - *curRow;
                curRow++;

                *curRow = 255 - *curRow;
                curRow++;

                *curRow = 255 - *curRow;
                curRow++;

                *curRow = 255 - *curRow;
                curRow++;
            }
        }

        // Unlock the bits:
        statusCode = UnlockBits( &bmpData );

        return statusCode;
    }

    /////////////////////////////////////////////////////////////////
    // Mask a source bitmap with a mask bitmap and store the result here
    //
    // [in]     src                         Pointer to the source image
    // [in]     mask                        Pointer to the mask image
    // [in]     invertMask                  If true, then wherever the mask has alpha > 0,
    //                                      the source pixel is kept.  If false, keep the
    //                                      source pixel where alpha == 0.
    /////////////////////////////////////////////////////////////////
    Status BitmapExt::Mask( Bitmap *src, Bitmap *mask, BOOL invertMask )
    {
        return Mask( src, mask, 0, 0, src->GetWidth(), src->GetHeight(), invertMask );
    }
    Status BitmapExt::Mask( Bitmap *src, Bitmap *mask, unsigned int x, unsigned int y, unsigned int width, unsigned int height, BOOL invertMask )
    {
        if ( !src || !mask )
            return InvalidParameter;
        Status statusCode = Ok;

        // Make sure the mask region stays inside the buffers:
        if ( src->GetWidth() < x || src->GetHeight() < y )
            return InvalidParameter;
        if ( x < 0 )
        {
            width += x;
            x = 0;
        }
        if ( y < 0 )
        {
            height += y;
            y = 0;
        }
        width  = min( min( width, this->GetWidth() ), min( src->GetWidth() - x, mask->GetWidth() ) );
        height = min( min( height, this->GetHeight() ), min( src->GetHeight() - y, mask->GetHeight() ) );
        Rect    thisRect( x, y, width, height );
        Rect    srcRect( x, y, width, height );
        Rect    maskRect( 0, 0, width, height );

        // Lock the bitmaps:
        BitmapData  outBmpData, srcBmpData, maskBmpData;
        statusCode = this->LockBits(  &thisRect,\
                                ImageLockModeWrite,\
                                PixelFormat32bppARGB,
                                &outBmpData );
        if ( statusCode != Ok )
            return statusCode;

        statusCode = src->LockBits( &srcRect,\
                                ImageLockModeRead,\
                                PixelFormat32bppARGB,
                                &srcBmpData );
        if ( statusCode != Ok )
            return statusCode;

        statusCode = mask->LockBits( &maskRect,\
                                ImageLockModeRead,\
                                PixelFormat32bppARGB,
                                &maskBmpData );
        if ( statusCode != Ok )
            return statusCode;

        // Get the pixel data:
        DWORD *outPixels    = (DWORD*)outBmpData.Scan0;
        DWORD *srcPixels    = (DWORD*)srcBmpData.Scan0;
        DWORD *maskPixels   = (DWORD*)maskBmpData.Scan0;

        // Iterate over the bitmaps, masking the source into this bitmap:
        Color srcColor, maskColor, destColor;
        for ( int y = 0; y < height; y++ )
        {
            for ( int x = 0; x < width; x++ )
            {
                getPixelARGB( &maskBmpData, x, y, &maskColor );
                getPixelARGB( &srcBmpData, x, y, &srcColor );
                if ( srcColor.GetA() )
                {
                    double alphaMultiplier = maskColor.GetA() / 255.0;
                    if ( invertMask )   alphaMultiplier = 1.0 - alphaMultiplier;
                    destColor = Color( srcColor.GetA() * alphaMultiplier, srcColor.GetR(), srcColor.GetG(), srcColor.GetB() );
                }
                else
                    destColor = Color::Transparent;
                setPixelARGB( &outBmpData, x, y, destColor );
            }
        }

/*
        for ( int y = 0; y < height; y++ )
        {
            // Get the current rows:
            DWORD *outRow   = &outPixels[ y * outBmpData.Stride / 4 ];
            DWORD *srcRow   = &srcPixels[ y * srcBmpData.Stride / 4 ];
            DWORD *maskRow  = &maskPixels[ y * maskBmpData.Stride / 4 ];

            for ( int x = 0; x < width; x++ )
            {
                // Examine the alpha value of the mask to determine if the source pixel
                // should be copied to the output:
                BYTE maskAlpha = ColorExt::GetA( *maskRow );

//                if ( maskAlpha )
                {
                    // Copy the source to the output, multiplying by the
                    // mask's alpha value:
                    BYTE srcAlpha = ColorExt::GetA( *srcRow );
                    if ( srcAlpha )
                    {
                        BYTE srcRed   = ColorExt::GetR( *srcRow );
                        BYTE srcGreen = ColorExt::GetG( *srcRow );
                        BYTE srcBlue  = ColorExt::GetB( *srcRow );
Color srcColor( *srcRow );

                        // Copy the source to the output:
                        double alphaMultiplier = maskAlpha / 255.0;
                        if ( invertMask )   alphaMultiplier = 1.0 - alphaMultiplier;
                        srcAlpha *= alphaMultiplier;
                        *outRow = Color::MakeARGB( srcAlpha, srcRed, srcGreen, srcBlue );
Color outColor( *outRow );
LogFileManager::getInstance() << "point( " << x << ", " << y << " ), mask alpha = " << (int)maskAlpha << ", srcAlpha = " << (int)srcAlpha << ", src Red = " << (int)srcRed << ", src Green = " << (int)srcGreen << ", src Blue = " << (int)srcBlue << ", src = " << srcColor << ", out = " << outColor << std::endl;
                    }
                }

                outRow++;
                srcRow++;
                maskRow++;
            }
        }
*/

        // Unlock the bitmaps:
        statusCode = mask->UnlockBits( &maskBmpData );
        if ( statusCode != Ok )
            return statusCode;
        statusCode = src->UnlockBits( &srcBmpData );
        if ( statusCode != Ok )
            return statusCode;
        statusCode = this->UnlockBits( &outBmpData );

        return statusCode;
    }

    //FloodFill-----------------------------------------------------------------
    Status BitmapExt::FloodFill( INT x, INT y, const Color& newColor )
    {
        // Lock the bitmap for reading and writing:
        // Lock the bitmaps:
        Status      status;
        BitmapData  bmpData;
        Rect        rect( 0, 0, GetWidth(), GetHeight() );
        status = this->LockBits(  &rect,\
                                ImageLockModeRead | ImageLockModeWrite,\
                                PixelFormat32bppARGB,
                                &bmpData );
        if ( status != Ok )
            return SetStatus( status );

        // Get the color of the flood start pixel:
        if ( x < 0 || x >= bmpData.Width || y < 0 || y >= bmpData.Height )
            return SetStatus( InvalidParameter );

//        BYTE*   pixels = (BYTE*)bmpData.Scan0 + y * ( bmpData.Stride );
//        BYTE*   pixels = (BYTE*)bmpData.Scan0;
//        BYTE*   curRow = &pixels[ y * bmpData.Stride ];
//        Color   oldColor( curRow[ x ], curRow[ x + 1 ], curRow[ x + 2 ], curRow[ x + 3 ] );
        Color oldColor;
        if ( ( status = getPixelARGB( &bmpData, x, y, &oldColor ) ) != Ok )
            return SetStatus( status );

        // Go into the recursive area, replacing all adjacent pixels of oldColor
        // with the new color.
        status = floodFill( &bmpData, x, y, newColor, oldColor );

        // Unlock the bitmap:
        this->UnlockBits( &bmpData );

        return SetStatus( status );
    }

    Status BitmapExt::floodFill( BitmapData* bmpData, INT x, INT y, const Color& newColor, const Color& oldColor )
    {
        if ( !bmpData )
            return InvalidParameter;

        // If a point is out of bounds, return Ok to indicate that this branch
        // is done.
        if ( x < 0 || x >= bmpData->Width || y < 0 || y >= bmpData->Height )
            return Ok;

        // If the current pixel is not the same as the old color, stop filling
        Status status;
        Color curColor;
        if ( ( status = getPixelARGB( bmpData, x, y, &curColor ) ) != Ok )
            return status;

        if ( curColor.GetValue() != oldColor.GetValue() )
            return Ok;

        // Set the current pixel:
        if ( ( status = setPixelARGB( bmpData, x, y, newColor ) ) != Ok )
            return status;

        int left = x - 1, right = x + 1;

        // Fill to the left:
        while ( left-- )
        {
            // If this pixel is not the same as the old color, then break the loop
            if ( ( status = getPixelARGB( bmpData, left, y, &curColor ) ) != Ok )
                return status;

            if ( curColor.GetValue() != oldColor.GetValue() )
                break;

            // Set the pixel to the new color:
            if ( ( status = setPixelARGB( bmpData, left, y, newColor ) ) != Ok )
                return status;
        }

        // Fill to the right:
        while ( right++ < bmpData->Width )
        {
            // If this pixel is not the same as the old color, then break the loop
            if ( ( status = getPixelARGB( bmpData, right, y, &curColor ) ) != Ok )
                return status;

            if ( curColor.GetValue() != oldColor.GetValue() )
                break;

            // Set the pixel to the new color:
            if ( ( status = setPixelARGB( bmpData, right, y, newColor ) ) != Ok )
                return status;
        }

        // Go over the scanline from left to right, recursively filling the top
        // and bottom rows
        while ( left++ < right )
        {
            if ( ( status = floodFill( bmpData, left, y - 1, newColor, oldColor ) ) != Ok )
                return status;
            if ( ( status = floodFill( bmpData, left, y + 1, newColor, oldColor ) ) != Ok )
                return status;
        }

        return Ok;
    }

    //setPixelARGB
    Status BitmapExt::setPixelARGB( BitmapData* bmpData, INT x, INT y, const Color& col )
    {
        if ( !bmpData )
            return InvalidParameter;

        int bpp = abs( bmpData->Stride ) / bmpData->Width;
        BYTE* pixels = (BYTE*)bmpData->Scan0 + y * ( bmpData->Stride );
        pixels[ x * bpp     ] = col.GetA();
        pixels[ x * bpp + 1 ] = col.GetR();
        pixels[ x * bpp + 2 ] = col.GetG();
        pixels[ x * bpp + 3 ] = col.GetB();

        return Ok;
    }

    //getPixelARGB
    Status BitmapExt::getPixelARGB( const BitmapData* bmpData, INT x, INT y, Color* col )
    {
        if ( !bmpData )
            return InvalidParameter;

        int bpp = abs( bmpData->Stride ) / bmpData->Width;
        BYTE* pixels = (BYTE*)bmpData->Scan0 + y * ( bmpData->Stride );
        *col = Color( pixels[ x * bpp ], pixels[ x * bpp + 1 ], pixels[ x * bpp + 2 ], pixels[ x * bpp + 3 ] );

        return Ok;
    }

} // namespace AggOO
