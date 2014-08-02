
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
// $Id: AggOOStringUtil.h,v 1.4 2008/03/13 20:20:33 dratek Exp $
//
// AggOOStringUtil.h -- Utilities for working on strings.
//
// Author       : Chad M. Draper
// Date         : July 14, 2007
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2007-07-14      Initial implementation
//

#ifndef AggOOStringUtil_H_
#define AggOOStringUtil_H_

#include "AggOOBase.h"
#include "AggOOEnums.h"
#include "AggOOTypes.h"
#include <vector>
#include <sstream>

namespace AggOO
{
    class _AggOOExport StringUtil : public Base
    {
    public:
        static const STRING WHITE_SPACE;    /** Defines the whitespace characters */
        static const STRING BLANK;          /** Empty string */

        typedef std::vector< STRING > STRING_VECTOR;   /**< A vector of strings */

        /** Trim whitespace from either end of a string */
        static void Trim( STRING& str, bool removeLeft = true, bool removeRight = true );
        static void TrimLeft( STRING& str )     { Trim( str, true, false ); }
        static void TrimRight( STRING& str )    { Trim( str, false, true ); }

        /** Split the string into sub-strings using a user-defined delimiter.
            @param  src         String to be split
            @param  delim       User-defined delimiter
            @param  results     Vector of resultant substrings
            @param  maxSplits   Maximum number of splits (0 = ignore)
            @param  addBlank    Indicates if empty strings should be added to the array
            @return             Number of delimiters found (NOT the number of sub-strings.)
        */
        static size_t Split( const STRING& src, const STRING& delim, STRING_VECTOR& results, size_t maxSplits = 0, bool addBlank = false );

        /** Convert a value to a string */
        template< typename T >
        static STRING ToString( const T& val )
        {
            std::stringstream stream;
            stream << val;
            return stream.str();
        }

        /** Convert a string to upper case */
        static void ToUpper( STRING& str );

        /** Convert a string to lower case */
        static void ToLower( STRING& str );

        /** Convert a string to a REAL.  If the string is not a valid REAL, 0.0
            is returned.
        */
        static REAL ToReal( const STRING& str );

        /** Test if a string represents a number.  This is rigid, and if the
            entire string is not a number, will fail.
        */
        static bool IsNumber( const STRING& str );

        /** Convert a string to an INT.  If the string is not a valid INT, 0
            is returned.
        */
        static INT ToInt( const STRING& str );

        /** Test whether the string starts with a supplied substring.
            @param  str         String to search
            @param  pattern     Pattern to look for
            @param  lowerCase   If true, the string is converted to lower-case
                                before testing.  The pattern is assumed to
                                already be lower-case.
        */
        static bool StartsWith( const STRING& str, const STRING& pattern, bool lowerCase = false );

        /** Test whether the string ends with a supplied substring.
            @param  str         String to search
            @param  pattern     Pattern to look for
            @param  lowerCase   If true, the string is converted to lower-case
                                before testing.  The pattern is assumed to
                                already be lower-case.
        */
        static bool EndsWith( const STRING& str, const STRING& pattern, bool lowerCase = false );

        /** Search for a substring and replace it with a different string.
            @param  str         String to modify
            @param  findStr     String to search for
            @param  newStr      String to use for the replacement
        */
        static STRING FindReplace( const STRING& str, const STRING& findStr, const STRING& newStr );

        /** Method for creating standardized paths.  This replaces back-slash
            pairs ('\\') with forward slashes ('/').
        */
        static STRING StandardizePath( const STRING& path, bool addTrailingSlash = true );

        /** Method to split a full filename and return the path and base name. */
        static void SplitFilename( const STRING& str, STRING& outPath, STRING& outFilename );

        /** Method to split a filename into the extension, and everything else... */
        static void SplitFileExtension( const STRING& str, STRING& outFilename, STRING& outExt );

        /** Method that will split all elements of a file path (path, title,
            and extension) given its path + extension.  This is the same as
            calling splitFilename followed by splitFileExtension.
        */
        inline static void SplitFile( const STRING& str, STRING& outPath, STRING& outTitle, STRING& outExt )
        {
            STRING filename;
            SplitFilename( str, outPath, filename );
            SplitFileExtension( filename, outTitle, outExt );
        }

        /** Split a numbered filename (i.e. file0000.txt) into the basename (file),
            number (0), and extension (txt).
        */
        static void SplitNumberedFilename( const STRING& full, STRING& base, int& number, STRING& ext );

        /** Build a path string, allowing for directory modifiers ('.' and '..')
            @param  basePath    Initial path
            @param  dir         Directory to append to the path
            @param  outPath     Resultant path
            @remarks
                'dir' is first appended to 'basePath.'  The result is then searched
                for modifiers.  If any are found, the path is split at the modifier
                and the necessary directory changes are made.
        */
        static void BuildPath( const STRING& basePath, const STRING& dir, STRING& outPath );
        static void CleanPath( const STRING& inPath, STRING& outPath );

        /** Method to search a String for a user-defined pattern.  The
            pattern may use the '*' wildcard.
            @param  str         String to test
            @param  pattern     Pattern to look for
            @param  caseSensitive   Indicates if the search should be case sensitive
        */
        static bool Match( const STRING& str, const STRING& pattern, bool caseSensitive = true );

        /** Convert an ANSI string to UNICODE
            @remarks    Requires the destination string to be big enough to
                        hold the conversion
        */
        static void AsciiToUnicode( const char* src, wchar_t* dest );
        static std::wstring AsciiToUnicode( const std::string& src );

        /** Convert an UNICODE string to ANSI
            @remarks    Uses ATL to perform the conversion
        */
        static void UnicodeToAscii( const wchar_t* src, char* dest );
        static std::string UnicodeToAscii( const std::wstring& src );
    };
} // namespace AggOO

#endif  // AggOOStringUtil_H_
