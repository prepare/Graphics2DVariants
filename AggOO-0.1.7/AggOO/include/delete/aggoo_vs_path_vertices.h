
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


/*! $Id: aggoo_vs_path_vertices.h,v 1.1 2009/03/13 23:00:36 dratek Exp $
 *  @file   aggoo_vs_path_vertices.h
 *  @author Chad M. Draper
 *  @date   November 26, 2007
 *  @brief  Used for parsing a vector shape file.  Allows retrieval of the
 *          vertices.  The idea is for it to be generalized so that any
 *          parser can use a single path_vertices object.
 *  
 */

#ifndef aggoo_vs_path_vertices_H_
#define aggoo_vs_path_vertices_H_

#include "agg_path_storage.h"
#include "agg_conv_transform.h"
#include "agg_conv_stroke.h"
#include "agg_conv_contour.h"
#include "agg_conv_curve.h"
#include "agg_color_rgba.h"
#include "agg_renderer_scanline.h"
#include "agg_bounding_rect.h"
#include "agg_rasterizer_scanline_aa.h"
#include "AggOOColorExt.h"
#include <vector>

#include "AggOOVectorShapeData.h"
#include "svg/agg_svg_path_tokenizer.h"

namespace AggOO
{
    namespace vs
    {
        //class VectorShapeData;

        template<class VertexSource> class conv_count
        {
        public:
            conv_count(VertexSource& vs) : m_source(&vs), m_count(0) {}

            void count(unsigned n) { m_count = n; }
            unsigned count() const { return m_count; }

            void rewind(unsigned path_id) { m_source->rewind(path_id); }
            unsigned vertex(double* x, double* y) 
            { 
                ++m_count; 
                return m_source->vertex(x, y); 
            }

        private:
            VertexSource* m_source;
            unsigned m_count;
        };

        /** @struct path_attributes
            @brief  Describes the attributes of a path (i.e. the fill color,
                    outline, etc.)
        */
        struct path_attributes
        {
            unsigned            index;
            agg::rgba8          fill_color;
            agg::rgba8          stroke_color;
            bool                fill_flag;
            bool                stroke_flag;
            bool                even_odd_flag;
            bool                path_closed;
            agg::line_join_e    line_join;
            agg::line_cap_e     line_cap;
            double              miter_limit;
            double              stroke_width;
            agg::trans_affine   transform;

            // Empty constructor
            path_attributes() :
                index(0),
                fill_color(agg::rgba(0,0,0)),
                stroke_color(agg::rgba(0,0,0)),
                fill_flag(true),
                stroke_flag(false),
                even_odd_flag(false),
                path_closed( false ),
                line_join(agg::miter_join),
                line_cap(agg::butt_cap),
                miter_limit(4.0),
                stroke_width(1.0),
                transform()
            {
            }

            // Copy constructor
            path_attributes(const path_attributes& attr) :
                index(attr.index),
                fill_color(attr.fill_color),
                stroke_color(attr.stroke_color),
                fill_flag(attr.fill_flag),
                stroke_flag(attr.stroke_flag),
                even_odd_flag(attr.even_odd_flag),
                line_join(attr.line_join),
                line_cap(attr.line_cap),
                miter_limit(attr.miter_limit),
                stroke_width(attr.stroke_width),
                transform(attr.transform)
            {
            }

            // Copy constructor with new index value
            path_attributes(const path_attributes& attr, unsigned idx) :
                index(idx),
                fill_color(attr.fill_color),
                stroke_color(attr.stroke_color),
                fill_flag(attr.fill_flag),
                stroke_flag(attr.stroke_flag),
                even_odd_flag(attr.even_odd_flag),
                line_join(attr.line_join),
                line_cap(attr.line_cap),
                miter_limit(attr.miter_limit),
                stroke_width(attr.stroke_width),
                transform(attr.transform)
            {
            }
        };

        /** @class  path_vertices
            @brief  Defines a path.  Allows for retrieving the vertices of the
                    path.
        */
        class _AggOOExport path_vertices
        {
        public:
            path_vertices(void);

        public:
            typedef agg::pod_bvector<path_attributes> attr_storage;

            typedef agg::conv_curve<agg::path_storage>  curved;
            typedef conv_count<curved>                  curved_count;

            typedef agg::conv_stroke<curved_count>      curved_stroked;
            typedef agg::conv_transform<curved_stroked> curved_stroked_trans;

            typedef agg::conv_transform<curved_count>   curved_trans;
            typedef agg::conv_contour<curved_trans>     curved_trans_contour;

            void remove_all();

            // Use these functions as follows:
            // begin_path() when the XML tag <path> comes ("start_element" handler)
            // parse_path() on "d=" tag attribute
            // end_path() when parsing of the entire tag is done.
            void begin_path();
            void parse_path( agg::svg::path_tokenizer& tok );
            void end_path();

            // The following functions are essentially a "reflection" of
            // the respective SVG path commands.
            void arc( double rx, double ry,                     // A, a
                      double angle, bool large_arc_flag,
                      bool sweep_flag,
                      double x, double y,
                      bool rel = false );
            void move_to(double x, double y, bool rel=false);   // M, m
            void line_to(double x,  double y, bool rel=false);  // L, l
            void hline_to(double x, bool rel=false);            // H, h
            void vline_to(double y, bool rel=false);            // V, v
            void curve3(double x1, double y1,                   // Q, q
                        double x,  double y, bool rel=false);
            void curve3(double x, double y, bool rel=false);    // T, t
            void curve4(double x1, double y1,                   // C, c
                        double x2, double y2, 
                        double x,  double y, bool rel=false);
            void curve4(double x2, double y2,                   // S, s
                        double x,  double y, bool rel=false);
            void close_subpath();                               // Z, z

            unsigned vertex_count() const { return m_curved_count.count(); }
            

            // Call these functions on <g> tag (start_element, end_element respectively)
            void push_attr();
            void pop_attr();

            // Attribute setting functions.
            void fill(const agg::rgba8& f);
            void stroke(const agg::rgba8& s);
            void even_odd(bool flag);
            void stroke_width(double w);
            void fill_none();
            void stroke_none();
            void fill_opacity(double op);
            void stroke_opacity(double op);
            void line_join( agg::line_join_e join );
            void line_cap( agg::line_cap_e cap );
            void miter_limit(double ml);
            agg::trans_affine& transform();

            // Make all polygons CCW-oriented
            void arrange_orientations()
            {
                m_storage.arrange_orientations_all_paths( agg::path_flags_ccw );
            }

            // Expand all polygons 
            void expand(double value)
            {
                m_curved_trans_contour.width(value);
            }

            unsigned operator [](unsigned idx)
            {
                m_transform = m_attr_storage[idx].transform;
                return m_attr_storage[idx].index;
            }

            void bounding_rect(double* x1, double* y1, double* x2, double* y2)
            {
                agg::conv_transform<agg::path_storage> trans(m_storage, m_transform);
                agg::bounding_rect(trans, *this, 0, m_attr_storage.size(), x1, y1, x2, y2);
            }

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
                    const path_attributes& attr = m_attr_storage[i];
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

            /** "Render" the shapes to an array.  The idea is to tessellate the
                curves so that the vertices will resemble the original shapes as
                much as possible.
            */
            void render_shape_data( const agg::trans_affine& mtx, std::vector< VectorShapeData >& shapes )
            {
                unsigned i;

                for ( i = 0; i < m_attr_storage.size(); i++ )
                {
                    const path_attributes& attr = m_attr_storage[i];
                    m_transform = attr.transform;
                    m_transform *= mtx;
                    double scl = m_transform.scale();
                    m_curved.approximation_scale(scl);
                    m_curved.angle_tolerance(0.0);

                    // Add the path to the vertex array:
                    AddPath( shapes, m_curved_trans, attr );
                }
            }

        private:
            /** Add a path to a vertex array. */
            template< class VertexSource >
            size_t AddPath( std::vector< VectorShapeData >& shapes, VertexSource& vs, const path_attributes& attr )
            {
                double x;
                double y;
                size_t path_id = attr.index;

                VectorShapeData newShape;
                VectorSubPath   newSubShape;
                int shapesAdded = 0;

                // As far as the fill mode is concerned, use the value of the
                // attribute, unless there are more than one path, and they
                // alternate in direction.  I'm not sure, but I don't think AGG
                // is setting the fill mode at all...
                bool evenOddMode = attr.even_odd_flag;

                Matrix shapeMtx;
                newShape.SetFillColor( ColorExt::convertColor( attr.fill_color ) );
                newShape.SetStrokeColor( ColorExt::convertColor( attr.stroke_color ) );
                newShape.SetStrokeWidth( attr.stroke_width );
                newShape.SetStrokeFlag( attr.stroke_flag );
                newShape.SetFillFlag( attr.fill_flag );
                //newShape.SetEvenOddFill( attr.even_odd_flag );
                newShape.SetLineJoin( convertLineJoin( attr.line_join ) );
                newShape.SetLineCap( convertLineCap( attr.line_cap ) );
                newShape.SetMiterLimit( attr.miter_limit );
                shapeMtx.SetAffine( attr.transform );
                newShape.SetTransform( &shapeMtx );

                unsigned cmd;
                vs.rewind(path_id);
                while(!agg::is_stop(cmd = vs.vertex(&x, &y)))
                {
                    if(agg::is_move_to(cmd)) 
                    {
                        // If the path has vertices, add it to the list
                        if ( newSubShape.GetVertexCount() )
                        {
                            newShape.AddPath( newSubShape );
                            shapesAdded++;
                        }

                        // Reset the shape:
                        newSubShape.Reset();
                        evenOddMode = true;
                        //newSubShape.SetFillColor( ColorExt::convertColor( attr.fill_color ) );
                        //newSubShape.SetStrokeColor( ColorExt::convertColor( attr.stroke_color ) );
                        //newSubShape.SetStrokeWidth( attr.stroke_width );
                        //newSubShape.SetStrokeFlag( attr.stroke_flag );
                        //newSubShape.SetFillFlag( attr.fill_flag );
                        //newSubShape.SetEvenOddFill( attr.even_odd_flag );
                        //newSubShape.SetLineJoin( convertLineJoin( attr.line_join ) );
                        //newSubShape.SetLineCap( convertLineCap( attr.line_cap ) );
                        //newSubShape.SetMiterLimit( attr.miter_limit );
                        //shapeMtx.SetAffine( attr.transform );
                        //newSubShape.SetTransform( &shapeMtx );

                        // Add the vertex:
                        newSubShape.AddVertex( x, y );
                    }
                    else
                    if(agg::is_vertex(cmd))
                    {
                        // Add the vertex:
                        newSubShape.AddVertex( x, y );
                    }
                    else
                    if ( agg::is_close( cmd ) )
                    {
                        newSubShape.SetClosed( true );
                    }
                }

                // Add the path:
                if ( newSubShape.GetVertexCount() )
                {
                    newShape.AddPath( newSubShape );
                    shapesAdded++;
                }
                newShape.SetEvenOddFill( evenOddMode );

                // Add the shape:
                shapes.push_back( newShape );

                return shapesAdded;
            }

        private:
            path_attributes& cur_attr();

            agg::path_storage   m_storage;
            attr_storage        m_attr_storage;
            attr_storage        m_attr_stack;
            agg::trans_affine   m_transform;

            curved                       m_curved;
            curved_count                 m_curved_count;

            curved_stroked               m_curved_stroked;
            curved_stroked_trans         m_curved_stroked_trans;

            curved_trans                 m_curved_trans;
            curved_trans_contour         m_curved_trans_contour;
        };
    } // namespace vs
} // namespace AggOO

#endif  // aggoo_vs_path_vertices_H_