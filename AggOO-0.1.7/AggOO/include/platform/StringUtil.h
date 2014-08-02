
/////////////////////////////////////////////////////////////////////
// $Id: StringUtil.h,v 1.2 2009/06/19 23:12:36 dratek Exp $
//
// StringUtil.h -- Header file for the StringUtil class.
//
// Author       : Chad M. Draper
// Date         : February 1, 2007
// Description  : Handles some basic string operations.
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2007-02-01      Initial implementation
//

#ifndef STRINGUTIL_H_
#define STRINGUTIL_H_

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
//#include <stdlib.h>
#include <ctype.h>

#ifdef  BOOST_FILESYSTEM
#include <boost/filesystem/operations.hpp>
#endif

namespace cmd
{
    /** @class StringUtil
        @remark
            A simple class for performing some basic string operations.    
    */
    class StringUtil
    {
    public:
    
        /////////////////////////////////////////////////////////////
        /// Convert a string to upper case
        static void toUpper( std::string& str )
        {
            // Iterate over the string, converting each character's case:
            std::transform( str.begin(), str.end(), str.begin(), toupper );
        }
    
        /////////////////////////////////////////////////////////////
        /// Convert a string to lower case
        static void toLower( std::string& str )
        {
            // Iterate over the string, converting each character's case:
            std::transform( str.begin(), str.end(), str.begin(), tolower );
        }

        /** Compare two strings, allowing for wildcards.

            @note   This is lifted from the draper library.
        */
        static bool match( const std::string& str, const std::string& pattern, bool caseSensitive = true )
        {
            // Make temporary copies of the string and pattern.  If case-sensitivity
            // is off, then make the temp strings lowercase.
            std::string tempStr = str;
            std::string tempPattern = pattern;

            if ( !caseSensitive )
            {
                toLower( tempStr );
                toLower( tempPattern );
            }

            // Iterators for the string and pattern:
            std::string::const_iterator patternIter  = tempPattern.begin();
            std::string::const_iterator strIter      = tempStr.begin();

            // Temporary pointers.  When a wildcard is found, store the
            // wildcard, and the next character of the source string.  Then
            // when a character match is not found, use the previous wildcard
            // and the next source character.
            std::string::const_iterator lastWildcard = tempPattern.end();
            std::string::const_iterator nextSource   = tempStr.end();

            // Compare initial characters until the search character is
            // a multi-character wildcard:
            while ( ( strIter != tempStr.end() ) && ( patternIter != tempPattern.end() ) && ( *patternIter != '*' ) )
            {
                // If the characters do not match and the current search
                // character is not a single character wildcard, the
                // strings do not match:
                if ( ( *strIter != *patternIter ) && ( *patternIter != '?' ) )
                    return false;

                // Increment the pointers:
                ++patternIter;
                ++strIter;
            }

            // There has been a multi-character wildcard...
            while ( ( strIter != tempStr.end() ) && ( patternIter != tempPattern.end() ) )
            {
                if ( *patternIter == '*' )
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
                else if ( ( *patternIter == *strIter ) || ( *patternIter == '?' ) )
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
            while ( ( patternIter != tempPattern.end() ) && ( *patternIter == '*' ) )
                ++patternIter;

            // If the pattern is at the end, then the strings match:
            return ( patternIter == tempPattern.end() );
        }

        /** Split a string into substrings based on a specified delimiter.
            @param  src             Source string to be split
            @param  delim           String containing delimiter characters (there can be multiple delimiters)
            @param  results         Vector containing the substrings which will be returned
            @param  addEmptyStrings Indicates if empty strings should be included in the results array
    
            @return Number of delimiters found (not the number of substrings)
        */
        static size_t split( const std::string& src, const std::string& delim, std::vector< std::string >& results, bool addEmptyStrings = false )
        {
            // Make sure the source and delimiter are specified
            if ( src.size() == 0 || delim.size() == 0 )
            {
                return 0;
            }
    
            size_t delimFound = 0, curPos, startPos = 0;
            std::string emptyString( "" );
            do
            {
                // Find the first delimiter
                curPos = src.find_first_of( delim, startPos );
                if ( curPos == startPos )
                {
                    // There is a delimiter at the start of the string.  Increment
                    // the delimiter count, and if adding empty strings, add one.
                    delimFound++;
                    if ( addEmptyStrings )
                    {
                        results.push_back( emptyString );
                    }
    
                    startPos = curPos + 1;
                }
    
                // Check for the last match
                else if ( curPos == std::string::npos )
                {
                    // Add the remainder of the source to the results, checking
                    // for an empty string.
                    emptyString = src.substr( startPos );
                    if ( emptyString.length() || addEmptyStrings )
                        results.push_back( emptyString );
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
            } while ( curPos != std::string::npos );
    
            return delimFound;
        }
    
        /** Convert a value to a string */
        template< typename T >
        static std::string toString( const T& val )
        {
            std::stringstream stream;
            stream << val;
            return stream.str();
        }
        
        ////////////////////////////////////////////////////////////////////////
        // Some directory routines
        ////////////////////////////////////////////////////////////////////////
        /** Fix the delimiters in a path.  Windows uses the backslash, but most
            others use the forward slash (Windows is actually fine with both.)
        */
        static std::string FixPath( const std::string& path )
        {
            // Replace any occurrances of '\' with '/'.
            std::string fixed = path;
            std::replace( fixed.begin(), fixed.end(), '\\', '/' );
            
            return fixed;
        }
        
        /** Create a path name from a path and file name
        */
        static std::string MakePathName( const std::string& path, const std::string& filename )
        {
            std::string fixPath = FixPath( path );
            std::string fixFile = FixPath( filename );
            if ( fixPath.length() && fixPath.at( fixPath.length() - 1 ) != '/' )
                fixPath += '/';
            return fixPath + fixFile;
        }
        
        /** Split a string into the path and filename.  The returned path is
            guaranteed to have and end slash.
        */
        static void SplitFilename( const std::string& full, std::string* path, std::string* filename )
        {
            FixPath( full );
            
            // Find the last slash:
            size_t i = full.find_last_of( '/' );
            
            // If not occurrance was found, the path is empty.  Set the filename
            // to whatever was given for the string.
            if ( i == std::string::npos )
            {
                *path = "";
                *filename = full;                
            }
            else
            {
                *path = full.substr( 0, i + 1 );
                *filename = full.substr( i + 1, ( full.length() - i ) - 1 );
            }
        }

#ifdef  BOOST_FILESYSTEM
        /** Create full path
            @remark
                Starting at the given root, this will work its way up the path,
                creating any sub-directory that does not yet exist.
                
            @param  path        Path to be created
            @return true if the path was created, false otherwise        
        */
        static void CreatePath( const std::string& path )
        {
            std::string fixedPath = FixPath( path );
            
            // Split the path into each directory
            std::vector< std::string > dirs;
            split( fixedPath, "/", dirs );

            // Go over the components, making sure each directory exists
            std::vector< std::string >::iterator iter;
            std::string curPath = "";
            for ( iter = dirs.begin(); iter != dirs.end(); iter++ )
            {
                const char* curDir = iter->c_str();
                if ( *( curDir + 1 ) != ':' )
                {
                    curPath += curDir;
                    std::string dirStr;
                    dirStr += curPath;
                    try
                    {
                        namespace fs = boost::filesystem;
                        fs::path p( dirStr.c_str(), fs::native );
                        fs::create_directory( p );
                    }
                    catch ( std::exception& e )
                    {
                        std::string msg( "Error creating directory:" );
                        msg += e.what();
                    }

                    curPath += "/";
                }
                else
                    curPath = std::string( curDir ) + "/";
            }
        }
#endif

    }; // class StringUtil

} // namespace cmd

#endif  // STRINGUTIL_H_
