
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

/*! $Id: aggoo_vs_svg_parser.h,v 1.1 2009/03/13 23:00:36 dratek Exp $
 *  @file   aggoo_vs_svg_parser.h
 *  @author Chad M. Draper
 *  @date   November 27, 2007
 *  @brief  Class for parsing svg shape files.
 *  
 */

#ifndef aggoo_vs_svg_parser_H_
#define aggoo_vs_svg_parser_H_

#include "AggOOPrereqs.h"

#if ( AGGOO_DYNAMIC_LINKAGE == 0 )
#   define XML_STATIC
#endif
#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
#   if ( AGGOO_DYNAMIC_LINKAGE == 1 )
#       pragma comment( lib, "libexpat.lib" )
#   else
#       pragma comment( lib, "libexpatMT.lib" )
#   endif
#endif

#include "aggoo_vs_base_parser.h"
//#include "svg/agg_svg_parser.h"
#include "svg/agg_svg_path_tokenizer.h"
//#include "svg/agg_svg_path_renderer.h"

#include "expat.h"

namespace AggOO
{
    namespace vs
    {
        /** @class svg_parser
            Parser for svg shape files.
        */
        class svg_parser : public base_parser
        {
        private:
            agg::svg::path_tokenizer m_tokenizer;
            bool    mPatternFlag;       /**< Indicates a pattern is being parsed */
            bool    mSymbolFlag;

        public:
            svg_parser( path_vertices* path );

            //------------------------------------------------------------------------
            bool parse(const char* fname);

            //------------------------------------------------------------------------
            static void start_element(void* data, const char* el, const char** attr);

            //------------------------------------------------------------------------
            static void end_element(void* data, const char* el);


            //------------------------------------------------------------------------
            static void content(void* data, const char* s, int len);

            //------------------------------------------------------------------------
            void parse_attr(const char** attr);

            //-------------------------------------------------------------
            void parse_path(const char** attr);

            //-------------------------------------------------------------
            static int cmp_color(const void* p1, const void* p2);

            //-------------------------------------------------------------
            agg::rgba8 parse_color(const char* str);

            double parse_double(const char* str);

            //-------------------------------------------------------------
            bool parse_attr(const char* name, const char* value);

            //-------------------------------------------------------------
            void copy_name(const char* start, const char* end);

            //-------------------------------------------------------------
            void copy_value(const char* start, const char* end);

            //-------------------------------------------------------------
            bool parse_name_value(const char* nv_start, const char* nv_end);

            //-------------------------------------------------------------
            void parse_style(const char* str);

            //-------------------------------------------------------------
            void parse_rect(const char** attr);

            //-------------------------------------------------------------
            void parse_ellipse(const char** attr);

            //-------------------------------------------------------------
            void parse_line(const char** attr);

            //-------------------------------------------------------------
            void parse_poly(const char** attr, bool close_flag);

            //-------------------------------------------------------------
            void parse_transform(const char* str);

            //-------------------------------------------------------------
            static bool is_numeric(char c);

            //-------------------------------------------------------------
            static unsigned parse_transform_args(const char* str, 
                                                double* args, 
                                                unsigned max_na, 
                                                unsigned* na);

            //-------------------------------------------------------------
            unsigned parse_matrix(const char* str);

            //-------------------------------------------------------------
            unsigned parse_translate(const char* str);

            //-------------------------------------------------------------
            unsigned parse_rotate(const char* str);

            //-------------------------------------------------------------
            unsigned parse_scale(const char* str);

            //-------------------------------------------------------------
            unsigned parse_skew_x(const char* str);

            //-------------------------------------------------------------
            unsigned parse_skew_y(const char* str);

        };
    } // namespace vs
} // namespace AggOO

#endif  // aggoo_vs_svg_parser_H_
