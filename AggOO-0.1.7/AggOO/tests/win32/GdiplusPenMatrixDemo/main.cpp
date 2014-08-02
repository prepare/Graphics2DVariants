
// Demo of the GDI+ Pen Matrix

#include <windows.h>
#include "AggOO.h"
#include "Gdiplus.h"
#include "platform/platform_app.h"
#include "ctrl/agg_slider_ctrl.h"

#include <platform/LogFile.h>
cmd::LogFile logFile( "GdiplusPenMatrixDemo.log" );

#pragma comment( lib, "gdiplus.lib" )


/*
    The demo application is derived from the modified AGG framework.
*/
class MyApplication : public AggOO::modified_agg_app
{
private:

	ULONG_PTR	mGdiplusToken;

    agg::slider_ctrl<agg::rgba8>* mScale;

public:
    MyApplication( AggOO::PixelFormat pixFormat, bool flipY )
        : AggOO::modified_agg_app( pixFormat, flipY ),
          mScale( 0 )
    {
	    Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	    Gdiplus::GdiplusStartup( &mGdiplusToken, &gdiplusStartupInput, NULL );
    }

    virtual ~MyApplication()
    {

    	Gdiplus::GdiplusShutdown( mGdiplusToken );
    }

    virtual void on_init()
    {
        int w = width();
        int h = height();
        mScale = new agg::slider_ctrl<agg::rgba8>( 10, h - 20, w - 20, h - 10, flip_y() );

        mScale->range( 0.0, 5.0 );
        mScale->value( 1.0 );
        mScale->label( "Scale = %f" );
        mScale->no_transform();
        add_ctrl( *mScale );
    }

    virtual void on_key( int x, int y, unsigned int key, unsigned int flags )
    {
        AggOO::modified_agg_app::on_key( x, y, key, flags );

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

        Gdiplus::REAL scale = mScale->value();
        Gdiplus::PointF scalePt( w / 2.0 * scale, h / 2.0 * scale );
//        gfx.TranslateTransform( w / 2.0, h / 2.0 );
//        gfx.ScaleTransform( scalePt.X, scalePt.Y );

        Gdiplus::Matrix partMtx, shapeMtx;
        partMtx.Translate( w / 2.0, h / 2.0 );

        shapeMtx.Rotate( 0.0 );
        shapeMtx.Scale( scalePt.X, scalePt.Y );
//        shapeMtx.Rotate( 90.0 );
        float angle = rand() % 360;
        shapeMtx.Rotate( angle );

        gfx.SetTransform( &shapeMtx );
        gfx.MultiplyTransform( &partMtx, Gdiplus::MatrixOrderAppend );

        Gdiplus::Bitmap bmpComposite( w, h, pixFmt );
        Gdiplus::Graphics gfxComposite( &bmpComposite );

        Gdiplus::BitmapData bmpData;
        Gdiplus::Rect rcBmp( 0, 0, bmp.GetWidth(), bmp.GetHeight() );

        Gdiplus::SolidBrush br( Gdiplus::Color::Blue );
//        gfx.FillEllipse( &br, -1.0f, -1.0f, 2.0f, 2.0f );
//        gfx.FillEllipse( &br, rcBmp );

        float penWidth = 5.0 / scalePt.X;
//        Gdiplus::Pen pen( Gdiplus::Color::Magenta, 10 / scalePt.X );
        Gdiplus::Pen pen( Gdiplus::Color::Magenta, penWidth );
        gfx.DrawEllipse( &pen, -1.0f, -1.0f, 2.0f, 2.0f );
//        gfx.DrawEllipse( &pen, rcBmp );

/*
        Gdiplus::PointF pts[ 3 ];
        Gdiplus::REAL angPerVert = ( AggOO::pi / 180.0 ) * ( 120.0 );
        Gdiplus::REAL ang = 0.0;
        Gdiplus::REAL rad = 1.0;

        AggOO::PolarToRect( ang, rad, &pts[ 0 ].X, &pts[ 0 ].Y, 0.0, 0.0 );
        ang += angPerVert;

        AggOO::PolarToRect( ang, rad, &pts[ 1 ].X, &pts[ 1 ].Y, 0.0, 0.0 );
        ang += angPerVert;

        AggOO::PolarToRect( ang, rad, &pts[ 2 ].X, &pts[ 2 ].Y, 0.0, 0.0 );
        ang += angPerVert;

        gfx.DrawPolygon( &pen, pts, 3 );
*/

        gfxComposite.DrawImage( &bmp, 0, 0 );
        bmpComposite.LockBits( &rcBmp, Gdiplus::ImageLockModeRead, pixFmt, &bmpData );

        //----------------------------------------------------------------------
        // Render the controls
        typedef agg::renderer_base<agg::pixfmt_argb32> ren_base;
        agg::rendering_buffer rbuf( (unsigned char*)bmpData.Scan0, bmpData.Width, bmpData.Height, bmpData.Stride );
        agg::pixfmt_argb32 pixf( rbuf );
        ren_base renb(pixf);
        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_u8 sl;

        agg::render_ctrl( ras, sl, renb, *mScale );
        //

        setPixelData( static_cast< unsigned char* >( bmpData.Scan0 ), bmpData.Width, bmpData.Height, bmpData.PixelFormat );

        bmpComposite.UnlockBits( &bmpData );
    }
};

int agg_main( int argc, char* argv[] )
{
    // Rendering with AGG is quite a bit simpler than using GDI+, since the
    // framework uses AGG underneath the hood.  Basically, we can render
    // directly to the rendering buffer used by the application.
    MyApplication app( AggOO::PixelFormat32bppBGRA, true );
    app.caption( "GDI+ Pen Width Demo" );

    if ( app.init( 640, 480, AggOO::WindowStyleResize ) )
    {
        return app.run();
    }

    return 1;
}
