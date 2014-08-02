
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

/*! $Id: aggoo_vs_base_parser.h,v 1.1 2008/01/10 18:02:18 dratek Exp $
 *  @file   aggoo_vs_base_parser.h
 *  @author Chad M. Draper
 *  @date   November 27, 2007
 *  @brief  Base class for vector shape parsers.
 *  
 */

#ifndef aggoo_vs_base_parser_H_
#define aggoo_vs_base_parser_H_

#include <stdio.h>
#include <agg_color_rgba.h>
#include "aggoo_vs_path_vertices.h"

namespace AggOO
{
    namespace vs
    {
        class path_vertices;

        /** @class  NamedColor
            @brief  Allows colors to be specified by name (i.e. Red, Green, Blue.)
        */
        class NamedColor
        {
        public:
            struct color_def
            {
                char  name[22];
                agg::int8u r, g, b, a;
            };

            static color_def colors[];
            static size_t    numColors;

            NamedColor();
        };

        /** @class  base_parser
            @brief  Base class used for the vector shape file parsers.  This way,
                    a wrapper can be implemented which will take care of calling
                    the proper parser.
        */
        class base_parser
        {
        protected:
            enum buf_size_e { buf_size = BUFSIZ };

            path_vertices&  m_path;
            char*           m_buf;
            char*           m_title;
            unsigned        m_title_len;
            bool            m_title_flag;
            bool            m_path_flag;
            char*           m_attr_name;
            char*           m_attr_value;
            unsigned        m_attr_name_len;
            unsigned        m_attr_value_len;

        public:
            base_parser( path_vertices* path );
            virtual ~base_parser(void);

            /** Parse a vector shape file
                @param  fname       Name of the shape file to parse
            */
            virtual bool parse( const char* fname ) = 0;

            /** Get the title of the file being parsed */
            char* title() const
            {
                return m_title;
            }

        };
    } // namespace vs
} // namespace AggOO

#endif  // aggoo_vs_base_parser_H_
