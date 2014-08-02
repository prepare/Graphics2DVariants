
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

/*! $Id: aggoo_vs_svg_parser.cpp,v 1.3 2008/04/18 20:35:12 dratek Exp $
 *  @file   aggoo_vs_svg_parser.h
 *  @author Chad M. Draper
 *  @date   November 27, 2007
 *  
 */

#include <ctype.h>

#include "aggoo_vs_svg_parser.h"

namespace AggOO
{
    namespace vs
    {
        svg_parser::svg_parser( path_vertices* path )
            : base_parser( path ),
              m_tokenizer(),
              mPatternFlag( false ),
              mSymbolFlag( false )
        {
        }

        //------------------------------------------------------------------------
        bool svg_parser::parse(const char* fname)
        {
            char msg[1024];
	        XML_Parser p = XML_ParserCreate(NULL);
	        if(p == 0) 
	        {
		        throw agg::svg::exception("Couldn't allocate memory for parser");
	        }

            XML_SetUserData(p, this);
	        XML_SetElementHandler(p, start_element, end_element);
	        XML_SetCharacterDataHandler(p, content);

            FILE* fd = fopen(fname, "r");
            if(fd == 0)
            {
                sprintf(msg, "Couldn't open file %s", fname);
		        throw agg::svg::exception(msg);
            }

            bool done = false;
            do
            {
                size_t len = fread(m_buf, 1, buf_size, fd);
                done = len < buf_size;
                if(!XML_Parse(p, m_buf, len, done))
                {
                    sprintf(msg,
                        "%s at line %d\n",
                        XML_ErrorString(XML_GetErrorCode(p)),
                        XML_GetCurrentLineNumber(p));
                    throw agg::svg::exception(msg);
                }
            }
            while(!done);
            fclose(fd);
            XML_ParserFree(p);

            char* ts = m_title;
            while(*ts)
            {
                if(*ts < ' ') *ts = ' ';
                ++ts;
            }

            return true;
        }

        //------------------------------------------------------------------------
        void svg_parser::start_element(void* data, const char* el, const char** attr)
        {
            svg_parser& self = *(svg_parser*)data;

            if ( self.mPatternFlag )
            {
                return;
            }
            else
            if ( self.mSymbolFlag )
            {
                return;
            }
            else
            if(strcmp(el, "title") == 0)
            {
                self.m_title_flag = true;
            }
            else
            if ( strcmp( el, "pattern" ) == 0 )
            {
                // Currently, all pattern information should be discarded.
                self.mPatternFlag = true;
            }
            else
            //if ( strcmp( el, "symbol" ) == 0 )
            //{
            //    self.mSymbolFlag = true;
            //}
            //else
            if(strcmp(el, "g") == 0)
            {
                self.m_path.push_attr();
                self.parse_attr(attr);
            }
            else
            if(strcmp(el, "path") == 0)
            {
                if(self.m_path_flag)
                {
                    throw agg::svg::exception("start_element: Nested path");
                }
                self.m_path.begin_path();
                self.parse_path(attr);
                self.m_path.end_path();
                self.m_path_flag = true;
            }
            else
            if(strcmp(el, "rect") == 0) 
            {
                self.parse_rect(attr);
            }
            else
            if(strcmp(el, "ellipse") == 0) 
            {
                self.parse_ellipse(attr);
            }
            else
            if(strcmp(el, "line") == 0) 
            {
                self.parse_line(attr);
            }
            else
            if(strcmp(el, "polyline") == 0) 
            {
                self.parse_poly(attr, false);
            }
            else
            if(strcmp(el, "polygon") == 0) 
            {
                self.parse_poly(attr, true);
            }
            //else
            //if(strcmp(el, "<OTHER_ELEMENTS>") == 0) 
            //{
            //}
            // . . .
        }

        //------------------------------------------------------------------------
        void svg_parser::end_element(void* data, const char* el)
        {
            svg_parser& self = *(svg_parser*)data;

            if(strcmp(el, "title") == 0)
            {
                self.m_title_flag = false;
            }
            else
            if(strcmp(el, "g") == 0)
            {
                if ( !self.mPatternFlag && !self.mSymbolFlag )
                    self.m_path.pop_attr();
            }
            else
            if(strcmp(el, "path") == 0)
            {
                self.m_path_flag = false;
            }
            else
            if ( strcmp( el, "pattern" ) == 0 )
            {
                self.mPatternFlag = false;
            }
            else
            if ( strcmp( el, "symbol" ) == 0 )
            {
                self.mSymbolFlag = false;
            }
            //else
            //if(strcmp(el, "<OTHER_ELEMENTS>") == 0) 
            //{
            //}
            // . . .
        }


        //------------------------------------------------------------------------
        void svg_parser::content(void* data, const char* s, int len)
        {
            svg_parser& self = *(svg_parser*)data;

            // m_title_flag signals that the <title> tag is being parsed now.
            // The following code concatenates the pieces of content of the <title> tag.
            if(self.m_title_flag)
            {
                if(len + self.m_title_len > 255) len = 255 - self.m_title_len;
                if(len > 0) 
                {
                    memcpy(self.m_title + self.m_title_len, s, len);
                    self.m_title_len += len;
                    self.m_title[self.m_title_len] = 0;
                }
            }
        }

        //------------------------------------------------------------------------
        void svg_parser::parse_attr(const char** attr)
        {
            int i;
            for(i = 0; attr[i]; i += 2)
            {
                if(strcmp(attr[i], "style") == 0)
                {
                    parse_style(attr[i + 1]);
                }
                else
                {
                    parse_attr(attr[i], attr[i + 1]);
                }
            }
        }

        //-------------------------------------------------------------
        void svg_parser::parse_path(const char** attr)
        {
            int i;

            for(i = 0; attr[i]; i += 2)
            {
                // The <path> tag can consist of the path itself ("d=") 
                // as well as of other parameters like "style=", "transform=", etc.
                // In the last case we simply rely on the function of parsing 
                // attributes (see 'else' branch).
                if(strcmp(attr[i], "d") == 0)
                {
                    m_tokenizer.set_path_str(attr[i + 1]);
                    m_path.parse_path(m_tokenizer);
                }
                else
                {
                    // Create a temporary single pair "name-value" in order
                    // to avoid multiple calls for the same attribute.
                    const char* tmp[4];
                    tmp[0] = attr[i];
                    tmp[1] = attr[i + 1];
                    tmp[2] = 0;
                    tmp[3] = 0;
                    parse_attr(tmp);
                }
            }
        }

        //-------------------------------------------------------------
        int svg_parser::cmp_color(const void* p1, const void* p2)
        {
            return strcmp(((NamedColor::color_def*)p1)->name, ((NamedColor::color_def*)p2)->name);
        }

        //-------------------------------------------------------------
        agg::rgba8 svg_parser::parse_color(const char* str)
        {
            using namespace agg;

            while(*str == ' ') ++str;
            unsigned c = 0;
            char* colorDesc;
            if(*str == '#')
            {
                sscanf(str + 1, "%x", &c);
                return rgb8_packed(c);
            }
            else if ( ( colorDesc = strstr( str, "url(" ) ) != NULL )
            {
            }
            else
            {
                NamedColor::color_def c;
                unsigned len = strlen(str);
                if(len > sizeof(c.name) - 1)
                {
                    throw agg::svg::exception("parse_color: Invalid color name '%s'", str);
                }
                strcpy(c.name, str);
                const void* p = bsearch(&c, 
                                        NamedColor::colors, 
                                        NamedColor::numColors,
    //                                    sizeof(NamedColor::colors) / sizeof(NamedColor::colors[0]), 
                                        sizeof(NamedColor::colors[0]), 
                                        cmp_color);
                if(p == 0)
                {
                    throw agg::svg::exception("parse_color: Invalid color name '%s'", str);
                }
                const NamedColor::color_def* pc = (const NamedColor::color_def*)p;
                return rgba8(pc->r, pc->g, pc->b, pc->a);
            }
        }

        double svg_parser::parse_double(const char* str)
        {
            while(*str == ' ') ++str;
            return atof(str);
        }

        //-------------------------------------------------------------
        bool svg_parser::parse_attr(const char* name, const char* value)
        {
            using namespace agg;

            if(strcmp(name, "style") == 0)
            {
                parse_style(value);
            }
            else
            if(strcmp(name, "fill") == 0)
            {
                if(strcmp(value, "none") == 0)
                {
                    m_path.fill_none();
                }
                else
                {
                    m_path.fill(parse_color(value));
                }
            }
            else
            if(strcmp(name, "fill-opacity") == 0)
            {
                m_path.fill_opacity(parse_double(value));
            }
            else
            if(strcmp(name, "stroke") == 0)
            {
                if(strcmp(value, "none") == 0)
                {
                    m_path.stroke_none();
                }
                else
                {
                    m_path.stroke(parse_color(value));
                }
            }
            else
            if(strcmp(name, "stroke-width") == 0)
            {
                m_path.stroke_width(parse_double(value));
            }
            else
            if(strcmp(name, "stroke-linecap") == 0)
            {
                if(strcmp(value, "butt") == 0)        m_path.line_cap(agg::butt_cap);
                else if(strcmp(value, "round") == 0)  m_path.line_cap(agg::round_cap);
                else if(strcmp(value, "square") == 0) m_path.line_cap(agg::square_cap);
            }
            else
            if(strcmp(name, "stroke-linejoin") == 0)
            {
                if(strcmp(value, "miter") == 0)      m_path.line_join(agg::miter_join);
                else if(strcmp(value, "round") == 0) m_path.line_join(agg::round_join);
                else if(strcmp(value, "bevel") == 0) m_path.line_join(agg::bevel_join);
            }
            else
            if(strcmp(name, "stroke-miterlimit") == 0)
            {
                m_path.miter_limit(parse_double(value));
            }
            else
            if(strcmp(name, "stroke-opacity") == 0)
            {
                m_path.stroke_opacity(parse_double(value));
            }
            else
            if(strcmp(name, "transform") == 0)
            {
                parse_transform(value);
            }
            //else
            //if(strcmp(el, "<OTHER_ATTRIBUTES>") == 0) 
            //{
            //}
            // . . .
            else
            {
                return false;
            }
            return true;
        }

        //-------------------------------------------------------------
        void svg_parser::copy_name(const char* start, const char* end)
        {
            unsigned len = unsigned(end - start);
            if(m_attr_name_len == 0 || len > m_attr_name_len)
            {
                delete [] m_attr_name;
                m_attr_name = new char[len + 1];
                m_attr_name_len = len;
            }
            if(len) memcpy(m_attr_name, start, len);
            m_attr_name[len] = 0;
        }



        //-------------------------------------------------------------
        void svg_parser::copy_value(const char* start, const char* end)
        {
            unsigned len = unsigned(end - start);
            if(m_attr_value_len == 0 || len > m_attr_value_len)
            {
                delete [] m_attr_value;
                m_attr_value = new char[len + 1];
                m_attr_value_len = len;
            }
            if(len) memcpy(m_attr_value, start, len);
            m_attr_value[len] = 0;
        }

        //-------------------------------------------------------------
        bool svg_parser::parse_name_value(const char* nv_start, const char* nv_end)
        {
            const char* str = nv_start;
            while(str < nv_end && *str != ':') ++str;

            const char* val = str;

            // Right Trim
            while(str > nv_start && 
                (*str == ':' || isspace(*str))) --str;
            ++str;

            copy_name(nv_start, str);

            while(val < nv_end && (*val == ':' || isspace(*val))) ++val;
            
            copy_value(val, nv_end);
            return parse_attr(m_attr_name, m_attr_value);
        }

        //-------------------------------------------------------------
        void svg_parser::parse_style(const char* str)
        {
            while(*str)
            {
                // Left Trim
                while(*str && isspace(*str)) ++str;
                const char* nv_start = str;
                while(*str && *str != ';') ++str;
                const char* nv_end = str;

                // Right Trim
                while(nv_end > nv_start && 
                    (*nv_end == ';' || isspace(*nv_end))) --nv_end;
                ++nv_end;

                parse_name_value(nv_start, nv_end);
                if(*str) ++str;
            }

        }

        //-------------------------------------------------------------
        void svg_parser::parse_rect(const char** attr)
        {
            int i;
            double x = 0.0;
            double y = 0.0;
            double w = 0.0;
            double h = 0.0;

            m_path.begin_path();
            for(i = 0; attr[i]; i += 2)
            {
                if(!parse_attr(attr[i], attr[i + 1]))
                {
                    if(strcmp(attr[i], "x") == 0)      x = parse_double(attr[i + 1]);
                    if(strcmp(attr[i], "y") == 0)      y = parse_double(attr[i + 1]);
                    if(strcmp(attr[i], "width") == 0)  w = parse_double(attr[i + 1]);
                    if(strcmp(attr[i], "height") == 0) h = parse_double(attr[i + 1]);
                    // rx - to be implemented 
                    // ry - to be implemented
                }
            }


            if(w != 0.0 && h != 0.0)
            {
                if(w < 0.0) throw agg::svg::exception("parse_rect: Invalid width: %f", w);
                if(h < 0.0) throw agg::svg::exception("parse_rect: Invalid height: %f", h);

                m_path.move_to(x,     y);
                m_path.line_to(x + w, y);
                m_path.line_to(x + w, y + h);
                m_path.line_to(x,     y + h);
                m_path.close_subpath();
            }
            m_path.end_path();
        }

        //-------------------------------------------------------------
        void svg_parser::parse_ellipse(const char** attr)
        {
            try
            {
                int i;
                double cx = 0.0;
                double cy = 0.0;
                double rx = 0.0;
                double ry = 0.0;

                m_path.begin_path();
                for(i = 0; attr[i]; i += 2)
                {
                    if(!parse_attr(attr[i], attr[i + 1]))
                    {
                        if(strcmp(attr[i], "cx") == 0)      cx = parse_double(attr[i + 1]);
                        if(strcmp(attr[i], "cy") == 0)      cy = parse_double(attr[i + 1]);
                        if(strcmp(attr[i], "rx") == 0)      rx = parse_double(attr[i + 1]);
                        if(strcmp(attr[i], "ry") == 0)      ry = parse_double(attr[i + 1]);
                    }
                }


                if( rx != 0.0 && ry != 0.0)
                {
                    if ( rx < 0.0 ) throw agg::svg::exception("parse_ellipse: Invalid rx: %f", rx );
                    if ( ry < 0.0 ) throw agg::svg::exception("parse_ellipse: Invalid ry: %f", ry );

                    //m_path.move_to( cx - rx, cy - ry );
                    //m_path.line_to( cx - rx, cy + ry );
                    //m_path.line_to( cx + rx, cy + ry );
                    //m_path.line_to( cx + rx, cy - ry );

                    //m_path.move_to( 1, 1 );
                    //m_path.arc( 50, 50, 0, 1, 1, 150, 150, false );
                    m_path.move_to( cx - rx, cy );
                    m_path.arc( rx, ry, 0, 0, 0, cx + rx, cy, false );
                    m_path.arc( rx, ry, 0, 0, 0, cx - rx, cy, false );
                    m_path.close_subpath();
                }
                m_path.end_path();
            }
            catch ( agg::svg::exception& e )
            {
            }
        }

        //-------------------------------------------------------------
        void svg_parser::parse_line(const char** attr)
        {
            int i;
            double x1 = 0.0;
            double y1 = 0.0;
            double x2 = 0.0;
            double y2 = 0.0;

            m_path.begin_path();
            for(i = 0; attr[i]; i += 2)
            {
                if(!parse_attr(attr[i], attr[i + 1]))
                {
                    if(strcmp(attr[i], "x1") == 0) x1 = parse_double(attr[i + 1]);
                    if(strcmp(attr[i], "y1") == 0) y1 = parse_double(attr[i + 1]);
                    if(strcmp(attr[i], "x2") == 0) x2 = parse_double(attr[i + 1]);
                    if(strcmp(attr[i], "y2") == 0) y2 = parse_double(attr[i + 1]);
                }
            }

            m_path.move_to(x1, y1);
            m_path.line_to(x2, y2);
            m_path.end_path();
        }

        //-------------------------------------------------------------
        void svg_parser::parse_poly(const char** attr, bool close_flag)
        {
            int i;
            double x = 0.0;
            double y = 0.0;

            m_path.begin_path();
            for(i = 0; attr[i]; i += 2)
            {
                if(!parse_attr(attr[i], attr[i + 1]))
                {
                    if(strcmp(attr[i], "points") == 0) 
                    {
                        m_tokenizer.set_path_str(attr[i + 1]);
                        if(!m_tokenizer.next())
                        {
                            throw agg::svg::exception("parse_poly: Too few coordinates");
                        }
                        x = m_tokenizer.last_number();
                        if(!m_tokenizer.next())
                        {
                            throw agg::svg::exception("parse_poly: Too few coordinates");
                        }
                        y = m_tokenizer.last_number();
                        m_path.move_to(x, y);
                        while(m_tokenizer.next())
                        {
                            x = m_tokenizer.last_number();
                            if(!m_tokenizer.next())
                            {
                                throw agg::svg::exception("parse_poly: Odd number of coordinates");
                            }
                            y = m_tokenizer.last_number();
                            m_path.line_to(x, y);
                        }
                    }
                }
            }
            if(close_flag) 
            {
                m_path.close_subpath();
            }
            m_path.end_path();
        }

        //-------------------------------------------------------------
        void svg_parser::parse_transform(const char* str)
        {
            while(*str)
            {
                if(islower(*str))
                {
                    if(strncmp(str, "matrix", 6) == 0)    str += parse_matrix(str);    else 
                    if(strncmp(str, "translate", 9) == 0) str += parse_translate(str); else 
                    if(strncmp(str, "rotate", 6) == 0)    str += parse_rotate(str);    else 
                    if(strncmp(str, "scale", 5) == 0)     str += parse_scale(str);     else 
                    if(strncmp(str, "skewX", 5) == 0)     str += parse_skew_x(str);    else 
                    if(strncmp(str, "skewY", 5) == 0)     str += parse_skew_y(str);    else
                    {
                        ++str;
                    }
                }
                else
                {
                    ++str;
                }
            }
        }

        //-------------------------------------------------------------
        bool svg_parser::is_numeric(char c)
        {
            return strchr("0123456789+-.eE", c) != 0;
        }

        //-------------------------------------------------------------
        unsigned svg_parser::parse_transform_args(const char* str, 
                                            double* args, 
                                            unsigned max_na, 
                                            unsigned* na)
        {
            *na = 0;
            const char* ptr = str;
            while(*ptr && *ptr != '(') ++ptr;
            if(*ptr == 0)
            {
                throw agg::svg::exception("parse_transform_args: Invalid syntax");
            }
            const char* end = ptr;
            while(*end && *end != ')') ++end;
            if(*end == 0)
            {
                throw agg::svg::exception("parse_transform_args: Invalid syntax");
            }

            while(ptr < end)
            {
                if(is_numeric(*ptr))
                {
                    if(*na >= max_na)
                    {
                        throw agg::svg::exception("parse_transform_args: Too many arguments");
                    }
                    args[(*na)++] = atof(ptr);
                    while(ptr < end && is_numeric(*ptr)) ++ptr;
                }
                else
                {
                    ++ptr;
                }
            }
            return unsigned(end - str);
        }

        //-------------------------------------------------------------
        unsigned svg_parser::parse_matrix(const char* str)
        {
            double args[6];
            unsigned na = 0;
            unsigned len = parse_transform_args(str, args, 6, &na);
            if(na != 6)
            {
                throw agg::svg::exception("parse_matrix: Invalid number of arguments");
            }
            m_path.transform().premultiply(agg::trans_affine(args[0], args[1], args[2], args[3], args[4], args[5]));
            return len;
        }

        //-------------------------------------------------------------
        unsigned svg_parser::parse_translate(const char* str)
        {
            double args[2];
            unsigned na = 0;
            unsigned len = parse_transform_args(str, args, 2, &na);
            if(na == 1) args[1] = 0.0;
            m_path.transform().premultiply(agg::trans_affine_translation(args[0], args[1]));
            return len;
        }

        //-------------------------------------------------------------
        unsigned svg_parser::parse_rotate(const char* str)
        {
            using namespace agg;

            double args[3];
            unsigned na = 0;
            unsigned len = parse_transform_args(str, args, 3, &na);
            if(na == 1) 
            {
                m_path.transform().premultiply(trans_affine_rotation(deg2rad(args[0])));
            }
            else if(na == 3)
            {
                trans_affine t = trans_affine_translation(-args[1], -args[2]);
                t *= trans_affine_rotation(deg2rad(args[0]));
                t *= trans_affine_translation(args[1], args[2]);
                m_path.transform().premultiply(t);
            }
            else
            {
                throw agg::svg::exception("parse_rotate: Invalid number of arguments");
            }
            return len;
        }

        //-------------------------------------------------------------
        unsigned svg_parser::parse_scale(const char* str)
        {
            double args[2];
            unsigned na = 0;
            unsigned len = parse_transform_args(str, args, 2, &na);
            if(na == 1) args[1] = args[0];
            m_path.transform().premultiply(agg::trans_affine_scaling(args[0], args[1]));
            return len;
        }

        //-------------------------------------------------------------
        unsigned svg_parser::parse_skew_x(const char* str)
        {
            double arg;
            unsigned na = 0;
            unsigned len = parse_transform_args(str, &arg, 1, &na);
            m_path.transform().premultiply(agg::trans_affine_skewing(agg::deg2rad(arg), 0.0));
            return len;
        }

        //-------------------------------------------------------------
        unsigned svg_parser::parse_skew_y(const char* str)
        {
            double arg;
            unsigned na = 0;
            unsigned len = parse_transform_args(str, &arg, 1, &na);
            m_path.transform().premultiply(agg::trans_affine_skewing(0.0, agg::deg2rad(arg)));
            return len;
        }

    } // namespace vs
} // namespace AggOO
