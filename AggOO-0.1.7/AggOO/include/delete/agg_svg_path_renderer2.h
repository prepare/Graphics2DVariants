
/*! $Id: agg_svg_path_renderer2.h,v 1.1 2009/03/13 23:00:36 dratek Exp $
 *  @file   agg_svg_path_renderer2.h
 *  @author Chad M. Draper
 *  @date   May 7, 2007
 *  @brief  A subclass of agg::svg::path_renderer which allows for vertex retrieval.
 *  
 *  Derived from agg::svg::path_renderer.  path_renderer2 implements an
 *  additional method which will return the vertices of the SVG path in an
 *  array.  See method documentation for more information.
 */

#ifndef AGG_SVG_PATH_RENDERER2_H_
#define AGG_SVG_PATH_RENDERER2_H_

#include "AggOOTypes.h"
#include "svg/agg_svg_path_renderer.h"
#include <vector>

namespace AggOO
{
    namespace svg
    {
        class path_renderer2 : public agg::svg::path_renderer
        {
        public:

            /** Structure to define a shape in the path.
            */
            struct ShapeData
            {
                size_t  numVertices;    /**< Number of vertices in the shape */
                std::vector< PointF > vertices; /**< Vertices in the shape */
            };

            /** Retrieve the vertices of the path. */
            bool RenderVertices( const agg::trans_affine& mtx )
            {
                using namespace agg;
                unsigned i;

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

                    rgba8 color;

/*
                    if(attr.fill_flag)
                    {
                        ras.reset();
                        ras.filling_rule(attr.even_odd_flag ? fill_even_odd : fill_non_zero);
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
                        m_curved_stroked.inner_join(inner_round);
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
*/
                }
            }

        }; // class path_renderer2
    } // namespace svg
} // namespace AggOO

#endif  // AGG_SVG_PATH_RENDERER2_H_
