
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
// $Id: AggOOGraphicsExt.cpp,v 1.6 2009/02/26 22:13:38 dratek Exp $
//
// AggOOGraphicsExt.cpp
//

#include "AggOOGraphicsExt.h"
#include "AggOOGraphicsPathExt.h"

//#include "aggoo_vs_path_vertices.h"
#include "AggOOBrush.h"
#include "AggOOPen.h"

namespace AggOO
{
    GraphicsExt::GraphicsExt( Image* image, ColorFormat colFmt )
        : Graphics( image, colFmt )
    {
    }

    //GraphicsExt::~GraphicsExt(void)
    //{
    //}

    //DrawRoundRectangle--------------------------------------------------------
    Status GraphicsExt::DrawRoundRectangle( const Pen* pen, const Rect& rect, REAL radius )
    {
        GraphicsPathExt path;
        path.AddRoundRectangle( rect, radius );
        return DrawPath( pen, &path );
    }

    Status GraphicsExt::DrawRoundRectangle( const Pen* pen, const RectF& rect, REAL radius )
    {
        GraphicsPathExt path;
        path.AddRoundRectangle( rect, radius );
        return DrawPath( pen, &path );
    }

    Status GraphicsExt::DrawRoundRectangle( const Pen* pen, INT x, INT y, INT width, INT height, REAL radius )
    {
        GraphicsPathExt path;
        path.AddRoundRectangle( Rect( x, y, width, height ), radius );
        return DrawPath( pen, &path );
    }

    Status GraphicsExt::DrawRoundRectangle( const Pen* pen, REAL x, REAL y, REAL width, REAL height, REAL radius )
    {
        GraphicsPathExt path;
        path.AddRoundRectangle( RectF( x, y, width, height ), radius );
        return DrawPath( pen, &path );
    }

    //FillRoundRectangle--------------------------------------------------------
    Status GraphicsExt::FillRoundRectangle( const Brush* brush, const Rect& rect, REAL radius )
    {
        GraphicsPathExt path;
        path.AddRoundRectangle( rect, radius );
        return FillPath( brush, &path );
    }

    Status GraphicsExt::FillRoundRectangle( const Brush* brush, const RectF& rect, REAL radius )
    {
        GraphicsPathExt path;
        path.AddRoundRectangle( rect, radius );
        return FillPath( brush, &path );
    }

    Status GraphicsExt::FillRoundRectangle( const Brush* brush, INT x, INT y, INT width, INT height, REAL radius )
    {
        GraphicsPathExt path;
        path.AddRoundRectangle( Rect( x, y, width, height ), radius );
        return FillPath( brush, &path );
    }

    Status GraphicsExt::FillRoundRectangle( const Brush* brush, REAL x, REAL y, REAL width, REAL height, REAL radius )
    {
        GraphicsPathExt path;
        path.AddRoundRectangle( RectF( x, y, width, height ), radius );
        return FillPath( brush, &path );
    }

    //DrawRegularPolygon--------------------------------------------------------
    Status GraphicsExt::DrawRegularPolygon( const Pen* pen, INT numPoints, const Point& center, REAL radius )
    {
        GraphicsPathExt path;
        path.AddRegularPolygon( numPoints, center, radius );
        return DrawPath( pen, &path );
    }

    Status GraphicsExt::DrawRegularPolygon( const Pen* pen, INT numPoints, const PointF& center, REAL radius )
    {
        GraphicsPathExt path;
        path.AddRegularPolygon( numPoints, center, radius );
        return DrawPath( pen, &path );
    }

    //FillRegularPolygon--------------------------------------------------------
    Status GraphicsExt::FillRegularPolygon( const Brush* brush, INT numPoints, const Point& center, REAL radius )
    {
        GraphicsPathExt path;
        path.SetFillMode( FillModeWinding );
        path.AddRegularPolygon( numPoints, center, radius );
        return FillPath( brush, &path );
    }

    Status GraphicsExt::FillRegularPolygon( const Brush* brush, INT numPoints, const PointF& center, REAL radius )
    {
        GraphicsPathExt path;
        path.SetFillMode( FillModeWinding );
        path.AddRegularPolygon( numPoints, center, radius );
        return FillPath( brush, &path );
    }

    Status GraphicsExt::FillRegularPolygon( const Brush* brush, INT numPoints, const Point& center, REAL radius, FillMode fillMode )
    {
        GraphicsPathExt path;
        path.SetFillMode( fillMode );
        path.AddRegularPolygon( numPoints, center, radius );
        return FillPath( brush, &path );
    }

    Status GraphicsExt::FillRegularPolygon( const Brush* brush, INT numPoints, const PointF& center, REAL radius, FillMode fillMode )
    {
        GraphicsPathExt path;
        path.SetFillMode( fillMode );
        path.AddRegularPolygon( numPoints, center, radius );
        return FillPath( brush, &path );
    }

    //DrawStar------------------------------------------------------------------
    Status GraphicsExt::DrawStar( const Pen* pen, INT numPoints, const Point& center, INT radius1, INT radius2 )
    {
        GraphicsPathExt path;
        path.AddStar( numPoints, center, radius1, radius2 );
        return DrawPath( pen, &path );
    }

    Status GraphicsExt::DrawStar( const Pen* pen, INT numPoints, const PointF& center, REAL radius1, REAL radius2 )
    {
        GraphicsPathExt path;
        path.AddStar( numPoints, center, radius1, radius2 );
        return DrawPath( pen, &path );
    }

    //FillStar------------------------------------------------------------------
    Status GraphicsExt::FillStar( const Brush* brush, INT numPoints, const Point& center, INT radius1, INT radius2 )
    {
        GraphicsPathExt path;
        path.SetFillMode( FillModeWinding );
        path.AddStar( numPoints, center, radius1, radius2 );
        return FillPath( brush, &path );
    }

    Status GraphicsExt::FillStar( const Brush* brush, INT numPoints, const PointF& center, REAL radius1, REAL radius2 )
    {
        GraphicsPathExt path;
        path.SetFillMode( FillModeWinding );
        path.AddStar( numPoints, center, radius1, radius2 );
        return FillPath( brush, &path );
    }

    Status GraphicsExt::FillStar( const Brush* brush, INT numPoints, const Point& center, INT radius1, INT radius2, FillMode fillMode )
    {
        GraphicsPathExt path;
        path.SetFillMode( fillMode );
        path.AddStar( numPoints, center, radius1, radius2 );
        return FillPath( brush, &path );
    }

    Status GraphicsExt::FillStar( const Brush* brush, INT numPoints, const PointF& center, REAL radius1, REAL radius2, FillMode fillMode )
    {
        GraphicsPathExt path;
        path.SetFillMode( fillMode );
        path.AddStar( numPoints, center, radius1, radius2 );
        return FillPath( brush, &path );
    }

    //DrawLineStrip-------------------------------------------------------------
    Status GraphicsExt::DrawLineStrip( const Pen* pen, const Point* points, INT count )
    {
        GraphicsPathExt path;
        path.AddLineStrip( points, count );
        return DrawPath( pen, &path );
    }

    Status GraphicsExt::DrawLineStrip( const Pen* pen, const PointF* points, INT count )
    {
        GraphicsPathExt path;
        path.AddLineStrip( points, count );
        return DrawPath( pen, &path );
    }

    ////DrawSVGShape--------------------------------------------------------------
    //Status GraphicsExt::DrawVectorShape( const Pen* pen, const vs::VectorShapeData& shape )
    //{
    //    GraphicsPathExt path;
    //    path.AddVectorShape( shape );
    //    return DrawPath( pen, &path );
    //}

    ////FillSVGShape--------------------------------------------------------------
    //Status GraphicsExt::FillVectorShape( const Brush* brush, const vs::VectorShapeData& shape )
    //{
    //    GraphicsPathExt path;
    //    path.AddVectorShape( shape );
    //    return FillPath( brush, &path );
    //}

    ////FillSVGShape--------------------------------------------------------------
    //Status GraphicsExt::FillVectorShape( const Brush* brush, const vs::VectorShapeData& shape, FillMode fillMode )
    //{
    //    GraphicsPathExt path;
    //    path.SetFillMode( fillMode );
    //    path.AddVectorShape( shape );
    //    return FillPath( brush, &path );
    //}


    ////RenderAIShape-------------------------------------------------------------
    //Status GraphicsExt::RenderAIShape( const ai::AIPath* aipath, const Matrix* mtx, const Rect& clipBox, REAL opacity )
    //{
    //    GraphicsPathExt path;
    //    ai::path_vertices aggPath = aipath->getAggPath();
    //    Status status = Ok;
    //    Matrix curMtx;
    //    for ( int attrIdx = 0; attrIdx < aggPath.m_attr_storage.size(); attrIdx++ )
    //    {
    //        GraphicsState state = Save();

    //        const agg::svg::path_attributes& attr = aggPath.m_attr_storage[ attrIdx ];
    //        aggPath.m_transform = attr.transform;
    //        aggPath.m_transform *= mtx->GetAffine();
    //        curMtx.SetAffine( aggPath.m_transform );
    //        double scl = aggPath.m_transform.scale();
    //        //m_curved.approximation_method(curve_inc);

    //        SetTransform( curMtx );

    //        if ( attr.fill_flag )
    //        {
    //            path.SetFillMode( attr.even_odd_flag ? FillModeAlternate : FillModeWinding );
    //            if(fabs(aggPath.m_curved_trans_contour.width()) < 0.0001)
    //            {
    //                path.addAggPath( aggPath.m_curved_trans, attr.index );
    //            }
    //            else
    //            {
    //                aggPath.m_curved_trans_contour.miter_limit(attr.miter_limit);
    //                path.addAggPath( aggPath.m_curved_trans_contour, attr.index );
    //            }

    //            Color color( attr.fill_color.a * opacity, attr.fill_color.r, attr.fill_color.g, attr.fill_color.b );
    //            SolidBrush br( color );
    //            FillPath( &br, &path );
    //        }
    //        if ( attr.stroke_flag )
    //        {
    //            aggPath.m_curved_stroked.width(attr.stroke_width);
    //            aggPath.m_curved_stroked.line_join(attr.line_join);
    //            aggPath.m_curved_stroked.line_cap(attr.line_cap);
    //            aggPath.m_curved_stroked.miter_limit(attr.miter_limit);
    //            aggPath.m_curved_stroked.inner_join(agg::inner_round);
    //            aggPath.m_curved_stroked.approximation_scale(scl);

    //            // If the *visual* line width is considerable we 
    //            // turn on processing of curve cusps.
    //            //---------------------
    //            if(attr.stroke_width * scl > 1.0)
    //            {
    //                aggPath.m_curved.angle_tolerance(0.2);
    //            }

    //            path.Reset();
    //            path.SetFillMode( attr.even_odd_flag ? FillModeAlternate : FillModeWinding );
    //            path.addAggPath( aggPath.m_curved_stroked_trans, attr.index );

    //            Color color( attr.stroke_color.a * opacity, attr.stroke_color.r, attr.stroke_color.g, attr.stroke_color.b );
    //            Pen pen( color, attr.stroke_width );
    //            DrawPath( &pen, &path );
    //        }
    //        Restore( state );
    //    }

    //    return Ok;
    //}

/*

        // Rendering. One can specify two additional parameters: 
        // trans_affine and opacity. They can be used to transform the whole
        // image and/or to make it translucent.
        template<class Rasterizer, class Scanline, class Renderer> 
        void render(Rasterizer& ras, 
                    Scanline& sl,
                    Renderer& ren, 
                    const agg::trans_affine& mtx, 
                    const agg::rect_i& cb,
                    double opacity=1.0)
        {
            using namespace agg;
            unsigned i;

            ras.clip_box(cb.x1, cb.y1, cb.x2, cb.y2);
            m_curved_count.count(0);

            for(i = 0; i < m_attr_storage.size(); i++)
            {
                const agg::svg::path_attributes& attr = m_attr_storage[i];
                m_transform = attr.transform;
                m_transform *= mtx;
                double scl = m_transform.scale();
                //m_curved.approximation_method(curve_inc);
                m_curved.approximation_scale(scl);
                m_curved.angle_tolerance(0.0);

                agg::rgba8 color;

                if(attr.fill_flag)
                {
                    ras.reset();
                    ras.filling_rule(attr.even_odd_flag ? agg::fill_even_odd : agg::fill_non_zero);
                    if(fabs(m_curved_trans_contour.width()) < 0.0001)
                    {
                        ras.add_path(m_curved_trans, attr.index);
                    }
                    else
                    {
                        m_curved_trans_contour.miter_limit(attr.miter_limit);
                        ras.add_path(m_curved_trans_contour, attr.index);
                    }

                    color = attr.fill_color;
                    color.opacity(color.opacity() * opacity);
                    ren.color(color);
                    agg::render_scanlines(ras, sl, ren);
                }

                if(attr.stroke_flag)
                {
                    m_curved_stroked.width(attr.stroke_width);
                    //m_curved_stroked.line_join((attr.line_join == miter_join) ? miter_join_round : attr.line_join);
                    m_curved_stroked.line_join(attr.line_join);
                    m_curved_stroked.line_cap(attr.line_cap);
                    m_curved_stroked.miter_limit(attr.miter_limit);
                    m_curved_stroked.inner_join(agg::inner_round);
                    m_curved_stroked.approximation_scale(scl);

                    // If the *visual* line width is considerable we 
                    // turn on processing of curve cusps.
                    //---------------------
                    if(attr.stroke_width * scl > 1.0)
                    {
                        m_curved.angle_tolerance(0.2);
                    }
                    ras.reset();
                    ras.filling_rule(fill_non_zero);
                    ras.add_path(m_curved_stroked_trans, attr.index);
                    color = attr.stroke_color;
                    color.opacity(color.opacity() * opacity);
                    ren.color(color);
                    agg::render_scanlines(ras, sl, ren);
                }
            }
        }
*/
} // namespace AggOO
