
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

/*! $Id: aggoo_vs_ai_parser.h,v 1.1 2009/03/13 23:00:36 dratek Exp $
 *  @file   aggoo_vs_ai_parser.h
 *  @author Chad M. Draper
 *  @date   November 27, 2007
 *  @brief  Class for parsing ai shape files.
 *  
 */

#ifndef aggoo_vs_ai_parser_H_
#define aggoo_vs_ai_parser_H_

#include <fstream>
#include <string>
#include <stdio.h>
#include "aggoo_vs_base_parser.h"
#include "aggoo_ai_tokenizer.h"

namespace AggOO
{
    namespace vs
    {
        /** @class AIConverter
            Some values in the .ai file will need to be converted in order to be
            used in AggOO.  This class provides static methods to handle the
            conversions.
        */
        class AIConverter
        {
        public:
            /** Converts CMYK to RGB, using the method used by the Adobe prolog
                in the case where the interpreter cannot use CMYK.  This is
                defined as:
                    r = 1 - min( 1, C + K )
                    g = 1 - min( 1, M + K )
                    b = 1 - min( 1, Y + K )
            */
            static void cmykToRgb( double c, double m, double y, double k, double *r, double *g, double *b );
        };

        /** Parser for Adobe Illustrator (*.ai) files. */
        class ai_parser : public base_parser
        {
        private:
            AggOO::ai::Tokenizer m_tokenizer;

        public:
            ai_parser( path_vertices* path );

            bool parse( const char* fname );

        };
    } // namespace vs
} // namespace AggOO

#endif  // aggoo_vs_ai_parser_H_
