
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
// $Id: AggOODemo.cpp,v 1.6 2007/06/05 20:01:20 dratek Exp $
//
// AggOODemo.cpp
//

#include "AggOO.h"
#include "platform/platform_app.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"

#include <vector>

#include <platform/LogFile.h>
#include <platform/LogFileManager.h>
using cmd::LogFile;
using cmd::LogFileManager;
using cmd::LogFileSection;
//cmd::LogFile logFile( "AggOODemo.log" );

//#pragma comment( lib, "gdiplus.lib" )


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
    AggOO::Color    mBgColor;
    std::vector< agg::ctrl* > mCtrls;

public:
    DemoState( int w, int h ) : mWidth( w ), mHeight( h ), mBgColor( AggOO::Color::Beige )
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

    //// Number of controls:
    //int ControlCount() const    { return mCtrls.size(); }

    //// Get a control at a specified index:
    //agg::ctrl* GetCtrl( size_t index ) const
    //{
    //    try
    //    {
    //        return mCtrls.at( index );
    //    }
    //    catch ( std::exception& e )
    //    {
    //    }

    //    return 0;
    //}

    //// Add a control:
    //void AddCtrl( agg::ctrl& ctrl )
    //{
    //    mCtrls.push_back( &ctrl );
    //}

    virtual void on_init() = 0;
    virtual void on_key( int x, int y, unsigned int key, unsigned int flags ) = 0;
    virtual void on_draw( AggOO::Image* img ) = 0;


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
};

class DotsDemo : public DemoState
{
protected:
    AggOO::Color    mColor;
    AggOO::INT      mCount;

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
        mColor = AggOO::Color::Blue;
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
        AggOO::Bitmap* bmp = static_cast< AggOO::Bitmap* >( img );

        // Plot some points:
        for ( int i = 0; i < 100; i++ )
        {
            AggOO::INT x = rand() / AggOO::REAL( RAND_MAX ) * mWidth;
            AggOO::INT y = rand() / AggOO::REAL( RAND_MAX ) * mHeight;

            bmp->SetPixel( x, y, mColor );
        }
    }
};

class LineDemo : public DemoState
{
private:
    AggOO::PointF*  mVertices;
    AggOO::INT      mVertCount;
    AggOO::REAL     mSize;
    AggOO::Pen*     mPen;

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
        mPen = new AggOO::Pen( AggOO::Color::Black, mSize );
        mVertices = new AggOO::PointF [ mVertCount ];
        for ( int i = 0; i < mVertCount; i++ )
        {
            mVertices[ i ].X = rand() % mWidth;
            mVertices[ i ].Y = rand() % mHeight;
        }
    }
    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }
    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
        gfx.DrawLines( mPen, mVertices, mVertCount );
    }
};

class ArcDemo : public DemoState
{
private:
    AggOO::Pen*     mPen;
    AggOO::REAL     mSize;
    AggOO::INT      mCount;

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
        mPen = new AggOO::Pen( AggOO::Color::Cyan, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
        for ( int i = 0; i < mCount; i++ )
        {
            AggOO::REAL x = rand() % mWidth;
            AggOO::REAL y = rand() % mHeight;
            AggOO::REAL radius = rand() % 200;
            AggOO::REAL startAngle = rand() % 360;
            AggOO::REAL endAngle = rand() % 360;

            gfx.DrawArc( mPen, x, y, radius, radius, startAngle, endAngle );
        }
    }
};

class EllipseDemo : public DemoState
{
private:
    AggOO::SolidBrush* mBrush;
    AggOO::Pen*     mPen;
    AggOO::REAL     mSize;
    AggOO::INT      mCount;

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
        mBrush = new AggOO::SolidBrush( AggOO::Color::Red );
        mPen = new AggOO::Pen( mBrush, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
        for ( int i = 0; i < mCount; i++ )
        {
            AggOO::REAL x = rand() % mWidth;
            AggOO::REAL y = rand() % mHeight;
            AggOO::REAL w = rand() % mWidth;
            AggOO::REAL h = rand() % mHeight;

            gfx.DrawEllipse( mPen, x, y, w, h );
        }
        for ( int i = 0; i < mCount; i++ )
        {
            AggOO::REAL x = rand() % mWidth;
            AggOO::REAL y = rand() % mHeight;
            AggOO::REAL w = rand() % mWidth;
            AggOO::REAL h = rand() % mHeight;

            gfx.FillEllipse( mBrush, x, y, w, h );
        }
    }
};

class RectangleDemo : public DemoState
{
private:
    AggOO::Pen*     mPen;
    AggOO::REAL     mSize;
    AggOO::INT      mCount;

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
        mPen = new AggOO::Pen( AggOO::Color::DarkOliveGreen, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
        for ( int i = 0; i < mCount; i++ )
        {
            AggOO::REAL x = rand() % mWidth;
            AggOO::REAL y = rand() % mHeight;
            AggOO::REAL w = rand() % mWidth;
            AggOO::REAL h = rand() % mHeight;

            gfx.DrawRectangle( mPen, x, y, w, h );
        }
    }
};

class RoundRectangleDemo : public DemoState
{
private:
    AggOO::Pen*     mPen;
    AggOO::REAL     mSize;
    AggOO::INT      mCount;

public:
    RoundRectangleDemo( int w, int h )
        : DemoState( w, h ), mPen( 0 ), mSize( 4 ), mCount( 20 )
    {
    }
    virtual ~RoundRectangleDemo()
    {
        delete mPen;
    }

    virtual void on_init()
    {
        mPen = new AggOO::Pen( AggOO::Color::MistyRose, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::GraphicsExt gfx( img );
        gfx.Clear( mBgColor );

        // Round-Rect
        gfx.DrawRoundRectangle( mPen, 10, 10, mWidth - 20, mHeight - 20, 20 );
    }
};

class PolygonDemo : public DemoState
{
private:
    AggOO::Pen*     mPen;
    AggOO::REAL     mSize;
    AggOO::INT      mCount;
    AggOO::PointF*  mVertices;
    AggOO::INT      mVertCount;

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
        mVertices = new AggOO::PointF [ mVertCount ];
        for ( int i = 0; i < mVertCount; i++ )
        {
            AggOO::REAL x = rand() % mWidth;
            AggOO::REAL y = rand() % mHeight;
            mVertices[ i ] = AggOO::PointF( x, y );
        }

        mPen = new AggOO::Pen( AggOO::Color::DarkMagenta, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
        gfx.DrawPolygon( mPen, mVertices, mVertCount );
    }
};

class PieDemo : public DemoState
{
private:
    AggOO::Pen*     mPen;
    AggOO::REAL     mSize;
    AggOO::INT      mCount;

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
        mPen = new AggOO::Pen( AggOO::Color::YellowGreen, mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
        for ( int i = 0; i < 20; i++ )
        {
            AggOO::REAL x = rand() % mWidth;
            AggOO::REAL y = rand() % mHeight;
            AggOO::REAL radius = rand() % 200;
            AggOO::REAL startAngle = rand() % 360;
            AggOO::REAL endAngle = rand() % 360;

            gfx.DrawPie( mPen, x, y, radius, radius, startAngle, endAngle );
        }
    }
};

class MatrixDemo : public DemoState
{
private:
    AggOO::SolidBrush* mBrush;
    AggOO::REAL     mSize;
    AggOO::INT      mCount;
    AggOO::INT      mSegments;
    AggOO::PointF   mScale;

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
        mBrush = new AggOO::SolidBrush( AggOO::Color( 255, 0, 0, 0 ) );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
        gfx.ResetTransform();
        AggOO::REAL rotAngle = 360.0 / AggOO::REAL( mSegments );
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
    AggOO::Pen*     mPen;
    AggOO::REAL     mSize;
    AggOO::INT      mCount;
    AggOO::PointF*  mVertices;
    AggOO::INT      mVertCount;

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
        mVertices = new AggOO::PointF [ mVertCount ];
        mVertices[ 0 ] = AggOO::PointF( 50, 50 );
        mVertices[ 1 ] = AggOO::PointF( 200, 50 );
        mVertices[ 2 ] = AggOO::PointF( 180, 190 );
        mVertices[ 3 ] = AggOO::PointF( 70, 210 );
        mVertices[ 4 ] = AggOO::PointF( 120, 160 );
        mVertices[ 5 ] = AggOO::PointF( 103, 100 );
        mVertices[ 6 ] = AggOO::PointF( 70, 60 );

        mPen = new AggOO::Pen( AggOO::Color( 255, 200, 90, 60 ), mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
        gfx.DrawBeziers( mPen, mVertices, mVertCount );
    }
};

class CurveDemo : public DemoState
{
private:
    AggOO::Pen*     mCardinalPen;
    AggOO::Pen*     mClosedPen;
    AggOO::REAL     mSize;
    AggOO::INT      mCount;
    AggOO::PointF*  mCardinalVerts;
    AggOO::PointF*  mClosedVerts;
    AggOO::INT      mVertCount;
    AggOO::REAL     mCurAngle;

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
        mBgColor = AggOO::ColorExt::RandomRGB();

        mCardinalVerts = new AggOO::PointF [ mVertCount ];
        mCardinalVerts[ 0 ] = AggOO::PointF( 100, 100 );
        mCardinalVerts[ 1 ] = AggOO::PointF( mWidth - 100, 100 );
        mCardinalVerts[ 2 ] = AggOO::PointF( mWidth - 100, mHeight - 100 );
        mCardinalVerts[ 3 ] = AggOO::PointF( 100, mHeight - 100 );
        mCardinalVerts[ 4 ] = AggOO::PointF( mWidth / 2.0, mHeight / 2.0 );
        mCardinalVerts[ 5 ] = AggOO::PointF( mWidth / 2.0, mHeight / 3.0 );

        mClosedVerts = new AggOO::PointF [ mVertCount ];
        mClosedVerts[ 0 ] = AggOO::PointF( 150, 150 );
        mClosedVerts[ 1 ] = AggOO::PointF( mWidth - 150, 150 );
        mClosedVerts[ 2 ] = AggOO::PointF( mWidth - 150, mHeight - 150 );
        mClosedVerts[ 3 ] = AggOO::PointF( 150, mHeight - 150 );
        mClosedVerts[ 4 ] = AggOO::PointF( mWidth / 2.0 + 20, mHeight / 2.0 - 20 );
        mClosedVerts[ 5 ] = AggOO::PointF( mWidth / 2.0 + 20, mHeight / 3.0 + 20 );

        mCardinalPen = new AggOO::Pen( AggOO::Color( 255, 10, 180, 60 ), mSize );
        mClosedPen   = new AggOO::Pen( AggOO::Color( 255, 10, 25, 170 ), mSize );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        if ( key == 'r' )
            mCurAngle += 5.0;
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
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
    AggOO::SolidBrush* mBrush;
    AggOO::PointF*  mVertices;
    AggOO::INT      mVertCount;
    AggOO::FillMode mFillMode;

public:
    FillModeDemo( int w, int h )
        : DemoState( w, h ), mBrush( 0 ), mVertCount( 20 ), mVertices( 0 ), mFillMode( AggOO::FillModeAlternate )
    {
    }
    virtual ~FillModeDemo()
    {
        delete mBrush;
        delete mVertices;
    }

    virtual void on_init()
    {
        mVertices = new AggOO::PointF [ mVertCount ];
        for ( int i = 0; i < mVertCount; i++ )
        {
            AggOO::REAL x = rand() % mWidth;
            AggOO::REAL y = rand() % mHeight;
            mVertices[ i ] = AggOO::Point( x, y );
        }

        mBrush = new AggOO::SolidBrush( AggOO::Color::DarkMagenta );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        if ( key == AggOO::KV_F1 )
        {
            mFillMode = ( mFillMode == AggOO::FillModeAlternate ) ? AggOO::FillModeWinding : AggOO::FillModeAlternate;
        }
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
        gfx.FillPolygon( mBrush, mVertices, mVertCount, mFillMode );
    }
};

class TextureBrushDemo : public DemoState
{
private:
    AggOO::TextureBrush* mBrush;
    AggOO::Bitmap* mTexture;

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
        AggOO::Rect texRect( 0, 0, 40, 40 );
        mTexture = new AggOO::Bitmap( texRect.Width, texRect.Height, AggOO::PixelFormat32bppARGB );
//        AggOO::Bitmap tex( texRect.Width, texRect.Height, AggOO::PixelFormat32bppARGB );
        AggOO::BitmapData texData;
        mTexture->LockBits( &texRect, AggOO::ImageLockModeWrite, mTexture->GetPixelFormat(), &texData );
//        tex.LockBits( &texRect, AggOO::ImageLockModeWrite, tex.GetPixelFormat(), &texData );

        unsigned char *pixels = (unsigned char*)texData.Scan0;
        for ( size_t y = 0; y < texRect.Height; y++ )
        {
            AggOO::REAL yRatio = y / AggOO::REAL( texRect.Height );
            for ( size_t x = 0; x < texRect.Width; x++ )
            {
                AggOO::REAL xRatio = x / AggOO::REAL( texRect.Width );

                *pixels++ = 255;
                *pixels++ = 255 * xRatio;
                *pixels++ = 255 * yRatio;
                *pixels++ = 0;
            }
        }

        mTexture->UnlockBits( &texData );
//        tex.UnlockBits( &texData );

        // Create a brush:
        mBrush = new AggOO::TextureBrush( mTexture, AggOO::WrapModeTileFlipXY );
        mBrush->RotateTransform( 45 );
        mBrush->ScaleTransform( 0.5, 0.5 );
//        mBrush = new AggOO::TextureBrush( &tex, AggOO::WrapModeTileFlipXY );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        LogFileManager& lfm = LogFileManager::getInstance();
        LogFileSection sect( lfm.GetDefaultLog(), "TextureBrushDemo::on_draw(...)" );

        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );

        // Fill a rectangle:
        AggOO::SolidBrush br( AggOO::ColorExt::RandomRGB() );
//        gfx.FillRectangle( &br, 50, 50, mWidth - 100, mHeight - 100 );
        gfx.FillRectangle( mBrush, 50, 50, mWidth - 100, mHeight - 100 );
//        gfx.DrawImage( mBrush->GetImage(), 0, 0, mTexture->GetWidth(), mTexture->GetHeight() );
    }
};

class LinearGradientBrushDemo : public DemoState
{
private:
    AggOO::LinearGradientBrush* mBrush;
    AggOO::Rect mBounds;
    AggOO::LinearGradientMode mGradientDir;
    AggOO::GradientInterpolation mInterpMode;

public:
    LinearGradientBrushDemo( int w, int h )
        : DemoState( w, h ),
          mBrush( 0 ),
          mGradientDir( AggOO::LinearGradientModeHorizontal ),
          mInterpMode( AggOO::GradientInterpolationLinear )
    {
    }
    virtual ~LinearGradientBrushDemo()
    {
        delete mBrush;
    }

    virtual void on_init()
    {
        mBounds = AggOO::Rect( 100, 100, mWidth - 200, mHeight - 200 );

        // Create a brush:
        if ( mBrush )   delete mBrush;
//        mBrush = new AggOO::LinearGradientBrush( AggOO::Point( mBounds.X, mBounds.Y ), AggOO::Point( mBounds.GetRight(), mBounds.Y ), AggOO::Color::Blue, AggOO::Color::Green );
        mBrush = new AggOO::LinearGradientBrush( mBounds, AggOO::Color::Blue, AggOO::Color::Green, mGradientDir );
//        mBrush = new AggOO::LinearGradientBrush( mBounds, AggOO::Color::Blue, AggOO::Color::Green, AggOO::LinearGradientModeHorizontal );

        AggOO::Color gradColors[] = {\
            AggOO::Color( 255, 255, 0, 0 ),\
            AggOO::Color( 255, 255, 255, 0 ),\
            AggOO::Color( 255, 0, 255, 255 ),\
            AggOO::Color( 255, 255, 0, 255 ),\
            AggOO::Color( 255, 255, 255, 255 ),\
            AggOO::Color( 255, 0, 0, 0 ) };
        AggOO::REAL gradPos[] = {\
            0.0,\
            0.2,\
            0.25,\
            0.4,\
            0.89,\
            1.0 };

        int numColors = sizeof( gradPos ) / sizeof( gradPos[ 0 ] );
        mBrush->SetInterpolationColors( gradColors, gradPos, numColors );
        mBrush->SetGradientInterpolationMode( mInterpMode );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        switch ( key )
        {
        case 'r':
            mGradientDir = AggOO::LinearGradientMode( ( mGradientDir + 1 ) % ( AggOO::LinearGradientModeBackwardDiagonal + 1 ) );
            on_init();
            break;

        case 's':
            {
                AggOO::GradientStyle style = AggOO::GradientStyle( ( mBrush->GetGradientStyle() + 1 ) % ( AggOO::GradientStyleSpiral + 1 ) );
                mBrush->SetGradientStyle( style );
            }
            break;

        case 'w':
            {
                AggOO::WrapMode mode = AggOO::WrapMode( ( mBrush->GetWrapMode() + 1 ) % ( AggOO::WrapModeClamp + 1 ) );
                mBrush->SetWrapMode( mode );
            }
            break;

        case 'i':
            {
                mInterpMode = AggOO::GradientInterpolation( ( mInterpMode + 1 ) % ( AggOO::GradientInterpolationStep + 1 ) );
                mBrush->SetGradientInterpolationMode( mInterpMode );
            }
            break;
        }
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );

        // Fill a rectangle:
        gfx.FillRectangle( mBrush, mBounds );
    }
};

class DrawImageDemo : public DemoState
{
private:
    AggOO::Bitmap* mTexture;

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
        AggOO::Rect texRect( 0, 0, 40, 40 );
        mTexture = new AggOO::Bitmap( texRect.Width, texRect.Height, AggOO::PixelFormat32bppARGB );
        AggOO::BitmapData texData;
        mTexture->LockBits( &texRect, AggOO::ImageLockModeWrite, mTexture->GetPixelFormat(), &texData );

        unsigned char *pixels = (unsigned char*)texData.Scan0;
        for ( size_t y = 0; y < texRect.Height; y++ )
        {
            int yCell = y / 20;
            for ( size_t x = 0; x < texRect.Width; x++ )
            {
                int xCell = x / 20;

                if ( ( xCell % 2 ) == ( yCell % 2 ) )
                {
                    *pixels++ = 255;
                    *pixels++ = 255;
                    *pixels++ = 0;
                    *pixels++ = 0;
                }
                else
                {
                    *pixels++ = 255;
                    *pixels++ = 0;
                    *pixels++ = 0;
                    *pixels++ = 0;
                }

            }
        }
        mTexture->UnlockBits( &texData );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::Graphics gfx( img );
        gfx.Clear( mBgColor );
gfx.TranslateTransform( 10, 30 );
gfx.RotateTransform( 45.0 );

        gfx.DrawImage( mTexture, 0, 0 );
    }
};

class StarsDemo : public DemoState
{
private:
    AggOO::Pen*     mPen;
    AggOO::SolidBrush* mBrush;
    AggOO::INT      mCount;
    AggOO::FillMode mFillMode;
    AggOO::INT      mPointCount;

public:
    StarsDemo( int w, int h )
        : DemoState( w, h ), mBrush( 0 ), mPen( 0 ), mCount( 10 ), mPointCount( 5 )
    {
    }
    virtual ~StarsDemo()
    {
        delete mBrush;
        delete mPen;
    }

    virtual void on_init()
    {
        mBrush = new AggOO::SolidBrush( AggOO::ColorExt::RandomRGB() );
        mPen   = new AggOO::Pen( AggOO::ColorExt::RandomRGB(), 3 );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        switch ( key )
        {
        case ' ':
            mFillMode = AggOO::FillMode( ( mFillMode + 1 ) % ( AggOO::FillModeWinding + 1 ) );
            break;

        case '-':
            mPointCount = AggOO::IMinClamp( mPointCount - 1, 2 );
            break;

        case '=':
        case '+':
            ++mPointCount;
            break;
        }
    }

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::GraphicsExt gfx( img );
        gfx.Clear( mBgColor );

        // Fill some stars:
        for ( int i = 0; i < mCount; i++ )
        {
            AggOO::INT  numPoints   = AggOO::IRangeRandom( 3, 6 );
            AggOO::REAL outerRadius = AggOO::RangeRandom( 10, mWidth / 2.0 );
            AggOO::REAL innerRadius = outerRadius * AggOO::RangeRandom( 0.25, 0.75 );
            AggOO::REAL xPos        = AggOO::RangeRandom( 0, mWidth );
            AggOO::REAL yPos        = AggOO::RangeRandom( 0, mHeight );

            mBrush->SetColor( AggOO::ColorExt::RandomRGB() );
//            gfx.FillStar( mBrush, numPoints, AggOO::PointF( xPos, yPos ), innerRadius, outerRadius, mFillMode );
            gfx.FillStar( mBrush, numPoints, AggOO::PointF( xPos, yPos ), innerRadius, outerRadius );
        }

        // Stroke some stars:
        for ( int i = 0; i < mCount; i++ )
        {
            AggOO::INT  numPoints   = AggOO::IRangeRandom( 2, 10 );
            AggOO::REAL outerRadius = AggOO::RangeRandom( 10, mWidth / 2.0 );
            AggOO::REAL innerRadius = outerRadius * AggOO::RangeRandom( 0.25, 0.75 );
            AggOO::REAL xPos        = AggOO::RangeRandom( 0, mWidth );
            AggOO::REAL yPos        = AggOO::RangeRandom( 0, mHeight );

            gfx.DrawStar( mPen, numPoints, AggOO::PointF( xPos, yPos ), innerRadius, outerRadius );
        }
    }
};

class DropShadowDemo : public DemoState
{
private:
//    agg::slider_ctrl<agg::rgba8>     mShadowOffsetX;
//    agg::slider_ctrl<agg::rgba8>     mShadowOffsetY;
//    agg::slider_ctrl<agg::rgba8>     mShadowDarkness;
    AggOO::PointF   mShadowOffset;
    AggOO::REAL     mShadowDarkness;
    AggOO::INT      mCount;

    struct RectData
    {
        AggOO::Rect    bounds;
        AggOO::REAL    radius;
        AggOO::Color   color;
    };
    RectData*       mRectArray;

public:
    DropShadowDemo( int w, int h )
        : DemoState( w, h ), mRectArray( 0 ), mCount( 20 ),
          mShadowOffset( AggOO::PointF( 10, 10 ) ),
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
            AggOO::INT width  = AggOO::IRangeRandom( 1, mWidth );
            AggOO::INT height = AggOO::IRangeRandom( 1, mHeight );
            AggOO::INT xPos   = AggOO::IRangeRandom( 0, mWidth );
            AggOO::INT yPos   = AggOO::IRangeRandom( 0, mHeight );

            mRectArray[ i ].bounds = AggOO::Rect( xPos, yPos, width, height );
            mRectArray[ i ].radius = AggOO::RangeRandom( 0.0, min( width, height ) / 2.0 );
            mRectArray[ i ].color  = AggOO::ColorExt::RandomRGB();
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

    virtual void on_draw( AggOO::Image* img )
    {
        AggOO::GraphicsExt gfx( img );
        gfx.Clear( mBgColor );

        // Brush for the shadows:
        AggOO::SolidBrush shadowBrush( AggOO::Color( mShadowDarkness * 255, 0, 0, 0 ) );

        // Render the shapes:
        for ( int i = 0; i < mCount; i++ )
        {
            // Render the shadow first:
            AggOO::GraphicsState state = gfx.Save();
            gfx.TranslateTransform( mShadowOffset.X, mShadowOffset.Y );
            gfx.FillRoundRectangle( &shadowBrush, mRectArray[ i ].bounds, mRectArray[ i ].radius );
//            gfx.TranslateTransform( -mShadowOffset.X, -mShadowOffset.Y );
            gfx.Restore( state );

            AggOO::SolidBrush br( mRectArray[ i ].color );
            gfx.FillRoundRectangle( &br, mRectArray[ i ].bounds, mRectArray[ i ].radius );
        }
    }
};



/*
    The demo application is derived from the modified AGG framework.
*/
class MyApplication : public AggOO::modified_agg_app
{
private:
    enum { Dots, Lines, Arcs, Ellipses, Rectangles, RoundRects, Polygons, Pies, Matrix, Beziers, Curves, FillMode, TextureBrush, DrawImage, LinearGradientBrush, Stars, DropShadows, SceneCount };

    DemoState**     mStates;
    DemoState*      mCurState;
    unsigned int    mCurStateIdx;

	ULONG_PTR	mGdiplusToken;

    agg::ctrl_container* mCtrls;

public:
    MyApplication( AggOO::PixelFormat pixFormat, bool flipY )
        : AggOO::modified_agg_app( pixFormat, flipY ),
          mCurStateIdx( 0 ),
          mStates( 0 ),
          mCurState( 0 ),
          mCtrls( 0 )
    {
	    AggOO::GdiplusStartupInput	gdiplusStartupInput;
	    AggOO::GdiplusStartup( &mGdiplusToken, &gdiplusStartupInput, NULL );

        // Create the log file:
        LogFileManager& lfm = LogFileManager::getInstance();
        std::string logFile = "AggOODemo.log";
        lfm.CreateLog( logFile, true, LogFile::LoggingLevel::Everything );

/*
        m_trans_type.add_item("Affine Parallelogram");
        m_trans_type.add_item("Bilinear");
        m_trans_type.add_item("Perspective");
        m_trans_type.cur_item(2);
        add_ctrl(m_trans_type);
*/
    }

    virtual ~MyApplication()
    {
        for ( int i = 0; i < SceneCount; i++ )
            delete mStates[ i ];
        delete mStates;

        delete mCtrls;

    	AggOO::GdiplusShutdown( mGdiplusToken );
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
        mStates[ RoundRects     ] = new RoundRectangleDemo( w, h );
        mStates[ Polygons       ] = new PolygonDemo( w, h );
        mStates[ Pies           ] = new PieDemo( w, h );
        mStates[ Matrix         ] = new MatrixDemo( w, h );
        mStates[ Beziers        ] = new BezierDemo( w, h );
        mStates[ Curves         ] = new CurveDemo( w, h );
        mStates[ FillMode       ] = new FillModeDemo( w, h );
        mStates[ TextureBrush   ] = new TextureBrushDemo( w, h );
        mStates[ DrawImage      ] = new DrawImageDemo( w, h );
        mStates[ LinearGradientBrush ] = new LinearGradientBrushDemo( w, h );
        mStates[ Stars          ] = new StarsDemo( w, h );
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

        AggOO::Bitmap bmp( width(), height(), AggOO::PixelFormat32bppARGB );
        AggOO::GraphicsExt gfx( &bmp );
        gfx.Clear( AggOO::Color::Blue );

        // Draw the current state:
        mCurState->on_draw( &bmp );

        AggOO::BitmapData bmpData;
        AggOO::Rect bmpRect( 0, 0, width(), height() );
        bmp.LockBits( &bmpRect, AggOO::ImageLockModeWrite, format_aggoo(), &bmpData );

        setPixelData( static_cast< unsigned char* >( bmpData.Scan0 ), bmpData.Width, bmpData.Height, bmpData.PixelFormat );

        bmp.UnlockBits( &bmpData );
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
};


//int aggoo_main( int argc, char* argv[] )
int agg_main( int argc, char* argv[] )
{
    // Rendering with AGG is quite a bit simpler than using GDI+, since the
    // framework uses AGG underneath the hood.  Basically, we can render
    // directly to the rendering buffer used by the application.
    MyApplication app( AggOO::PixelFormat32bppARGB, true );
    app.caption( "AggOO Basic Demo" );

    if ( app.init( 640, 480, AggOO::WindowStyleResize ) )
    {
        return app.run();
    }

    return 1;
}
