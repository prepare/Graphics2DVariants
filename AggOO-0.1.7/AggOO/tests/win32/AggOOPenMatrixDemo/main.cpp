
// Demo of the AggOO Pen Matrix

#include "AggOO.h"
#include "platform/platform_app.h"
#include "ctrl/agg_slider_ctrl.h"

#include <platform/LogFile.h>
cmd::LogFile logFile( "AggOOPenMatrixDemo.log" );


/*
    The demo application is derived from the modified AGG framework.
*/
class MyApplication : public AggOO::modified_agg_app
{
private:

	ULONG_PTR	mAggOOToken;

    agg::slider_ctrl<agg::rgba8>* mScale;

public:
    MyApplication( AggOO::PixelFormat pixFormat, bool flipY )
        : AggOO::modified_agg_app( pixFormat, flipY ),
          mScale( 0 )
    {
    }

    virtual ~MyApplication()
    {
        delete mScale;
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
        AggOO::PixelFormat pixFmt = AggOO::PixelFormat32bppARGB;
//        AggOO::PixelFormat pixFmt = format_aggoo();
        AggOO::Bitmap bmp( w, h, pixFmt );
        AggOO::Graphics gfx( &bmp );
//-        gfx.SetSmoothingMode( AggOO::SmoothingModeAntiAlias );
        gfx.Clear( AggOO::Color::Beige );

        AggOO::REAL scale = mScale->value();
//        gfx.TranslateTransform( w / 2.0, h / 2.0 );
        AggOO::REAL xScale = w / 2.0 * scale;
        AggOO::REAL yScale = h / 2.0 * scale;
//        gfx.ScaleTransform( xScale, yScale );

        AggOO::Matrix partMtx, shapeMtx;
        partMtx.Translate( w / 2.0, h / 2.0 );

        shapeMtx.Rotate( 0.0 );
        shapeMtx.Scale( xScale, yScale );
        float angle = 24;
        shapeMtx.Rotate( angle );

        gfx.SetTransform( &shapeMtx );
        gfx.MultiplyTransform( &partMtx, AggOO::MatrixOrderAppend );

        AggOO::Bitmap bmpComposite( w, h, pixFmt );
        AggOO::Graphics gfxComposite( &bmpComposite );

        AggOO::BitmapData bmpData;
        AggOO::Rect rcBmp( 0, 0, bmp.GetWidth(), bmp.GetHeight() );

        AggOO::SolidBrush br( AggOO::Color::Blue );
//        gfx.FillEllipse( &br, -1.0f, -1.0f, 2.0f, 2.0f );

        float penWidth = 5.0 / xScale;
//        AggOO::Pen pen( AggOO::Color::Magenta, 20 );
//        AggOO::Pen pen( AggOO::Color::Red, 10 / xScale );
        AggOO::Pen pen( AggOO::Color::Red, penWidth );

        gfx.DrawEllipse( &pen, -1.0f, -1.0f, 2.0f, 2.0f );

        AggOO::PointF pts[ 3 ];
        AggOO::REAL angPerVert = ( AggOO::pi / 180.0 ) * ( 120.0 );
        AggOO::REAL ang = 0.0;
        AggOO::REAL rad = 1.0;

        AggOO::PolarToRect( ang, rad, &pts[ 0 ].X, &pts[ 0 ].Y, 0.0, 0.0 );
        ang += angPerVert;

        AggOO::PolarToRect( ang, rad, &pts[ 1 ].X, &pts[ 1 ].Y, 0.0, 0.0 );
        ang += angPerVert;

        AggOO::PolarToRect( ang, rad, &pts[ 2 ].X, &pts[ 2 ].Y, 0.0, 0.0 );
        ang += angPerVert;

//        gfx.DrawPolygon( &pen, pts, 3 );


        gfxComposite.DrawImage( &bmp, 0, 0 );
        bmpComposite.LockBits( &rcBmp, AggOO::ImageLockModeRead, pixFmt, &bmpData );

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
//    MyApplication app( AggOO::PixelFormat32bppBGRA, true );
    MyApplication app( AggOO::PixelFormat32bppARGB, true );
    app.caption( "AggOO Pen Width Demo" );

    if ( app.init( 640, 480, AggOO::WindowStyleResize ) )
    {
        return app.run();
    }

    return 1;
}
