
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
// $Id: agg_svg_path_vertices.cpp,v 1.2 2008/01/10 18:02:23 dratek Exp $
//
// agg_svg_path_vertices.cpp
//


#include "agg_svg_path_vertices.h"

#include <sstream>

namespace AggOO
{
namespace svg
{

    //------------------------------------------------------------------------
    path_vertices::path_vertices() :
        m_curved(m_storage),
        m_curved_count(m_curved),

        m_curved_stroked(m_curved_count),
        m_curved_stroked_trans(m_curved_stroked, m_transform),

        m_curved_trans(m_curved_count, m_transform),
        m_curved_trans_contour(m_curved_trans)
    {
        m_curved_trans_contour.auto_detect_orientation(false);
    }


    //------------------------------------------------------------------------
    void path_vertices::remove_all()
    {
        m_storage.remove_all();
        m_attr_storage.remove_all();
        m_attr_stack.remove_all();
        m_transform.reset();
    }

    //------------------------------------------------------------------------
    void path_vertices::begin_path()
    {
        push_attr();
        unsigned idx = m_storage.start_new_path();
        m_attr_storage.add(agg::svg::path_attributes(cur_attr(), idx));
    }

    //------------------------------------------------------------------------
    void path_vertices::end_path()
    {
        if(m_attr_storage.size() == 0) 
        {
            throw agg::svg::exception("end_path : The path was not begun");
        }
        agg::svg::path_attributes attr = cur_attr();
        unsigned idx = m_attr_storage[m_attr_storage.size() - 1].index;
        attr.index = idx;
        m_attr_storage[m_attr_storage.size() - 1] = attr;
        pop_attr();
    }

    //------------------------------------------------------------------------
    void path_vertices::move_to(double x, double y, bool rel)          // M, m
    {
        if(rel) m_storage.rel_to_abs(&x, &y);
        m_storage.move_to(x, y);
    }

    //------------------------------------------------------------------------
    void path_vertices::line_to(double x,  double y, bool rel)         // L, l
    {
        if(rel) m_storage.rel_to_abs(&x, &y);
        m_storage.line_to(x, y);
    }

    //------------------------------------------------------------------------
    void path_vertices::hline_to(double x, bool rel)                   // H, h
    {
        double x2 = 0.0;
        double y2 = 0.0;
        if(m_storage.total_vertices())
        {
            m_storage.vertex(m_storage.total_vertices() - 1, &x2, &y2);
            if(rel) x += x2;
            m_storage.line_to(x, y2);
        }
    }

    //------------------------------------------------------------------------
    void path_vertices::vline_to(double y, bool rel)                   // V, v
    {
        double x2 = 0.0;
        double y2 = 0.0;
        if(m_storage.total_vertices())
        {
            m_storage.vertex(m_storage.total_vertices() - 1, &x2, &y2);
            if(rel) y += y2;
            m_storage.line_to(x2, y);
        }
    }

    //------------------------------------------------------------------------
    void path_vertices::curve3(double x1, double y1,                   // Q, q
                               double x,  double y, bool rel)
    {
        if(rel) 
        {
            m_storage.rel_to_abs(&x1, &y1);
            m_storage.rel_to_abs(&x,  &y);
        }
        m_storage.curve3(x1, y1, x, y);
    }

    //------------------------------------------------------------------------
    void path_vertices::curve3(double x, double y, bool rel)           // T, t
    {
//        throw agg::svg::exception("curve3(x, y) : NOT IMPLEMENTED YET");
        if(rel) 
        {
            m_storage.curve3_rel(x, y);
        } else 
        {
            m_storage.curve3(x, y);
        }
    }

    //------------------------------------------------------------------------
    void path_vertices::curve4(double x1, double y1,                   // C, c
                               double x2, double y2, 
                               double x,  double y, bool rel)
    {
        if(rel) 
        {
            m_storage.rel_to_abs(&x1, &y1);
            m_storage.rel_to_abs(&x2, &y2);
            m_storage.rel_to_abs(&x,  &y);
        }
        m_storage.curve4(x1, y1, x2, y2, x, y);
    }

    //------------------------------------------------------------------------
    void path_vertices::curve4(double x2, double y2,                   // S, s
                               double x,  double y, bool rel)
    {
        //throw agg::svg::exception("curve4(x2, y2, x, y) : NOT IMPLEMENTED YET");
        if(rel) 
        {
            m_storage.curve4_rel(x2, y2, x, y);
        } else 
        {
            m_storage.curve4(x2, y2, x, y);
        }
    }

    //------------------------------------------------------------------------
    void path_vertices::close_subpath()
    {
        m_storage.end_poly( agg::path_flags_close );
    }

    //------------------------------------------------------------------------
    agg::svg::path_attributes& path_vertices::cur_attr()
    {
        if(m_attr_stack.size() == 0)
        {
            throw agg::svg::exception("cur_attr : Attribute stack is empty");
        }
        return m_attr_stack[m_attr_stack.size() - 1];
    }

    //------------------------------------------------------------------------
    void path_vertices::push_attr()
    {
        m_attr_stack.add(m_attr_stack.size() ? 
                         m_attr_stack[m_attr_stack.size() - 1] :
                         agg::svg::path_attributes());
    }

    //------------------------------------------------------------------------
    void path_vertices::pop_attr()
    {
        if(m_attr_stack.size() == 0)
        {
            throw agg::svg::exception("pop_attr : Attribute stack is empty");
        }
        m_attr_stack.remove_last();
    }

    //------------------------------------------------------------------------
    void path_vertices::fill(const agg::rgba8& f)
    {
        agg::svg::path_attributes& attr = cur_attr();
        attr.fill_color = f;
        attr.fill_flag = true;
    }

    //------------------------------------------------------------------------
    void path_vertices::stroke(const agg::rgba8& s)
    {
        agg::svg::path_attributes& attr = cur_attr();
        attr.stroke_color = s;
        attr.stroke_flag = true;
    }

    //------------------------------------------------------------------------
    void path_vertices::even_odd(bool flag)
    {
        cur_attr().even_odd_flag = flag;
    }
    
    //------------------------------------------------------------------------
    void path_vertices::stroke_width(double w)
    {
        cur_attr().stroke_width = w;
    }

    //------------------------------------------------------------------------
    void path_vertices::fill_none()
    {
        cur_attr().fill_flag = false;
    }

    //------------------------------------------------------------------------
    void path_vertices::stroke_none()
    {
        cur_attr().stroke_flag = false;
    }

    //------------------------------------------------------------------------
    void path_vertices::fill_opacity(double op)
    {
        cur_attr().fill_color.opacity(op);
    }

    //------------------------------------------------------------------------
    void path_vertices::stroke_opacity(double op)
    {
        cur_attr().stroke_color.opacity(op);
    }

    //------------------------------------------------------------------------
    void path_vertices::line_join( agg::line_join_e join )
    {
        cur_attr().line_join = join;
    }

    //------------------------------------------------------------------------
    void path_vertices::line_cap( agg::line_cap_e cap )
    {
        cur_attr().line_cap = cap;
    }

    //------------------------------------------------------------------------
    void path_vertices::miter_limit(double ml)
    {
        cur_attr().miter_limit = ml;
    }

    //------------------------------------------------------------------------
    agg::trans_affine& path_vertices::transform()
    {
        return cur_attr().transform;
    }

    //------------------------------------------------------------------------
    void path_vertices::parse_path(agg::svg::path_tokenizer& tok)
    {
        while(tok.next())
        {
            double arg[10];
            char cmd = tok.last_command();
            unsigned i;
            switch(cmd)
            {
                case 'M': case 'm':
                    arg[0] = tok.last_number();
                    arg[1] = tok.next(cmd);
                    move_to(arg[0], arg[1], cmd == 'm');
                    break;

                case 'L': case 'l':
                    arg[0] = tok.last_number();
                    arg[1] = tok.next(cmd);
                    line_to(arg[0], arg[1], cmd == 'l');
                    break;

                case 'V': case 'v':
                    {
                        double lastNum = tok.last_number();
                        vline_to( lastNum, cmd == 'v' );
                    //vline_to(tok.last_number(), cmd == 'v');
                    }
                    break;

                case 'H': case 'h':
                    {
                        double lastNum = tok.last_number();
                        hline_to( lastNum, cmd == 'h' );
                    //hline_to(tok.last_number(), cmd == 'h');
                    }
                    break;
                
                case 'Q': case 'q':
                    arg[0] = tok.last_number();
                    for(i = 1; i < 4; i++)
                    {
                        arg[i] = tok.next(cmd);
                    }
                    curve3(arg[0], arg[1], arg[2], arg[3], cmd == 'q');
                    break;

                case 'T': case 't':
                    arg[0] = tok.last_number();
                    arg[1] = tok.next(cmd);
                    curve3(arg[0], arg[1], cmd == 't');
                    break;

                case 'C': case 'c':
                    arg[0] = tok.last_number();
                    for(i = 1; i < 6; i++)
                    {
                        arg[i] = tok.next(cmd);
                    }
                    curve4(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], cmd == 'c');
                    break;

                case 'S': case 's':
                    arg[0] = tok.last_number();
                    for(i = 1; i < 4; i++)
                    {
                        arg[i] = tok.next(cmd);
                    }
                    curve4(arg[0], arg[1], arg[2], arg[3], cmd == 's');
                    break;

                case 'A': case 'a':
                    throw agg::svg::exception("parse_path: Command A: NOT IMPLEMENTED YET");

                case 'Z': case 'z':
                    close_subpath();
                    break;

                default:
                {
                    char buf[100];
                    sprintf(buf, "parse_path: Invalid Command %c", cmd);
                    throw agg::svg::exception(buf);
                }
            }
        }
    }

} // namespace svg
} // namespace agg