
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
// $Id: AggOOStringUtil.cpp,v 1.5 2009/02/26 22:13:38 dratek Exp $
//
// AggOOStringUtil.h -- Utilities for working on strings.
//

#include "AggOOStringUtil.h"
#include <algorithm>
#include <sstream>
#include <tchar.h>

namespace AggOO
{
    const STRING StringUtil::WHITE_SPACE = _T( " \t\n\r" );
    const STRING StringUtil::BLANK       = _T( "" );

    //Trim--------------------------------------------------------------------------
    void StringUtil::Trim( STRING& str, bool removeLeft, bool removeRight )
    {
        // Method (for trim start.  end is similar)
        // 1.) find the location of the first character that is not whitespace.
        // 2.) erase all characters up to that index.
        if ( removeLeft )
            str.erase( 0, str.find_first_not_of( StringUtil::WHITE_SPACE ) );
        if ( removeRight )
            str.erase( str.find_last_not_of( StringUtil::WHITE_SPACE ) + 1 );
    }

    //Split---------------------------------------------------------------------
    size_t StringUtil::Split( const STRING& src, const STRING& delim, StringUtil::STRING_VECTOR& results, size_t maxSplits, bool addBlank )
    {
        // Make sure the source and delimiter are specified
        if ( src.size() == 0 || delim.size() == 0 )
        {
            return 0;
        }
    
        size_t delimFound = 0, curPos, startPos = 0;
        STRING curString = StringUtil::BLANK;
        do
        {
            // Find the first delimiter
            curPos = src.find_first_of( delim, startPos );
            if ( curPos == startPos )
            {
                // There is a delimiter at the start of the string.  Increment
                // the delimiter count, and if adding empty strings, add one.
                delimFound++;
                if ( addBlank )
                {
                    results.push_back( curString );
                }
    
                startPos = curPos + 1;
            }
    
            // Check for the last match
            else if ( curPos == STRING::npos )
            {
                // Add the remainder of the source to the results, checking
                // for an empty string.
                curString = src.substr( startPos );
                if ( curString.length() || addBlank )
                    results.push_back( curString );
                break;
            }
    
            // Store the current substring
            else
            {
                delimFound++;
                results.push_back( src.substr( startPos, curPos - startPos ) );
    
                // Set the start position just past the delimiter
                startPos = curPos + 1;
            }
        } while ( curPos != STRING::npos );
    
        return delimFound;
    }

    //ToUpper-------------------------------------------------------------------
    void StringUtil::ToUpper( STRING& str )
    {
        // Iterate over the string, converting each character's case:
		std::transform( str.begin(), str.end(), str.begin(), toupper );
    }

    //ToLower-------------------------------------------------------------------
    void StringUtil::ToLower( STRING& str )
    {
        // Iterate over the string, converting each character's case:
        std::transform( str.begin(), str.end(), str.begin(), tolower );
    }

    //ToReal--------------------------------------------------------------------
    REAL StringUtil::ToReal( const STRING& str )
    {
        REAL val;
        std::istringstream( str ) >> val;
        return val;
    }

    //ToInt---------------------------------------------------------------------
    INT StringUtil::ToInt( const STRING& str )
    {
        INT val;
        std::istringstream( str ) >> val;
        return val;
    }

    //IsNumber------------------------------------------------------------------
    bool StringUtil::IsNumber( const STRING& str )
    {
        REAL val;
        STRING leftover;
        std::istringstream iss( str );
        if ( ( iss >> val ).fail() )
            return false;
        if ( ( iss >> leftover ).fail() )
            return true;
        return false;
    }

    //StartsWith----------------------------------------------------------------
    bool StringUtil::StartsWith( const STRING& str, const STRING& pattern, bool lowerCase )
    {
        // First examine the length of the string and the pattern.  The
        // string should be at least the length of the pattern.
        size_t strLen = str.length();
        size_t patLen = pattern.length();
        if ( strLen < patLen || patLen == 0 )
            return false;

        // Examine the first part of the string to see if it matches the pattern:
        STRING strStart = str.substr( 0, patLen );
        if ( lowerCase )
            StringUtil::ToLower( strStart );

        return ( strStart == pattern );
    }

    //EndsWith------------------------------------------------------------------
    bool StringUtil::EndsWith( const STRING& str, const STRING& pattern, bool lowerCase )
    {
        // First examine the length of the string and the pattern.  The
        // string should be at least the length of the pattern.
        size_t strLen = str.length();
        size_t patLen = pattern.length();
        if ( strLen < patLen || patLen == 0 )
            return false;

        // Examine the first part of the string to see if it matches the pattern:
        STRING strEnd = str.substr( strLen - patLen, patLen );
        if ( lowerCase )
            StringUtil::ToLower( strEnd );

        return ( strEnd == pattern );
    }

    //FindReplace---------------------------------------------------------------
    STRING StringUtil::FindReplace( const STRING& str, const STRING& findStr, const STRING& newStr )
    {
        // As long as the substring exists, replace it in the string:
        int index;
        STRING result = str;
        while ( ( index = result.find( findStr ) ) != STRING::npos )
        {
            result.replace( index, findStr.length(), newStr );
        }

        return result;
    }

    //StandardizePath-----------------------------------------------------------
    STRING StringUtil::StandardizePath( const STRING& path, bool addTrailingSlash )
    {
        // Replace all occurrances of '\\' with '/':
        STRING fixedPath = path;
        std::replace( fixedPath.begin(), fixedPath.end(), _T('\\'), _T('/') );

        // Make sure the last character is a slash:
        if ( addTrailingSlash && ( fixedPath[ fixedPath.length() - 1 ] != _T('/') ) )
            fixedPath += _T('/');

        return fixedPath;
    }

    //SplitFilename-------------------------------------------------------------
    void StringUtil::SplitFilename( const STRING& str, STRING& outPath, STRING& outFilename )
    {
        // Fix the path, as in standardizePath, but don't add the trailing slash:
        STRING fixedPath = str;
        std::replace( fixedPath.begin(), fixedPath.end(), _T('\\'), _T('/') );

        // Find the last occurrance of the slash:
        size_t i = fixedPath.find_last_of( _T('/') );

        // If no occurrances were found, the path was empty, but the base name
        // is the given string:
        if ( i == STRING::npos )
        {
            outPath = _T("");
            outFilename = fixedPath;
        }
        else
        {
            outPath = fixedPath.substr( 0, i + 1 );
            outFilename = fixedPath.substr( i + 1, ( fixedPath.length() - i ) - 1 );
        }
    }

    //SplitFileExtension--------------------------------------------------------
    void StringUtil::SplitFileExtension( const STRING& str, STRING& outFilename, STRING& outExt )
    {
        // Fix the path, as in standardizePath, but don't add the trailing slash:
        STRING fixedPath = str;
        std::replace( fixedPath.begin(), fixedPath.end(), _T('\\'), _T('/') );

        // Find the last occurrance of the slash:
        size_t i = fixedPath.find_last_of( _T('.') );

        // If no occurrances were found, the path was empty, but the base name
        // is the given string:
        if ( i == STRING::npos )
        {
            outExt = _T("");
            outFilename = fixedPath;
        }
        else
        {
            outFilename = fixedPath.substr( 0, i );
            outExt = fixedPath.substr( i + 1, ( fixedPath.length() - i ) - 1 );
        }
    }

    //SplitNumberedFilename
    void StringUtil::SplitNumberedFilename( const STRING& full, STRING& base, int& number, STRING& ext )
    {
        // Separate the extension from the filename:
        SplitFileExtension( full, base, ext );
        
        // Start at the end of the filename, and move toward the front as long
        // as the character is a number:
        STRING numStr = "";
        STRING::reverse_iterator numIter = base.rbegin();
        while ( numIter != base.rend() && *numIter >= '0' && *numIter <= '9' )
        {
            numStr = *numIter + numStr;
            numIter++;
        }
        base.resize( base.size() - numStr.size() );
        number = ToInt( numStr );
    }

    //BuildPath-----------------------------------------------------------------
    void StringUtil::BuildPath( const STRING& basePath, const STRING& dir, STRING& outPath )
    {
        CleanPath( basePath + dir, outPath );
    }

    //CleanPath-----------------------------------------------------------------
    void StringUtil::CleanPath( const STRING& inPath, STRING& outPath )
    {
        // Standardize the directory strings (don't add trailing slashes, though):
        STRING result = StandardizePath( inPath, false );

        // Make sure there are no double slashes:
        result = FindReplace( result, _T("//"), _T("/") );

        // Similarly, the '/./' pattern can be replaced with '/' since the
        // '.' just means the current directory.
        result = FindReplace( result, _T("/./"), _T("/") );

        // Now for some magic.  Any occurrances of the '/../' pattern require
        // the string be split, and the rightmost directory before the pattern
        // be removed.
        int index;
        while ( ( index = result.find( _T("/../") ) ) != STRING::npos )
        {
            STRING left = result.substr( 0, index );
            STRING right = result.substr( index );

            // The way the string was split, the left side does not end with a
            // slash, which means the path just has to be reverse searched for a
            // slash to remove the last directory.  Also, the right string starts
            // with the '/../' pattern, which needs to be replaced by '/'.
            int dirIndex = left.find_last_of( _T('/') );
            left = left.substr( 0, dirIndex );
            right.replace( 0, 4, _T("/") );
            result = left + right;
        }

        outPath = result;
    }

    //Match---------------------------------------------------------------------
    bool StringUtil::Match( const STRING& str, const STRING& pattern, bool caseSensitive )
    {
        // Make temporary copies of the string and pattern.  If case-sensitivity
        // is off, then make the temp strings lowercase.
        STRING tempStr = str;
        STRING tempPattern = pattern;
        if ( !caseSensitive )
        {
            ToLower( tempStr );
            ToLower( tempPattern );
        }

        // Iterators for the string and pattern:
        STRING::const_iterator patternIter  = tempPattern.begin();
        STRING::const_iterator strIter      = tempStr.begin();

        // Temporary pointers.  When a wildcard is found, store the
        // wildcard, and the next character of the source string.  Then
        // when a character match is not found, use the previous wildcard
        // and the next source character.
        STRING::const_iterator lastWildcard = tempPattern.end();
        STRING::const_iterator nextSource   = tempStr.end();

        // Compare initial characters until the search character is
        // a multi-character wildcard:
        while ( ( strIter != tempStr.end() ) && ( patternIter != tempPattern.end() ) && ( *patternIter != _T('*') ) )
        {
            // If the characters do not match and the current search
            // character is not a single character wildcard, the
            // strings do not match:
            if ( ( *strIter != *patternIter ) && ( *patternIter != _T('?') ) )
                return false;

            // Increment the pointers:
            ++patternIter;
            ++strIter;
        }

        // There has been a multi-character wildcard...
        while ( ( strIter != tempStr.end() ) && ( patternIter != tempPattern.end() ) )
        {
            if ( *patternIter == _T('*') )
            {
                // If the current wildcard is at the end of the pattern string,
                // then the strings definitely match:
                ++patternIter;
                if ( patternIter == tempPattern.end() )
                    return true;

                // Otherwise, store the pattern character right after the wildcard
                // and the next source character.  At this point, the source string
                // will be searched until this next pattern character is found.
                // If there is a point where the pattern and source do not match,
                // come back to these values:
                lastWildcard = patternIter;
                nextSource   = strIter;
            }

            // If the pattern and source strings match, simply advance the pointers:
            else if ( ( *patternIter == *strIter ) || ( *patternIter == _T('?') ) )
            {
                ++patternIter;
                ++strIter;
            }

            else
            {
                // The pattern and source characters do not match here.  Set the
                // pattern back to the character just after the previous wildcard,
                // and set the source character to the next one to compare:
                patternIter = lastWildcard;
                strIter     = nextSource;

                // Advance the source character.  The wildcard will apply to as
                // many source characters as necessary until either the source string
                // ends, or there is a positive match.  The character just after the
                // wildcard needs to match a character in the string.
                nextSource++;
            }
        }

        // Eat trailing '*'s in the pattern:
        while ( ( patternIter != tempPattern.end() ) && ( *patternIter == _T('*') ) )
            ++patternIter;

        // If the pattern is at the end, then the strings match:
        return ( patternIter == tempPattern.end() );
    }

    //AsciiToUnicode------------------------------------------------------------
    void StringUtil::AsciiToUnicode( const char* src, wchar_t* dest )
    {
        int length = strlen( src );
        //assert( length == wcslen( dest ) );
        if ( length == wcslen( dest ) )
            mbstowcs( dest, src, length );
    }
    //AsciiToUnicode------------------------------------------------------------
    std::wstring StringUtil::AsciiToUnicode( const std::string& src )
    {
        return std::wstring( src.begin(), src.end() );
    }

    //UnicodeToAscii------------------------------------------------------------
    void StringUtil::UnicodeToAscii( const wchar_t* src, char* dest )
    {
        int length = wcslen( src );
        //assert( length == strlen( dest ) );
        if ( length == strlen( dest ) )
            wcstombs( dest, src, length );
    }

    //UnicodeToAscii------------------------------------------------------------
    std::string StringUtil::UnicodeToAscii( const std::wstring& src )
    {
        int length = src.length();
        char* asciiStr = new char [ length + 1 ];
        wcstombs( asciiStr, src.c_str(), length );
        asciiStr[ length ] = '\0';
        std::string convStr( asciiStr );
        delete [] asciiStr;
        return convStr;
    }

} // namespace AggOO
