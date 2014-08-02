
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
// $Id: AggOODirSearch.h,v 1.1 2009/06/19 23:12:35 dratek Exp $
//
// AggOODirSearch.h -- Utilities for searching directories.
//
// Author       : Chad M. Draper
// Date         : June 18, 2009
//
/////////////////////////////////////////////////////////////////////
// Update Log
//      Date            Description
//      ----            ----------
//      2009-06-18      Initial implementation
//

#ifndef AggOODirSearch_H_
#define AggOODirSearch_H_

#include "AggOOTypes.h"
#include "AggOOStringUtil.h"
#include <vector>

#if ( AGGOO_PLATFORM == AGGOO_PLATFORM_WIN32 )
#   include <io.h>
#   include <direct.h>
#else
#   include <sys/types.h>
#   include <dirent.h>
#endif

namespace AggOO
{
    /** @class DirSearch
        Class for searching a directory for files.
    */
    class DirSearch
    {
    public:
        /** Attributes which may be used to filter the search results.  These
            match the attributes used by Win32 _find.
        */
        typedef enum SearchAttributes
        {
            Normal          = 0x00,
            ReadOnly        = 0x01,
            Hidden          = 0x02,
            System          = 0x04,
            SubDirectory    = 0x10,
            Archive         = 0x20
        };

    private:
        /** @class FileSys
            Performs the actual searching, depending on the current platform.
        */
        class FileSys
        {
        private:
#if ( AGGOO_PLATFORM != AGGOO_PLATFORM_WIN32 )
            struct _finddata_t
            {
                char*   name;           ///< File name
                int     attrib;         ///< File attributes
                unsigned long size;     ///< Size of the file in bytes
            };

            /** @struct SearchData
                Contains information about the current search.  The pointer is returned as
                a long value from _findfirst and _findnext.
            */
            struct SearchData
            {
                char*   pattern;        ///< Pattern for finding files
                char*   directory;      ///< Directory in which to find the files
                char*   curFileName;    ///< Current filename
                int     dirLength;      ///< Length of the directory string
                DIR     dirHandle;      ///< Handle to the directory opened by opendir

                SearchData()
                    : pattern( 0 )
                    , directory( 0 )
                    , curFileName( 0 )
                    , dirLength( 0 )
                    , dirHandle( 0 )
                {
                }
            };

            /** Find the first file matching a given pattern.  This emulates Win32 _findfirst. */
            long _findfirst( const char* pattern, struct _finddata_t* data );

            /** Find the next file in the current search.  This emulates Win32 _findnext. */
            int _findnext( long id, struct _finddata_t* data );

            /** Close the current search.  This emulates Win32 _findclose. */
            int _findclose( long id );
#endif
            struct _finddata_t  mFindData;          ///< Structure which holds data about the file.
            long                mFileHandle;        ///< Handle to the last found file.
            STRING              mPattern;           ///< Pattern to use for file searching.
            SearchAttributes    mAttributes;        ///< Attributes for searching

        public:
            /** Constructor */
            FileSys( SearchAttributes attrib );
            ~FileSys();

            /** Find all files that match a pattern and return them in an array, sorted by
                ascending filename.
            */
            size_t FindAll( const STRING& pattern, std::vector< STRING >& fileNames );

            /** Find the first file that matches a pattern
                @param      pattern     Pattern for the file to match.  This can include wild cards (* and ?)
                @return     true if a file was found, false if no files exist.
            */
            bool FindFirst( const STRING& pattern );

            /** Find the next file using the current pattern.
                @return     true if a file was found, false if no files exist.
            */
            bool FindNext();

            /** Get the current file name */
            STRING GetFileName() const;

            /** Close the file finding operation.  This will reset the find data so another
                search can be performed.

                @return     true if successful, false if an error occurred.
            */
            bool FindClose();
        };

    public:
        DirSearch( SearchAttributes attrib );

    }; // class DirSearch

} // namespace AggOO

#endif  // AggOODirSearch_H_
