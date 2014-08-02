
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
// $Id: AggOOSVGPath.h,v 1.3 2007/09/06 21:49:44 dratek Exp $
//
// AggOOSVGPath.h -- Header file for the SVGPath class
//
// Author       : Chad M. Draper
// Date         : May 1, 2006
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2007-05-01      Initial implementation
//

#ifndef AggOOSVGPath_H_
#define AggOOSVGPath_H_

#include "AggOOBase.h"
#include "AggOOEnums.h"
#include "AggOOTypes.h"
#include "AggOOMatrix.h"
#include "AggOOSVGShapeData.h"

#include "agg_svg_path_vertices.h"

namespace AggOO
{
    class Graphics;

    namespace svg
    {
        /** @class SVGPath
            Wrapper for the SVG storage path.

            @note   This requires ExpatXML for parsing.  Include the appropriate
                    expat library (i.e. libexpat.lib, libexpatMT.lib,
                    libexpatML.lib, etc.)  Also, if using the static version, be
                    sure to define XML_STATIC.
        */
        class _AggOOExport SVGPath : public Base
        {
        public:
            friend class Graphics;
            friend class GraphicsRendererBase;
            friend class SVGParser;

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

            void copyFrom( const SVGPath* src );

            AggOO::svg::path_vertices mPath;

    //        bool isPathClosed() const;

        public:
            SVGPath(void);
            virtual ~SVGPath(void);

            /** Return the AGG path */
            AggOO::svg::path_vertices* getAggPath()  { return &mPath; }

            /** Reset the path, clearing its current contents */
            Status Reset();

            // Implement SVG methods
            /** Move to a position on the path */
            Status MoveTo( REAL x, REAL y, bool rel = false );
            /** Add a line to a point on the path */
            Status LineTo( REAL x, REAL y, bool rel = false );
            /** Add a horizontal line */
            Status HLineTo( REAL x, bool rel = false );
            /** Add a vertical line */
            Status VLineTo( REAL y, bool rel = false );
            /** Add a curve */
            Status Curve3( REAL x1, REAL y1, REAL x, REAL y, bool rel = false );
            /** Add a curve */
            Status Curve3( REAL x, REAL y, bool rel = false );
            /** Add a curve */
            Status Curve4( REAL x1, REAL y1, REAL x2, REAL y2, REAL x, REAL y, bool rel = false );
            /** Add a curve */
            Status Curve4( REAL x2, REAL y2, REAL x, REAL y, bool rel = false );
            /** Close the sub-path */
            Status CloseSubPath();

            /** Return the number of vertices in the path */
            unsigned GetVertexCount() const     { return mPath.vertex_count(); }

            // Set the path attributes (reflection methods)
            Status SetFill( const Color& col );
            Status SetStroke( const Color& col );
            Status SetEvenOdd( bool flag );
            Status SetStrokeWidth( REAL w );
            Status SetFillNone();
            Status SetStrokeNone();
            Status SetFillOpacity( REAL op );
            Status SetStrokeOpacity( REAL op );
            Status SetLineJoin( LineJoin join );
            Status SetLineCap( LineCap cap );
            Status SetMiterLimit( REAL limit );
            Matrix Transform();

            // Arrange all polygons to be counter-clockwise
            Status ArrangeOrientations();

            // Expand all polygons
            Status Expand( REAL value );

            unsigned operator[]( unsigned idx )
            {
                return mPath[ idx ];
            }

            /** Get the bounding rect of the path */
            Status BoundingRect( RectF* rect )
            {
                double x1, y1, x2, y2;
                mPath.bounding_rect( &x1, &y1, &x2, &y2 );
                rect->X      = x1;
                rect->Y      = y1;
                rect->Width  = x2 - rect->X;
                rect->Height = y2 - rect->Y;

                return SetStatus( Ok );
            }
            Status BoundingRect( REAL* x1, REAL* y1, REAL* x2, REAL* y2 )
            {
                double tx1, ty1, tx2, ty2;
                mPath.bounding_rect( &tx1, &ty1, &tx2, &ty2 );
                *x1 = tx1;
                *y1 = ty1;
                *x2 = tx2;
                *y2 = ty2;
                return SetStatus( Ok );
            }

            /** Render the paths
            */
            template<class Rasterizer, class Scanline, class Renderer> 
            void Render(Rasterizer& ras, 
                        Scanline& sl,
                        Renderer& ren, 
                        const agg::trans_affine& mtx, 
                        const agg::rect_i& cb,
                        double opacity=1.0)
            {
                mPath.render( ras, sl, ren, mtx, cb, opacity );
            }

            /** Render the paths to a Graphics object */
            void Render( AggOO::Graphics* gfx );

            /** Render the shape data in the path to an array, which will allow
                access to the vertex information.
            */
            void RenderShapeData( const Matrix& mtx, std::vector< svg::SVGShapeData >& shapes )
            {
                mPath.render_shape_data( mtx.GetAffine(), shapes );
            }

        }; // class SVGPath

    } // namespace svg
} // namespace AggOO

#endif  // AggOOSVGPath_H_
