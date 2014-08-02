
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
// $Id: AggOODirSearch.cpp,v 1.1 2009/06/19 23:12:36 dratek Exp $
//
// AggOODirSearch.cpp
//
// Author       : Chad M. Draper
// Date         : June 18, 2009
//

#include "AggOODirSearch.h"

namespace AggOO
{

    ////////////////////////////////////////////////////////////////////////////
    // FileSys
    ////////////////////////////////////////////////////////////////////////////

#if ( AGGOO_PLATFORM != AGGOO_PLATFORM_WIN32 )

    //_findfirst
    long DirSearch::FileSys::_findfirst( const char* pattern, struct _finddata_t* data )
    {
        // Create a new SearchData object.  This will become the search handle.
        SearchData* searchData = new SearchData;

        // Get the name of the directory:
        const char* mask = strrchr( pattern, '/' );
        if ( mask )
        {
            searchData->dirLength   = mask - pattern;   // length of the directory name
            searchData->directory   = new char [ searchData->dirLength + 1 ];
            strncpy( searchData->directory, pattern, searchData->dirLength );
            searchData->directory[ searchData->dirLength ] = '\0';
        }
        else
        {
            // Entire pattern is the mask
            mask = pattern;

            // Directory is the current working directory
            searchData->dirLength   = 1;
            searchData->directory   = strdup( "." );
        }

        // Open the directory
        searchData->dirHandle = opendir( searchData->directory );
        if ( !searchData->dirHandle )
        {
            _findclose( long( searchData ) );
            return -1;
        }

        if ( strcmp( mask, "*.*" ) == 0 )
            mask += 2;

        // Store the current search mask
        searchData->pattern = strdup( mask );

        // Get the first item
        if ( _findnext( long( searchData ), data ) < 0 )
        {
            _findclose( long( searchData ) );
            return -1;
        }

        // Return the search handle
        return long( searchData );
    }

    //_findnext
    int DirSearch::FileSys::_findnext( long id, struct _finddata_t* data )
    {
        // Get the current search data:
        SearchData* searchData = (SearchData)id;

        // Search for the next entry, or until there are no more entries left.
        dirent* entry;
        while ( true )
        {
            // Read the directory.  If this fails, then there are no more entries.
            if ( !( entry = readdir( searchData->dirHandle ) ) )
                return -1;

            // Check whether the entry matches the filename pattern
            if ( fnmatch( searchData->pattern, entry, 0 ) == 0 )
                break;
        }

        // Set the current filename
        if ( searchData->curFileName )
            delete searchData->curFileName;
        data->name = searchData->curFileName = strdup( entry->d_name );

        // Set the full path name of the file
        int nameLength = strlen( entry->d_name );
        char* pathName = new char [ searchData->dirLength + 1 + nameLength + 1 ];
        sprintf( pathName, "%s/%s", dir->directory, data->name );

        // We need to check some attributes of the file.  First, whether it is a sub-directory:
        struct stat statBuf;
        if ( stat( pathName, &statBuf ) )
        {
            // Error.  Set as empty file.
            data->attrib    = Normal;
            data->size      = 0;
        }
        else
        {
            // If this is a directory, set it as a sub-directory.  Otherwise, set it as a normal file.
            if ( S_ISDIR( statBuf.st_mode ) )
                data->attrib = SubDirectory;
            else
                data->attrib = Normal;

            // Set the file size
            data->size = statBuf.st_size;
        }

        delete pathName;

        // In *nix, files starting with a '.' are hidden
        if ( data->name[ 0 ] == '.' )
            data->attrib |= Hidden;

        return 0;
    }

    //_findclose
    int DirSearch::FileSys::_findclose( long id )
    {
        // Get the current search data:
        SearchData* searchData = (SearchData)id;

        // If the directory is open, close it.
        int retVal = ( searchData->dirHandle ) ? closedir( searchData->dirHandle ) : 0;

        // Free the pointers:
        delete searchData->pattern;
        delete searchData->directory;
        if ( searchData->curFileName )
            delete searchData->curFileName;
        delete searchData;

        return retVal;
    }

#endif

    //Constructor
    DirSearch::FileSys::FileSys( SearchAttributes attrib )
        : mFileHandle( -1L )
        , mAttributes( attrib )
    {
        memset( &mFindData, 0, sizeof( mFindData ) );
    }

    //Destructor
    DirSearch::FileSys::~FileSys()
    {
        FindClose();
    }

    //FindAll
    size_t DirSearch::FileSys::FindAll( const STRING& pattern, std::vector< STRING >& fileNames )
    {
        fileNames.clear();
        if ( FindFirst( pattern ) )
        {
            fileNames.push_back( GetFileName() );
            while ( FindNext() )
            {
                fileNames.push_back( GetFileName() );
            }
            FindClose();
        }
        return fileNames.size();
    }

    //FindFirst
    bool DirSearch::FileSys::FindFirst( const STRING& pattern )
    {
        // Make sure the find operation is currently closed:
        FindClose();

        // Find the first file:
        if ( ( mFileHandle = _findfirst( pattern.c_str(), &mFindData ) ) != -1L )
        {
            if ( mFindData.attrib == mAttributes || mFindData.attrib & mAttributes )
                return true;
            else
                return FindNext();
        }
        return false;
    }

    //FindNext
    bool DirSearch::FileSys::FindNext()
    {
        while ( _findnext( mFileHandle, &mFindData ) == 0 )
        {
            if ( mFindData.attrib == mAttributes || mFindData.attrib & mAttributes )
                return true;
        }
        return false;
    }

    //GetfileName
    STRING DirSearch::FileSys::GetFileName() const
    {
        return mFindData.name;
    }

    //FindClose
    bool DirSearch::FileSys::FindClose()
    {
        if ( _findclose( mFileHandle ) == 0 )
        {
            mFileHandle = -1L;
            memset( &mFindData, 0, sizeof( mFindData ) );
            return true;
        }
        return false;

    }

/*
#else

    //Constructor
    FileSys::FileSys()
    {
    }

    //Destructor
    FileSys::~FileSys()
    {
    }

    //FindAll
    size_t FileSys::FindAll( const STRING& pattern )
    {
    }

    //FindFirst
    bool FileSys::FindFirst( const STRING& pattern )
    {
    }

    //FindNext
    bool FileSys::FindNext()
    {
    }

    //FindClose
    bool FileSys::FindClose()
    {
    }

#endif
*/

} // namespace AggOO
