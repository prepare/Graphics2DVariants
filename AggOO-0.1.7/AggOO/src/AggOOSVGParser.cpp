
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
// $Id: AggOOSVGParser.cpp,v 1.1 2007/05/23 12:36:37 dratek Exp $
//
// AggOOSVGParser.cpp
//

#include "AggOOSVGParser.h"
#include "AggOOSVGPath.h"

namespace AggOO
{
    namespace svg
    {
        SVGParser::SVGParser( SVGPath& path )
        {
            mParser = new AggOO::svg::parser_templ<AggOO::svg::path_vertices>( path.mPath );
        }

        SVGParser::~SVGParser(void)
        {
            delete mParser;
        }

        Status SVGParser::Parse(const char* fname)
        {
            try
            {
                if ( mParser )
                {
                    mParser->parse( fname );
                    return SetStatus( Ok );
                }
            }
            catch ( ... )
            {
                return SetStatus( GenericError );
            }
            return SetStatus( InsufficientBuffer );
        }

        const char* SVGParser::GetTitle() const
        {
            if ( mParser )
            {
                SetStatus( Ok );
                return mParser->title();
            }

            SetStatus( InsufficientBuffer );
            return "";
        }

    } // namespace svg
} // namespace AggOO
