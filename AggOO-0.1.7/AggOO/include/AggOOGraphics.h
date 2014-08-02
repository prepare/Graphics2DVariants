
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
// $Id: AggOOGraphics.h,v 1.14 2010/03/12 04:20:57 dratek Exp $
//
// AggOOGraphics.h -- Defines the rendering class
//
// Author       : Chad M. Draper
// Date         : September 14, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2006-09-14      Initial implementation
//

#ifndef AggOOGraphics_H_
#define AggOOGraphics_H_

#include "AggOOPlatform.h"
#include "AggOOEnums.h"
#include "AggOOBase.h"
#include "AggOOTypes.h"
#include "AggOOColor.h"
#include "AggOOImaging.h"
#include "AggOOMatrix.h"
#include "AggOOSharedPtr.h"
/*
#include "AggOOPixelFormats.h"
#include "AggOOEnums.h"
#include "AggOOBase.h"
#include "AggOOTypes.h"
#include "AggOOPrereqs.h"
#include "AggOOColor.h"
#include "AggOOImaging.h"
*/

#include "agg_basics.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_math_stroke.h"
#include "agg_scanline_p.h"
#include "agg_scanline_u.h"
//#include "agg_path_storage.h"
//#include "agg_renderer_base.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_compound_aa.h"

#include <stack>

namespace AggOO
{
    class Color;
    class Brush;
    class Pen;
    class Image;
    class GraphicsPath;
    class GraphicsPathCompound;
    class GraphicsRendererBase;
    class BitmapData;
    class ImageAttributes;
    class AbstractPixelFormat;
    class AbstractRendererBase;
    class AbstractScalineRenderer;

    //namespace svg
    //{
    //    class SVGPath;
    //}

    /** @class Graphics

            Class which provides methods for rendering image data.
    */
    class _AggOOExport Graphics
    {
    protected:
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
        typedef SharedPtr< AbstractPixelFormat >        PixFmtPtr;
        typedef SharedPtr< AbstractRendererBase >       RendererPtr;
        typedef SharedPtr< AbstractScanlineRenderer >   ScanlineRendererPtr;

    protected:
        BitmapData*             mActiveBitmap;
        agg::rendering_buffer   mRenderingBuffer;
        GraphicsRendererBase*   mRenderer;
        PixFmtPtr               mPixelFormat;
        RendererPtr             mRendererBase;
        ScanlineRendererPtr     mScanlineRendererAA;
        ScanlineRendererPtr     mScanlineRendererBin;
        //AbstractPixelFormat*    mPixelFormat;
        //AbstractRendererBase*   mRendererBase;
        //AbstractScanlineRenderer*       mScanlineRenderer;

        // Platform-specific attributes for rendering directly to the display
#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
        HDC     mHDC;           ///< Windows device context
        RECT    mClipRectDC;    ///< Clipping rectangle for the device context
        //HBITMAP mHBitmap;       ///< Windows bitmap, attached to mHDC
        //HBITMAP mOldBitmap;     ///< Bitmap originally attached to the device context

        /** Create the GraphicsObject from an HDC */
        void createFromHDC( HDC hdc, ColorFormat colFmt = ColorFormatDefault );
#endif

        agg::rasterizer_scanline_aa<>   mRasterizer;
        typedef agg::scanline_u8        scanline_type;
        scanline_type                   mScanline;

        Matrix                      mMatrix;
        //FillMode                    mFillMode;
        PixelFormat                 mAggOOPixelFormat;
        CompositingMode             mCompositeMode;
        CompositingQuality          mCompositeQuality;
        InterpolationMode           mInterpolationMode;
        REAL                        mPageScale;
        Unit                        mPageUnit;
        PixelOffsetMode             mPixelOffsetMode;
        Point                       mRenderingOrigin;
        SmoothingMode               mSmoothingMode;
        REAL                        mGammaThreshold;
        UINT                        mBusyCount;             ///<< Counter for when an object is locked/released.  Unless this is 0, the ObjectBusy error should be returned.

        // Create the rendering_buffer
        //Status createRenderingBuffer( BYTE* scan0, INT width, INT height, INT rowWidth );

        // There are multiple ways to instantiate the graphics class.  This
        // function will be used to create the object.
        Status createGraphics( INT width, INT height, INT rowWidth, VOID* scan0, PixelFormat pixelFormat, ColorFormat colorFormat );

        /** Method which will render the path.  Whether the path is drawn or
            filled, this routine is the same.
        */
        Status renderPath( const Brush* brush, const GraphicsPath* path );

    private:
        // Structure to hold the GraphicsState data for Save/Restore
        struct GraphicsStateData
        {
            Matrix              matrix;
//            FillMode            fillMode;
            CompositingMode     compositeMode;
            CompositingQuality  compositeQuality;
            InterpolationMode   interpolationMode;
            REAL                pageScale;
            Unit                pageUnit;
            PixelOffsetMode     pixelOffsetMode;
            Point               renderingOrigin;
            SmoothingMode       smoothingMode;
            REAL                gammaThreshold;

            /*
                // TODO:
                    clipping region
                    dpi
                    text contrast ?
                    text rendering hint ?
            */
        };

        // Stack which stores the saved graphics states
        std::stack< GraphicsStateData > mGraphicsStates;

    public:
        /** Constructor */
        Graphics( Image* image, ColorFormat colFmt = ColorFormatPlain );
        /** Create a Graphics object from an image */
        static Graphics* FromImage( Image* image );
        /** Destructor */
        virtual ~Graphics(void);

#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )

        /** Constructor */
        Graphics( HDC hdc, ColorFormat colFmt = ColorFormatDefault );
        /** Create a Graphics object from an HDC */
        static Graphics* FromHDC( HDC hdc, ColorFormat colFmt = ColorFormatDefault );
        /** Constructor */
        Graphics( HDC hdc, HANDLE hdevice, ColorFormat colFmt = ColorFormatDefault );
        /** Create a Graphics object from an HDC */
        static Graphics* FromHDC( HDC hdc, HANDLE hdevice, ColorFormat colFmt = ColorFormatDefault );
        /** Constructor */
        Graphics( HWND hwnd, bool icm, ColorFormat colFmt = ColorFormatDefault );
        /** Create a Graphics object from an HWND */
        static Graphics* FromHWND( HWND hWnd, BOOL icm, ColorFormat colFmt = ColorFormatDefault );

        /** Returns the handle to the device context associated with the Graphics
            object.  Each call to GetHDC must be paired with a call to ReleaseHDC,
            and no calls on the Graphics object must take place between these.
            This allows mixing GDI calls with AggOO.
        */
        HDC GetHDC(void);

        /** Whenever GetHDC is called, the device context should be considered
            locked, and no calls on the Graphics object should take place until
            ReleaseHDC is called.
        */
        VOID ReleaseHDC( HDC hdc );

#endif


        /** Return the previously set status */
        Status GetLastStatus() const    { return mLastStatus; }

        /** Clear the graphics object to the specified color
        */
        Status Clear( const Color& color );

        /** Save the current graphics state
            @remarks
            Saves the current transformation matrix, clipping region, and
            quality and mode settings.
        */
        GraphicsState Save(void);

        //! Restore the graphics state
        Status Restore( GraphicsState gstate );

        /** Draw a line segment */
        Status DrawLine( const Pen* pen, const Point& pt1, const Point& pt2 );
        /** Draw a line segment */
        Status DrawLine( const Pen* pen, const PointF& pt1, const PointF& pt2 );
        /** Draw a line segment */
        Status DrawLine( const Pen* pen, INT x1, INT y1, INT x2, INT y2 );
        /** Draw a line segment */
        Status DrawLine( const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2 );

        /** Draw a sequence of lines.  Each line consists of a starting and end
            point.
        */
        Status DrawLines( const Pen* pen, const Point* points, INT count );
        /** Draw a sequence of lines.  Each line consists of a starting and end
            point.
        */
        Status DrawLines( const Pen* pen, const PointF* points, INT count );

        /** Draw an arc */
        Status DrawArc( const Pen* pen, const Rect& rect, REAL startAngle, REAL sweepAngle );
        /** Draw an arc */
        Status DrawArc( const Pen* pen, const RectF& rect, REAL startAngle, REAL sweepAngle );
        /** Draw an arc */
        Status DrawArc( const Pen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle );
        /** Draw an arc */
        Status DrawArc( const Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle );

        /** Draw an ellipse */
        Status DrawEllipse( const Pen* pen, const Rect& rect ); 
        /** Draw an ellipse */
        Status DrawEllipse( const Pen* pen, const RectF& rect ); 
        /** Draw an ellipse */
        Status DrawEllipse( const Pen* pen, INT x, INT y, INT width, INT height ); 
        /** Draw an ellipse */
        Status DrawEllipse( const Pen* pen, REAL x, REAL y, REAL width, REAL height ); 

        /** Fill an ellipse */
        Status FillEllipse( const Brush* brush, const Rect& rect ); 
        /** Fill an ellipse */
        Status FillEllipse( const Brush* brush, const RectF& rect ); 
        /** Fill an ellipse */
        Status FillEllipse( const Brush* brush, INT x, INT y, INT width, INT height ); 
        /** Fill an ellipse */
        Status FillEllipse( const Brush* brush, REAL x, REAL y, REAL width, REAL height ); 

        /** Draw a GraphicsPath */
        Status DrawPath( const Pen* pen, const GraphicsPath* path );

        /** Fill the interior of a GraphicsPath */
        Status FillPath( const Brush* brush, const GraphicsPath* path );

        /** Render a GraphicsPathCompound */
        Status RenderCompoundPath( const GraphicsPathCompound* path );

        /** Draw a polygon */
        Status DrawPolygon( const Pen* pen, const Point* points, INT count );
        /** Draw a polygon */
        Status DrawPolygon( const Pen* pen, const PointF* points, INT count );

        /** Fill a polygon */
        Status FillPolygon( const Brush* brush, const Point* points, INT count );
        /** Fill a polygon */
        Status FillPolygon( const Brush* brush, const PointF* points, INT count );
        /** Fill a polygon */
        Status FillPolygon( const Brush* brush, const Point* points, INT count, FillMode fillMode );
        /** Fill a polygon */
        Status FillPolygon( const Brush* brush, const PointF* points, INT count, FillMode fillMode );

        /** Draw a rectangle */
        Status DrawRectangle( const Pen* pen, const Rect& rect );
        /** Draw a rectangle */
        Status DrawRectangle( const Pen* pen, const RectF& rect );
        /** Draw a rectangle */
        Status DrawRectangle( const Pen* pen, INT x, INT y, INT width, INT height );
        /** Draw a rectangle */
        Status DrawRectangle( const Pen* pen, REAL x, REAL y, REAL width, REAL height );

        /** Draw a sequence of rectangles.  Each polygon consists of 4 points. */
        Status DrawRectangles( const Pen* pen, const Rect* rects, INT count );
        /** Draw a sequence of rectangles.  Each polygon consists of 4 points. */
        Status DrawRectangles( const Pen* pen, const RectF* rects, INT count );

        /** Fill a rectangle */
        Status FillRectangle( const Brush* brush, const Rect& rect );
        /** Fill a rectangle */
        Status FillRectangle( const Brush* brush, const RectF& rect );
        /** Fill a rectangle */
        Status FillRectangle( const Brush* brush, INT x, INT y, INT width, INT height );
        /** Fill a rectangle */
        Status FillRectangle( const Brush* brush, REAL x, REAL y, REAL width, REAL height );

        /** Fill a sequence of rectangles */
        Status FillRectangles( const Brush* brush, const Rect* rects, INT count );
        /** Fill a sequence of rectangles */
        Status FillRectangles( const Brush* brush, const RectF* rects, INT count );

        /** Draw a pie shape */
        Status DrawPie( const Pen* pen, const Rect& rect, REAL startAngle, REAL sweepAngle );
        /** Draw a pie shape */
        Status DrawPie( const Pen* pen, const RectF& rect, REAL startAngle, REAL sweepAngle );
        /** Draw a pie shape */
        Status DrawPie( const Pen* pen, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle );
        /** Draw a pie shape */
        Status DrawPie( const Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle );

        /** Fill a pie shape */
        Status FillPie( const Brush* brush, const Rect& rect, REAL startAngle, REAL sweepAngle );
        /** Fill a pie shape */
        Status FillPie( const Brush* brush, const RectF& rect, REAL startAngle, REAL sweepAngle );
        /** Fill a pie shape */
        Status FillPie( const Brush* brush, INT x, INT y, INT width, INT height, REAL startAngle, REAL sweepAngle );
        /** Fill a pie shape */
        Status FillPie( const Brush* brush, REAL x, REAL y, REAL width, REAL height, REAL startAngle, REAL sweepAngle );

        /** Draw a bezier spline */
        Status DrawBezier( const Pen* pen, const Point& pt1, const Point& pt2, const Point& pt3, const Point& pt4 );
        /** Draw a bezier spline */
        Status DrawBezier( const Pen* pen, const PointF& pt1, const PointF& pt2, const PointF& pt3, const PointF& pt4 );
        /** Draw a bezier spline */
        Status DrawBezier( const Pen* pen, INT x1, INT y1, INT x2, INT y2, INT x3, INT y3, INT x4, INT y4 );
        /** Draw a bezier spline */
        Status DrawBezier( const Pen* pen, REAL x1, REAL y1, REAL x2, REAL y2, REAL x3, REAL y3, REAL x4, REAL y4 );

        /** Draw a sequence of bezier splines
            @remarks
                Each bezier spline is made of 4 points.  The first and fourth
                points are the end-points, and the second and third are the
                control points.  When adding multiple splines, the fourth point
                of the previous spline is used as the first point of the current
                spline.  In other words, the first 4 points in the array are
                used for the first spline, the next 3, along with the last point
                from the first spline, are used for the second spline, and so on.
        */
        Status DrawBeziers( const Pen* pen, const Point* points, INT count );
        /** Draw a sequence of bezier splines
            @remarks
                Each bezier spline is made of 4 points.  The first and fourth
                points are the end-points, and the second and third are the
                control points.  When adding multiple splines, the fourth point
                of the previous spline is used as the first point of the current
                spline.  In other words, the first 4 points in the array are
                used for the first spline, the next 3, along with the last point
                from the first spline, are used for the second spline, and so on.
        */
        Status DrawBeziers( const Pen* pen, const PointF* points, INT count );

        /** Draw a closed curve */
        Status DrawClosedCurve( const Pen* pen, const Point* points, INT count );
        /** Draw a closed curve */
        Status DrawClosedCurve( const Pen* pen, const PointF* points, INT count );
        /** Draw a closed curve */
        Status DrawClosedCurve( const Pen* pen, const Point* points, INT count, REAL tension );
        /** Draw a closed curve */
        Status DrawClosedCurve( const Pen* pen, const PointF* points, INT count, REAL tension );

        /** Fill a closed curve */
        Status FillClosedCurve( const Brush* brush, const Point* points, INT count );
        /** Fill a closed curve */
        Status FillClosedCurve( const Brush* brush, const PointF* points, INT count );
        /** Fill a closed curve */
        Status FillClosedCurve( const Brush* brush, const Point* points, INT count, REAL tension );
        /** Fill a closed curve */
        Status FillClosedCurve( const Brush* brush, const PointF* points, INT count, REAL tension );

        /** Draw a curve which passes through each point */
        Status DrawCurve( const Pen* pen, const Point* points, INT count );
        /** Draw a curve which passes through each point */
        Status DrawCurve( const Pen* pen, const PointF* points, INT count );
        /** Draw a curve which passes through each point */
        Status DrawCurve( const Pen* pen, const Point* points, INT count, INT offset, INT numberOfSegments, REAL tension );
        /** Draw a curve which passes through each point */
        Status DrawCurve( const Pen* pen, const PointF* points, INT count, INT offset, INT numberOfSegments, REAL tension );
        /** Draw a curve which passes through each point */
        Status DrawCurve( const Pen* pen, const Point* points, INT count, REAL tension );
        /** Draw a curve which passes through each point */
        Status DrawCurve( const Pen* pen, const PointF* points, INT count, REAL tension );

        /** Draw an image */
        //Status DrawImage( Image* image, const Point* destPoints, INT count );
        /** Draw an image */
        //Status DrawImage( Image* image, const PointF* destPoints, INT count );
        /** Draw an image */
        //Status DrawImage( Image* image, const Point* destPoints, INT count, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL );
        /** Draw an image */
        //Status DrawImage( Image* image, const PointF* destPoints, INT count, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL );
        /** Draw an image */
//        Status DrawImage( Image* image, const Rect& destRect, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL );
        Status DrawImage( Image* image, const Rect& destRect, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit, ImageAttributes* imageAttributes = NULL );
        /** Draw an image */
//        Status DrawImage( Image* image, const RectF& destRect, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL );
        Status DrawImage( Image* image, const RectF& destRect, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit, ImageAttributes* imageAttributes = NULL );
        /** Draw an image */
        Status DrawImage( Image* image, const Point& point );
        /** Draw an image */
        Status DrawImage( Image* image, const PointF& point );
        /** Draw an image */
        Status DrawImage( Image* image, const Rect& rect );
        /** Draw an image */
        Status DrawImage( Image* image, const RectF& rect );
        /** Draw an image */
        Status DrawImage( Image* image, INT x, INT y );
        /** Draw an image */
        Status DrawImage( Image* image, REAL x, REAL y );
        /** Draw an image */
        Status DrawImage( Image* image, INT x, INT y, INT srcx, INT srcy, INT srcwidth, INT srcheight, Unit srcUnit );
        /** Draw an image */
        Status DrawImage( Image* image, REAL x, REAL y, REAL srcx, REAL srcy, REAL srcwidth, REAL srcheight, Unit srcUnit );
        /** Draw an image */
        Status DrawImage( Image* image, INT x, INT y, INT width, INT height );
        /** Draw an image */
        Status DrawImage( Image* image, REAL x, REAL y, REAL width, REAL height );

        ///** Render an SVG path
        //    @param  path        Path containing the SVG shape
        //    @param  mtx         Transformation matrix
        //    @param  clipBox     Clipping region
        //    @param  opacity     Opacity ratio
        //*/
        //Status RenderSVGPath( svg::SVGPath* path, const Matrix* mtx, const Rect& clipBox, REAL opacity = 1.0 );

        /** Translate the origin */
        Status TranslateTransform( REAL dx, REAL dy, MatrixOrder order = MatrixOrderPrepend );
        /** Rotate the surface */
        Status RotateTransform( REAL angle, MatrixOrder order = MatrixOrderPrepend );
        /** Scale the surface */
        Status ScaleTransform( REAL sx, REAL sy, MatrixOrder order = MatrixOrderPrepend );
        /** Multiply the current transformation with another matrix */
        Status MultiplyTransform( Matrix* matrix, MatrixOrder order = MatrixOrderPrepend );
        /** Get the current transformation */
        Status GetTransform( Matrix* matrix );
        /** Set the current transformation */
        Status SetTransform( const Matrix* matrix );
        /** Reset the transformation */
        Status ResetTransform(void);

        /** Get the current compositing mode */
        CompositingMode GetCompositingMode(void);
        /** Set the compositing mode */
        Status SetCompositingMode( CompositingMode compositingMode );

        /** Get the compositing quality */
        CompositingQuality GetCompositingQuality(void);
        /** Set the compositing quality */
        Status SetCompositingQuality( CompositingQuality compositingQuality );

        /** Get the smoothing (anti-aliasing) mode */
        SmoothingMode GetSmoothingMode(void) const;
        /** Set the smoothing mode */
        Status SetSmoothingMode( SmoothingMode smoothingMode );

        /** Set the gamma threshold (used when not antialiasing) */
        Status SetGammaThreshold( REAL gamma );
        /** Get the gamma threshold value */
        REAL GetGammaThreshold() const;
    };

    /** @remarks
        // TODO: The following methods exist in GDI+, but not AggOO (yet):
                 Note that some of these use Windows objects, such as HDC
                 and HWND.  In order to implement these on other platforms, it
                 will probably be necessary to create those structures in AggOO,
                 or simply ignore them.  We can use the preprocessor command
                    #if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
                 to exclude them, or to specify that the local structures should
                 be used.

                 Status AddMetafileComment( const BYTE* data, UINT sizeData );
                 Status EnumerateMetafile( const Metafile* metafile, const Point& destPoint, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const PointF& destPoint, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const Point& destPoint, const Rect& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const PointF& destPoint, const RectF& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const Point* destPoints, INT count, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const PointF* destPoints, INT count, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const Point* destPoints, INT count, const Rect& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const PointF* destPoints, INT count, const RectF& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const Rect& destRect, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const RectF& destRect, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const Rect& destRect, const Rect& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );
                 Status EnumerateMetafile( const Metafile* metafile, const RectF& destRect, const RectF& srcRect, Unit srcUnit, EnumerateMetafileProc callback, VOID* callbackData, ImageAttributes* imageAttributes );

                 GraphicsContainer BeginContainer(void);
                 GraphicsContainer BeginContainer(const Rect& dstrect, const Rect& srcrect, Unit unit);
                 GraphicsContainer BeginContainer(const RectF& dstrect, const RectF& srcrect, Unit unit);
                 Status EndContainer( GraphicsContainer state );

                 Status DrawCachedBitmap( CachedBitmap* cb, INT x, INT y );

                 Status ExcludeClip( const Rect& rect );
                 Status ExcludeClip( const RectF& rect );
                 Status ExcludeClip( const Region* rect );
                 Status GetClip( Region* region ) const;
                 Status GetClipBounds( Rect* rect ) const;
                 Status GetClipBounds( RectF* rect ) const;
                 Status IntersectClip( const Rect& rect );
                 Status IntersectClip( const RectF& rect );
                 Status IntersectClip( const Region* region);
                 BOOL IsClipEmpty(void) const;
                 Status ResetClip(void);
                 Status SetClip( const Graphics* g, CombineMode combineMode );
                 Status SetClip( HRGN hRgn, CombineMode combineMode );
                 Status SetClip( const GraphicsPath* path, CombineMode combineMode );
                 Status SetClip( const Rect& rect, CombineMode combineMode );
                 Status SetClip( const RectF& rect, CombineMode combineMode );
                 Status SetClip( const Region* region, CombineMode combineMode );
                 Status TranslateClip( INT dx, INT dy );
                 Status TranslateClip( REAL dx, REAL dy );

                 BOOL IsVisible( const Point& point ) const;
                 BOOL IsVisible( const PointF& point ) const;
                 BOOL IsVisible( const Rect& rect ) const;
                 BOOL IsVisible( const RectF& rect ) const;
                 BOOL IsVisible( INT x, INT y ) const;
                 BOOL IsVisible( REAL x, REAL y ) const;
                 BOOL IsVisible( INT x, INT y, INT width, INT height ) const;
                 BOOL IsVisible( REAL x, REAL y, REAL width, REAL height ) const;
                 BOOL IsVisibleClipEmpty(void) const;

                 Status FillRegion( const Brush* brush, const Region* region );

                 VOID Flush( FlushIntention intention );

                 static HPALETTE GetHalftonePalette(void);

                 InterpolationMode GetInterpolationMode(void);
                 Status SetInterpolationMode( InterpolationMode interpolationMode );

                 Status GetNearestColor( Color* color ) const;
                 REAL GetPageScale(void);
                 Status SetPageScale( REAL scale );
                 Unit GetPageUnit(void);
                 Status SetPageUnit( Unit unit );

                 PixelOffsetMode GetPixelOffsetMode(void);
                 Status SetPixelOffsetMode( PixelOffsetMode pixelOffsetMode );

                 Status GetRenderingOrigin( INT* x, INT* y );
                 Status SetRenderingOrigin( INT x, INT y );

                 UINT GetTextContrast(void) const;
                 Status SetTextContrast( UINT contrast );
                 TextRenderingHint GetTextRenderingHint(void) const;
                 Status SetTextRenderingHint( TextRenderingHint newMode );
                 Status MeasureCharacterRanges( const WCHAR* string, INT length, const Font* font, const RectF& layoutRect, const StringFormat* stringFormat, INT regionCount, Region* regions ) const;
                 Status MeasureDriverString( const UINT16* text, INT length, const Font* font, const PointF* positions, INT flags, const Matrix* matrix, RectF* boundingBox ) const;
                 Status MeasureString( const WCHAR* string, INT length, const Font* font, const RectF& layoutRect, RectF* boundingBox ) const;
                 Status MeasureString( const WCHAR* string, INT length, const Font* font, const RectF& layoutRect, const StringFormat* stringFormat, RectF* boundingBox, INT* codepointsFitted, INT* linesFilled ) const;
                 Status MeasureString( const WCHAR* string, INT length, const Font* font, const SizeF& layoutRectSize, const StringFormat* stringFormat, SizeF* size, INT* codepointsFitted, INT* linesFilled ) const;
                 Status MeasureString( const WCHAR* string, INT length, const Font* font, const PointF& origin, RectF* boundingBox ) const;
                 Status MeasureString( const WCHAR* string, INT length, const Font* font, const PointF& origin, const StringFormat* stringFormat, RectF* boundingBox ) const;

                 Status DrawDriverString( const UINT16* text, INT length, const Font* font, const Brush* brush, const PointF* positions, INT flags, const Matrix* matrix );

                 Status DrawString( const WCHAR* string, INT length, const Font* font, const RectF& layoutRect, const StringFormat* stringFormat, const Brush* brush );
                 Status DrawString( const WCHAR* string, INT length, const Font* font, const PointF& origin, const Brush* brush );
                 Status DrawString( const WCHAR* string, INT length, const Font* font, const PointF& origin, const StringFormat* stringFormat, const Brush* brush );

                 Status TransformPoints( CoordinateSpace destSpace, CoordinateSpace srcSpace, Point* pts, INT count );

                 Status GetVisibleClipBounds( Rect* rect ) const;
                 Status GetVisibleClipBounds( RectF* rect ) const;



        //! Get the resolution in dots-per-inch
        REAL GetDpiX(void);
        REAL GetDpiY(void);

     */


    //==========================================================================
    // GraphicsRendererBase
    //
    // This is the abstract class which allows us to interface between the
    // Graphics class and the renderer that supports a templatized pixel format.
    // Basically, the Graphics class (during creation) will have a big switch
    // statement where it examines the desired pixel format, and then creates a
    // new GraphicsRenderer object, using the appropriate format as the pixel
    // template.  This class is needed, however, as the pointer in the Graphics
    // class to the class being used.
    //==========================================================================

    class GraphicsRendererBase
    {
    public:
        // Since the AggOO Color class uses 8 bits-per-component, use rgba8 as
        // the color type
        typedef agg::rgba8          color_type;
        typedef agg::scanline_u8    scanline_type;
        Matrix                      mMatrix;
        //FillMode                    mFillMode;
        PixelFormat                 mAggOOPixelFormat;
        CompositingMode             mCompositeMode;
        CompositingQuality          mCompositeQuality;
        InterpolationMode           mInterpolationMode;
        REAL                        mPageScale;
        Unit                        mPageUnit;
        PixelOffsetMode             mPixelOffsetMode;
        Point                       mRenderingOrigin;
        SmoothingMode               mSmoothingMode;
        REAL                        mGammaThreshold;

        virtual ~GraphicsRendererBase()     { }

        //void    SetFillMode( FillMode fillMode )    { mFillMode = fillMode; }
        //FillMode GetFillMode()                      { return mFillMode; }

        //! Save the current graphics state
        GraphicsState Save();
        //! Restore a specified graphics state
        Status Restore( GraphicsState state );

        /** Set the gamma threshold (used when not antialiasing) */
        Status SetGammaThreshold( REAL gamma )
        {
            mGammaThreshold = gamma;
            return Ok;
        }
        /** Get the gamma threshold value */
        REAL GetGammaThreshold() const
        {
            return mGammaThreshold;
        }

    private:
        // Structure to hold the GraphicsState data for Save/Restore
        struct GraphicsStateData
        {
            Matrix              matrix;
//            FillMode            fillMode;
            CompositingMode     compositeMode;
            CompositingQuality  compositeQuality;
            InterpolationMode   interpolationMode;
            REAL                pageScale;
            Unit                pageUnit;
            PixelOffsetMode     pixelOffsetMode;
            Point               renderingOrigin;
            SmoothingMode       smoothingMode;
            REAL                gammaThreshold;

            /*
                // TODO:
                    clipping region
                    dpi
                    text contrast ?
                    text rendering hint ?
            */
        };

        // Stack which stores the saved graphics states
        std::stack< GraphicsStateData > mGraphicsStates;

    protected:

        //! Get a pointer to the bitmap data of an Image:
        BitmapData* getBitmapData( const Image* image ) const;

        agg::rasterizer_scanline_aa<>   mRasterizer;
        //agg::rasterizer_compound_aa< agg::rasterizer_sl_clip_dbl > mCompoundRasterizer;
        scanline_type                   mScanline;

        //----------------------------------------------------------------------

    public:

        /** Matrix tranformations
        */
        Status TranslateTransform( REAL dx, REAL dy, MatrixOrder order )
        {
            return mMatrix.Translate( dx, dy, order );
        }
        Status RotateTransform( REAL angle, MatrixOrder order )
        {
            return mMatrix.Rotate( angle, order );
        }
        Status ScaleTransform( REAL sx, REAL sy, MatrixOrder order )
        {
            return mMatrix.Scale( sx, sy, order );
        }
        Status MultiplyTransform( Matrix* matrix, MatrixOrder order )
        {
            return mMatrix.Multiply( matrix, order );
        }
        Status GetTransform( Matrix* matrix )
        {
            Status status;
            REAL m[ 6 ];
            if ( ( status = mMatrix.GetElements( m ) ) != Ok )
                return status;
            return matrix->SetElements( m[ 0 ], m[ 1 ], m[ 2 ], m[ 3 ], m[ 4 ], m[ 5 ] );
        }
        Status SetTransform( const Matrix* matrix )
        {
            Status status;
            REAL m[ 6 ];
            if ( ( status = matrix->GetElements( m ) ) != Ok )
                return status;
            return mMatrix.SetElements( m[ 0 ], m[ 1 ], m[ 2 ], m[ 3 ], m[ 4 ], m[ 5 ] );
        }
        Status ResetTransform(void)
        {
            return mMatrix.Reset();
        }

        /** Get the current compositing mode */
        CompositingMode GetCompositingMode(void)
        {
            return mCompositeMode;
        }

        /** Set the compositing mode */
        Status SetCompositingMode( CompositingMode compositingMode )
        {
            mCompositeMode = compositingMode;
            return Ok;
        }

        /** Get the compositing quality */
        CompositingQuality GetCompositingQuality(void)
        {
            return mCompositeQuality;
        }

        /** Set the compositing quality */
        Status SetCompositingQuality( CompositingQuality compositingQuality )
        {
            mCompositeQuality = compositingQuality;
            return Ok;
        }

        /** Set the smoothing mode */
        Status SetSmoothingMode( SmoothingMode mode )
        {
            mSmoothingMode = mode;
            return Ok;
        }
        /** Get the smoothing mode */
        SmoothingMode GetSmoothingMode() const
        {
            return mSmoothingMode;
        }

        virtual Status Clear( const Color& color ) = 0;
        virtual Status DrawPath( const Pen* pen, const GraphicsPath* path ) = 0;
        virtual Status FillPath( const Brush* brush, const GraphicsPath* path ) = 0;
        virtual Status RenderCompoundPath( const GraphicsPathCompound* path ) = 0;
        //virtual Status RenderSVGPath( svg::SVGPath* path, const Matrix* mtx, const Rect& clipBox, REAL opacity = 1.0 ) = 0;

        virtual Status DrawImage( Image* image, REAL x, REAL y, REAL width, REAL height ) = 0;
        virtual Status DrawImage( Image* image, const RectF& destRect, REAL srcX, REAL srcY, REAL srcW, REAL srcH, Unit srcUnit, ImageAttributes* imageAttributes = NULL, DrawImageAbort callback = NULL, VOID* callbackData = NULL ) = 0;
    };

} // namespace AggOO


#endif  // AggOOGraphics_H_
