

/*******************************************************************\

This file is part of the Gdiplus library.  AggOO provides a way to emulate
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
// $Id: GdiplusDemo.cpp,v 1.2 2006/11/03 21:07:31 dratek Exp $
//
// GdiplusDemo.cpp
//

#include <windows.h>
#include "AggOO.h"
#include "Gdiplus.h"
#include "platform/platform_app.h"
#include "ctrl/agg_slider_ctrl.h"

#include <platform/LogFile.h>
cmd::LogFile logFile( "GdiplusDemo.log" );

#pragma comment( lib, "gdiplus.lib" )


/*
    The demo will be made of various states, derived from a common base.  This
    will allow each state to function as an individual program (handle input,
    animation, etc.)
*/
class DemoState
{
private:

protected:
    int             mWidth, mHeight;
    Gdiplus::Color    mBgColor;

public:
    DemoState( int w, int h ) : mWidth( w ), mHeight( h ), mBgColor( Gdiplus::Color::Beige )
    {
    }
    virtual ~DemoState()    { }

    // Add any controls
    virtual void AddControls( agg::ctrl_container* ctrls, const agg::trans_affine& mtx )
    {
    }

    // Draw any controls
    template< class Rasterizer, class Scanline, class Renderer >
    void DrawControls( const Rasterizer& ras, const Scanline& sl, const Renderer& ren )
    {
    }

    virtual void on_init() = 0;
    virtual void on_key( int x, int y, unsigned int key, unsigned int flags ) = 0;
    virtual void on_draw( Gdiplus::Image* img ) = 0;


    virtual void on_mouse_button_down( int x, int y, unsigned int flags )
    {
    }

    virtual void on_mouse_button_up( int x, int y, unsigned int flags )
    {
    }

    virtual void on_mouse_move( int x, int y, unsigned int flags )
    {
    }

    virtual void on_post_draw( void* raw_handler )
    {
    }

    virtual void on_resize( int sx, int sy )
    {
        mWidth = sx;
        mHeight = sy;
    }

    virtual void on_idle()
    {
    }

    virtual void on_ctrl_change()
    {
    }
};

class DotsDemo : public DemoState
{
protected:
    Gdiplus::Color    mColor;
    INT      mCount;

public:
    DotsDemo( int w, int h )
        : DemoState( w, h ), mCount( 100 )
    {
    }
    virtual ~DotsDemo()
    {
    }

    virtual void on_init()
    {
        mColor = Gdiplus::Color::Blue;
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
        Gdiplus::Bitmap* bmp = static_cast< Gdiplus::Bitmap* >( img );

        // Plot some points:
        for ( int i = 0; i < 100; i++ )
        {
            INT x = rand() / Gdiplus::REAL( RAND_MAX ) * mWidth;
            INT y = rand() / Gdiplus::REAL( RAND_MAX ) * mHeight;

            bmp->SetPixel( x, y, mColor );
        }
    }
};

class LineDemo : public DemoState
{
private:
    Gdiplus::PointF*  mVertices;
    INT      mVertCount;
    Gdiplus::REAL     mSize;
    Gdiplus::Pen*     mPen;

public:
    LineDemo( int w, int h )
        : DemoState( w, h ), mSize( 10 ), mVertCount( 50 ), mVertices( 0 ), mPen( 0 )
    {
    }
    virtual ~LineDemo()
    {
        delete mVertices;
        delete mPen;
    }

    virtual void on_init()
    {
        mPen = new Gdiplus::Pen( Gdiplus::Color::Black, mSize );
        mVertices = new Gdiplus::PointF [ mVertCount ];
        for ( int i = 0; i < mVertCount; i++ )
        {
            mVertices[ i ].X = rand() % mWidth;
            mVertices[ i ].Y = rand() % mHeight;
        }
    }
    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }
    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
        gfx.DrawLines( mPen, mVertices, mVertCount );
    }
};

class ArcDemo : public DemoState
{
private:
    Gdiplus::Pen*     mPen;
    Gdiplus::REAL     mSize;
    INT      mCount;

public:
    ArcDemo( int w, int h )
        : DemoState( w, h ), mPen( 0 ), mSize( 4 ), mCount( 20 )
    {
    }
    virtual ~ArcDemo()
    {
        delete mPen;
    }

    virtual void on_init()
    {
        mPen = new Gdiplus::Pen( Gdiplus::Color::Cyan, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
        for ( int i = 0; i < mCount; i++ )
        {
            Gdiplus::REAL x = rand() % mWidth;
            Gdiplus::REAL y = rand() % mHeight;
            Gdiplus::REAL radius = rand() % 200;
            Gdiplus::REAL startAngle = rand() % 360;
            Gdiplus::REAL endAngle = rand() % 360;

            gfx.DrawArc( mPen, x, y, radius, radius, startAngle, endAngle );
        }
    }
};

class EllipseDemo : public DemoState
{
private:
    Gdiplus::SolidBrush* mBrush;
    Gdiplus::Pen*     mPen;
    Gdiplus::REAL     mSize;
    INT      mCount;

public:
    EllipseDemo( int w, int h )
        : DemoState( w, h ), mBrush( 0 ), mPen( 0 ), mSize( 2 ), mCount( 20 )
    {
    }
    virtual ~EllipseDemo()
    {
        delete mPen;
        delete mBrush;
    }

    virtual void on_init()
    {
        mBrush = new Gdiplus::SolidBrush( Gdiplus::Color::Red );
        mPen = new Gdiplus::Pen( mBrush, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
        for ( int i = 0; i < mCount; i++ )
        {
            Gdiplus::REAL x = rand() % mWidth;
            Gdiplus::REAL y = rand() % mHeight;
            Gdiplus::REAL w = rand() % mWidth;
            Gdiplus::REAL h = rand() % mHeight;

            gfx.DrawEllipse( mPen, x, y, w, h );
        }
        for ( int i = 0; i < mCount; i++ )
        {
            Gdiplus::REAL x = rand() % mWidth;
            Gdiplus::REAL y = rand() % mHeight;
            Gdiplus::REAL w = rand() % mWidth;
            Gdiplus::REAL h = rand() % mHeight;

            gfx.FillEllipse( mBrush, x, y, w, h );
        }
    }
};

class RectangleDemo : public DemoState
{
private:
    Gdiplus::Pen*     mPen;
    Gdiplus::REAL     mSize;
    INT      mCount;

public:
    RectangleDemo( int w, int h )
        : DemoState( w, h ), mPen( 0 ), mSize( 2 ), mCount( 20 )
    {
    }
    virtual ~RectangleDemo()
    {
        delete mPen;
    }

    virtual void on_init()
    {
        mPen = new Gdiplus::Pen( Gdiplus::Color::DarkOliveGreen, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
        for ( int i = 0; i < mCount; i++ )
        {
            Gdiplus::REAL x = rand() % mWidth;
            Gdiplus::REAL y = rand() % mHeight;
            Gdiplus::REAL w = rand() % mWidth;
            Gdiplus::REAL h = rand() % mHeight;

            gfx.DrawRectangle( mPen, x, y, w, h );
        }
    }
};

//class RoundRectangleDemo : public DemoState
//{
//private:
//    Gdiplus::Pen*     mPen;
//    Gdiplus::REAL     mSize;
//    INT      mCount;
//
//public:
//    RoundRectangleDemo( int w, int h )
//        : DemoState( w, h ), mPen( 0 ), mSize( 4 ), mCount( 20 )
//    {
//    }
//    virtual ~RoundRectangleDemo()
//    {
//        delete mPen;
//    }
//
//    virtual void on_init()
//    {
//        mPen = new Gdiplus::Pen( Gdiplus::Color::MistyRose, mSize );
//    }
//
//    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
//    {
//    }
//
//    virtual void on_draw( Gdiplus::Image* img )
//    {
//        Gdiplus::GraphicsExt gfx( img );
//        gfx.Clear( mBgColor );
//
//        // Round-Rect
//        gfx.DrawRoundRectangle( mPen, 10, 10, mWidth - 20, mHeight - 20, 20 );
//    }
//};

class PolygonDemo : public DemoState
{
private:
    Gdiplus::Pen*     mPen;
    Gdiplus::REAL     mSize;
    INT      mCount;
    Gdiplus::PointF*  mVertices;
    INT      mVertCount;

public:
    PolygonDemo( int w, int h )
        : DemoState( w, h ), mPen( 0 ), mSize( 6 ), mCount( 20 ), mVertCount( 20 ), mVertices( 0 )
    {
    }
    virtual ~PolygonDemo()
    {
        delete mPen;
    }

    virtual void on_init()
    {
        mVertices = new Gdiplus::PointF [ mVertCount ];
        for ( int i = 0; i < mVertCount; i++ )
        {
            Gdiplus::REAL x = rand() % mWidth;
            Gdiplus::REAL y = rand() % mHeight;
            mVertices[ i ] = Gdiplus::PointF( x, y );
        }

        mPen = new Gdiplus::Pen( Gdiplus::Color::DarkMagenta, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
        gfx.DrawPolygon( mPen, mVertices, mVertCount );
    }
};

class PieDemo : public DemoState
{
private:
    Gdiplus::Pen*     mPen;
    Gdiplus::REAL     mSize;
    INT      mCount;

public:
    PieDemo( int w, int h )
        : DemoState( w, h ), mPen( 0 ), mSize( 2 ), mCount( 20 )
    {
    }
    virtual ~PieDemo()
    {
        delete mPen;
    }

    virtual void on_init()
    {
        mPen = new Gdiplus::Pen( Gdiplus::Color::YellowGreen, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
        for ( int i = 0; i < 20; i++ )
        {
            Gdiplus::REAL x = rand() % mWidth;
            Gdiplus::REAL y = rand() % mHeight;
            Gdiplus::REAL radius = rand() % 200;
            Gdiplus::REAL startAngle = rand() % 360;
            Gdiplus::REAL endAngle = rand() % 360;

            gfx.DrawPie( mPen, x, y, radius, radius, startAngle, endAngle );
        }
    }
};

class MatrixDemo : public DemoState
{
private:
    Gdiplus::SolidBrush* mBrush;
    Gdiplus::REAL     mSize;
    INT      mCount;
    INT      mSegments;
    Gdiplus::PointF   mScale;

public:
    MatrixDemo( int w, int h )
        : DemoState( w, h ), mBrush( 0 ), mSize( 2 ), mCount( 20 ), mSegments( 8 ), mScale( 50, 50 )
    {
    }
    virtual ~MatrixDemo()
    {
        delete mBrush;
    }

    virtual void on_init()
    {
        mBrush = new Gdiplus::SolidBrush( Gdiplus::Color( 255, 0, 0, 0 ) );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
        gfx.ResetTransform();
        Gdiplus::REAL rotAngle = 360.0 / Gdiplus::REAL( mSegments );
        gfx.TranslateTransform( mWidth / 2.0, mHeight / 2.0 );
        gfx.ScaleTransform( mScale.X, mScale.Y );
        for ( int i = 0; i < mSegments; i++ )
        {
            gfx.RotateTransform( rotAngle );
            gfx.FillPie( mBrush, -1, -1, 2, 2, -rotAngle / 4, rotAngle / 2 );
        }
        gfx.ResetTransform();
    }
};

class BezierDemo : public DemoState
{
private:
    Gdiplus::Pen*     mPen;
    Gdiplus::REAL     mSize;
    INT      mCount;
    Gdiplus::PointF*  mVertices;
    INT      mVertCount;

public:
    BezierDemo( int w, int h )
        : DemoState( w, h ), mPen( 0 ), mSize( 7 ), mCount( 20 ), mVertCount( 7 ), mVertices( 0 )
    {
    }
    virtual ~BezierDemo()
    {
        delete mPen;
        delete mVertices;
    }

    virtual void on_init()
    {
        mVertices = new Gdiplus::PointF [ mVertCount ];
        mVertices[ 0 ] = Gdiplus::PointF( 50, 50 );
        mVertices[ 1 ] = Gdiplus::PointF( 200, 50 );
        mVertices[ 2 ] = Gdiplus::PointF( 180, 190 );
        mVertices[ 3 ] = Gdiplus::PointF( 70, 210 );
        mVertices[ 4 ] = Gdiplus::PointF( 120, 160 );
        mVertices[ 5 ] = Gdiplus::PointF( 103, 100 );
        mVertices[ 6 ] = Gdiplus::PointF( 70, 60 );

        mPen = new Gdiplus::Pen( Gdiplus::Color( 255, 200, 90, 60 ), mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
        gfx.DrawBeziers( mPen, mVertices, mVertCount );
    }
};

class CurveDemo : public DemoState
{
private:
    Gdiplus::Pen*     mCardinalPen;
    Gdiplus::Pen*     mClosedPen;
    Gdiplus::REAL     mSize;
    INT      mCount;
    Gdiplus::PointF*  mCardinalVerts;
    Gdiplus::PointF*  mClosedVerts;
    INT      mVertCount;
    Gdiplus::REAL     mCurAngle;

public:
    CurveDemo( int w, int h )
        : DemoState( w, h ), mCardinalPen( 0 ), mClosedPen( 0 ), mSize( 6 ),
          mCount( 20 ), mVertCount( 6 ), mCardinalVerts( 0 ), mClosedVerts( 0 ),
          mCurAngle( 0 )
    {
    }
    virtual ~CurveDemo()
    {
        delete mCardinalPen;
        delete mClosedPen;
        delete mCardinalVerts;
        delete mClosedVerts;
    }

    virtual void on_init()
    {
        Gdiplus::ARGB argb = AggOO::ColorExt::RandomRGB().GetValue();
        mBgColor.SetValue( argb );
//        mBgColor = AggOO::ColorExt::RandomRGB();

        mCardinalVerts = new Gdiplus::PointF [ mVertCount ];
        mCardinalVerts[ 0 ] = Gdiplus::PointF( 100, 100 );
        mCardinalVerts[ 1 ] = Gdiplus::PointF( mWidth - 100, 100 );
        mCardinalVerts[ 2 ] = Gdiplus::PointF( mWidth - 100, mHeight - 100 );
        mCardinalVerts[ 3 ] = Gdiplus::PointF( 100, mHeight - 100 );
        mCardinalVerts[ 4 ] = Gdiplus::PointF( mWidth / 2.0, mHeight / 2.0 );
        mCardinalVerts[ 5 ] = Gdiplus::PointF( mWidth / 2.0, mHeight / 3.0 );

        mClosedVerts = new Gdiplus::PointF [ mVertCount ];
        mClosedVerts[ 0 ] = Gdiplus::PointF( 150, 150 );
        mClosedVerts[ 1 ] = Gdiplus::PointF( mWidth - 150, 150 );
        mClosedVerts[ 2 ] = Gdiplus::PointF( mWidth - 150, mHeight - 150 );
        mClosedVerts[ 3 ] = Gdiplus::PointF( 150, mHeight - 150 );
        mClosedVerts[ 4 ] = Gdiplus::PointF( mWidth / 2.0 + 20, mHeight / 2.0 - 20 );
        mClosedVerts[ 5 ] = Gdiplus::PointF( mWidth / 2.0 + 20, mHeight / 3.0 + 20 );

        mCardinalPen = new Gdiplus::Pen( Gdiplus::Color( 255, 10, 180, 60 ), mSize );
        mClosedPen   = new Gdiplus::Pen( Gdiplus::Color( 255, 10, 25, 170 ), mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        if ( key == 'r' )
            mCurAngle += 5.0;
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );

        gfx.TranslateTransform( mWidth / 2.0, mHeight / 2.0 );
        gfx.RotateTransform( mCurAngle );
        gfx.TranslateTransform( -mWidth / 2.0, -mHeight / 2.0 );

        // Cardinal spline
        gfx.DrawClosedCurve( mCardinalPen, mCardinalVerts, mVertCount );

        // Closed curve:
        gfx.DrawClosedCurve( mClosedPen, mClosedVerts, mVertCount );
    }
};

class FillModeDemo : public DemoState
{
private:
    Gdiplus::SolidBrush* mBrush;
    Gdiplus::PointF*  mVertices;
    INT      mVertCount;
    Gdiplus::FillMode mFillMode;

public:
    FillModeDemo( int w, int h )
        : DemoState( w, h ), mBrush( 0 ), mVertCount( 20 ), mVertices( 0 ), mFillMode( Gdiplus::FillModeAlternate )
    {
    }
    virtual ~FillModeDemo()
    {
        delete mBrush;
        delete mVertices;
    }

    virtual void on_init()
    {
        mVertices = new Gdiplus::PointF [ mVertCount ];
        for ( int i = 0; i < mVertCount; i++ )
        {
            Gdiplus::REAL x = rand() % mWidth;
            Gdiplus::REAL y = rand() % mHeight;
            mVertices[ i ] = Gdiplus::PointF( x, y );
        }

        mBrush = new Gdiplus::SolidBrush( Gdiplus::Color::DarkMagenta );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        if ( key == AggOO::KV_F1 )
        {
            mFillMode = ( mFillMode == Gdiplus::FillModeAlternate ) ? Gdiplus::FillModeWinding : Gdiplus::FillModeAlternate;
        }
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
        gfx.FillPolygon( mBrush, mVertices, mVertCount, mFillMode );
    }
};

class TextureBrushDemo : public DemoState
{
private:
    Gdiplus::TextureBrush*  mBrush;

public:
    TextureBrushDemo( int w, int h )
        : DemoState( w, h ), mBrush( 0 )
    {
    }
    virtual ~TextureBrushDemo()
    {
        delete mBrush;
    }

    virtual void on_init()
    {
        // Generate the texture
        Gdiplus::Rect texRect( 0, 0, 40, 40 );
        Gdiplus::Bitmap tex( texRect.Width, texRect.Height );
        Gdiplus::BitmapData texData;
        tex.LockBits( &texRect, Gdiplus::ImageLockModeWrite, tex.GetPixelFormat(), &texData );

        unsigned char *pixels = (unsigned char*)texData.Scan0;
        for ( size_t y = 0; y < texRect.Height; y++ )
        {
            Gdiplus::REAL yRatio = y / Gdiplus::REAL( texRect.Height );
            for ( size_t x = 0; x < texRect.Width; x++ )
            {
                Gdiplus::REAL xRatio = x / Gdiplus::REAL( texRect.Width );

                *pixels++ = 0;
                *pixels++ = 255 * xRatio;
                *pixels++ = 255 * yRatio;
                *pixels++ = 255;
            }
        }

        tex.UnlockBits( &texData );

/*
                // Generate the texture:
                Gdiplus::Rect texRect( 0, 0, 40, 40 );
                Gdiplus::Bitmap tex( texRect.Width, texRect.Height );
                Gdiplus::BitmapData texData;
                tex.LockBits( &texRect, Gdiplus::ImageLockModeWrite, tex.GetPixelFormat(), &texData );

                unsigned int *pixels = (unsigned int*)texData.Scan0;
                for ( size_t y = 0; y < texRect.Height; y++ )
                {
                    int yCell = y / 10;
                    for ( size_t x = 0; x < texRect.Width; x++ )
                    {
                        int xCell = x / 10;
                        if ( ( xCell % 2 ) == ( yCell % 2 ) )
                            *pixels = 0xffff0000;
                        else
                            *pixels = 0xff000000;
                        ++pixels;
                    }
                }
                tex.UnlockBits( &texData );
*/

        // Create a brush:
        mBrush = new Gdiplus::TextureBrush( &tex, Gdiplus::WrapModeTileFlipXY );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );

        // Fill a rectangle:
        gfx.FillRectangle( mBrush, 50, 50, mWidth - 100, mHeight - 100 );
    }
};

class LinearGradientBrushDemo : public DemoState
{
private:
    Gdiplus::LinearGradientBrush* mBrush;
    Gdiplus::Rect mBounds;
    Gdiplus::LinearGradientMode mGradientDir;
//    Gdiplus::GradientInterpolation mInterpMode;

public:
    LinearGradientBrushDemo( int w, int h )
        : DemoState( w, h ),
          mBrush( 0 ),
          mGradientDir( Gdiplus::LinearGradientModeHorizontal )
    {
    }
    virtual ~LinearGradientBrushDemo()
    {
        delete mBrush;
    }

    virtual void on_init()
    {
        mBounds = Gdiplus::Rect( 100, 100, mWidth - 200, mHeight - 200 );

        // Create a brush:
        if ( mBrush )   delete mBrush;
        mBrush = new Gdiplus::LinearGradientBrush( mBounds, Gdiplus::Color::Blue, Gdiplus::Color::Green, mGradientDir );

        Gdiplus::Color gradColors[] = {\
            Gdiplus::Color( 255, 255, 0, 0 ),\
            Gdiplus::Color( 255, 255, 255, 0 ),\
            Gdiplus::Color( 255, 0, 255, 255 ),\
            Gdiplus::Color( 255, 255, 0, 255 ),\
            Gdiplus::Color( 255, 255, 255, 255 ),\
            Gdiplus::Color( 255, 0, 0, 0 ) };
        Gdiplus::REAL gradPos[] = {\
            0.0,\
            0.2,\
            0.25,\
            0.4,\
            0.89,\
            1.0 };

        int numColors = sizeof( gradPos ) / sizeof( gradPos[ 0 ] );
        mBrush->SetInterpolationColors( gradColors, gradPos, numColors );
//        mBrush->SetGradientInterpolationMode( mInterpMode );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        switch ( key )
        {
        case 'r':
            mGradientDir = Gdiplus::LinearGradientMode( ( mGradientDir + 1 ) % ( Gdiplus::LinearGradientModeBackwardDiagonal + 1 ) );
            on_init();
            break;

        case 's':
            {
//                Gdiplus::GradientStyle style = Gdiplus::GradientStyle( ( mBrush->GetGradientStyle() + 1 ) % ( Gdiplus::GradientStyleXY + 1 ) );
//                mBrush->SetGradientStyle( style );
            }
            break;

        case 'w':
            {
                Gdiplus::WrapMode mode = Gdiplus::WrapMode( ( mBrush->GetWrapMode() + 1 ) % ( Gdiplus::WrapModeClamp + 1 ) );
                mBrush->SetWrapMode( mode );
            }
            break;

        case 'i':
            {
//                mInterpMode = Gdiplus::GradientInterpolation( ( mInterpMode + 1 ) % ( Gdiplus::GradientInterpolationStep + 1 ) );
//                mBrush->SetGradientInterpolationMode( mInterpMode );
            }
            break;
        }
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );

        // Fill a rectangle:
        gfx.FillRectangle( mBrush, mBounds );
    }
};

class DrawImageDemo : public DemoState
{
private:
    Gdiplus::Bitmap* mTexture;

public:
    DrawImageDemo( int w, int h )
        : DemoState( w, h ), mTexture( 0 )
    {
    }
    virtual ~DrawImageDemo()
    {
        delete mTexture;
    }

    virtual void on_init()
    {
        // Generate the texture:
        Gdiplus::Rect texRect( 0, 0, 40, 40 );
        mTexture = new Gdiplus::Bitmap( texRect.Width, texRect.Height, PixelFormat32bppARGB );
        Gdiplus::BitmapData texData;
        mTexture->LockBits( &texRect, Gdiplus::ImageLockModeWrite, mTexture->GetPixelFormat(), &texData );

        unsigned char *pixels = (unsigned char*)texData.Scan0;
        for ( size_t y = 0; y < texRect.Height; y++ )
        {
            int yCell = y / 20;
            for ( size_t x = 0; x < texRect.Width; x++ )
            {
                int xCell = x / 20;

                if ( ( xCell % 2 ) == ( yCell % 2 ) )
                {
                    *pixels++ = 0;
                    *pixels++ = 0;
                    *pixels++ = 255;
                    *pixels++ = 255;
                }
                else
                {
                    *pixels++ = 0;
                    *pixels++ = 0;
                    *pixels++ = 0;
                    *pixels++ = 255;
                }

            }
        }
        mTexture->UnlockBits( &texData );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );
gfx.TranslateTransform( 10, 30 );
gfx.RotateTransform( 45.0 );

        gfx.DrawImage( mTexture, 0, 0 );
    }
};

//class StarsDemo : public DemoState
//{
//private:
//    Gdiplus::Pen*     mPen;
//    Gdiplus::SolidBrush* mBrush;
//    INT      mCount;
//    Gdiplus::FillMode mFillMode;
//    INT      mPointCount;
//
//public:
//    StarsDemo( int w, int h )
//        : DemoState( w, h ), mBrush( 0 ), mPen( 0 ), mCount( 10 ), mPointCount( 5 )
//    {
//    }
//    virtual ~StarsDemo()
//    {
//        delete mBrush;
//        delete mPen;
//    }
//
//    virtual void on_init()
//    {
//        mBrush = new Gdiplus::SolidBrush( Gdiplus::ColorExt::RandomRGB() );
//        mPen   = new Gdiplus::Pen( Gdiplus::ColorExt::RandomRGB(), 3 );
//    }
//
//    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
//    {
//        switch ( key )
//        {
//        case ' ':
//            mFillMode = Gdiplus::FillMode( ( mFillMode + 1 ) % ( Gdiplus::FillModeWinding + 1 ) );
//            break;
//
//        case '-':
//            mPointCount = Gdiplus::IMinClamp( mPointCount - 1, 2 );
//            break;
//
//        case '=':
//        case '+':
//            ++mPointCount;
//            break;
//        }
//    }
//
//    virtual void on_draw( Gdiplus::Image* img )
//    {
//        Gdiplus::GraphicsExt gfx( img );
//        gfx.Clear( mBgColor );
//
//        // Fill some stars:
//        for ( int i = 0; i < mCount; i++ )
//        {
//            INT  numPoints   = Gdiplus::IRangeRandom( 3, 6 );
//            Gdiplus::REAL outerRadius = Gdiplus::RangeRandom( 10, mWidth / 2.0 );
//            Gdiplus::REAL innerRadius = outerRadius * Gdiplus::RangeRandom( 0.25, 0.75 );
//            Gdiplus::REAL xPos        = Gdiplus::RangeRandom( 0, mWidth );
//            Gdiplus::REAL yPos        = Gdiplus::RangeRandom( 0, mHeight );
//
//            mBrush->SetColor( Gdiplus::ColorExt::RandomRGB() );
////            gfx.FillStar( mBrush, numPoints, Gdiplus::PointF( xPos, yPos ), innerRadius, outerRadius, mFillMode );
//            gfx.FillStar( mBrush, numPoints, Gdiplus::PointF( xPos, yPos ), innerRadius, outerRadius );
//        }
//
//        // Stroke some stars:
//        for ( int i = 0; i < mCount; i++ )
//        {
//            INT  numPoints   = Gdiplus::IRangeRandom( 2, 10 );
//            Gdiplus::REAL outerRadius = Gdiplus::RangeRandom( 10, mWidth / 2.0 );
//            Gdiplus::REAL innerRadius = outerRadius * Gdiplus::RangeRandom( 0.25, 0.75 );
//            Gdiplus::REAL xPos        = Gdiplus::RangeRandom( 0, mWidth );
//            Gdiplus::REAL yPos        = Gdiplus::RangeRandom( 0, mHeight );
//
//            gfx.DrawStar( mPen, numPoints, Gdiplus::PointF( xPos, yPos ), innerRadius, outerRadius );
//        }
//    }
//};

class DropShadowDemo : public DemoState
{
private:
//    agg::slider_ctrl<agg::rgba8>     mShadowOffsetX;
//    agg::slider_ctrl<agg::rgba8>     mShadowOffsetY;
//    agg::slider_ctrl<agg::rgba8>     mShadowDarkness;
    Gdiplus::PointF   mShadowOffset;
    Gdiplus::REAL     mShadowDarkness;
    INT      mCount;

    struct RectData
    {
        Gdiplus::Rect    bounds;
        Gdiplus::REAL    radius;
        Gdiplus::Color   color;
    };
    RectData*       mRectArray;

public:
    DropShadowDemo( int w, int h )
        : DemoState( w, h ), mRectArray( 0 ), mCount( 20 ),
          mShadowOffset( Gdiplus::PointF( 10, 10 ) ),
          mShadowDarkness( 0.5 )
          //mShadowOffsetX( 5, 5, w - 10, 10, true ),
          //mShadowOffsetY( 5, 15, w - 10, 10 + 15, true ),
          //mShadowDarkness( 5, 25, w - 10, 10 + 25, true )
    {
        //mShadowOffsetX.label( "Shadow Offset X = %3.2f" );
        //mShadowOffsetX.value( 8 );

        //mShadowOffsetY.label( "Shadow Offset Y = %3.2f" );
        //mShadowDarkness.label( "Shadow Darkness = %.2f" );

    }
    virtual ~DropShadowDemo()
    {
        delete mRectArray;
    }

    void AddControls( agg::ctrl_container* ctrls, const agg::trans_affine& mtx )
    {
        //ctrls->add( mShadowOffsetX );      mShadowOffsetX.transform( mtx );
        //ctrls->add( mShadowOffsetY );      mShadowOffsetY.transform( mtx );
        //ctrls->add( mShadowDarkness );     mShadowDarkness.transform( mtx );
    }

    template< class Rasterizer, class Scanline, class Renderer >
    void DrawControls( const Rasterizer& ras, const Scanline& sl, const Renderer& ren )
    {
        //agg::rasterizer_scanline_aa<> ras;
        //agg::scanline_u8 sl;
        //agg::renderer_scanline_aa_solid<prim_ren_base_type> ren(rb);

        //agg::render_ctrl_rs( ras, sl, ren, mShadowOffsetX );
        //agg::render_ctrl_rs( ras, sl, ren, mShadowOffsetY );
        //agg::render_ctrl_rs( ras, sl, ren, mShadowDarkness );
    }


    virtual void on_init()
    {
        // Generate the rect data
        mRectArray = new RectData [ mCount ];
        for ( int i = 0; i < mCount; i++ )
        {
            INT width  = AggOO::IRangeRandom( 1, mWidth );
            INT height = AggOO::IRangeRandom( 1, mHeight );
            INT xPos   = AggOO::IRangeRandom( 0, mWidth );
            INT yPos   = AggOO::IRangeRandom( 0, mHeight );

            Gdiplus::ARGB argb = AggOO::ColorExt::RandomRGB().GetValue();
            mRectArray[ i ].bounds = Gdiplus::Rect( xPos, yPos, width, height );
            mRectArray[ i ].radius = AggOO::RangeRandom( 0.0, min( width, height ) / 2.0 );
            mRectArray[ i ].color.SetValue( argb );
        }
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        switch ( key )
        {
        case 'w':
            // Move the shadow up:
            mShadowOffset.Y--;
            break;

        case 's':
            // Move the shadow down:
            mShadowOffset.Y++;
            break;

        case 'a':
            // Move the shadow left:
            mShadowOffset.X--;
            break;

        case 'd':
            // Move the shadow right:
            mShadowOffset.X++;
            break;

        case ',':
            // Decrease the shadow opacity:
            mShadowDarkness = AggOO::Clamp( mShadowDarkness - 0.1, 0.0, 1.0 );
            break;

        case '.':
            // Decrease the shadow opacity:
            mShadowDarkness = AggOO::Clamp( mShadowDarkness + 0.1, 0.0, 1.0 );
            break;
        }
    }

    virtual void on_draw( Gdiplus::Image* img )
    {
        Gdiplus::Graphics gfx( img );
        gfx.Clear( mBgColor );

        // Brush for the shadows:
        Gdiplus::SolidBrush shadowBrush( Gdiplus::Color( mShadowDarkness * 255, 0, 0, 0 ) );

        // Render the shapes:
        for ( int i = 0; i < mCount; i++ )
        {
            // Render the shadow first:
            Gdiplus::GraphicsState state = gfx.Save();
            gfx.TranslateTransform( mShadowOffset.X, mShadowOffset.Y );
            gfx.FillRectangle( &shadowBrush, mRectArray[ i ].bounds );
//            gfx.TranslateTransform( -mShadowOffset.X, -mShadowOffset.Y );
            gfx.Restore( state );

            Gdiplus::SolidBrush br( mRectArray[ i ].color );
            gfx.FillRectangle( &br, mRectArray[ i ].bounds );
        }
    }
};



/*
    The demo application is derived from the modified AGG framework.
*/
class MyApplication : public AggOO::modified_agg_app
{
private:
//    enum { Dots, Lines, Arcs, Ellipses, Rectangles, RoundRects, Polygons, Pies, Matrix, Beziers, Curves, FillMode, TextureBrush, DrawImage, LinearGradientBrush, Stars, DropShadows, SceneCount };
    enum { Dots, Lines, Arcs, Ellipses, Rectangles, Polygons, Pies, Matrix, Beziers, Curves, FillMode, TextureBrush, DrawImage, LinearGradientBrush, DropShadows, SceneCount };

    DemoState**     mStates;
    DemoState*      mCurState;
    unsigned int    mCurStateIdx;

	ULONG_PTR	mGdiplusToken;

public:
    MyApplication( AggOO::PixelFormat pixFormat, bool flipY )
        : AggOO::modified_agg_app( pixFormat, flipY ), mCurStateIdx( 0 ), mStates( 0 ), mCurState( 0 )
    {
	    Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	    Gdiplus::GdiplusStartup( &mGdiplusToken, &gdiplusStartupInput, NULL );
    }

    virtual ~MyApplication()
    {
        if ( mStates )
        {
            for ( int i = 0; i < SceneCount; i++ )
                delete mStates[ i ];
            delete mStates;
        }

    	Gdiplus::GdiplusShutdown( mGdiplusToken );
    }

    virtual void on_init()
    {
        // Create the states.  These are all going to be created at startup (as
        // opposed to creating them as needed.)  The main reason for this is to
        // maintain the state's variables for the duration of the demo.
        int w = width();
        int h = height();
        mStates = new DemoState* [ SceneCount ];
        mStates[ Dots           ] = new DotsDemo( w, h );
        mStates[ Lines          ] = new LineDemo( w, h );
        mStates[ Arcs           ] = new ArcDemo( w, h );
        mStates[ Ellipses       ] = new EllipseDemo( w, h );
        mStates[ Rectangles     ] = new RectangleDemo( w, h );
//        mStates[ RoundRects     ] = new RoundRectangleDemo( w, h );
        mStates[ Polygons       ] = new PolygonDemo( w, h );
        mStates[ Pies           ] = new PieDemo( w, h );
        mStates[ Matrix         ] = new MatrixDemo( w, h );
        mStates[ Beziers        ] = new BezierDemo( w, h );
        mStates[ Curves         ] = new CurveDemo( w, h );
        mStates[ FillMode       ] = new FillModeDemo( w, h );
        mStates[ TextureBrush   ] = new TextureBrushDemo( w, h );
        mStates[ DrawImage      ] = new DrawImageDemo( w, h );
        mStates[ LinearGradientBrush ] = new LinearGradientBrushDemo( w, h );
//        mStates[ Stars          ] = new StarsDemo( w, h );
        mStates[ DropShadows    ] = new DropShadowDemo( w, h );

        // Initialize the states:
        for ( int i = 0; i < SceneCount; i++ )
        {
            mStates[ i ]->on_init();
            mStates[ i ]->AddControls( &m_ctrls, trans_affine_resizing() );
        }

        mCurStateIdx = Curves;
        mCurState = mStates[ mCurStateIdx ];
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        AggOO::modified_agg_app::on_key( x, y, key, flags );

        // If the left or right arrow key was pressed, change state:
        if ( key == AggOO::KV_Left )
        {
            mCurStateIdx = ( ( mCurStateIdx - 1 ) + SceneCount ) % SceneCount;
            mCurState    = mStates[ mCurStateIdx ];
        }
        else if ( key == AggOO::KV_Right )
        {
            mCurStateIdx = ( mCurStateIdx + 1 ) % SceneCount;
            mCurState    = mStates[ mCurStateIdx ];
        }

        // Pass the key press to the current state:
        mCurState->on_key( x, y, key, flags );

        force_redraw();
    }

    virtual void on_draw()
    {
        srand( 12345 );

        int w = width();
        int h = height();
        Gdiplus::PixelFormat pixFmt = PixelFormat32bppARGB;
//        Gdiplus::PixelFormat pixFmt = format_aggoo();
        Gdiplus::Bitmap bmp( w, h, pixFmt );
        Gdiplus::Graphics gfx( &bmp );
        gfx.Clear( AggOO::Color::Beige );

        Gdiplus::Bitmap bmpComposite( w, h, pixFmt );
        Gdiplus::Graphics gfxComposite( &bmpComposite );

        Gdiplus::BitmapData bmpData;
        Gdiplus::Rect rcBmp( 0, 0, bmp.GetWidth(), bmp.GetHeight() );

        // Draw the current state:
        mCurState->on_draw( &bmp );

        gfxComposite.DrawImage( &bmp, 0, 0 );
        bmpComposite.LockBits( &rcBmp, Gdiplus::ImageLockModeRead, pixFmt, &bmpData );

        setPixelData( static_cast< unsigned char* >( bmpData.Scan0 ), bmpData.Width, bmpData.Height, bmpData.PixelFormat );

        bmpComposite.UnlockBits( &bmpData );
    }

    virtual void on_mouse_button_down( int x, int y, unsigned int flags )
    {
        mCurState->on_mouse_button_down( x, y, flags );
    }

    virtual void on_mouse_button_up( int x, int y, unsigned int flags )
    {
        mCurState->on_mouse_button_up( x, y, flags );
    }

    virtual void on_mouse_move( int x, int y, unsigned int flags )
    {
        mCurState->on_mouse_move( x, y, flags );
    }

    virtual void on_post_draw( void* raw_handler )
    {
        mCurState->on_post_draw( raw_handler );
    }

    virtual void on_resize( int sx, int sy )
    {
        AggOO::modified_agg_app::on_resize( sx, sy );

        mCurState->on_resize( sx, sy );
    }

    virtual void on_idle()
    {
        mCurState->on_idle();
    }

    virtual void on_ctrl_change()
    {
        mCurState->on_ctrl_change();
    }
};

int agg_main( int argc, char* argv[] )
{
    // Rendering with AGG is quite a bit simpler than using GDI+, since the
    // framework uses AGG underneath the hood.  Basically, we can render
    // directly to the rendering buffer used by the application.
    MyApplication app( AggOO::PixelFormat32bppBGRA, true );
    app.caption( "GDI+ Demo" );

    if ( app.init( 640, 480, AggOO::WindowStyleResize ) )
    {
        return app.run();
    }

    return 1;
}
