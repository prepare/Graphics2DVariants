
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
// $Id: aggoo_ai_tokenizer.h,v 1.1 2009/03/13 23:00:36 dratek Exp $
//
// aggoo_ai_tokenizer.h -- Tokenizes a string for .ai file parsing.
//
// Author       : Chad M. Draper
// Date         : July 18, 2007
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2007-07-18      Initial implementation
//

#ifndef aggoo_ai_tokenizer_H_
#define aggoo_ai_tokenizer_H_

#include <string>
#include <map>
#include "AggOOStringUtil.h"

namespace AggOO
{
    namespace ai
    {
        class _AggOOExport Tokenizer
        {
        public:
            // Tokens.  Remember to update the token map in aggoo_ai_tokenizer.cpp
            // when making changes to this list.
            enum TokID { undefined, B, b, C, c, d, F, f, G, g, H, h, J, j, K, k, L, l, M, m, N, n, S, s, star_u, star_U, V, v, w, Xa, XA, XR, Y, y };

        private:
            std::string mPath;              /**< Current line to parse */
            AggOO::StringUtil::STRING_VECTOR mElems;    /**< Elements in the parsing line */
            AggOO::StringUtil::STRING_VECTOR::reverse_iterator mElemIter;

            /** Create a string of allowed commands.  These are single character
                values, as defined in the .ai spec.  Some commands, such as those
                used for text, might be multiple characters (To begins a text
                object.)  The tokenizer only looks at the first character of an
                element to determine if it is a command.  The parser should look
                at the first element, see that it is a 'T', then look at the entire
                element and act on it.
            */
            
            //static AggOO::StringUtil::STRING_VECTOR svCommands; /**< Allowable commands */
            typedef std::map< std::string, TokID >  TokMap;
            typedef TokMap::value_type              TokMapType;
            TokMap              mapCommands;        /**< Allowable commands */
            const std::string   sNumeric;           /**< Allowed numeric characters */
            const std::string   sDelimiters;        /**< Characters used to delimit elements */

            /** Test whether an element is a command */
            bool isCommand( const std::string& str ) const;
            /** Test whether an element is a number */
            bool isNumeric( const std::string& str ) const;

        public:
            Tokenizer(void);
            ~Tokenizer(void);

            /** Go to the end of the token path, discarding the line. */
            void GoToEnd()      { mElemIter = mElems.rend(); }

            /** Set the path to tokenize */
            void SetPath( const std::string& path );

            /** Get the next element.  Thise will return whatever the next
                element is, regardless of its type.
            */
            bool Next( std::string& elem );

            /** Get the next COMMAND ELEMENT.  Advances the elem iterator.
                @param  cmd             Receives the next command string
                @return  true is successful, false if the end of the line is reached.
            */
            bool NextCommand( std::string& cmd );

            /** Get the next COMMAND ELEMENT.  Advances the elem iterator.
                @return ID of the next command element.  If the end of the line
                        is reached, TokID::undefined is returned.
            */
            TokID NextCommandID();

            /** Get the ID for a given command name
                @param  cmd             Name of the command
                @return ID of the next command element.  If the end of the line
                        is reached, TokID::undefined is returned.
            */
            TokID GetCommandID( const std::string& cmd ) const;

            /** Get the name of a command from its ID
                @param  id              ID of the command
                @param  cmd             Name of the command (empty if the ID
                                        is not found.)
                @return true if the command name is found, false otherwise.
            */
            bool GetCommandName( TokID id, std::string& cmd ) const;

            /** Get the next n parameters.
                @param  n           Number of parameters to extract
                @param  params      Vector to hold the parameters
                @return Number of parameters actually read.

                @note   The parameters return in params are in the correct order.
                        In other words, if the last command was a moveto, which
                        is listed as posx posy m, it would be read by the
                        tokenizer as m posy posx.  However, params will have the
                        order of [posx, posy] (m was already discarded by
                        NextCommand.)
            */
            size_t GetParameters( size_t n, AggOO::StringUtil::STRING_VECTOR& params );
            
        };
    } // namespace ai
} // namespace AggOO

#endif  // aggoo_ai_tokenizer_H_
