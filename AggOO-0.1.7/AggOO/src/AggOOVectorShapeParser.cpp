
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

/*! $Id: AggOOVectorShapeParser.cpp,v 1.4 2008/03/13 20:20:37 dratek Exp $
 *  @file   AggOOVectorShapeParser.h
 *  @author Chad M. Draper
 *  @date   November 27, 2007
 *  
 */

#include "AggOOVectorShapeParser.h"
#include "AggOOVectorShapePath.h"
#include "AggOOStringUtil.h"

namespace AggOO
{
    namespace vs
    {
        VectorShapeParser::VectorShapeParser( VectorShapePath* path )
        {
            mParser = NULL;
            mPath = path;
            //mParser = new AggOO::svg::parser_templ<AggOO::svg::path_vertices>( path.mPath );
        }

        VectorShapeParser::~VectorShapeParser(void)
        {
            delete mParser;
        }

        Status VectorShapeParser::Parse(const char* fname)
        {

            try
            {
                // Clear out any existing parser
                delete mParser;
                mParser = NULL;

                // Assign the parser based on file type:
                STRING  name, ext;
                StringUtil::SplitFileExtension( fname, name, ext );
                StringUtil::ToLower( ext );
                if ( StringUtil::StartsWith( ext, "svg" ) )
                {
                    mParser = new svg_parser( &mPath->mPath );
                    mPath->mFlipVertical = false;
                    mPath->mFlipHorizontal = false;
                }
                else if ( StringUtil::StartsWith( ext, "ai" ) )
                {
                    mParser = new ai_parser( &mPath->mPath );
                    mPath->mFlipVertical = true;
                    mPath->mFlipHorizontal = false;
                }

                if ( mParser )
                {
                    mParser->parse( fname );
                    return SetStatus( Ok );
                    //try
                    //{
                    //    if ( mParser->parse( fname ) )
                    //        return SetStatus( Ok );
                    //}
                    //catch ( agg::svg::exception& e )
                    //{
                    //    cmd::LogFileManager::getInstance() << "Parse Error: " << e.msg() << std::endl;
                    //}
                    //return SetStatus( GenericError );
                }
            }
            catch ( ... )
            {
                return SetStatus( GenericError );
            }
            return SetStatus( InsufficientBuffer );
        }

        const char* VectorShapeParser::GetTitle() const
        {
            if ( mParser )
            {
                SetStatus( Ok );
                return mParser->title();
            }

            SetStatus( InsufficientBuffer );
            return "";
        }

    } // namespace vs
} // namespace AggOO
