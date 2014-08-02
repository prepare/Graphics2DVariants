
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

/*! $Id: aggoo_vs_ai_parser.cpp,v 1.1 2008/01/10 18:02:25 dratek Exp $
 *  @file   aggoo_vs_ai_parser.cpp
 *  @author Chad M. Draper
 *  @date   November 27, 2007
 *  
 */

#include "aggoo_vs_ai_parser.h"

namespace AggOO
{
    namespace vs
    {
        ////////////////////////////////////////////////////////////////////////
        // AIConverter class

        //cmykToRgb-------------------------------------------------------------
        void AIConverter::cmykToRgb( double c, double m, double y, double k, double *r, double *g, double *b )
        {
            //unsigned int r, g, b, a = 255;
            //r = ( 1.0 - min( 1.0, c + k ) ) * 255;
            //g = ( 1.0 - min( 1.0, c + k ) ) * 255;
            //b = ( 1.0 - min( 1.0, c + k ) ) * 255;
            *r = ( 1.0 - min( 1.0, c + k ) );
            *g = ( 1.0 - min( 1.0, m + k ) );
            *b = ( 1.0 - min( 1.0, y + k ) );
            //return agg::rgba8( ( 1.0 - min( 1.0, c + k ) ) * 255,
            //                   ( 1.0 - min( 1.0, m + k ) ) * 255,
            //                   ( 1.0 - min( 1.0, y + k ) ) * 255,
            //                   255 );
        }


        //
        // ai_parser
        //

        ai_parser::ai_parser( path_vertices* path )
            : base_parser( path ),
              m_tokenizer()
        {
        }

        bool ai_parser::parse( const char* fname )
        {
            using AggOO::ai::Tokenizer;

            std::ifstream fp;
            fp.open( fname, std::ios::in );
            if ( !fp.is_open() )
                return false;

            std::string filename, ext;
            StringUtil::SplitFileExtension( fname, filename, ext );

            // Begin the path
            m_path.begin_path();

            AggOO::ai::Tokenizer tok;
            bool    joinPath = false;
            while ( !fp.eof() )
            {
                // Read a line of text:
                std::string line;
                std::getline( fp, line, '\n' );

                tok.SetPath( line );

                std::string cmd;
                Tokenizer::TokID tokID;
                AggOO::StringUtil::STRING_VECTOR params;
                while ( ( tokID = tok.NextCommandID() ) != Tokenizer::undefined )
                {
                    params.clear();
                    switch ( tokID )
                    {
                    case Tokenizer::b:   // Close, fill, and stroke path
                        m_path.close_subpath();
                    case Tokenizer::B:   // Fill and stroke path
                        if ( !joinPath )
                        {
                            m_path.end_path();
                            m_path.begin_path();
                        }
                        break;

                    case Tokenizer::c:   // curve4
                    case Tokenizer::C:
                        // Get the control points:
                        if ( tok.GetParameters( 6, params ) == 6 )
                        {
                            m_path.curve4( StringUtil::ToReal( params[ 0 ] ),
                                           StringUtil::ToReal( params[ 1 ] ),
                                           StringUtil::ToReal( params[ 2 ] ),
                                           StringUtil::ToReal( params[ 3 ] ),
                                           StringUtil::ToReal( params[ 4 ] ),
                                           StringUtil::ToReal( params[ 5 ] ) );
                        }
                        break;

                    case Tokenizer::d:   // Line dash
                        break;

                    case Tokenizer::f:  // close and fill path
                        m_path.close_subpath();
                    case Tokenizer::F:  // Fill path
                        if ( !joinPath )
                        {
                            m_path.end_path();
                            m_path.begin_path();
                        }
                        break;

                    case Tokenizer::g:   // fill gray
                        if ( tok.GetParameters( 1, params ) == 1 )
                        {
                            REAL gray = StringUtil::ToReal( params[ 0 ] ) * 255;
                            agg::rgba8 color( gray, gray, gray, 255 );
                            m_path.fill( color );
                        }
                        break;

                    case Tokenizer::G:   // Stroke gray
                        if ( tok.GetParameters( 1, params ) == 1 )
                        {
                            REAL gray = StringUtil::ToReal( params[ 0 ] ) * 255;
                            agg::rgba8 color( gray, gray, gray, 255 );
                            m_path.stroke( color );
                        }
                        break;

                    case Tokenizer::h:   // Close path
                    case Tokenizer::H:
                        m_path.close_subpath();
                        if ( !joinPath )
                        {
                            m_path.end_path();
                            m_path.begin_path();
                        }
                        break;

                    case Tokenizer::j:   // Line join
                        if ( tok.GetParameters( 1, params ) == 1 )
                        {
                            if ( params[ 0 ] == "0" )
                                m_path.line_join( agg::miter_join );
                            else if ( params[ 0 ] == "1" )
                                m_path.line_join( agg::round_join );
                            else if ( params[ 0 ] == "2" )
                                m_path.line_join( agg::bevel_join );
                        }
                        break;

                    case Tokenizer::J:   // Line cap
                        if ( tok.GetParameters( 1, params ) == 1 )
                        {
                            if ( params[ 0 ] == "0" )
                                m_path.line_cap( agg::butt_cap );
                            else if ( params[ 0 ] == "1" )
                                m_path.line_cap( agg::round_cap );
                            else if ( params[ 0 ] == "2" )
                                m_path.line_cap( agg::square_cap );
                        }
                        break;

                    case Tokenizer::k:   // Fill CMYK color
                        if ( tok.GetParameters( 4, params ) == 4 )
                        {
                            //agg::rgba8 color = AIConverter::cmykToRgb(
                            //    StringUtil::ToReal( params[ 0 ] ),
                            //    StringUtil::ToReal( params[ 1 ] ),
                            //    StringUtil::ToReal( params[ 2 ] ),
                            //    StringUtil::ToReal( params[ 3 ] ) );
                            double r, g, b;
                            AIConverter::cmykToRgb(
                                StringUtil::ToReal( params[ 0 ] ),
                                StringUtil::ToReal( params[ 1 ] ),
                                StringUtil::ToReal( params[ 2 ] ),
                                StringUtil::ToReal( params[ 3 ] ), &r, &g, &b );
                            agg::rgba8 color( r * 255, g * 255, b * 255 );
                            m_path.fill( color );
                        }
                        break;

                    case Tokenizer::K:   // Stroke CMYK color
                        if ( tok.GetParameters( 4, params ) == 4 )
                        {
                            //agg::rgba8 color = AIConverter::cmykToRgb(
                            //    StringUtil::ToReal( params[ 0 ] ),
                            //    StringUtil::ToReal( params[ 1 ] ),
                            //    StringUtil::ToReal( params[ 2 ] ),
                            //    StringUtil::ToReal( params[ 3 ] ) );
                            //m_path.stroke( color );
                            double r, g, b;
                            AIConverter::cmykToRgb(
                                StringUtil::ToReal( params[ 0 ] ),
                                StringUtil::ToReal( params[ 1 ] ),
                                StringUtil::ToReal( params[ 2 ] ),
                                StringUtil::ToReal( params[ 3 ] ), &r, &g, &b );
                            m_path.stroke( agg::rgba8( unsigned int( r * 255 ),
                                                       unsigned int( g * 255 ),
                                                       unsigned int( b * 255 ) ) );
                        }
                        break;

                    case Tokenizer::l:   // line_to
                    case Tokenizer::L:
                        // Get the x and y pos:
                        if ( tok.GetParameters( 2, params ) == 2 )
                        {
                            m_path.line_to( StringUtil::ToReal( params[ 0 ] ), StringUtil::ToReal( params[ 1 ] ) );
                        }
                        break;

                    case Tokenizer::m:   // move_to
                        // Get the x and y pos:
                        if ( tok.GetParameters( 2, params ) == 2 )
                        {
                            m_path.move_to( StringUtil::ToReal( params[ 0 ] ), StringUtil::ToReal( params[ 1 ] ) );
                        }
                        break;

                    case Tokenizer::M:   // Miter limit
                        // Get the x and y pos:
                        if ( tok.GetParameters( 1, params ) == 1 )
                        {
                            REAL miterLimit = StringUtil::ToReal( params[ 0 ] );
                            m_path.miter_limit( miterLimit );
                        }
                        break;

                    case Tokenizer::N:   // Path is not filled or stroked.  Close path and create a new path.
                        m_path.close_subpath();
                    case Tokenizer::n:   // Path is not filled or stroked.  Create a new path.
                        m_path.fill_none();
                        m_path.stroke_none();
                        m_path.end_path();
                        m_path.begin_path();
                        break;

                    case Tokenizer::s:   // Close and stroke path
                        m_path.close_subpath();
                    case Tokenizer::S:   // Stroke path
                        m_path.fill_none();
                        if ( !joinPath )
                        {
                            m_path.end_path();
                            m_path.begin_path();
                        }
                        break;

                    case Tokenizer::star_u: // Begin joined path
                        joinPath = true;
                        break;

                    case Tokenizer::star_U: // End joined path
                        joinPath = false;
                        m_path.end_path();
                        break;

                    case Tokenizer::v:   // curve4 (2 parameters)
                    case Tokenizer::V:
                    case Tokenizer::y:   // This isn't quite right, as the current point should be x3,y3...
                    case Tokenizer::Y:
                        if ( tok.GetParameters( 4, params ) == 4 )
                        {
                            m_path.curve4( StringUtil::ToReal( params[ 0 ] ),
                                           StringUtil::ToReal( params[ 1 ] ),
                                           StringUtil::ToReal( params[ 2 ] ),
                                           StringUtil::ToReal( params[ 3 ] ) );
                        }
                        break;

                    case Tokenizer::w:   // line width
                        if ( tok.GetParameters( 1, params ) == 1 )
                            m_path.stroke_width( StringUtil::ToReal( params[ 0 ] ) );
                        break;

                    case Tokenizer::Xa: // Fill RGB color (r g b Xa)
                        if ( tok.GetParameters( 3, params ) == 3 )
                        {
                            agg::rgba8 color(
                                StringUtil::ToReal( params[ 0 ] ) * 255,
                                StringUtil::ToReal( params[ 1 ] ) * 255,
                                StringUtil::ToReal( params[ 2 ] ) * 255 );
                            m_path.fill( color );
                        }
                        break;

                    case Tokenizer::XA: // Stroke RGB color (r g b XA)
                        if ( tok.GetParameters( 3, params ) == 3 )
                        {
                            agg::rgba8 color(
                                StringUtil::ToReal( params[ 0 ] ) * 255,
                                StringUtil::ToReal( params[ 1 ] ) * 255,
                                StringUtil::ToReal( params[ 2 ] ) * 255 );
                            m_path.stroke( color );
                        }
                        break;

                    case Tokenizer::XR: // Fill rule (n XR, where n=0 = use non-zero, n=1 = even-odd.)
                        if ( tok.GetParameters( 1, params ) == 1 )
                        {
                            if ( params[ 0 ] == "0" )
                                m_path.even_odd( false );
                            else
                                m_path.even_odd( true );
                        }
                        break;
                    }
                }
            }

            return true;
        }
    }
}
