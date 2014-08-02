
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
// $Id: aggoo_ai_tokenizer.cpp,v 1.4 2008/03/03 17:50:23 dratek Exp $
//
// aggoo_ai_tokenizer.cpp
//

#include "aggoo_ai_tokenizer.h"
#include <algorithm>

#include "AggOOStringUtil.h"

namespace AggOO
{
    namespace ai
    {
        //AggOO::StringUtil::STRING_VECTOR Tokenizer::svCommands;
//        const std::string Tokenizer::sCommands   = "%BbCcdGgHhJjKkLlMmNnSsVvwXYy";
        //const std::string Tokenizer::sNumeric    = "-+.Ee0123456789";
        //const std::string Tokenizer::sDelimiters = " ,\t\n\r";

        Tokenizer::Tokenizer(void)
            : mPath(""),
              sNumeric( "-+.Ee0123456789" ),
              sDelimiters( " ,\t\n\r" )
        {
            TokMapType  tokens[] = {
                            TokMapType( "B", B ), TokMapType( "b", b ),\
                            TokMapType( "C", C ), TokMapType( "c", c ),\
                            TokMapType( "d", d ),\
                            TokMapType( "F", F ), TokMapType( "f", f ),\
                            TokMapType( "G", G ), TokMapType( "g", g ),\
                            TokMapType( "H", H ), TokMapType( "h", h ),\
                            TokMapType( "J", J ), TokMapType( "j", j ),\
                            TokMapType( "K", K ), TokMapType( "k", k ),\
                            TokMapType( "L", L ), TokMapType( "l", l ),\
                            TokMapType( "M", M ), TokMapType( "m", m ),\
                            TokMapType( "N", N ), TokMapType( "n", n ),\
                            TokMapType( "S", S ), TokMapType( "s", s ),\
                            TokMapType( "*u", star_u ), TokMapType( "*U", star_U ),\
                            TokMapType( "V", V ), TokMapType( "v", v ),\
                            TokMapType( "w", w ),\
                            TokMapType( "Xa", Xa ), TokMapType( "XA", XA ), TokMapType( "XR", XR ),\
                            TokMapType( "Y", Y ), TokMapType( "y", y )\
            };
            mapCommands.insert( tokens, tokens + sizeof( tokens ) / sizeof( *tokens ) );
            //std::string commands[] = {\
            //                        "B", "b",\
            //                        "C", "c",\
            //                        "d",\
            //                        "G", "g",\
            //                        "H", "h",\
            //                        "J", "j",\
            //                        "K", "k",\
            //                        "L", "l",\
            //                        "M", "m",\
            //                        "N", "n",\
            //                        "S", "s",\
            //                        "V", "v",\
            //                        "w",\
            //                        "X",\
            //                        "Y", "y" };
            //svCommands.assign( commands, commands + sizeof( commands ) / sizeof( *commands ) );
        }

        Tokenizer::~Tokenizer(void)
        {
        }

        //isCommand-------------------------------------------------------------
        bool Tokenizer::isCommand( const std::string& str ) const
        {
            // str is a command only if it is in the commands list.
            return ( mapCommands.find( str ) != mapCommands.end() );
        }

        //isNumeric-------------------------------------------------------------
        bool Tokenizer::isNumeric( const std::string& str ) const
        {
            // A numeric string must only contain numeric characters.
            return AggOO::StringUtil::IsNumber( str );
        }

        //SetPath---------------------------------------------------------------
        void Tokenizer::SetPath( const std::string& path )
        {
            mPath = path;
            mPath = AggOO::StringUtil::FindReplace( mPath, "\n", "" );
            mPath = AggOO::StringUtil::FindReplace( mPath, "\r", "" );
            mPath = AggOO::StringUtil::FindReplace( mPath, "\t", " " );
            AggOO::StringUtil::Trim( mPath );

            // Break the path into elements
            mElems.clear();
            AggOO::StringUtil::Split( mPath, sDelimiters, mElems );

            // Command operators come after the parameters.  For instance, the
            // moveto operator would be given as posx posy m.  Because of this,
            // the elements will be extracted in reverse order (i.e. m posy posx).
            // However, comments don't adhere to this rule (assuming comments are
            // even needed.)  If this line is a comment, reverse the order of the
            // elements so the reverse iterator will take them in the right order.
            if ( mPath.length() > 0 && mPath.at( 0 ) == '%' )
            {
                std::reverse( mElems.begin(), mElems.end() );
            }

            // Start the iterator at the end of the line
            mElemIter = mElems.rbegin();
        }

        //Next------------------------------------------------------------------
        bool Tokenizer::Next( std::string& elem )
        {
            // Get the element and advance the iterator
            if ( mElemIter == mElems.rend() )
                return false;
            elem = *mElemIter;
            mElemIter++;
            return true;
        }

        //NextCommand-----------------------------------------------------------
        bool Tokenizer::NextCommand( std::string& cmd )
        {
            // Advance the iterator until a new command string is found.
            bool result = false;
            cmd = "";
            std::string tempCmd;
            while ( Next( tempCmd ) )
            {
                if ( isCommand( tempCmd ) )
                {
                    cmd = tempCmd;
                    return true;
                }

                // If the element wasn't a command, and is a comment (some, but
                // not all, comments can be commands) then skip to the end of
                // the token path.
                if ( tempCmd[ 0 ] == '%' )
                    GoToEnd();
            }
            return false;
        }

        //NextCommandID---------------------------------------------------------
        Tokenizer::TokID Tokenizer::NextCommandID()
        {
            // Get the name of the next command:
            std::string cmd;
            if ( NextCommand( cmd ) )
            {
                return mapCommands.find( cmd )->second;
            }
            return undefined;
        }

        //GetCommandID----------------------------------------------------------
        Tokenizer::TokID Tokenizer::GetCommandID( const std::string& cmd ) const
        {
            TokMap::const_iterator iter = mapCommands.find( cmd );
            if ( iter != mapCommands.end() )
                return iter->second;
            return undefined;
        }

        //GetCommandName--------------------------------------------------------
        bool Tokenizer::GetCommandName( TokID id, std::string& cmd ) const
        {
            // While getting the ID is pretty quick, getting the name from the ID
            // is a bit more involved.  Loop over the map until the ID is found,
            // or until the end of the map is reached.
            TokMap::const_iterator iter = mapCommands.begin();
            cmd = "";
            while ( iter != mapCommands.end() )
            {
                if ( iter->second == id )
                {
                    cmd = iter->first;
                    return true;
                }
                iter++;
            }
            return false;
        }

        //GetParameters---------------------------------------------------------
        size_t Tokenizer::GetParameters( size_t n, AggOO::StringUtil::STRING_VECTOR& params )
        {
            // Clear params
            params.clear();

            // Starting with the current element, read n elements and store them
            // in params.  Stop reading if a non-numeric element is read, or the
            // end of the line is reached.
            size_t numParams = 0;
            while ( numParams < n && isNumeric( *mElemIter ) && mElemIter != mElems.rend() )
            {
                params.push_back( *mElemIter );
                numParams++;
                mElemIter++;
            }

            // Reverse params so the parameters are in the correct order
            std::reverse( params.begin(), params.end() );

            return numParams;
        }

    } // namespace ai
} // namespace AggOO
