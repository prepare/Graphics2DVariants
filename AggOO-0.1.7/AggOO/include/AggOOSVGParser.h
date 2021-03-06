
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
// $Id: AggOOSVGParser.h,v 1.2 2007/08/10 18:38:42 dratek Exp $
//
// AggOOSVGParser.h -- Header file for the SVGParser class
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

#ifndef AggOOSVGParser_H_
#define AggOOSVGParser_H_

#include "AggOOBase.h"
#include "AggOOEnums.h"
#include "AggOOTypes.h"

#include "agg_svg_parser_templ.h"
#include "agg_svg_path_vertices.h"

namespace AggOO
{
    namespace svg
    {
        class SVGPath;

        /** @class SVGParser
            Wrapper for the SVG path parser.

            @note   This requires ExpatXML for parsing.  Include the appropriate
                    expat library (i.e. libexpat.lib, libexpatMT.lib,
                    libexpatML.lib, etc.)  Also, if using the static version, be
                    sure to define XML_STATIC.
        */
        class _AggOOExport SVGParser : public Base
        {
        private:
            svg::parser_templ<svg::path_vertices>*   mParser;

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
            SVGParser( SVGPath& path );
            virtual ~SVGParser(void);

            Status Parse(const char* fname);
            const char* GetTitle() const;

        }; // class SVGParser

    } // namespace svg
} // namespace AggOO

#endif  // AggOOSVGParser_H_
